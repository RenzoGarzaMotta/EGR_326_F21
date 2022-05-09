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
#include "systick.h"
#include "macros.h"
#include "leds.h"

void gpioSetup(void);               //Set-up gpio
uint8_t debounceBtn1(void);         //Debounce function for button1
uint8_t debounceBtn2(void);         //Debounce function for button2

uint8_t btn1Flag = 0;               //Flag to indicate button1 has been pressed
uint8_t btn2Flag = 0;               //Flag to indicate button2 has been pressed
uint32_t blinkRate = 0;             //Rate at which the LED is blinking

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    gpioSetup();                                    //Call function to set up system gpio
    sysTickSetup();                                 //Call function to set up SysTick timer

    NVIC->ISER[1] = 1 << (PORT4_IRQn & 31);         //Set interrupts for Port 4
    __enable_irq();                                 //Enable all interrupts

    while(1){
        if(btn1Flag){                               //Check if button1 has been pressed
            if(blinkRate == 0){                     //Check if the button blink delay is 0
                blinkRate = 1000000;                //Initialize as 0.5Hz
            }else{
                blinkRate = blinkRate / 2;          //Double blinking rate by reducing delay time
                if(blinkRate <= 2){                 //Check if blink delay is less than two microseconds
                    blinkRate = 2;                  //Set blinkRate to 0 to avoid truncation.
                }
            }
            btn1Flag = 0;                           //Reset button1 flag
        }else if(btn2Flag){                         //Check if button2 has been pressed
            blinkRate = blinkRate * 2;              //Half blinking rate by increasing delay time
            if(blinkRate > 1000000){                //Check if blink rate is lower 0.5Hz
                blinkRate = 0;                      //Turn off LED
            }
            btn2Flag = 0;                           //Reset button2 flag
        }

        if(blinkRate != 0){                         //Blink LED when there is a delay available
            LED_PORT->OUT ^= LED_RED;               //Toggle all LEDs
            usDelay(blinkRate);                     //delay microseconds
        }else{
            LED_PORT->OUT &=~ LED_RED;              //Turn off LEDs
        }
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
    LED_PORT->SEL0 &=~ LEDS;   //Set LEDs' (P2.0,2.1,2.2) and buttons'(P4.3 & P4.4) pins as GPIO
    LED_PORT->SEL1 &=~ LEDS;
    LED_PORT->DIR |= LEDS;
    LED_PORT->OUT &=~ LEDS;

    BTN_PORT->SEL0 &=~ BTNS;   //Set LEDs' (P2.0,2.1,2.2) and buttons'(P4.3 & P4.4) pins as GPIO
    BTN_PORT->SEL1 &=~ BTNS;
    BTN_PORT->DIR &=~ BTNS;    //Set buttons as inputs (P4.3 P4.4)
    BTN_PORT->REN |= BTNS;     //Enable MSP432 internal resistor ( P4.3 P4.4)
    BTN_PORT->OUT |= BTNS;     //Set internal resistor to pull-up (P4.3 P4.4)
    BTN_PORT->IES |= BTNS;     //Set interrupt edge from high to low (P4.3 P4.4)
    BTN_PORT->IE  |= BTNS;     //Enable interrupt (P4.3 P4.4)
    BTN_PORT->IFG = 0;         //Clear interrupt flag
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
