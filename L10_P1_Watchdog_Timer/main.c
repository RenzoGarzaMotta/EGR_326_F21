#include <stdio.h>
#include "msp.h"
#include "systick.h"
#include "ST7735.h"

#define LED_PORT    P1
#define LED         BIT0
#define BTN_PORT    P1
#define BTN         BIT1

void initialization(void);
void GPIO(void);
void watchdog_init(void);
void timer32_init(void);
int CHECK(uint8_t BUTTON);

double timer = 0, WDT_FLAG;
char timeStr[20];

void main(void){
    //timer = 0;
    initialization();

    NVIC->ISER[0] = 1 << ((WDT_A_IRQn) & 31);
    NVIC->ISER[0] = 1 << ((T32_INT1_IRQn) & 31);
    __enable_interrupt();

    LED_PORT->OUT |= LED;   //Flash LED prior to countdown.
    msDelay(50);
    LED_PORT->OUT &=~ LED;

    WDT_A->CTL = (WDT_A_CTL_PW|             //Enter WDT password
                  WDT_A_CTL_SSEL__ACLK |    //Select ACLK as source
                  WDT_A_CTL_CNTCL |         //Clear Timer
                  WDT_A_CTL_IS_4);          //Select 1s intervals (@ 32kHz)

    while(1){
        if(CHECK(BTN)){                     //Check if user input happens
            WDT_A->CTL = (WDT_A_CTL_PW|             //Enter WDT password
                          WDT_A_CTL_SSEL__ACLK |    //Select ACLK as source
                          WDT_A_CTL_CNTCL |         //Clear Timer
                          WDT_A_CTL_IS_4);          //Select 1s intervals (@ 32kHz)
            timer = 0;                      //Reset sec counter
        }
        sprintf(timeStr, "%g", timer/1000);      //format time counter to string
        ST7735_DrawString(1,7,3, timeStr, ST7735_WHITE);   //Print string to LCD screen
    }
}

void initialization(void){
    watchdog_init();
    sysTickSetup();
    ST7735_InitR(INITR_REDTAB);
    GPIO();
    timer32_init();
}

void GPIO(void){
    LED_PORT->SEL0 &=~ LED;
    LED_PORT->SEL1 &=~ LED;
    LED_PORT->DIR |= LED;
    LED_PORT->OUT &=~ LED;

    BTN_PORT->SEL0 &=~ BTN;
    BTN_PORT->SEL1 &=~ BTN;
    BTN_PORT->DIR &=~ BTN;
    BTN_PORT->REN |= BTN;
    BTN_PORT->OUT |= BTN;
}

void watchdog_init(void){
    SYSCTL->WDTRESET_CTL = 0;               //Setup WDT with a soft reset (0) instead of a hard reset (1)

    CS->KEY = CS_KEY_VAL;                   //Enter CS password
    CS->CTL1 |= (CS_CTL1_DIVA__4|           //Set ACLK source with a 1/4 clkc divider (REFOCLK / 4)
                 CS_CTL1_SELA__REFOCLK);    //Sel REFOCLK as ACLK source
    CS->KEY = 1;                            //Close CS
}

void timer32_init(void){
    TIMER32_1->LOAD = (1 * 48000) - 1;            //Set 1 msec intervals with Timer32
    TIMER32_1->BGLOAD = (1 * 48000) - 1;          //Set 1 msec reload value
    TIMER32_1->CONTROL |= (TIMER32_CONTROL_IE|      //Enable interrupt
                          TIMER32_CONTROL_SIZE);    //Select 32bit timer
    TIMER32_1->INTCLR = 0;                          //Clear interrupt
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;   //Enable Timer32 after setup
}

int CHECK(uint8_t BUTTON){
    int pressed = 0;
    do{
        msDelay(25);
        if((BTN_PORT->IN & BUTTON) == 0){            //CHECK TO SEE IF THE BUTTON IS PRESSED
            pressed = 1;                                //(IF BUTTON WAS PRESSED) FLAG THAT BUTTON WAS PRESSED
            if((BTN_PORT->IN & BUTTON) == 0){        //CHECK IF THE BUTTON IS STILL PRESSED
                pressed = 1;                            //CONFIRM BUTTON PRESSED
            }else{
                pressed = 0;
            }
        }else{
        }
    }while((BTN_PORT->IN & BUTTON) == 0);

    return pressed;
}

void T32_INT1_IRQHandler(void){
    if((TIMER32_1->MIS & TIMER32_MIS_IFG)){ //Check if flag is set
        timer += 1; //Increase timer counter every 1ms
    }
    TIMER32_1->INTCLR = 0;  //Clear timer
    TIMER32_1->LOAD = (1 * 48000) - 1;  //Reload with 1 ms
    TIMER32_1->BGLOAD = (1 * 48000) - 1;
}

void WDT_A_IRQHadler(void){
    LED_PORT->OUT ^= LED;
    WDT_FLAG = 1;
}
