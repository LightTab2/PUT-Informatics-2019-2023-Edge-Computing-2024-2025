import Adafruit_BBIO.UART as UART
import serial
import Adafruit_DHT
import sqlite3

UART.setup("UART1")
con = sqlite3.connect('example.db')
cur = con.cursor()
cur.execute('''CREATE TABLE pomiary(id INTEGER PRIMARY KEY AUTOINCREMENT, wartosc NUMBER(4,2), data DATE DEFAULT CURRENT_TIMESTAMP)''')

ser = serial.Serial(port = "/dev/ttyO1", baudrate=9600)
ser.close()
ser.open()
if ser.isOpen():
    pomiar = float(ser.readline())
ser.close()

cur.execute("INSERT INTO pomiary(wartosc) VALUES('%f')" % pomiar)


cur.execute("select * from pomiary")
print(cur.fetchall())
con.commit()
con.close()

