#include "Keypad.h"
#include "SysTick.h"
#include <stdio.h>
#include <string.h>

/*-----------GLOBAL VARIABLE DEFINITION-----------*/
char keyPress = 0;
char input[4] = {0};
char PIN2[4] = {0};
int entry = 0;
int terminate = 0;
int flag = 0;

void KEYPAD_INIT(void){
    KEYPAD_PORT->SEL0 &=~ ROWS;
    KEYPAD_PORT->SEL1 &=~ ROWS;
    KEYPAD_PORT->REN |= ROWS;                                    //ENABLE INTERNAL PULL-UP RESISTOR
    KEYPAD_PORT->OUT |= ROWS;
}

/*----------------PIN INPUT SHIFT-----------------*/
void MENU_INPUT(char input[]){
    if(keyPress != '*'){                                //SHIFT PIN VALUES FROM ONE INDEX TO THE ONE PREVIOUS
        input[0] = input[1];
        input[1] = keyPress;
    }
}

/*------------READING KEYPAD FUNCTION-------------*/
int READ_KEYPAD(void){
    uint8_t col, row;
    keyPress = 0;
    for ( col = 0; col < 3; col++ ){
        KEYPAD_PORT->DIR = 0x00;                                 //SET COLUMNS AND ROWS TO INPUTS
        KEYPAD_PORT->REN |= ROWS;                                //ENABLE PULL UP RESISTOR FOR ROWS
        KEYPAD_PORT->OUT |= ROWS;
        KEYPAD_PORT->DIR |= BIT(4 + col);                        //SET COLUMN THREE TO OUTPUT
        KEYPAD_PORT->OUT &=~ BIT(4 + col);                       //SET COLUMN THREE TO LOW

        row = KEYPAD_PORT->IN & UNPRESSED;                       //READ ALL ROWS
        while (!(KEYPAD_PORT->IN & ROW0) | !(KEYPAD_PORT->IN & ROW1) | !( KEYPAD_PORT->IN & ROW2) | !( KEYPAD_PORT->IN & ROW3));

        if(row != UNPRESSED){
            //msDelay(20);                          //DELAY ACTS AS DEBOUNCE FOR BUTTONS
            if(row != UNPRESSED){                       //CHECK IF ANY BUTTON IN CURRENT COLUMN HAS BEEN PRESSED
                entry++;
                switch (col){
                    case 0: //LEFT COLUMN CHECK
                        if(row == (BUTTON_ONE  )) keyPress = 49; //BUTTON ONE WAS PRESSED
                        if(row == (BUTTON_FOUR )) keyPress = 52; //BUTTON FOUR WAS PRESSED
                        if(row == (BUTTON_SEVEN)) keyPress = 55; //BUTTON SEVEN WAS PRESSED
                        if(row == (STAR        )){               //ASTRISK WAS PRESSED
                            entry--;
                            keyPress = 42;
                            INPUT_TROUBLESHOOT();                //TROUBLESHOOT INPUT
                        }
                        break;
                    case 1: //MIDDLE COLUMN CHECK
                        if(row == (BUTTON_TWO  )) keyPress = 50; //BUTTON TWO WAS PRESSED
                        if(row == (BUTTON_FIVE )) keyPress = 53; //BUTTON FIVE WAS PRESSED
                        if(row == (BUTTON_EIGHT)) keyPress = 56; //BUTTON EIGHT WAS PRESSED
                        if(row == (BUTTON_ZERO )) keyPress = 48; //BUTTON ZERO WAS PRESSED
                        break;
                    case 2: //RIGHT COLUMN CHECK
                        if(row == (BUTTON_THREE)) keyPress = 51; //BUTTON THREE WAS PRESSED
                        if(row == (BUTTON_SIX  )) keyPress = 54; //BUTTON SIX WAS PRESSED
                        if(row == (BUTTON_NINE )) keyPress = 57; //BUTTON NINE WAS PRESSED
                        if(row == (POUND       )){               //POUND KEY WAS PRESSED
                            entry--;
                            keyPress = 35;
                            INPUT_TROUBLESHOOT();                //TROUBLESHOOT INPUT
                        }
                        break;
                }
                break;
            }else{}
        }else{}

    }

    KEYPAD_PORT->DIR = 0;                                        //SET COLUMNS TO INPUTS
    if(col == 3) return 0;                                       //CHECK TO SEE IF THERE WAS NOTHING PRESSED
    return 1;
}

