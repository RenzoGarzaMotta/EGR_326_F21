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
 */

#include "Keypad.h"
#include "SysTick.h"
#include <stdio.h>
#include <string.h>

/*-----------GLOBAL VARIABLE DEFINITION-----------*/
char keyPress = 0;
//char input[4] = {0};
char PIN2[4] = {0};
int entry = 0;
int terminate = 0;
int flag = 0;

void keypadInit(void){
    KEYPAD_PORT->SEL0 &=~ KEYPAD;                       //Initialize the Keypad as GPIO
    KEYPAD_PORT->SEL1 &=~ KEYPAD;
    KEYPAD_PORT->REN |= ROWS;                           //Enable internal resistor
    KEYPAD_PORT->OUT |= ROWS;                           //Set internal resistor as pull-up
}

/*------------READING KEYPAD FUNCTION-------------*/
int readKeypad(void){
    uint8_t col, row;
    keyPress = UNPRESSED;                                        //Indicate that the keypad is unpressed
    for ( col = 0; col < 3; col++ ){
        KEYPAD_PORT->DIR = 0x00;                                 //Set columns and rows to inputs
        KEYPAD_PORT->REN |= ROWS;                                //Enable the internal pull-up resistors for rows
        KEYPAD_PORT->OUT |= ROWS;
        KEYPAD_PORT->DIR |= BIT(4 + col);                        //Set columns three to an output
        KEYPAD_PORT->OUT &=~ BIT(4 + col);                       //Set columns three to low

        row = KEYPAD_PORT->IN & UNPRESSED;                       //Read all rows
        //Wait in an empty loop until the button being pressed has been unpressed.
        while (!(KEYPAD_PORT->IN & ROW0) | !(KEYPAD_PORT->IN & ROW1) | !( KEYPAD_PORT->IN & ROW2) | !( KEYPAD_PORT->IN & ROW3));

        if(row != UNPRESSED){                       //Check if any button in current column has been pressed
            entry++;
            switch (col){
                case 0: //Left column check
                    if(row == (BUTTON_ONE  )) keyPress = 49; //Button one was pressed
                    if(row == (BUTTON_FOUR )) keyPress = 52; //Button four was pressed
                    if(row == (BUTTON_SEVEN)) keyPress = 55; //Button seven was pressed
                    if(row == (STAR        )){               //Asterisk was pressed
                        keyPress = 42;
                        entry--;                             //Do not count special char as an entry in a number sequence
                    }
                    break;  //DO NOT DELETE
                case 1: //MIDDLE COLUMN CHECK
                    if(row == (BUTTON_TWO  )) keyPress = 50; //Button two was pressed
                    if(row == (BUTTON_FIVE )) keyPress = 53; //Button five was pressed
                    if(row == (BUTTON_EIGHT)) keyPress = 56; //Button eight was pressed
                    if(row == (BUTTON_ZERO )) keyPress = 48; //Button zero was pressed
                    break;  //DO NOT DELETE
                case 2: //RIGHT COLUMN CHECK
                    if(row == (BUTTON_THREE)) keyPress = 51; //Button three was pressed
                    if(row == (BUTTON_SIX  )) keyPress = 54; //Button six was pressed
                    if(row == (BUTTON_NINE )) keyPress = 57; //Button nine was pressed
                    if(row == (POUND       )){               //Pound key was pressed
                        keyPress = 35;
                        entry--;                             //Do not count special char as an entry in a number sequence
                    }
                    break;  //DO NOT DELETE
            }
            break;  //DO NOT DELETE
        }
    }
    KEYPAD_PORT->DIR = 0;                                        //Set columns to inputs
    if(col == 3) return 0;                                       //Check to see if anything was pressed or not
    return 1;
}

/*----------PRINT KEY PRESSES FUNCTION------------*/
void printKeys(char val){
    printf("%c", val);                              //Print keypad inputs to the console
    fflush(stdout);
}

/*--------------TROUBLESHOOT INPUT----------------*/
void inputTroubleshoot(void){                           //Not used for this project
    if((keyPress == '*') || (keyPress == '#')){
        if(keyPress == '*'){                            //'*' Is not an allowed input
            //printf("\nINVALID INPUT\n");
        }
        if((entry < 3) && (keyPress == '#')){           //PIN cannot be less than 4 digits
            //printf("\nPLEASE ENTER A FOUR DIGIT PIN CODE\n");
        }
        if((entry > 3) && (keyPress == '#')){           //'#' DEtermines termination of PIN input
            terminate = 1;
        }
    }
}
