//SLAVE CODE

#include "msp.h"
#include "bipolarMotor.h"
#include "systick.h"

#define I2C_PORT    P1
#define SDA         BIT6
#define SCL         BIT7
#define LED_PORT    P2
#define LED_RED     BIT0
#define LED_GREEN   BIT1
#define LED_BLUE    BIT2
#define LEDS        (LED_RED|LED_GREEN|LED_BLUE)

#define SLAVE_ADDRESS 0x48

void initialization(void);
void GPIO_init(void);
void I2C_init(void);
void decode(void);

char RXData;
uint8_t run;
uint8_t steps, cycles;
uint8_t cwFlag, ccwFlag;
uint8_t direction;

void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    initialization();

    NVIC->ISER[0] |= 1 << (EUSCIB0_IRQn & 31);                     // EUSCI_B0 interrupt is enabled in NVIC
    __enable_irq();                                 // All interrupts are enabled

    while(1){
        if(run){
            steps = 4 * cycles;
            if(cwFlag){
                CW(steps);
                cwFlag = 0;
            }else if(ccwFlag){
                CCW(steps);
                ccwFlag = 0;
            }
            run = 0;
        }
//        CW(100);
//        msDelay(500);
//        CCW(100);
//        msDelay(500);
    }
}

void initialization(void){
    GPIO_init();
    sysTickSetup();
    I2C_init();
    initializeBipolarMotor();
}

void GPIO_init(void){
    LED_PORT->DIR = LEDS;
    LED_PORT->OUT = 0;
    I2C_PORT->SEL0 |= SDA|SCL;                      // P1.6 and P1.7 as UCB0SDA and UCB0SCL
}

void I2C_init(void){
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         // Hold EUSCI_B0 module in reset state
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MODE_3|EUSCI_B_CTLW0_SYNC;     //Set up I2C Mode, and Synchronous communication
    EUSCI_B0->I2COA0 = SLAVE_ADDRESS | EUSCI_B_I2COA0_OAEN;         //Set up and enable Own Address as Slave Address
    EUSCI_B0->CTLW0 &=~ EUSCI_B_CTLW0_SWRST;        // Clear SWRST to resume operation
    EUSCI_B0->IFG &=~ EUSCI_B_IFG_RXIFG0;           // Clear EUSCI_B0 RX interrupt flag
    EUSCI_B0->IE |= EUSCI_B_IE_RXIE0;               // Enable EUSCI_B0 RX interrupt
}

void EUSCIB0_IRQHandler(void){
    uint32_t status = EUSCI_B0->IFG;                // Get EUSCI_B0 interrupt flag
    if(status & EUSCI_B_IFG_RXIFG0){                // Check if receive interrupt occurs
        RXData = EUSCI_B0->RXBUF;                   // Load current RXData value to transmit buffer
        decode();
        run = 1;
    }

    EUSCI_B0->IFG  &=~ EUSCI_B_IFG_RXIFG0;          // Clear EUSCI_B0 RX interrupt flag
}

void decode(void){
    char temp;
    temp = RXData;
    direction = (temp &=~ 0x7F);
    temp = RXData;
    cycles = (temp &=~ 0x80);


    if(direction == 0x00){
        LED_PORT->OUT ^= LED_BLUE;
        ccwFlag = 1;
    }else if(direction == 0x80){
        LED_PORT->OUT ^= LED_GREEN;
        cwFlag = 1;
    }
}
