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
    intSysTickSetup();                                 //Call function to set up SysTick timer

    NVIC->ISER[1] = 1 << (PORT4_IRQn & 31);         //Set interrupts
    __enable_irq();                                 //Enable all interrupts

    while(1){
        if(btn1Flag){                   //Check if button1 has been pressed
            sequenceControl();          //Call normal sequence control for the leds
            btn1Flag = 0;               //Reset button1 flag
        }
    }
}

void PORT4_IRQHandler(void){    //Port 4 interrupt handler
    int i;
    if(BTN_PORT->IFG & BTN1){                   //Check if button1 has been pressed
        for(i = 0; i < 150; i++){               //Cycle through for-loop to check state of button bounce
            if(debounceBtn1()){                 //Check that button1 has been pressed
                btn1Flag = 1;                   //Set button1 flag to indicate that button1 was pressed
            }else{
                btn1Flag = 0;
            }
        }
        BTN_PORT->IFG &=~ BTN1;                 //Clear button1 interrupt flag
    }else if(BTN_PORT->IFG & BTN2){             //Check if button2 has been pressed
        for(i = 0; i < 150; i++){               //Cycle through for-loop to check state of button bounce
            if(debounceBtn2()){                 //Check that button2 has been pressed
                btn2Flag = 1;                   //Set button2 flag to indicate that button2 was pressed
            }else{
                btn1Flag = 0;
            }
        }
        BTN_PORT->IFG &=~ BTN2;                 //Clear button1 interrupt flag
    }
    BTN_PORT->IFG = 0;
}

void SysTick_Handler(void){
    if(debounceBtn2()){
        if(btn2Flag){                   //Check if button1 has been pressed
            switch(fwdSequenceState){   //Check for the forward state
                case red:                   //If state is RED, toggle RED led
                    toggleLED(LED_RED);
                    turnOff(LED_GREEN);
                    turnOff(LED_BLUE);
                    break;
                case green:                 //If state is RED, toggle GREEN led
                    turnOff(LED_RED);
                    toggleLED(LED_GREEN);
                    turnOff(LED_BLUE);
                    break;
                case blue:                  //If state is RED, toggle BLUE led
                    turnOff(LED_RED);
                    turnOff(LED_GREEN);
                    toggleLED(LED_BLUE);
                    break;
                default:
                    fwdSequenceState = red;     //Set default state
                    break;
            }
        }
    }
}

void gpioSetup(void){
    LED_PORT->SEL0 &=~ LEDS;   //Set LEDs' (P2.0,2.1,2.2) and buttons'(P4.3 & P4.4) pins as GPIO
    LED_PORT->SEL1 &=~ LEDS;
    LED_PORT->DIR |= LEDS;
    LED_PORT->OUT &=~ LEDS;

    BTN_PORT->SEL0 &=~ BTNS;   //Set LEDs' (P2.0,2.1,2.2) and buttons'(P4.3 & P4.4) pins as GPIO
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
