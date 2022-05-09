/*
 * leds.c
 *
 *  Created on: Sep 8, 2021
 *      Author: renzogarzamotta
 */

#include "msp.h"
#include "macros.h"
#include "leds.h"

uint8_t fwdSequenceState = red;     //Set initial forward state as red

void turnOn(uint8_t led){
    LED_PORT->OUT |= led;   //Turn on LED
}

void turnOff(uint8_t led){
    LED_PORT->OUT &=~ led;  //Turn off LED
}

void toggleLED(uint8_t led){
    LED_PORT->OUT ^= led;   //Toggle LED
}

void sequenceControl(void){ //Forward sequence function
    turnOff(LED_RED);       //Turn off all LEDs
    turnOff(LED_GREEN);
    turnOff(LED_BLUE);
    switch(fwdSequenceState){   //Check for the forward state
        case red:
            turnOn(LED_RED);            //Turn on red LED
            turnOff(LED_GREEN);
            turnOff(LED_BLUE);
            fwdSequenceState = green;   //Set next state for forward sequence
            break;
        case green:
            turnOff(LED_RED);
            turnOn(LED_GREEN);          //Turn on green LED
            turnOff(LED_BLUE);
            fwdSequenceState = blue;    //Set next state for forward sequence
            break;
        case blue:
            turnOff(LED_RED);
            turnOff(LED_GREEN);
            turnOn(LED_BLUE);           //Turn on blue LED
            fwdSequenceState = red;     //Set next state for forward sequence
            break;
        default:
            fwdSequenceState = red;     //Set default state
            break;
    }
}
