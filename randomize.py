import random
import serial
import time

list_face = ['U', 'R', 'F', 'D', 'L', 'B']
list_turn = ['1', '2', '3']
path = ''

for _ in range(10):
    path += random.choice(list_face)
    path += random.choice(list_turn)

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM3'
# ser.stopbits = 'STOPBITS_TWO'
ser.open()
time.sleep(1)
for move in path:
    #time.sleep(1)
    ser.write(str.encode(move))
    print(ser.read())
ser.close()
