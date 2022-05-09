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

#include "msp.h"
#include "systick.h"

void sysTickSetup(void){
    //Program the value in the STRVR register
    //Clear the STCVR register by writing to it with any value
    //Configure the STCSR register for the required operation.
    SysTick->CTRL = 0;              //
    SysTick->LOAD = SYS_MAX;        //Load the timer to its max of 0x00FFFFFF
    SysTick->VAL = 0;               //
    SysTick->CTRL = SYS_SETUP;      //Enable SysTick without an interrupt
}

void intSysTickSetup(void){
    //Program the value in the STRVR register
    //Clear the STCVR register by writing to it with any value
    //Configure the STCSR register for the required operation.
    SysTick->CTRL = 0;                       //Disable SysTick for setup
    SysTick->LOAD = (3000 * 500) - 1;        //Load the timer to 500 to account for the 2Hz signal required for this lab
    SysTick->VAL = 0;                        //Any write clears the register
    SysTick->CTRL = SYS_SETUP_INT;           //Enable SysTick with an interrupt
}

void msDelay(uint16_t time){    //Function uses SysTick as a delay
    SysTick->LOAD = (3000 * time) - 1;  //Load value to SysTick
    SysTick->VAL = 0;   //Clear SysTick value register
    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
}

void usDelay(uint32_t time){    //Function uses SysTick as a delay
    SysTick->LOAD = (3 * time) - 1;  //Load value to SysTick
    SysTick->VAL = 0;   //Clear SysTick value register
    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
}
