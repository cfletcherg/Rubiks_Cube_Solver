import kociemba as kb
import numpy as np
import serial
import time

# path = kb.solve('UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB')
# Green = U
# Orange = R
# White = F
# Blue = D
# Red = L
# Yellow = B


def intake_cube():
    init_state = ''
    temp = ''
    sides = np.array((['Yellow', 'Red', 'Blue', 'White', 'Orange', 'Green']))

    while len(init_state) < 54:
        for col in sides:
            while len(temp) < 9:
                part = input('Enter Color of piece ' + str(len(temp) + 1) + ' for the ' + col + ' side: ')
                if part == 'r':
                    temp += 'R'
                elif part == 'o':
                    temp += 'L'
                elif part == 'g':
                    temp += 'B'
                elif part == 'w':
                    temp += 'D'
                elif part == 'b':
                    temp += 'F'
                elif part == 'y':
                    temp += 'U'
                else:
                    print('Enter a correct color!')
            init_state += temp
            temp = ''

    return init_state


def intake_cube_2():

    init_state = ''
    sides = np.array((['Yellow', 'Red', 'Blue', 'White', 'Orange', 'Green']))
    for col in sides:
        part = input('Enter Color of pieces for the ' + col + ' side: ')
        init_state += part
    init_state = init_state.replace('r', 'R')
    init_state = init_state.replace('o', 'L')
    init_state = init_state.replace('g', 'B')
    init_state = init_state.replace('w', 'D')
    init_state = init_state.replace('b', 'F')
    init_state = init_state.replace('y', 'U')

    return init_state


state = 'LFBUULBFULDUFRUDFRDUBBFDLBRDRFRDRDBUULRDLDFUBRLFBBRFLL'
# state = intake_cube_2()

path = kb.solve(state)

path = path.replace("' ", "3")
path = path.replace('2 ', '2')
path = path.replace(' ', '1')
print(path)

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM3'
# ser.stopbits = 'STOPBITS_TWO'
ser.open()
for move in path:
    ser.write(str.encode(move))
    print(ser.read())
ser.close()
