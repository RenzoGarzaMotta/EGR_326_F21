#include "msp.h"
#include "systick.h"
#include "7Segment.h"

#define BTN_PORT    P1
#define BTN1        BIT4

uint8_t btn1Flag, btnCount;
uint16_t secCount = 0;

uint8_t debounceBtn1(void);
void gpioInit(void);

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	intSysTickSetup();  //Initialize SysTick with an interrupt
	segmentInit();      //Initialize seven segment display
	gpioInit();         //Initialize button gpio

	NVIC->ISER[1] = 1 << (PORT3_IRQn & 31);
	__enable_irq();     //Enable port 3 interrupt
    clearAll();         //Clear the seven segment displays
	while(1){           //Enter while loop
	}
}

void gpioInit(void){
    BTN_PORT->SEL0 &=~ BTN1;
    BTN_PORT->SEL1 &=~ BTN1;     //Set up button as GPIO
    BTN_PORT->DIR  &=~ BTN1;     //Input
    BTN_PORT->REN  |= BTN1;      //Enable Internal resistance
    BTN_PORT->OUT  |= BTN1;      //Pull-Up Resistance
    BTN_PORT->IE   |= BTN1;      //Enable Interrupt
    BTN_PORT->IES  |= BTN1;      //High-to-Low Edge select
    BTN_PORT->IFG  = 0x00;       //Clear Interrupt Flag
}

void PORT3_IRQHandler(void){
    uint32_t status = BTN_PORT->IFG;
    uint8_t i, var;

    if(status & BTN1){
        for(i = 0; i < 150; i++){   //Debounce the external button
            var = debounceBtn1();
        }
        if(var){                    //If the button was debounced
            btnCount++;             //Increase button count
            var = 0;                //reset button debounce flag
        }
        if((btnCount % 2) == 1){            //Check if button count is odd
            encodeDigits(0, 1,btnCount);    //Send button count to segment display
            encodeDigits(0, 0,secCount);    //Send second count to segment display
        }else if((btnCount % 2) == 0){      //Check if button count is even
            secCount = 0;                   //Reset second count
            encodeDigits(0, 1,btnCount);    //Send button count to segment display
            encodeDigits(0, 0,secCount);    //Send second count to segment display
        }
    }

    BTN_PORT->IFG = 0;              //Clear button interrupt flag
}

void SysTick_Handler(void){
    if((btnCount % 2) == 1){        //Check if button count is odd
        secCount++;                 //Increase seccond count whenever interrupt is triggered
    }else if((btnCount % 2) == 0){  //Check if button count is even
        secCount = 0;               //Reset second count
    }
    encodeDigits(0, 1,btnCount);    //Send button count to segment display
    encodeDigits(0, 0,secCount);    //Send second count to segment display
}

uint8_t debounceBtn1(void){                                         //Debounce function for button1
    static uint16_t State1 = 0;                                     //Current debounce status
    State1 = (State1 << 1) | (BTN_PORT->IN & BTN1)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State1==0x8000)return 1;                                     //indicates 0 level is
    return 0;
}
