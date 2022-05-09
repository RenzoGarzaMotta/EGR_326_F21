/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 2 - Sequencing colors of an RGB LED using a timer interrupt and pushbutton
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
 *  This part controls the RGB LED sequence using a timer peripheral in the MSP432.
 *  -Use the three on-board RGB LEDs with one external pushbutton.
 *  -The pushbutton should be triggered by interrupts.
 *  -On RESET, the LEDs should be OFF.
 *  -When the pushbutton is press and held down the RGB LED should follow the sequence
 *   RED->GREEN->BLUE with a 1sec On-Time for each color (use SysTick).
 *  -This sequence should repeat as long as the pushbutton is depressed. When the
 *   pushbutton is released the sequencing pauses with the current LED remaining ON.
 *  -Upon the next press of the pushbutton, the LED sequence will start up again in
 *   reverse order and continue for as long as the pushbutton is held down.
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
    SysTick->CTRL = SYS_SETUP;      //Enable Systick without an interrupt
}

void intSysTickSetup(void){
    //Program the value in the STRVR register
    //Clear the STCVR register by writing to it with any value
    //Configure the STCSR register for the required operation.
    SysTick->CTRL = 0;              //
    SysTick->LOAD = (3000 * 1000) - 1;        //Load the timer to its max of 0x00FFFFFF
    SysTick->VAL = 0;               //
    SysTick->CTRL = SYS_SETUP_INT;  //Enable Systick with an interrupt
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
