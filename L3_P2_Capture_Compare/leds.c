/*Lab 3 - Input Interfacing with the MSP432 using interrupts
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
#include "macros.h"
#include "leds.h"

extern double distCentimeter;       //Use distCentimeter to determine cm distance from the sensor.
extern uint8_t toggleFlag;          //Get toggleFlag from main
extern uint16_t period;             //Get period variable from main

uint8_t fwdSequenceState = red;     //Set initial forward state as red
uint8_t bckSequenceState = blue;    //Set initial backward state as blue

void turnOn(uint8_t led){
    LED_PORT->OUT |= led;   //Turn on LED
}

void turnOff(uint8_t led){
    LED_PORT->OUT &=~ led;  //Turn off LED
}

void toggleLED(uint8_t led){
    LED_PORT->OUT ^= led;   //Toggle LED
}

void distanceBlinkerControl(void){
    //Check for distance range to change state of LED.
    if(distCentimeter > 25.4){
        //LED off
        TIMER_A2->CCR[1] = period * 0;
        toggleFlag = 0;
    }else if((25.4 >= distCentimeter) && (distCentimeter > 22.86)){
        //10% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.1;
        toggleFlag = 0;
    }else if((22.86 >= distCentimeter) && (distCentimeter > 20.32)){
        //20% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.2;
        toggleFlag = 0;
    }else if((20.32 >= distCentimeter) && (distCentimeter > 17.78)){
        //30% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.3;
        toggleFlag = 0;
    }else if((17.78 >= distCentimeter) && (distCentimeter > 15.24)){
        //40% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.4;
        toggleFlag = 0;
    }else if((15.24 >= distCentimeter) && (distCentimeter > 12.7)){
        //50% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.5;
        toggleFlag = 0;
    }else if((12.7 >= distCentimeter) && (distCentimeter > 10.16)){
        //60% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.6;
        toggleFlag = 0;
    }else if((10.16 >= distCentimeter) && (distCentimeter > 7.62)){
        //70% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.7;
        toggleFlag = 0;
    }else if((7.62 >= distCentimeter) && (distCentimeter > 5.08)){
        //80% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.8;
        toggleFlag = 0;
    }else if((5.08 >= distCentimeter) && (distCentimeter > 2.54)){
        //90% Duty Cycle
        TIMER_A2->CCR[1] = period * 0.9;
        toggleFlag = 0;
    }else if(2.54 >= distCentimeter){
        //100% Duty Cycle Blinking at 2Hz
        toggleFlag = 1;
    }
}
