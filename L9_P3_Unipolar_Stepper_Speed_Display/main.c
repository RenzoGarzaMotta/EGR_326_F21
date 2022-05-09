/*Lab 9 - Speed Sensing using Hall efect Sensors
 *Part 2 - Unipolar Stepper Speed Control
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
 *  This part will use the MSP432 to control the speed of a unipolar
 *  motor through a Serial interface with the console.
 */

#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unipolarMotor.h"
#include "macros.h"
#include "systick.h"
#include "ST7735.h"

extern uint8_t in_1[];      //Declare external motor sequence
extern uint8_t in_2[];      //Declare external motor sequence
extern uint8_t in_3[];      //Declare external motor sequence
extern uint8_t in_4[];      //Declare external motor sequence

uint8_t printFlag, toggleFlag, update, clear, doneCounting, state;
char rpm[3];
uint16_t currentEdge, lastEdge, deltaCycles, period = 1000;
uint16_t steps = 64, delay = 16;     //42ms delay per step is the slowest TimerA can accurately calculate rpms. This is due to overflow of the timer altough two /8 dividers are being used.
double rots, micro, sum, temp;
unsigned int i = 0, j = 100000, s = 0;
int rotCount;

void gpioSetup(void);
void setTrigger(void);
void sampleDistance(void);
void setupTimerA(void);

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    ST7735_InitR(INITR_REDTAB);     //Initialize the ST7735 as Red Tab for better pixel fit.
    intSysTickSetup();              //Set up SysTick timer
    initializeUnipolarMotor();      //Initialize Unipolar Motor
    gpioSetup();                    //GPIO setup
    setupTimerA();                  //Setup Timer A

    NVIC->ISER[0] = 1 << (TA3_N_IRQn & 31); //Determine interrupt vector
    __enable_irq(); //Enable all interrupts

    while(1){
        if(update){
            sprintf(rpm, "%d", (int)rots);              //Store rotarions per minute variable into a string
            ST7735_FillScreen(ST7735_BLACK);            //Clear the LCD
            ST7735_DrawString(7,7,3,rpm, ST7735_WHITE); //Update the RPM reading on the LCD
            update = 0;
        }
    }
}

void TA3_N_IRQHandler(void){
    rots = 0;
    rotCount++;
    currentEdge = TIMER_A3->CCR[2];                 //Grab timer at which interrupt was triggered from buffer
    if(lastEdge > currentEdge){                     //Check for overflow
//        currentEdge = currentEdge + 65534;     //If overflow happens, add a full period to current edge to 'extend' the values
        deltaCycles = currentEdge - lastEdge;       //Determine the time difference since last occurrence
    }else{
        deltaCycles = currentEdge - lastEdge;       //Determine the time difference since last occurrence
    }
    lastEdge = currentEdge;                         //Set the new lastEdge

    //Calculate the distance from deltaTime using speed of sound at sea level.
    //T = (1/3MHz) = 0.333 (us/clock cycle)
    micro = deltaCycles * 0.333;  //sigTime units is microseconds (us)
    if((rotCount % 2) == 0){
        rots = (1/ micro) * (1000) * (1000);
        update = 1;
    }

    TIMER_A3->CCTL[2] &=~ TIMER_A_CCTLN_CCIFG;      //Clear TimerA0 interrupts
}

void SysTick_Handler(void){
    unsigned int freqScalar, sysLoad;
    unsigned int count = 0, count1 = 0;
    unsigned int lim;

    if(SYS_CLK == 48000000){//     <48000000> 48 MHz
        if(doneCounting){
            if(s < steps){
                UNI_PORT->OUT  &=~ UNI_MOTOR;                       //Turn off output
                UNI_PORT->OUT  |= (in_1[s%SEQ_SIZE] << 4 & INT_1);  //Set output of A'
                UNI_PORT->OUT  |= (in_2[s%SEQ_SIZE] << 5 & INT_2);  //Set output of A
                UNI_PORT->OUT  |= (in_3[s%SEQ_SIZE] << 6 & INT_3);  //Set output of B'
                UNI_PORT->OUT  |= (in_4[s%SEQ_SIZE] << 7 & INT_4);  //Set output of B
                s++;
            }else{
                s = 0;  //Reset steps counter
            }
            doneCounting = 0;   //Clear doneCounting flag to set interrupt delay
        }else{
            freqScalar = (SYS_CLK / 1000);      //Calculate the frequency scalar for SysTick Timer
            sysLoad = (freqScalar * delay);     //Calculate the load for SysTick
            lim = (sysLoad / 1000000) % 100;    //Calculate the SysTick Timer limit of ~16e6
            switch(state){
                case 1:
                    if(lim > 16){               //Check if the load surpasses the limit
                        if(i < (sysLoad/1000000)){      //Check if the load can be divided into 1e6 blocks
                            count = (1 * 1000000);      //Calculate load blocks of 1e6
                            SysTick->LOAD = count;      //Load value to SysTick
                            SysTick->VAL = 0;           //Clear SysTick value register
                            i++;                        //Increase block counts
                        }else{
                            state = 2;                  //Move unto the next stage of SysTick loads
                        }
                    }else{  //Check if the load is smaller than the SysTick timer limit
                        SysTick->LOAD = (freqScalar * delay) - 1;  //Load value to SysTick
                        SysTick->VAL = 0;                          //Clear SysTick value register
                        doneCounting = 1;                          //Set the doneCounting flag
                        state = 1;                                 //Remain in the same state.
                    }
                    break;
                case 2:
                    count = 0;

                    if(lim > 16){               //Check if the load surpasses the limit
                        if((count1 + ((sysLoad / 1000000) * 1000000)) != sysLoad){  //Check if
                            count = 0;  //reset count
                            if((sysLoad / j) % 10){
                                lim = (sysLoad / j) % 10;
                                for(i = 0; i < lim; i++){
                                    count = count + (1 * j);
                                }
                                count1 += count;
                            }
                            if(count > 0){  //Check if count is greater  zero
                                SysTick->LOAD = count;  //Load value to SysTick
                                SysTick->VAL = 0;   //Clear SysTick value register
                            }else{
                                doneCounting = 1;   //If done counting set the flag
                                state = 1;          //Reset state back to 1
                            }
                            j = j/10;       //Reduce j count by 1/10th
                        }
                    }
                    break;
                default:
                    state = 1;
                    break;
            }
        }
    }

}

void setupTimerA(void){
    TIMER_A3->EX0 = TIMER_A_EX0_IDEX__8;

    TIMER_A3->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__CONTINUOUS|      //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__8|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P2.5 Timer A
    TIMER_A3->CCTL[2] = (TIMER_A_CCTLN_CM__FALLING|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source
}

void gpioSetup(void){
    //Set up 2.5 as TimerA instances to measure the time it take the signal to return to the sensor
    HALL_PORT->SEL0 |= HALL;
    HALL_PORT->SEL1 &=~ HALL;
    HALL_PORT->DIR &=~ HALL;
}