//int READ_KEYPAD_2(void){
//    uint8_t col, row;
//    keyPress = 0;
//    for (col = 0; col < 3; col++ ){
//        KEYPAD_PORT->DIR = 0x00;                                 //SET COLUMNS AND ROWS TO INPUTS
//        KEYPAD_PORT->REN |= ROWS;                                //ENABLE PULL UP RESISTOR FOR ROWS
//        KEYPAD_PORT->OUT |= ROWS;
//        KEYPAD_PORT->DIR |= BIT(4 + col);                        //SET COLUMN THREE TO OUTPUT
//        KEYPAD_PORT->OUT &=~ BIT(4 + col);                       //SET COLUMN THREE TO LOW
//
//        row = KEYPAD_PORT->IN & UNPRESSED;                       //READ ALL ROWS
//        while (!(KEYPAD_PORT->IN & BIT0) | !(KEYPAD_PORT->IN & BIT1) | !( KEYPAD_PORT->IN & BIT2) | !( KEYPAD_PORT->IN & BIT3));
//
//        if(row != UNPRESSED){
//            msDelay(20);                                //DELAY ACTS AS DEBOUNCE FOR BUTTONS
//            if(row != UNPRESSED){                       //CHECK IF ANY BUTTON IN CURRENT COLUMN HAS BEEN PRESSED
//                entry++;
//                switch (col){
//                    case 0: //LEFT COLUMN CHECK
//                        if(row == (BUTTON_ONE  )) keyPress = 49; //BUTTON ONE WAS PRESSED
//                        if(row == (BUTTON_FOUR )) keyPress = 52; //BUTTON FOUR WAS PRESSED
//                        if(row == (BUTTON_SEVEN)) keyPress = 55; //BUTTON SEVEN WAS PRESSED
//                        if(row == (STAR        )){               //ASTERISK WAS PRESSED
//                            keyPress = 42;
//                            entry--;
//                            INPUT_TROUBLESHOOT();                //TROUBLESHOOT INPUT
//                        }
//                        break;
//                    case 1: //MIDDLE COLUMN CHECK
//                        if(row == (BUTTON_TWO  )) keyPress = 50; //BUTTON TWO WAS PRESSED
//                        if(row == (BUTTON_FIVE )) keyPress = 53; //BUTTON FIVE WAS PRESSED
//                        if(row == (BUTTON_EIGHT)) keyPress = 56; //BUTTON EIGHT WAS PRESSED
//                        if(row == (BUTTON_ZERO )) keyPress = 48; //BUTTON ZERO WAS PRESSED
//                        break;
//                    case 2: //RIGHT COLUMN CHECK
//                        if(row == (BUTTON_THREE)) keyPress = 51; //BUTTON THREE WAS PRESSED
//                        if(row == (BUTTON_SIX  )) keyPress = 54; //BUTTON SIX WAS PRESSED
//                        if(row == (BUTTON_NINE )) keyPress = 57; //BUTTON NINE WAS PRESSED
//                        if(row == (POUND       )){               //POUND KEY WAS PRESSED
//                            entry--;
//                            keyPress = 35;
//                            INPUT_TROUBLESHOOT();                //TROUBLESHOOT INPUT
//                        }
//                        break;
//                }
//                break;
//            }else{}
//        }else{}
//    }
//
//    KEYPAD_PORT->DIR = 0;                                                 //SET COLUMNS TO INPUTS
//    if(col == 3) return 0;                                       //CHECK TO SEE IF THERE WAS NOTHING PRESSED
//    return 1;
//}

