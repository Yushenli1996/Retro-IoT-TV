import serial
import requests
import time
from pprint import pprint
from datetime import datetime
  
time.sleep(1)  
ser = serial.Serial("/dev/ttyACM0", 9600)
ser.flushInput()

while True:
    print(ser.readline())
    try: 
        if (int(ser.readline()) == 1010):
        #print(datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
            ser.write(datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
            ser.flushInput()
            ser.flushOutput()
        #print(ser.readline())
        #ser.write("Hello")
        if (int(ser.readline()) == 2000):
            #print("Hello")
            ser.flushInput()
            ser.flushOutput()
            url = 'https://api.openweathermap.org/data/2.5/weather?q=Chicago,us&appid=1c865cbb1dd3b0a02e6764a9a1ce0355&units=imperial'
            res = requests.get(url)
            request_result = res.json();
            temp = request_result['main']['temp']
            temp_max = request_result['main']['temp_max']
            temp_min = request_result['main']['temp_min']
            humidity = request_result['main']['humidity']
            result = "\n\n  Outdoor: {}F\n\n  Max:{}F, Min:{}F".format(temp, temp_max, temp_min, humidity)
            print(result)
            ser.write(result)
    except ValueError:
        pass
    ser.flushInput()
    ser.flushOutput()

        