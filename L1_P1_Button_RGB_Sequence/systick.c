/*
 * systick.c
 *
 *  Created on: Sep 8, 2021
 *      Author: renzogarzamotta
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
    SysTick->LOAD = SYS_MAX;        //Load the timer to its max of 0x00FFFFFF
    SysTick->VAL = 0;               //
    SysTick->CTRL = SYS_SETUP_INT;  //Enable Systick with an interrupt
}

void msDelay(uint16_t time){    //Function uses SysTick as a delay
    SysTick->LOAD = (3000 * time) - 1;  //Load value to SysTick
    SysTick->VAL = 0;   //Clear SysTick value register
    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
}

void usDelay(uint16_t time){    //Function uses SysTick as a delay
    SysTick->LOAD = (3 * time) - 1;  //Load value to SysTick
    SysTick->VAL = 0;   //Clear SysTick value register
    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
}
