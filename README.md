# MCU_UARTGamepadStandard
Developed by Timothy Goodale 
# Goal
The goal is to create a standard protocol where users on any MCU can send data to the uartToKeyboard program where the program decodes the data 
and converts it to keyboard input.
# Importance
Allows for easy creation of arcade-sticks, gamepads and other peripherals using UART. USB is difficult to implement and complex for beginners 
wanting to build a controller. UART also allows 
# Design
  ### Arcade-stick
  The design of an arcade-stick can be simplified down to how many buttons that it is going to have. If it has 16 buttons or less, use a unsigned 16 bit integer
  and map the buttons to a specific bit position. Joystick movement will ALWAYS be assigned to the first four most significant bits starting with UP-DOWN-LEFT-RIGHT.
  The buttons will be the next 8 bits, the first 4 bits of 8 will be the top row. 
  The top row is assigned to the keyboard as U-I-O-P 
  while the second row is assigned as        H-J-K-L.
  The last two bits will be used for start and pause. Start will be the first of the two and pause will be the last bit.
  Use bit-maksing to get the bits to work.
  ### Gamepad
  TBD
 
# Conclusion and Compiling
As of now the uartToKeyboard.c file is not following the protocol described above. This will be updated shortly, to compile the code we need conio.h in the same directory and 
Cygwin to do so. To compile the program, g++ uartToKeyboard.c -o <exe name>. To run the program, navigate to it in Cygwin and do ./<exe name> and it will run.



# Notes
If you do add any changes to this protocol please credit me. Thank you
  
  

