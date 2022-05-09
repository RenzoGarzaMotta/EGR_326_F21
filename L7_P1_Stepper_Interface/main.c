/*Lab 7 - Interfacing a DC Stepper Motor y=to the MSP432 MCU
 *Part 1 - Controlling the direction of motion of a stepper motor
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
 *  This program controls the stepper motor motion via a push-button.
 *      -First press increments the stepper forward by 10 steps.
 *      -Second press decrements the stepper in the opposite direction by 10 steps.
 *      -Bipolar Motor provided in class kit has a total of 650 steps (roughly) with the current configuration.
 */

#include "msp.h"
#include "systick.h"
#include "bipolarMotor.h"

#define BTN_PORT    P3
#define BTN1        BIT6


uint8_t debounceBtn1(void);
void initGPIO(void);
void buttonInit(void);

uint8_t increment = 0;
uint8_t clicked = 0;
//uint16_t State1 = 0;

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	sysTickSetup();
	initGPIO();

	P2->SEL0  &=~ BIT0;
    P2->SEL1 &=~ BIT0;
    P2->DIR |= BIT0;
    P2->OUT &=~ BIT0;

	NVIC->ISER[1] = 1 << (PORT3_IRQn & 31);
	__enable_irq();


	while(1){
//	    if(clicked){
//	        increment++;
//	        P2->OUT ^= BIT0;
//	        if((increment % 2) == 0){
//                CW(10);
//            }else if((increment % 2) == 1){
//                CCW(10);
//            }
//	        clicked = 0;
//	    }
	    CW(100);
	    msDelay(500);
	    CCW(100);
	    msDelay(500);
	}
}

void initGPIO(void){
    initializeBipolarMotor();
    buttonInit();
}

void buttonInit(void){
    BTN_PORT->SEL0  &=~ BTN1;
    BTN_PORT->SEL1 &=~ BTN1;
    BTN_PORT->DIR &=~ BTN1;
    BTN_PORT->REN |= BTN1;
    BTN_PORT->OUT |= BTN1;
    BTN_PORT->IE |= BTN1;
    BTN_PORT->IES |= BTN1;
    BTN_PORT->IFG = 0;
}

void PORT3_IRQHandler(void){
    int i, var = 0;

    if(BTN_PORT->IFG & BTN1){
        for(i = 0; i < 150; i++){                   //Cycle through for-loop to check state of button bounce
            var = debounceBtn1();
        }
        if(var == 1){   //Check that both buttons have been pressed and debounced
            clicked = 1;
            var = 0;
        }
    }
    BTN_PORT->IFG = 0;
}

uint8_t debounceBtn1(void){                                         //Debounce function for button1
    static uint16_t State1 = 0;                                     //Current debounce status
    State1 = (State1 << 1) | (BTN_PORT->IN & BTN1)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State1==0x8000)return 1;                                     //indicates 0 level is
    return 0;
}
