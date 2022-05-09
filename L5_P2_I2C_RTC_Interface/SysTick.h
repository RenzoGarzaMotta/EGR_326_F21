/****************************************************************
*                     SYSTICK HEADER FILE                       *
*                                                               *
*THIS HEADER FILE CONTAINS ALL NEEDED COMPONENTS TO INITIALIZE  *
*AND DEFINE DELAY FUNCTIONS THAT TAKE IN ARGUMENTS IN DIFFERENT *
*UNITS (MILLISECONDS, MICROSECONDS, AND NANOSECONDS).           *
*THIS CODE IS SPECIFICALLY MADE TO RUN IN TI's MSP432.          *
*ADD THIS SOURCE FILE ALONG WITH SYSTICK.C TO THE PROJECT FILE  *
*THAT YOU DESIRE TO USE THE TIMER IN.                           *
*                                                               *
*NOTE:                                                          *
*    THIS SOURCE FILE IS NOT MEANT TO BE CUSTOMIZABLE.          *
*                                                               *
*INDEX:                                                         *
*    -MACROS DEFINITION                                         *
*    -PROTOTYPE DEFINITION                                      *
*                                                               *
*AUTHOR: RENZO GARZA MOTTA                                      *
*DATE: NOV 12TH, 2019                                           *
*****************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H
#include "msp.h"

/****************************************************************
*                     MACROS DEFINITION                         *
****************************************************************/
#define SYS_FLAG                        0x00010000      //SYSTICK TIMER FLAG
#define MAX_LOAD                        0x00FFFFFF      //SYSTICK MAX LOAD

#define INTERRUPT                       0x00000007      //SYSTICK WITH INTERRUPT
#define NO_INTERRUPT                    0x00000005      //SYSTICK WITHOUT INTERRUPT


/****************************************************************
*                      PROTOTYPE DEFINITION                     *
****************************************************************/
void sysTick_init(void);
void sysTickInterrupt_init(void);
void msDelay(uint16_t delay);
void usDelay(uint16_t delay);

#endif // SYSTICK_H
