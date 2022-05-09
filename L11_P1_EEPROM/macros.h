/*Lab 4 - Interfacing a Keypad and Rotary Encoder with the MSP432
 *Part 3 - Merging the keypad and rotary encoder
 *
 *Authors:
 *  Renzo Garza Motta
 *  Cianna Janicke
 *
 *Class: EGR 326
 *Section: 903
 *
 *Instructor:
 *  Dr. Nabeeh Kandalaft
 *
 *Description:
 *  This part requires the user to enter a value using the keypad and then turning the rotary
 *  encoder until the absolution number of pulses (abosilute value of CW - CCW) exceeds the
 *  keypad value. If this occurs, the RED led should be illuminated. Pressing the pushbutton
 *  switch resets the count and turns off the LED.
 *
 *  -Prompt the user for a pulse range
 *  -Monitor the rotary encoder as in part II to keep track of number of counts (CW and CCW)
 *  -Illuminate a RED LED if the CW count is met (but not exceeded)
 *  -Illuminate a BLUE LED if the CCW count is met (but not exceeded)
 *  -Nothing then will happen until the user presses the pushbutton switch to reset the count and turn off the LED.
 *  -The program should return to step one of part 3 upon the button being pressed.
 */

#ifndef MACROS_H_
#define MACROS_H_

#define BTN_PORT    P1
#define BTN1        BIT1
#define BTN2        BIT4
#define BTNS        (BTN1|BTN2)

#define I2C_PORT    P1
#define I2C_SCL     BIT7
#define I2C_SDA     BIT6
#define I2C         (I2C_SCL|I2C_SDA)

#define EEPROM_ADDRESS 0b1010111

#endif /* MACROS_H_ */
