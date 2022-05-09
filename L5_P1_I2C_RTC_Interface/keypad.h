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
 *
 *Connection Map:
 * KEYPAD             MSP432
 *  ROW0---------------P4.0
 *  ROW1---------------P4.1
 *  ROW2---------------P4.2
 *  ROW3---------------P4.3
 *  COL0---------------P4.4
 *  COL1---------------P4.5
 *  COL2---------------P4.6
 *
 *Relevant ASCII Values:
 *  Dec             Chr
 *   35--------------#
 *   42--------------*
 *   48--------------0
 *   49--------------1
 *   50--------------2
 *   51--------------3
 *   52--------------4
 *   53--------------5
 *   54--------------6
 *   55--------------7
 *   56--------------8
 */

#ifndef KEYPAD_H
#define KEYPAD_H
/*------------PREPROCESSOR DEFINITION-------------*/
#include "msp.h"

/*----------------MACRO DEFINITION----------------*/
#define KEYPAD_PORT             P4
#define ROW0                    BIT0
#define ROW1                    BIT1
#define ROW2                    BIT2
#define ROW3                    BIT3
#define COL0                    BIT4
#define COL1                    BIT5
#define COL2                    BIT6

#define ROWS                    (ROW0|ROW1|ROW2|ROW3)
#define COLS                    (COL0|COL1|COL2)

#define KEYPAD                  (ROWS|COLS)

#define UNPRESSED               0x0F
#define PRESSED                 !UNPRESSED

#define BUTTON_ONE              0x0E
#define BUTTON_TWO              0x0E
#define BUTTON_THREE            0x0E
#define BUTTON_FOUR             0x0D
#define BUTTON_FIVE             0x0D
#define BUTTON_SIX              0x0D
#define BUTTON_SEVEN            0x0B
#define BUTTON_EIGHT            0x0B
#define BUTTON_NINE             0x0B
#define BUTTON_ZERO             0x07
#define STAR                    0x07
#define POUND                   0x07
#define CLEAR_PORT              0x00

/*--------------PROTOTYPE DEFINITION--------------*/
void keypadInit(void);
int readKeypad(void);
void printKeys(uint8_t* val);
void input(char input[]);
void inputTroubleshoot(void);
void confirmPin(void);
void pinValidated(char PIN2[]);
void shiftVal(uint8_t* val);

#endif //KEYPAD_H
