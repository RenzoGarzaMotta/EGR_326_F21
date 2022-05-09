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
