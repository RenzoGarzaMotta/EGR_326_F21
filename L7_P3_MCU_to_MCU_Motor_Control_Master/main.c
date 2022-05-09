//MASTER CODE
#include "msp.h"
#include <string.h>
#include <stdlib.h>
#include "systick.h"
#include "keypad.h"

#define I2C_PORT        P1
#define SDA             BIT6
#define SCL             BIT7
#define BTN_PORT        P1
#define BTN1            BIT1
#define BTN2            BIT4
#define BTNS            (BTN1|BTN2)
#define SLAVE_ADDRESS   0x48

void initialization(void);
void GPIO_init(void);
void I2C_init(void);
void encodeData(void);
uint8_t debounceBtn1(void);
uint8_t debounceBtn2(void);

//char TXData[12] = "RRGGBBRRGGBB";
char TXData;
int i = 0;
uint8_t cwFlag = 0, ccwFlag = 0;
uint8_t cycles;
char keyIn;
extern char keyPress;

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    initialization();

    NVIC->ISER[1] |= 1 << (PORT1_IRQn & 31);                        // PORT1 interrupt is enabled in NVIC
    NVIC->ISER[0] |= 1 << (EUSCIB0_IRQn & 31);                      // EUSCI_B0 interrupt is enabled in NVIC
    __enable_irq();                                                 // All interrupts are enabled

    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_TR | EUSCI_B_CTLW0_TXSTT);

    while(1){
        if(readKeypad()){
            keyIn = keyPress;
            cycles = keyIn - 48;
            printKeys(keyIn);
        }
    }
}

void initialization(void){
    GPIO_init();
    sysTickSetup();
    keypadInit();
    I2C_init();
}

void GPIO_init(void){
    I2C_PORT->SEL0 |= SDA | SCL;  // P1.6 and P1.7 as UCB0SDA and UCB0SCL
    I2C_PORT->SEL1 &=~ SDA | SCL; // P1.6 and P1.7 as UCB0SDA and UCB0SCL

    BTN_PORT->SEL0 &=~ BTNS;
    BTN_PORT->SEL1 &=~ BTNS;     //Set up button as GPIO
    BTN_PORT->DIR  &=~ BTNS;     //Input
    BTN_PORT->REN  |= BTNS;      //Enable Internal resistance
    BTN_PORT->OUT  |= BTNS;      //Pull-Up Resistance
    BTN_PORT->IE   |= BTNS;      //Enable Interrupt
    BTN_PORT->IES  |= BTNS;      //Low-to-High Edge select
    BTN_PORT->IFG  = 0x00;     //Clear Interrupt Flag
}

void I2C_init(void){
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;                         // Hold EUSCI_B0 module in reset state
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MODE_3|EUSCI_B_CTLW0_MST|EUSCI_B_CTLW0_SYNC;   //Set up device as Master with I2C Mode, and Synchronous communication.
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_UCSSEL_2;                      // Select SMCLK as EUSCI_B0 clock
    EUSCI_B0->BRW |= 0x001E;                                        // Set BITCLK = BRCLK / (UCBRx+1) = 3 MHz / 30 = 100 kHz
    EUSCI_B0->I2CSA |= SLAVE_ADDRESS;
    EUSCI_B0->CTLW0 &=~ EUSCI_B_CTLW0_SWRST;                        // Clear SWRST to resume operation
}

void EUSCIB0_IRQHandler(void){
    uint32_t status = EUSCI_B0->IFG;             // Get EUSCI_B0 interrupt flag
    encodeData();
    if(status & EUSCI_B_IFG_TXIFG0){             // Check if transmit interrupt occurs
       EUSCI_B0->TXBUF = TXData;        // Load current TXData value to transmit buffer
       EUSCI_B0->IE &=~ EUSCI_B_IE_TXIE0;       // Disable EUSCI_B0 TX interrupt
    }
    EUSCI_B0->IFG &=~ EUSCI_B_IFG_TXIFG0;        // Clear EUSCI_B0 TX inter. flag
}

void PORT1_IRQHandler(void){

    uint32_t status = BTN_PORT->IFG;
    if(status & BTN1){
        cwFlag = 1;
        EUSCI_B0->IE |= EUSCI_B_IE_TXIE0;   // Enable EUSCI_A0 TX interrupt
    }
    if(status & BTN2){
       ccwFlag = 1;
       EUSCI_B0->IE |= EUSCI_B_IE_TXIE0;   // Enable EUSCI_A0 TX interrupt
    }

    BTN_PORT->IFG = 0;
}

void encodeData(void){
    if(cwFlag == 1){
        TXData = 0x80;
        cwFlag = 0;
    }else if(ccwFlag == 1){
        TXData = 0x00;
        ccwFlag = 0;
    }
    TXData = TXData | (cycles &=~ 0x80);
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
