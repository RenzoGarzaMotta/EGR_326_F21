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
#include <math.h>
#include "systick.h"
#include "macros.h"
#include "leds.h"

void gpioSetup(void);               //Set-up gpio
uint8_t debounceBtn1(void);         //Debounce function for button1
uint8_t debounceBtn2(void);         //Debounce function for button2

extern uint8_t fwdSequenceState;    //Set initial forward state as red
extern uint8_t bckSequenceState;    //Set initial backward state as blue

uint8_t btn1Flag = 0;               //Flag to indicate button1 has been pressed
uint8_t btn2Flag = 0;               //Flag to indicate button2 has been pressed
uint8_t bothBtnFlag = 0;            //Flag to indicate that both buttons have been pressed

int clkCount = 0;

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    gpioSetup();                                    //Call function to set up system gpio
    intSysTickSetup();                                 //Call function to set up SysTick timer


    NVIC->ISER[1] = 1 << (PORT4_IRQn & 31);         //Set interrupts
    __enable_irq();                                 //Enable all interrupts

    while(1){

    }
}

void PORT4_IRQHandler(void){    //Port 4 interrupt handler
    int i;
    if(BTN_PORT->IFG & BTN1){                   //Check if button1 has been pressed
        for(i = 0; i < 150; i++){               //Cycle through for-loop to check state of button bounce
            if(debounceBtn1()){                 //Check that button1 has been pressed
                btn1Flag = 1;                   //Set button1 flag to indicate that button1 was pressed
            }
        }
        clkCount++;
        BTN_PORT->IFG &=~ BTN1;                 //Clear button1 interrupt flag
    }
}

void SysTick_Handler(void){
    int i;
    for(i = 0; i < 150; i++){               //Cycle through for-loop to check state of button bounce
        if(debounceBtn1()){                 //Check that button1 has been pressed
            btn1Flag = 1;                   //Set button1 flag to indicate that button1 was pressed
        }else{
            btn1Flag = 0;
            break;
        }
    }
    if(btn1Flag){                       //Check if button1 has been pressed
        if((clkCount % 2) != 0){
            sequenceControl();          //Call normal sequence control for the LEDs.
        }else if((clkCount % 2) == 0){
            reverseSequenceControl();   //Call reverse sequence control for the LEDs.
        }
    }
}

void gpioSetup(void){
    LED_PORT->SEL0 &=~ LEDS;        //Set LEDs' (P4.0,4.1,4.2) and buttons'(P4.3 & P4.4) pins as GPIO
    LED_PORT->SEL1 &=~ LEDS;
    LED_PORT->DIR |= LEDS;          //Set LEDs as outputs
    LED_PORT->OUT &=~ LEDS;         //Turn LEDs Off upon startup.

    BTN_PORT->SEL0 &=~ BTNS;        //Set LEDs' (P4.0,4.1,4.2) and buttons'(P4.3 & P4.4) pins as GPIO
    BTN_PORT->SEL1 &=~ BTNS;
    BTN_PORT->DIR &=~ BTNS;         //Set buttons as inputs (P4.3 P4.4)
    BTN_PORT->REN |= BTNS;          //Enable MSP432 internal resistor ( P4.3 P4.4)
    BTN_PORT->OUT |= BTNS;          //Set internal resistor to pull-up (P4.3 P4.4)
    BTN_PORT->IES |= BTNS;          //Set interrupt edge from high to low (P4.3 P4.4)
    BTN_PORT->IE  |= BTNS;          //Enable interrupt (P4.3 P4.4)
    BTN_PORT->IFG = 0;              //Clear interrupt flag
}

uint8_t debounceBtn1(void){                                         //Debounce function for button1
    static uint16_t State1 = 0;                                     //Current debounce status
    State1 = (State1 << 1) | (BTN_PORT->IN & BTN1)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State1==0x8000)return 1;                                     //indicates 0 level is
    return 0;
}

uint8_t debounceBtn2(void){                                         //Debounce function for button1
    static uint16_t State2 = 0;                                     //Current debounce status
    State2 = (State2 << 1) | (BTN_PORT->IN & BTN2)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State2==0x8000)return 1;                                     //indicates 0 level is
    return 0;
}
