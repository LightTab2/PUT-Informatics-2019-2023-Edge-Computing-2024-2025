import Adafruit_DHT
import sqlite3

con =sqlite3.connect('example.db')
cur = con.cursor()
#cur.execute('''CREATE TABLE pomiary(id INTEGER PRIMARY KEY AUTOINCREMENT, wartosc NUMBER(4,2), data DATE DEFAULT CURRENT_TIMESTAMP)''')


sensor = Adafruit_DHT.DHT11

pin = 'P8_12'
for j in range(5):
    pomiary = []
    for  i in  range(18):
        humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
        pomiary.append(temperature)
    pomiary.sort()
    del pomiary[0]
    del pomiary[-1]
    cur.execute("INSERT INTO pomiary(wartosc) VALUES('%f')" % (sum(pomiary)/len(pomiary)))


cur.execute("select * from pomiary")
print(cur.fetchall())
con.commit()
con.close()
