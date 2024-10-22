import json
import threading
from flask import Flask, request, render_template, redirect, url_for
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import text
from paho.mqtt import client as mqtt_client
from datetime import datetime
import serial 
import logging 
import time
import wave

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# MQTT configuration
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
POSTGRES_URI = 'postgresql://murloc:murloc@localhost:5432/senseORi'

MQTT_TOPIC = "sensor_data"
COM_PORT = 'COM9'

wav_cache = {}
ser = serial.Serial(COM_PORT, 921600)

def send_wav(file_name, i):
    # Check if the file data is already cached
    if file_name in wav_cache:
        frames = wav_cache[file_name]
    else:
        with wave.open("sounds/" + file_name + ".wav", 'rb') as wav_file:
            n_frames = wav_file.getnframes()
            frames = wav_file.readframes(n_frames)
            wav_cache[file_name] = frames

    total_frames = len(frames)

    start_index = (i * BUFFER_SIZE) % total_frames
    end_index   = start_index + BUFFER_SIZE

    if end_index <= total_frames:
        data_to_send = frames[start_index:end_index]
    else:
        data_to_send = frames[start_index:] + frames[:end_index % total_frames]

    logging.info(i)
    ser.write(data_to_send)

def serial_communication_loop():
    tick = 0
    try:
        while True:
            with app.app_context():
                if priority:
                    highest_priority_alarm = max(priority.items(), key=lambda item: item[1][0] if item[1] else -1)
                    if highest_priority_alarm:
                        alarm = AlarmSound.query.filter_by(sensor_name=highest_priority_alarm[0], value=highest_priority_alarm[1][1]).first()
                        if alarm is None:
                            time.sleep(0.01)
                            continue
                        alarm_sound = alarm.sound
                        if tick == 0:
                            start_time = time.time()
                            logging.info(F"started")
                        
                        send_wav(alarm_sound, tick)

                    else:
                        tick = 0
                        continue

                    # No need to track time: fill buffer and wait until its emptied
                    #elapsed_time = 1000000 * (time.time() - start_time)
                    #target_time = (tick - 0.5) * SAMPLE_WAIT
                    #if elapsed_time < target_time:
                    #    logging.info(target_time - elapsed_time)
                    #    time.sleep((target_time - elapsed_time)/1000000.0)
                    tick += 1
    except serial.SerialException as e:
        logging.error(f"Serial communication error: {e}")
    finally:
        ser.close()

# MQTT callback functions
def on_connect(client, userdata, flags, rc, properties):
    if rc == 0:
        logging.info("Connected to MQTT Broker!")
    else:
        logging.error("Failed to connect, return code %d\n", rc)
    client.subscribe(MQTT_TOPIC)

FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60
SAMPLE_RATE = 2500
SAMPLE_DELAY = 1000000 // SAMPLE_RATE/2
BUFFER_SIZE = 992
SAMPLE_WAIT = (BUFFER_SIZE - 1) * SAMPLE_DELAY

def on_disconnect(client, userdata, disconnect_flags, rc, properties):
    logging.info("Disconnected with result code: %s", rc)
    reconnect_count, reconnect_delay = 0, FIRST_RECONNECT_DELAY
    while reconnect_count < MAX_RECONNECT_COUNT:
        logging.info("Reconnecting in %d seconds...", reconnect_delay)
        time.sleep(reconnect_delay)

        try:
            client.reconnect()
            logging.info("Reconnected successfully!")
            return
        except Exception as err:
            logging.error("%s. Reconnect failed. Retrying...", err)

        reconnect_delay *= RECONNECT_RATE
        reconnect_delay = min(reconnect_delay, MAX_RECONNECT_DELAY)
        reconnect_count += 1
    logging.info("Reconnect failed after %s attempts. Exiting...", reconnect_count)

priority = {"papuga": (1, 0)}

def on_message(client, userdata, msg):
    logging.info(msg.topic+" "+str(msg.payload))
    # Assuming msg.payload is in JSON format: {"sensor_name": "papuga", "value": 123}
    data = json.loads(msg.payload)
    sensor_name = data.get('sensor_name')  # Assuming sensor_name contains sensor name
    value = data.get('value')
    time = datetime.now() 
    # Add data to history
    with app.app_context():
        sensor = Sensor.query.filter_by(name=sensor_name).first()
        if sensor:
            new_history = SensorHistory(sensor_name=sensor_name, value=value, time=time)
            db.session.add(new_history)
            db.session.commit()
            logging.info("Added " + sensor_name + " with value " + str(value))
        else:
            logging.warning("Couldnt add " + sensor_name + " with value " + str(value))
        
        # Add to priority dictionary
        if sensor:
            alarm = AlarmSound.query.filter_by(sensor_name=sensor_name, value=value).first()
            if alarm is None:
                priority[sensor_name] = None
            else:
                priority[sensor_name] = (alarm.priority, value)

