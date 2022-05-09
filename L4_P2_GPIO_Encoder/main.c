#include "msp.h"

#define ENCODER_PORT    P2

#define CLK             BIT6
#define DT              BIT5
#define SW              BIT4

#define ENCODER         (CLK|DT|SW)

void gpioSetup(void);
uint8_t debounceFallingCLK(void);
uint8_t debounceRisingCLK(void);

int counter;
int CW, CCW;                    //These counters keep track of rotation count in the CW or CCW direction
uint8_t CWflag, CCWflag;        //Flag to indicate if a CW or CCW rotation has taken place
uint8_t changeIES, clkFlag;

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	gpioSetup();

	NVIC->ISER[1] = 1 << (PORT2_IRQn & 31);
	__enable_irq();                                 //Enable all interrupts

	while(1){
	    if(changeIES){
	        if(CWflag){
	            CW++;
	            CWflag = 0;
	        }
	        if(CCWflag){
                CCW++;
                CCWflag = 0;
            }

	        ENCODER_PORT->SEL0 &=~ CLK;
            ENCODER_PORT->SEL1 &=~ CLK;
            ENCODER_PORT->DIR &=~ (CLK);
            if(counter%2){
                ENCODER_PORT->IES |= CLK;
            }else{
                ENCODER_PORT->IES &=~ CLK;
            }
            ENCODER_PORT->IE |= CLK;
            ENCODER_PORT->IFG = 0;

            NVIC->ISER[1] = 1 << (PORT2_IRQn & 31);
            __enable_irq();                                 //Enable all interrupts

//            P2->OUT ^= BIT0;

	        changeIES = 0;
	    }
	}
}

void PORT2_IRQHandler(void){
    if(ENCODER_PORT->IFG & CLK){
        if((ENCODER_PORT->IN & CLK) == 0){                  //Check if falling edge was triggered
            if((ENCODER_PORT->IN & DT) == 0x00000020){      //Check if DT is 1
                CWflag = 1;                                 //If DT is a 1 when CLK encounters a falling edge, the encoder is rotating CW
                changeIES = 1;
                counter++;
            }else if((ENCODER_PORT->IN & DT) == 0){         //Check if DT is 0
                CCWflag = 1;                                //If DT is a 0 when CLK encounters a falling edge, the encoder is rotating CCW
                changeIES = 1;
                counter++;
            }
        }else if((ENCODER_PORT->IN & CLK) == 0x00000040){   //Check if rising edge was triggered
            if((ENCODER_PORT->IN & DT) == 0x00000020){      //Check if DT is 1
                CCWflag = 1;                                //If DT is a 1 when CLK encounters a rising edge, the encoder is rotating CCW
                changeIES = 1;
                counter++;
            }else if((ENCODER_PORT->IN & DT) == 0){         //Check if DT is 0
                CWflag = 1;                                 //If DT is a 0 when CLK encounters a rising edge, the encoder is rotating CW
                changeIES = 1;
                counter++;
            }
        }
    }

    ENCODER_PORT->IFG = 0;
}

uint8_t debounceFallingCLK(void){                                             //Debounce function for button1
    static uint16_t State1 = 0;                                         //Current debounce status
    State1 = (State1 << 1) | (ENCODER_PORT->IN & CLK)>>1 | 0x8000;       //Shift state to indicate bounce
    if(State1==0x8000)return 1;                                         //indicates 0 level is
    return 0;
}

uint8_t debounceRisingCLK(void){                                             //Debounce function for button1
    static uint16_t State2 = 0;                                         //Current debounce status
    State2 = (State2 << 1) | (ENCODER_PORT->IN & CLK)>>1 | 0x7FFF;       //Shift state to indicate bounce
    if(State2==0x7FFF)return 1;                                         //indicates 0 level is
    return 0;
}

void gpioSetup(void){
    P2->SEL0 &=~ (BIT0|BIT1);
    P2->SEL1 &=~ (BIT0|BIT1);
    P2->DIR |= (BIT0|BIT1);
    P2->OUT &=~ (BIT0|BIT1);

    ENCODER_PORT->SEL0 &=~ ENCODER;
    ENCODER_PORT->SEL1 &=~ ENCODER;
    ENCODER_PORT->DIR &=~ (CLK | DT);
    ENCODER_PORT->IES &=~ (CLK);
    ENCODER_PORT->IE |= CLK;
    ENCODER_PORT->IFG = 0;
}
