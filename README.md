# Rubiks_Cube_Solver
Uses Python with Kociemba and an STM32F0 Discovery board to power a solver


Main.c 
----------
This file is the main.c file that is uploaded to the STM32F0-Discovery Board (STM32F072B).
The board was programed using STM Cube, making sure to choose:

- STM32F0 in the Series Filter
- STM32F0x2 in the Lines Filter
- LQFP64 in the Package Filter
- STM32F072RBTx from the list of available MCU's
- MDK-ARM V5 from Toolchain/IDE inside the settings

To change the speed the motors run at, change the PSC number on line 98 to anything above 3999 for NEMA 17 motors.

randomize.py
----------
This code can be run to randomize the cube as is, or you can change the number of changes it makes on line 9
by changing the number in "range()".

rubiks_solver.py
----------
This code has two methods for inputting the state of the faces of the cube:

1. intake_cube will ask you for each cube color one at a time.
2. intake_cube_2 will ask you for the 9 colors of each face at a time.

You need to make sure you comment out line 61 and uncomment line 62 and then replace the function on line 62
with the function you wish to use.

serial_test.py
----------
This code is for testing your serial connection to the STM32F0 Discovery board. If the terminal in python reads back to
you the string inside "path" on line 5, then your connection is functioning. 
