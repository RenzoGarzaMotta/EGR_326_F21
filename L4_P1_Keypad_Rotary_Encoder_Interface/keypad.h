/*Lab 4 - Interfacing a Keypad and Rotary Encoder with the MSP432
 *Part 1 - Displaying the key pressed on the CCS console
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
 *  This part will use the MSP432 serial port to send the key press information collected by the program
 *  to the CCS console window for display. This program also accommodates for bounce in the keypad buttons.
 *  The program will account for keyboard presses being held down.
 *
 *  -Connect your keypad to the MSP432 I/O port pins as designed in the prelab exercise
 *  -The program prompts the user to press a key on the keypad
 *  -Read the keypad and print the number (or symbol) of the  key that was pressed to the CCS
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
 * Decimal         Char
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
void printKeys(char val);
void inputTroubleshoot(void);

#endif //KEYPAD_H
