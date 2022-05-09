/*Lab 1 - Input Interfacing with the MSP432 using interrupts
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
 *  MSP432 interfaces with external inputs (pushbuttons) to control an external LED.
 */


#include "msp.h"
#include <math.h>
#include "systick.h"
#include "macros.h"
#include "leds.h"

void gpioSetup(void);           //Set-up gpio
uint8_t debounceBtn1(void);     //Debounce function for button1

extern uint8_t fwdSequenceState;     //Set initial forward state as red
extern uint8_t bckSequenceState;    //Set initial backward state as blue

uint8_t btn1Flag = 0;           //Flag to indicate button1 has been pressed

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    gpioSetup();        //Call function to set up system gpio
    sysTickSetup();     //Call function to set up SysTick timer
    NVIC->ISER[1] = 1 << (PORT4_IRQn & 31);     //Set interrupts
    __enable_irq();     //Enable all interrupts

    while(1){
        if(btn1Flag){                   //Check if button1 has been pressed
            sequenceControl();          //Call normal sequence control for the leds
            btn1Flag = 0;               //Reset button1 flag
        }
    }
}

void PORT4_IRQHandler(void){    //Port 4 interrupt handler
    int i;
    if(BTN_PORT->IFG & BTN1){           //Check if button1 has been pressed
        for(i = 0; i < 75; i++){       //Cycle through for-loop to check state of button bounce
            if(debounceBtn1()){         //Check that button1 has been pressed
                btn1Flag = 1;           //Set button1 flag to indicate that button1 was pressed
            }
        }
        BTN_PORT->IFG &=~ BTN1;         //Clear button1 interrupt flag
    }
}

void gpioSetup(void){
    LED_PORT->SEL0 &=~ LEDS|BTN1;   //Set LEDs' and buttons' pins as GPIO
    LED_PORT->SEL1 &=~ LEDS|BTN1;

    LED_PORT->DIR |= LEDS;          //Set LEDs as outputs
    LED_PORT->OUT &=~ LEDS;         //Set initial LED status as OFF

    BTN_PORT->DIR &=~ BTN1;         //Set buttons as inputs
    BTN_PORT->REN |= BTN1;          //Enable MSP432 internal resistor
    BTN_PORT->OUT |= BTN1;          //Set internal resistor to pull-up
    BTN_PORT->IES |= BTN1;          //Set interrupt edge from high to low
    BTN_PORT->IE  |= BTN1;          //Enable interrupt
    BTN_PORT->IFG = 0;              //Clear interrupt flag
}

uint8_t debounceBtn1(void){         //Debounce function for button1
    static uint16_t State1 = 0;     //Current debounce status
    State1 = (State1 << 1) | (BTN_PORT->IN & BTN1)>>1 | 0x8000;     //Shift state to indicate bounce
    msDelay(1);
    if(State1==0x8000)return 1;     //indicates 0 level is
    return 0;
}
