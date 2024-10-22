from flask import Flask, Response, render_template_string
import cv2
import threading
import time
import paho.mqtt.client as mqtt
import json
import logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

app = Flask(__name__)

mqtt_broker = '192.168.43.62'
mqtt_port = 1883
mqtt_topic = 'sensor_data'

def on_connect(client, userdata, flags, rc, properties):
    print(f"Connected with result code {rc}")

FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60

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

# MQTT client setup
client =mqtt.Client(client_id="publish-papuga", callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.connect(mqtt_broker, mqtt_port, 60)

# Start MQTT loop in a separate thread
client.loop_start()

# Global variables for the camera and the current frame
camera = cv2.VideoCapture(0)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
current_frame = None
time_sleep = 0.03

predictions = [None] * 10
current_index = 0
last_sent_value = None

def capture_frames():
    global current_frame, current_index, last_sent_value
    while True:
        ret, frame = camera.read()
        if ret:
            current_frame = cv2.flip(frame, 0)

            # Convert the frame to grayscale
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            # Detect faces in the frame
            faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(15, 15))
            
            frame_center = frame.shape[1] // 2

            # Draw rectangles around detected faces and send position via MQTT in JSON format
            if len(faces) > 0:
                # Select the face with the largest area
                largest_face = max(faces, key=lambda rect: rect[2] * rect[3])
                (x, y, w, h) = largest_face
                cv2.rectangle(current_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                # Determine if the face is on the left or right
                position = 1 if x + w // 2 < frame_center else 2
                # Store the prediction in the fixed-size array
                predictions[current_index] = position
                current_index = (current_index + 1) % 10
            else:
                predictions[current_index] = 0
                current_index = (current_index + 1) % 10
                
                # Check the last 10 predictions only if the array is fully populated
            if None not in predictions:
                most_frequent_prediction = max([(predictions.count(2), 2), (predictions.count(1), 1), (predictions.count(0), 0)])
                final_value = most_frequent_prediction[1]
                # Send only if the value has changed since the last send
                if final_value != last_sent_value:
                    # Create JSON message
                    message = json.dumps({"sensor_name": "papuga", "value": final_value})
                    # Send JSON message via MQTT
                    result = client.publish(mqtt_topic, message)
                    status = result.rc
                    if status == 0:
                        logging.info(f"Message sent to topic {mqtt_topic}")
                    else:
                        logging.error(f"Failed to send message to topic {mqtt_topic}")
                    last_sent_value = final_value
        time.sleep(time_sleep)

def generate_frame():
    global current_frame
    while True:
        if current_frame is None:
            continue
        
        ret, jpeg = cv2.imencode('.jpg', current_frame)
        if not ret:
            continue
        frame = jpeg.tobytes()
        # Yield the frame in byte format
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')
        time.sleep(time_sleep)

@app.route('/')
def index():
    # HTML template to display the video stream
    return render_template_string('''
        <html>
        <head>
            <title>Camera Stream</title>
        </head>
        <body>
            <h1>Camera Stream</h1>
            <img src="{{ url_for('video_feed') }}">
        </body>
        </html>
    ''')

@app.route('/video_feed')
def video_feed():
    # Video streaming route
    return Response(generate_frame(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    # Start the frame capture thread
    capture_thread = threading.Thread(target=capture_frames)
    capture_thread.daemon = True
    capture_thread.start()
    # Start the Flask app
    app.run(host='0.0.0.0', port=4898)
