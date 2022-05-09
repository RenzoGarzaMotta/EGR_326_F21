/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 1 - Controlling the blink rate of an LED using pushbutton switches
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
 *  This part will drive an external LED interfaced to a port pin on the MSP432. Also, two external
 *  pushbuttons are required to interface with other port pins on the MSP432. These pushbuttons
 *  will control the blink rate of the LED using interrupts
 *
 *  The first button will start the LED to blink at 2 sec intervals (one second on, one second off).
 *  Each press of the button will double the blink rate.
 *  The seond button will stop the LED from blinking (if the blink rate is 0.5 Hz), or reduce the
 *  blink rate by a factor of 2 if the blink rate is faster than 0.5 Hz.
 *
 *  Holding down a button will not change the blink rate more than pressing and releasing a button.
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
