/*Lab 9 - Speed Sensing using Hall efect Sensors
 *Part 1 - Hall Effect Sensor
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
 *  This part will use the MSP432 to detect the state of a Hall effect sensor
 *  If a magnet is near the sensor, the sensor voltage changes state
 *  from high to low.
 */

#include "msp.h"
#include <stdio.h>
#include "macros.h"
#include "systick.h"
#include "ST7735.h"

uint8_t status, clear;
uint16_t currentEdge, lastEdge, deltaCycles;
double micro;

void gpioSetup(void);
void setTrigger(void);
void sampleDistance(void);
void setupTimerA(void);

 void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    ST7735_InitR(INITR_REDTAB);                     //Initialize the ST7735 as Red Tab for better pixel fit.
    sysTickSetup();                                 //Set up SysTick timer
    gpioSetup();                                    //GPIO setup
    setupTimerA();                                  //Setup Timer A

    NVIC->ISER[0] = 1 << (TA2_N_IRQn & 31);         //Determine interrupt vector
    __enable_irq();                                 //Enable all interrupts

    while(1){
        if(status == 0){    //Check the status of the Hall Effect Sensor
            ST7735_DrawString(5,7,3,"OPEN", ST7735_WHITE);  //Print "OPEN" to the LCD if the Hall effect sensor is engaged.
        }else if(status == 1){  //Check the status of the Hall Effect Sensor
            ST7735_DrawString(2,7,3,"CLOSED", ST7735_WHITE);    //Print "CLOSED" to the LCD if the Hall effect sensor is engaged.
        }
        if(clear){  //Check if the "clear" flag is set
            ST7735_FillScreen(ST7735_BLACK);    //Clear the LCD
            clear = 0;  //Reset the "clear" flag
        }
    }
}

void TA2_N_IRQHandler(void){    //Enter the TA2 Interrupt handler on both rising and falling edges
    if(status == 0){    //Check the status of the Hall Effect Sensor flag
        status = 1;     //Toggle the status of the Hall Effect Sensor flag
        clear = 1;      //Set the clear flag when state of the Hall Effect Sensor changes from open to closed and vice versa
    }else if(status == 1){ //Check the stattus of the Hall Effect Sensor flag
        status = 0;     //Toogle the status of the Hall Effect Sensor flag
        clear = 1;      //Set the clear flag when state of the Hall Effect Sensor changes from open to closed and vice versa
    }
    currentEdge = TIMER_A2->CCR[1];                 //Grab timer at which interrupt was triggered from buffer
    if(lastEdge > currentEdge){                     //Check for overflow
        currentEdge = currentEdge + 65535 - 1;     //If overflow happens, add a full period to current edge to 'extend' the values
        deltaCycles = currentEdge - lastEdge;       //Determine the time difference since last occurrence
    }else{
        deltaCycles = currentEdge - lastEdge;       //Determine the time difference since last occurrence
    }
    lastEdge = currentEdge;                         //Set the new lastEdge

    //Calculate the distance from deltaTime using speed of sound at sea level.
    //T = (1/3MHz) = 0.333 (us/clock cycle)
    micro = deltaCycles * 0.333;  //sigTime units is microseconds (us)

    TIMER_A2->CCTL[2] &=~ TIMER_A_CCTLN_CCIFG;      //Clear TimerA0 interrupts
}

void setupTimerA(void){
    TIMER_A2->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__CONTINUOUS|      //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P5.6 Timer A
    TIMER_A2->CCTL[2] = (TIMER_A_CCTLN_CM__BOTH|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source
}

void gpioSetup(void){
    //Set up 5.6 as TimerA instances to measure the time it take the signal to return to the sensor
    HALL_PORT->SEL0 |= HALL;            //Set up Hall Effect Sensor as Primary Function.
    HALL_PORT->SEL1 &=~ HALL;
    HALL_PORT->DIR &=~ HALL;            //Set up Hall Effect Sensor as Input
}


