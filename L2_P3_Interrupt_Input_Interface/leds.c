/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 3 - Controlling the LED lighting sequence using a timer interrupt and two pushbutton interrupts
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
 *  -Modify Part II code so that the first pushbutton will select the LED colot. The second pushbutton
 *   is then used to turn that color LED on and off. (Both buttons should be triggered by interrupts.)
 *  -Selectr a color LED by sequencing through the colors with the first pushbutton. When the second
 *   pushbutton is pressed and held, the selected colot LED flashed on and off every 0.5s. When released,
 *   the LED stays lit with the last selected color.
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
    fwdSequenceState++;
    if(fwdSequenceState > 2) fwdSequenceState = red;
    turnOff(LED_RED);       //Turn off all LEDs
    turnOff(LED_GREEN);
    turnOff(LED_BLUE);
    switch(fwdSequenceState){   //Check for the forward state
        case red:
            turnOn(LED_RED);            //Turn on red LED
            turnOff(LED_GREEN);
            turnOff(LED_BLUE);
//            fwdSequenceState = green;   //Set next state for forward sequence
//            bckSequenceState = blue;    //Set next state for backward sequence
            break;
        case green:
            turnOff(LED_RED);
            turnOn(LED_GREEN);          //Turn on green LED
            turnOff(LED_BLUE);
//            fwdSequenceState = blue;    //Set next state for forward sequence
//            bckSequenceState = red;     //Set next state for backward sequence
            break;
        case blue:
            turnOff(LED_RED);
            turnOff(LED_GREEN);
            turnOn(LED_BLUE);           //Turn on blue LED
//            fwdSequenceState = red;     //Set next state for forward sequence
//            bckSequenceState = green;   //Set next state for backward sequence
            break;
        default:
//            fwdSequenceState = red;     //Set default state
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