# MQTT client setup
client = mqtt_client.Client(client_id="subscribe-mastermind", callback_api_version=mqtt_client.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message
client.on_disconnect = on_disconnect
logging.debug("Trying to connect to MQTT Broker...")
client.connect(MQTT_BROKER, MQTT_PORT)

# Start MQTT loop in a separate thread
client.loop_start()

db = SQLAlchemy()

class Sensor(db.Model):
    name = db.Column(db.String(50), nullable=False, primary_key=True)

    def as_dict(self):
        return {col.name: getattr(self, col.name) for col in self.__table__.columns}

class AlarmSound(db.Model):
    value = db.Column(db.Integer, nullable=False, primary_key=True)
    sensor_name = db.Column(db.String(50), db.ForeignKey('sensor.name'), nullable=False)
    sound = db.Column(db.String(100), nullable=False)
    priority = db.Column(db.Integer, nullable=False, default=0)  # Add priority column

    sensor = db.relationship('Sensor', backref=db.backref('alarms', lazy=True))

    def as_dict(self):
        return {col.name: getattr(self, col.name) for col in self.__table__.columns}

class SensorHistory(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    sensor_name = db.Column(db.String(50), db.ForeignKey('sensor.name'), nullable=False)
    value = db.Column(db.Integer, nullable=False)
    time = db.Column(db.DateTime, nullable=False)
    sensor = db.relationship('Sensor', backref=db.backref('history', lazy=True))

    def as_dict(self):
        return {'value': self.value, 'time': self.time}

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = POSTGRES_URI
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db.init_app(app)
with app.app_context():
    #db.session.execute(text('''DROP SCHEMA public CASCADE;'''))
    #db.session.execute(text('''CREATE SCHEMA public;'''))
    #db.session.commit()
    db.create_all()

@app.route('/', methods=['POST'])
def handle_sensor_operations():
    if 'sensor_name_to_remove' in request.form:
        sensor_name = request.form['sensor_name_to_remove']
        sensor = Sensor.query.filter_by(name=sensor_name).first()
        if sensor:
            db.session.delete(sensor)
            db.session.commit()
    elif 'sensor_name' in request.form:
        sensor_name = request.form['sensor_name']
        new_sensor = Sensor(name=sensor_name)
        db.session.add(new_sensor)
        db.session.commit()
    return redirect(url_for('get_sensors'))

@app.route('/', methods=['GET'])
def get_sensors():
    sensors = Sensor.query.all()
    return render_template('sensors.html', sensors=sensors)

@app.route('/alarms/<sensor_name>', methods=['POST'])
def handle_alarm_operations(sensor_name):
    if 'alarm_value_to_remove' in request.form:
        alarm_value = request.form['alarm_value_to_remove']
        alarm = AlarmSound.query.get(alarm_value)
        if alarm:
            db.session.delete(alarm)
            db.session.commit()
    elif 'value' in request.form and 'sound' in request.form and 'priority' in request.form:
        data = request.form
        new_alarm_sound = AlarmSound(sensor_name=sensor_name, value=data['value'], sound=data['sound'], priority=data['priority'])
        db.session.add(new_alarm_sound)
        db.session.commit()
    return redirect(url_for('get_sensor_alarms', sensor_name=sensor_name))

@app.route('/alarms/<sensor_name>', methods=['GET'])
def get_sensor_alarms(sensor_name):
    sensor = Sensor.query.filter_by(name=sensor_name).first_or_404()
    return render_template('alarms.html', sensor=sensor)

@app.route('/history/<sensor_name>', methods=['POST'])
def create_sensor_history(sensor_name):
    SensorHistory.query.filter_by(sensor_name=sensor_name).delete()
    db.session.commit()
    return redirect(url_for('get_sensor_history', sensor_name=sensor_name))

@app.route('/history/<sensor_name>', methods=['GET'])
def get_sensor_history(sensor_name):
    sensor = Sensor.query.filter_by(name=sensor_name).first_or_404()
    return render_template('history.html', sensor=sensor)

if __name__ == '__main__':
    serial_thread = threading.Thread(target=serial_communication_loop)
    serial_thread.daemon = True
    serial_thread.start()
    
    app.run(host='0.0.0.0', port=5000)