///*---------CHECK IF GIVEN BUTTON IS PRESSED--------*/
//int CHECK(uint8_t BUTTON){
//    do{
//        msDelay(25);
//        if((BUTTON_PORT->IN & BUTTON) == 0){            //CHECK TO SEE IF THE BUTTON IS PRESSED
//            pressed = 1;                                //(IF BUTTON WAS PRESSED) FLAG THAT BUTTON WAS PRESSED
//            if((BUTTON_PORT->IN & BUTTON) == 0){        //CHECK IF THE BUTTON IS STILL PRESSED
//                pressed = 1;                            //CONFIRM BUTTON PRESSED
//            }else{
//                pressed = 0;
//            }
//        }else{
//        }
//    }while((BUTTON_PORT->IN & BUTTON) == 0);
//
//    return pressed;
//}

/*----------PRINT KEY PRESSES FUNCTION------------*/
void PRINT_KEYS(char val){
    printf("%c", val);                       //PRINT KEYPAD INPUTS TO THE CONSOLE
    fflush(stdout);
}

/*--------------TROUBLESHOOT INPUT----------------*/
void INPUT_TROUBLESHOOT(void){
    if((keyPress == '*') || (keyPress == '#')){
        if(keyPress == '*'){                            //'*' IS NOT AN ALLOWED INPUT
            //printf("\nINVALID INPUT\n");
        }
        if((entry < 3) && (keyPress == '#')){           //PIN CANNOT BE LESS THAN 4 DIGITS
            //printf("\nPLEASE ENTER A FOUR DIGIT PIN CODE\n");
        }
        if(/*(entry > 3) && */(keyPress == '#')){           //'#' DETERMINES TERMINATION OF PIN INPUT
            terminate = 1;
        }
    }
}

/*---------------PIN CONFIRMATION-----------------*/
//void confirm_PIN(void){
//    PIN2[0] = 0;                                        //RESET PIN2
//    PIN2[1] = 0;
//    PIN2[2] = 0;
//    PIN2[3] = 0;
//
//    terminate = 0;                                      //RESET TERMINATE VARIABLE
//    char val;
//    if(flag == 0){                                      //CONFIRM THE PIN SUBMITTED TO THE USER
//        //printf("\nYour PIN has been successfully stored\n");
//        //printf("PIN: %c%c%c%c\n", PIN[0], PIN[1], PIN[2], PIN[3]);
//        flag = 1;
//        entry = 0;                                      //RESET ENTRY COUNT
//    }
////    printf("\nPlease enter a 4 digit pin code to confirm your PIN followed\n");
////    printf("by the # key to determine you have terminated the entry\n");
////    printf("The program will only take the last four digits\n");
////    printf("of the string of numbers you enter\n");
////    printf("\nCONFIRM PIN:\n");
//    while(1){
//        if(terminate != 1){
//            READ_KEYPAD();                              //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
//        }else{
//            if(input[0] == PIN2[0]){                      //CHECK FIRST DIGIT IN THE PIN
//                if(input[1] == PIN2[1]){                  //CHECK SECOND DIGIT IN THE PIN
//                    if(input[2] == PIN2[2]){              //CHECK THIRD DIGIT IN THE PIN
//                        if(input[3] == PIN2[3]){          //CHECK FOURTH DIGIT IN THE PIN
//                            //PIN_VALIDATED(PIN2);        //DISPLAY PIN CONFIRMATION WITH USER
//                        }
//                    }
//                }
//            }else{
//                //printf("\nINCORRECT PIN. TRY AGAIN\n"); //CHECK FOR INVALID INPUT
//                terminate = 0;
//                confirm_PIN();
//            }
//        }
//
//        val = keyPress;
//        if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
//            print_Keys(val);
//            INPUT(PIN2);                                  //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
//        }
//        msDelay(50);                              //DELAY
//    }
//}

///*-----------------VALIDATE PIN-------------------*/
//void PIN_VALIDATED(char PIN2[]){
//    printf("\n\nYour PIN has been successfully confirmed\n");
//    printf("PIN: %c%c%c%c\n", PIN2[0], PIN2[1], PIN2[2], PIN2[3]);
//    exit(1);
//}
