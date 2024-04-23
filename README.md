# TicTacToe

a simple tic-tac-toe game for Arduino board.
I wrote the code with [@Burgerlein](https://github.com/Burgerlein) in college class

# Arduino Board

| Amount      | Description | Img |
| ----------- | ----------- | ----------- |      
| 1      | Arduino Mega      | <img src="assets/ArduinoMega.png" width="200" height="200" alt="Arduino Board"> |
| 4      | Led Matrizen      | <img src="assets/Matrix.png" width="200" height="200" alt="Matrix"> |
| 1      | LCD Display      | <img src="assets/LCD.png" width="300" height="200" alt="LCD"> |
| 1      | KeyPad      | <img src="assets/KeyPad.png" width="200" height="200" alt="KeyPad"> |


# how it works 
The playing field is visualized on the 4 LED matrices. The playing field can also be played on a martix, but it's not as pretty.
<br>
Information and error messages are displayed on the LCD display, such as when a field is occupied or whose turn it is.
<br><br>
The keypad is intended for selecting the field. The numbers 1-9 represent the playing field.
<br><br>
The program is always waiting for input while no other code is being executed. After each entry, the entry is checked according to the following criteria:
1. is the input a valid number.
2. is the number taken
3. is the number the missing piece for a win
<br>
When the game ends, a message is output with the game result and all variables are reset for the next round.
