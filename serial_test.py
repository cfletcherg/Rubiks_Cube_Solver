import serial
import time


path = 'U1R1F1D1L1B1'
ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM3'
ser.timeout = 2
ser.open()
time.sleep(1)
for move in path:
    ser.write(str.encode(move))
    print(ser.read())


ser.close()
