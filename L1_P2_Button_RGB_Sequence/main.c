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

void gpioSetup(void);               //Set-up gpio
uint8_t debounceBtn1(void);         //Debounce function for button1
uint8_t debounceBtn2(void);         //Debounce function for button2

extern uint8_t fwdSequenceState;    //Set initial forward state as red
extern uint8_t bckSequenceState;    //Set initial backward state as blue

uint8_t btn1Flag = 0;               //Flag to indicate button1 has been pressed
uint8_t btn2Flag = 0;               //Flag to indicate button2 has been pressed
uint8_t bothBtnFlag = 0;            //Flag to indicate that both buttons have been pressed

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    gpioSetup();                                    //Call function to set up system gpio
    sysTickSetup();                                 //Call function to set up SysTick timer

    NVIC->ISER[1] = 1 << (PORT4_IRQn & 31);         //Set interrupts
    __enable_irq();                                 //Enable all interrupts

    while(1){
            if(btn1Flag){                   //Check if button1 has been pressed
                sequenceControl();          //Call normal sequence control for the leds
                btn1Flag = 0;               //Reset button1 flag
            }else if(btn2Flag){              //Check if button2 has been pressed
                reverseSequenceControl();       //Call normal sequence control for the leds
                btn2Flag = 0;                   //Reset button2 flag
        }
    }
}

void PORT4_IRQHandler(void){    //Port 4 interrupt handler

    int i;
    for(i = 0; i < 150; i++){                   //Cycle through for-loop to check state of button bounce
        if(debounceBtn1() && debounceBtn2()){   //Check that both buttons have been pressed and debounced
            LED_PORT->OUT &=~ LEDS;             //Turn off all LEDs
            fwdSequenceState = red;             //Set initial forward state as red
            bckSequenceState = blue;            //Set initial backward state as blue
            BTN_PORT->IFG = 0;                  //Clear all port 4 interrupt flags
        }
    }
    if(BTN_PORT->IFG & BTN1){                   //Check if button1 has been pressed
        for(i = 0; i < 150; i++){               //Cycle through for-loop to check state of button bounce
            if(debounceBtn1()){                 //Check that button1 has been pressed
                btn1Flag = 1;                   //Set button1 flag to indicate that button1 was pressed
            }
        }
        BTN_PORT->IFG &=~ BTN1;                 //Clear button1 interrupt flag
    }else if(BTN_PORT->IFG & BTN2){             //Check if button2 has been pressed
        for(i = 0; i < 150; i++){               //Cycle through for-loop to check state of button bounce
            if(debounceBtn2()){                 //Check that button2 has been pressed
                btn2Flag = 1;                   //Set button2 flag to indicate that button2 was pressed
            }
        }
        BTN_PORT->IFG &=~ BTN2;                 //Clear button1 interrupt flag
    }
}

void gpioSetup(void){

    LED_PORT->SEL0 &=~ LEDS|BTNS;   //Set LEDs' (P2.0,2.1,2.2) and buttons'(P4.3 & P4.4) pins as GPIO
    LED_PORT->SEL1 &=~ LEDS|BTNS;

    LED_PORT->DIR |= LEDS;          //Set LEDs as outputs
    LED_PORT->OUT &=~ LEDS;         //Set initial LED status as OFF

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

//uint8_t debounce(uint32_t btnPort, uint16_t btnBit){
////    uint8_t portMax = 10;
////    uint8_t bitMax = 8;
//    uint8_t port;
//    uint16_t bit;
//    uint8_t currentState;
//P1
//    static uint16_t state [10][8];
//    P1
//    if((btnPort->IN & btnBit) == 0){
//        currentState = 1;//Read input register (1 = High, 0 = Low)
//    }
//    bit = 1.442695040889*log(btnBit);
//
//    switch(btnPort){
//        case P1:
//            port = 0;
//            break;
//        case P2:
//            port = 1;
//            break;
//        case P3:
//            port = 2;
//            break;
//        case P4:
//            port = 3;
//            break;
//        case P5:
//            port = 4;
//            break;
//        case P6:
//            port = 5;
//            break;
//        case P7:
//            port = 6;
//            break;
//        case P8:
//            port = 7;
//            break;
//        case P9:
//            port = 8;
//            break;
//        case P10:
//            port = 9;
//            break;
//        default:
//            port = 0;
//            break;
//    }
//
//   state[port][bit] = ((state << 1) | (currentState >> btnBit) | 0xFC00);
//
//   if(state[port][bit] == 0xfC00){
//       return 1;
//   }
//
//   return 0;
//
//}
