/****************************************************************
*                     SYSTICK SOURCE FILE                       *
*                                                               *
*THIS SOURCE FILE CONTAINS ALL NEEDED COMPONENTS TO INITIALIZE  *
*AND DEFINE DELAY FUNCTIONS THAT TAKE IN ARGUMENTS IN DIFFERENT *
*UNITS (MILLISECONDS, MICROSECONDS, AND NANOSECONDS).           *
*THIS CODE IS SPECIFICALLY MADE TO RUN IN TI's MSP432.          *
*ADD THIS SOURCE FILE ALONG WITH SYSTICK.H TO THE PROJECT FILE  *
*THAT YOU DESIRE TO USE THE TIMER IN.                           *
*                                                               *
*NOTE:                                                          *
*    THIS SOURCE FILE IS NOT MEANT TO BE CUSTOMIZABLE.          *
*                                                               *
*INDEX:                                                         *
*    -PREPROCESSORS                                             *
*    -SYSTICK_INIT                                              *
*    -SYSTICK_INTERRUPT_INIT                                    *
*    -msDelay                                                   *
*    -usDelay                                                   *
*    -nsDelay                                                   *
*                                                               *
*AUTHOR: RENZO GARZA MOTTA                                      *
*DATE: NOV 12TH, 2019                                           *
****************************************************************/

/****************************************************************
*                          PREPROCESSORS                        *
****************************************************************/
#include "SysTick.h"

/****************************************************************
*                          SYSTICK_INIT                         *
*THIS FUNCTION INITIALIZES THE SYSTICK TIMER WITHOUT INTERRUPT  *
****************************************************************/
void sysTick_init(void){
    SysTick->CTRL = 0;                                    //DISABLE SYSTICK DURING STEP
    SysTick->LOAD = MAX_LOAD;                             //MAX RELOAD VALUE
    SysTick->VAL = 0;                                     //ANY WRITE TO CURRENT CLEARS IT
    SysTick->CTRL = NO_INTERRUPT;                         //ENABLE SYSTICK
}

/****************************************************************
*                      SYSTICK_INTERRUPT_INIT                   *
*THIS FUNCTION INITIALIZES THE SYSTICK TIMER WITH AN INTERRUPT  *
****************************************************************/
void sysTickInterrupt_init(void){
    SysTick->CTRL = 0;                                    //DISABLE SYSTICK DURING STEP
    SysTick->LOAD = (2000 * 3000) - 1;                     //Load initial delay
    SysTick->VAL = 0;                                     //ANY WRITE TO CURRENT CLEARS IT
    SysTick->CTRL = INTERRUPT;                            //ENABLE SYSTICK WITH AN INTERRUPT
}

/****************************************************************
*                           msDelay                             *
*    THIS FUNCTION USES SYSTICK TIMER AS A DELAY THAT TAKES IN  *
*AN ARGUMENT IN MILLISECONDS FOR THE DELAY AMOUNT               *
****************************************************************/
 void msDelay(uint16_t delay){
     SysTick->LOAD = ((delay * 3000) - 1);                 //DELAY FOR 1ms PER DELAY VALUE
     SysTick->VAL = 0;                                     //ANY WRITE TO CURRENT CLEARS IT
     while((SysTick->CTRL & SYS_FLAG) == 0);               //WAIT FOR FLAG TO SET
 }

/****************************************************************
*                           usDelay                             *
*    THIS FUNCTION USES SYSTICK TIMER AS A DELAY THAT TAKES IN  *
*AN ARGUMENT IN MICROSECONDS FOR THE DELAY AMOUNT               *
****************************************************************/
void usDelay(uint16_t delay){
    SysTick->LOAD = ((delay * 3) - 1);                    //DELAY FOR 1us PER DELAY VALUE
    SysTick->VAL = 0;                                     //ANY WRITE TO CURRENT CLEARS IT
    while((SysTick->CTRL & SYS_FLAG) == 0);               //WAIT FOR FLAG TO SET
}

/****************************************************************
*                           nsDelay                             *
*    THIS FUNCTION USES SYSTICK TIMER AS A DELAY THAT TAKES IN  *
*AN ARGUMENT IN NANOSECONDS FOR THE DELAY AMOUNT                *
****************************************************************/
void nsDelay(uint16_t delay){
    SysTick->LOAD = ((delay * 0.003) - 1);                //DELAY FOR 1ns PER DELAY VALUE
    SysTick->VAL = 0;                                     //ANY WRITE TO CURRENT CLEARS IT
    while((SysTick->CTRL & SYS_FLAG) == 0);               //WAIT FOR FLAG TO SET
}
