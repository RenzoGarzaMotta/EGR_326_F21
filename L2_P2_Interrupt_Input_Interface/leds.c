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
#include "macros.h"
#include "leds.h"

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
            bckSequenceState = blue;    //Set next state for backward sequence
            break;
        case green:
            turnOff(LED_RED);
            turnOn(LED_GREEN);          //Turn on green LED
            turnOff(LED_BLUE);
            fwdSequenceState = blue;    //Set next state for forward sequence
            bckSequenceState = red;     //Set next state for backward sequence
            break;
        case blue:
            turnOff(LED_RED);
            turnOff(LED_GREEN);
            turnOn(LED_BLUE);           //Turn on blue LED
            fwdSequenceState = red;     //Set next state for forward sequence
            bckSequenceState = green;   //Set next state for backward sequence
            break;
        default:
            fwdSequenceState = red;     //Set default state
            break;
    }
}

void reverseSequenceControl(void){
    turnOff(LED_RED);       //Turn off all LEDs
    turnOff(LED_GREEN);
    turnOff(LED_BLUE);
    switch(bckSequenceState){   //Check for the forward state
        case blue:
            turnOff(LED_RED);
            turnOff(LED_GREEN);
            turnOn(LED_BLUE);           //Turn on blue LED
            fwdSequenceState = red;     //Set next state for forward sequence
            bckSequenceState = green;   //Set next state for backward sequence
            break;
        case green:
            turnOff(LED_RED);
            turnOn(LED_GREEN);          //Turn on green LED
            turnOff(LED_BLUE);
            fwdSequenceState = blue;    //Set next state for forward sequence
            bckSequenceState = red;     //Set next state for backward sequence
            break;
        case red:
            turnOn(LED_RED);            //Turn on red LED
            turnOff(LED_GREEN);
            turnOff(LED_BLUE);
            fwdSequenceState = green;   //Set next state for forward sequence
            bckSequenceState = blue;    //Set next state for backward sequence
            break;
        default:
            bckSequenceState = blue;    //Set default state
            break;
    }
}
