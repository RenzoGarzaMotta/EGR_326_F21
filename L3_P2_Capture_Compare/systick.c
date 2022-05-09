/*Lab 3 - Input Interfacing with the MSP432 using interrupts
 *Part 2 - Controlling the LED lighting sequence using a timer interrupt and two pushbutton interrupts
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
 *  This part will use a second pushbutton in combination with the first to flash the selected LED on
 *  and off in 0.5 second intervals. Both the SysTick and each pushbutton  should be triggered using
 *  interrupts.
 *  -Modify Part II code so that the first pushbutton will select the LED color. The second pushbutton
 *   is then used to turn that color LED on and off. (Both buttons should be triggered by interrupts.)
 *  -Select a color LED by sequencing through the colors with the first pushbutton. When the second
 *   pushbutton is pressed and held, the selected color LED flashed on and off every 0.5s. When released,
 *   the LED stays lit with the last selected color.
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
