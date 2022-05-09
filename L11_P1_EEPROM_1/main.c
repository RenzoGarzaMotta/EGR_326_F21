//MASTER CODE
#include <stdlib.h>
#include <stdio.h>
#include "msp.h"
#include "sysTick.h"
#include "string.h"

#define I2C_PORT        P6                          //I2C communication protocol port
#define I2C_SDA         BIT4                        //Data line in I2C Protocol
#define I2C_SCL         BIT5                        //CLK line in I2C Protocol
#define I2C             (I2C_SDA|I2C_SCL)           //I2C reference
#define SLAVE_ADDR      0x68                        /* 0110 1000.    DS1337 (RTC Slave Address)*/

#define BTN_PORT        P3                          //Button port
#define RECORD_BTN      BIT2                        //Button to record data to EEPROM
#define READ_BTN        BIT3                        //Button to read data from EEPROM
#define BTNS            (RECORD_BTN|READ_BTN)       //Buttons reference

//#define EE_PORT         P1                          //I2C communication protocol port
//#define EE_SDA          BIT6                        //Data line in I2C Protocol
//#define EE_SCL          BIT7                        //CLK line in I2C Protocol
//#define EE_I2C          (EE_SDA|EE_SCL)             //I2C reference

#define EEPROM_ADD      0x57//0101 0111

#define MEM1            0x00
#define MEM2            0x04
#define MEM3            0x08
#define MEM4            0x0C
#define MEM5            0x10

void initialization(void);
void GPIO_init(void);
void I2C_init(void);
int CHECK(uint8_t BUTTON);
void I2C1_init(void);
int I2CB1_burstWrite(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data);
int I2CB1_burstRead(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data);
void manageData(void);
uint8_t secManage(void);
uint8_t minManage(void);
uint8_t hrManage(void);
uint8_t dayManage(void);
uint8_t dateManage(void);
uint8_t monthManage(void);
uint8_t yearManage(void);
void interpret(void);
void secInt(void);
void minInt(void);
void hrInt(void);
void dayInt(void);
void dateInt(void);
void monthInt(void);
void yearInt(void);
void reset(void);
void tempInt(void);
void formatData(int count);

int year = 2020, month = 12, date = 12, day = 6, hour = 3, period = 0, min = 25, sec = 00, century = 0;
int ten = 0, twnt = 0, ind = 0, result = 0, dataWrite = 0, format = 1, dec, qrtr;
int secRead, minRead, hrRead, dayRead, dateRead, monthRead, yrRead;
unsigned char lastAdd = 0, check = 0;
int hold;

float temp;

unsigned char dataEEPROM[20] = {0};
unsigned char readEEPROM[20] = {0};
unsigned char timeDateReadback[20] = {0};
unsigned char data[25] = {0};

char *monthPrint[20];
char *dayPrint[20];
char *periodPrint[5];
char TXData;


void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    initialization();

    NVIC->ISER[1] |= 1 << (PORT3_IRQn & 31);                        // PORT1 interrupt is enabled in NVIC
    //NVIC->ISER[0] |= 1 << (EUSCIB0_IRQn & 31);                      // EUSCI_B0 interrupt is enabled in NVIC
    __enable_irq();                                                 // All interrupts are enable

    while(1){
//        msDelay(1000);
//        I2C1_burstRead(SLAVE_ADDR, 0, 19, timeDateReadback);
//        interpret();
//        puts("\nDate:");
//        printf("%s, %s %d 20%d\n", *dayPrint, *monthPrint, date, year);
//        puts("\nTime:");
//        if(format == 0){ //24hr format selected
//            printf("%d:%d:%d\n", hrRead, minRead, secRead);
//        }else{           //12hr format selected
//            printf("%d:%d:%d %s\n", hrRead, minRead, secRead, *periodPrint);
//        }
//        puts("\nTemperature:");
//        printf("%g C\n", temp);
    }
}

void initialization(void){
    sysTick_init();
    GPIO_init();
    I2C_init();
    manageData();
    I2CB1_burstWrite(SLAVE_ADDR, 0,  6, data);
}

void GPIO_init(void){
    BTN_PORT->SEL0 &=~ BTNS;
    BTN_PORT->SEL1 &=~ BTNS;     //Set up button as GPIO
    BTN_PORT->DIR  &=~ BTNS;     //Input
    BTN_PORT->REN  |= BTNS;      //Enable Internal resistance
    BTN_PORT->OUT  |= BTNS;      //Pull-Up Resistance
    BTN_PORT->IE   |= BTNS;      //Enable Interrupt
    BTN_PORT->IES  |= BTNS;      //High-to-Low Edge select
    BTN_PORT->IFG  |= 0x00;      //Clear Interrupt Flag

    //Initialize LED to test port 3 interrupt - May be deleted
    P1->SEL0 &=~ BIT0;
    P1->SEL1 &=~ BIT0;
    P1->DIR |= BIT0;
    P1->OUT &=~ BIT0;
}

void I2C_init(void){
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;
    EUSCI_B1->CTLW0 |= (EUSCI_B_CTLW0_MST|EUSCI_B_CTLW0_MODE_3|EUSCI_B_CTLW0_SSEL__SMCLK|EUSCI_B_CTLW0_SWRST);
    I2C_PORT->SEL0 |= I2C;
    I2C_PORT->SEL1 &=~ I2C;
    EUSCI_B1->BRW = 120;
    EUSCI_B1->CTLW0 &=~ EUSCI_B_CTLW0_SWRST;
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

//int I2CB1_burstWrite(unsigned char slaveAddr, unsigned char memAddr, int byteCount, unsigned char * data){
//    if (byteCount <= 0)
//        return -1;              /* no write was performed */
//
////    EUSCI_B1->I2CSA = slaveAddr; //Write desired slave address to the UCBxI2CSA register
////    EUSCI_B1->CTLW0 &=~ EUSCI_B_CTLW0_SLA10; //Select slave address with  the USCLA10 bit
////    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR; //Set UCTR for transmitter mode
////    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT//Set UCTTXSTT to generate a START condition
////
////    while((EUSCI_B1->IFG & EUSCI_B_IFG_STTIFG) == 1);//Wait while UCTXSTT is 1 //Wait for START condition and slave address to be sent
////
////    EUSCI_B1->TXBUF = memAddr;//Write data into the Transmission buffer UCBxTXBUF
////
////    while((EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG) == 0);//Wait for data in TXBUF is transferred into the shift register
////
////    do{
////        EUSCI_B1->TXBUF = *data++;//Write data into the Transmission buffer UCBxTXBUF
////        while((EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG) == 0);//Wait for data in TXBUF is transferred into the shift register
////    }while(byteCount);
//
//
//    EUSCI_B1->I2CSA = slaveAddr;      /* setup slave address */
//    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;        /* enable transmitter */
//    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;        /* generate START and send slave address */
//    while(!(EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTT));     /* wait until slave address is sent */
//    EUSCI_B1->TXBUF = memAddr;        /* send memory address to slave */
//    /* send data one byte at a time */
//    do {
//        while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));  /* wait till it's ready to transmit */
//        EUSCI_B1->TXBUF = *data++;    /* send data to slave */
//        byteCount--;
//    }while(byteCount > 0);
//
//    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));      /* wait till last transmit is done */
//    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;        /* send STOP */
//    while(!(EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTP)) ;      /* wait until STOP is sent */
//
//    return 0;                         /* no error */
//}
//
//int I2CB1_burstRead(unsigned char slaveAddr, unsigned char memAddr, int byteCount, unsigned char * data){
//    if (byteCount <= 0)
//        return -1;              /* no read was performed */
//
//    EUSCI_B1->I2CSA = slaveAddr;      /* setup slave address */
//    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;        /* enable transmitter(1)*/
//    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;        /* generate START and send slave address */
//    while(!(EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTT));     /* wait until slave address is sent */
//    EUSCI_B1->TXBUF = memAddr;        /* send memory address to slave */
//
//
//    while(!(EUSCI_B1->IFG & EUSCI_B_IE_TXIE));      /* wait till last transmit is done */
//    EUSCI_B1->CTLW0 &=~ EUSCI_B_CTLW0_TR;       /* enable receiver (0)*/
//    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;        /* generate RESTART and send slave address */
//    while(EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTT);       /* wait till RESTART is finished */
//
//    /* receive data one byte at a time */
//    do {
//        if (byteCount == 1){     /* when only one byte of data is left */
//            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP; /* setup to send STOP after the last byte is received */
//        }
//        while(!(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0));  /* wait till data is received */
//        *data++ = EUSCI_B1->RXBUF;    /* read the received data */
//        byteCount--;
//    }while(byteCount);
//
//    while(EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTP) ;      /* wait until STOP is sent */
//
//    return 0;                   /* no error */
//}
//

int I2CB1_burstWrite(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data){
    if (byteCount <= 0)
        return -1;              /* no write was performed */

    EUSCI_B1->I2CSA = slaveAddr;      /* setup slave address */
    EUSCI_B1->CTLW0 |= 0x0010;        /* enable transmitter */
    EUSCI_B1->CTLW0 |= 0x0002;        /* generate START and send slave address */
    while((EUSCI_B1->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B1->TXBUF = memAddr;        /* send memory address to slave */

    /* send data one byte at a time */
    do {
        while(!(EUSCI_B1->IFG & 2));  /* wait till it's ready to transmit */
        EUSCI_B1->TXBUF = data[memAddr++];    /* send data to slave */
        byteCount--;
     } while (byteCount > 0);

    while(!(EUSCI_B1->IFG & 2));      /* wait till last transmit is done */
    EUSCI_B1->CTLW0 |= 0x0004;        /* send STOP */
    while(EUSCI_B1->CTLW0 & 4) ;      /* wait until STOP is sent */

    return 0;                   /* no error */
}

int I2CB1_burstRead(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data) {
    if (byteCount <= 0)
        return -1;              /* no read was performed */

    EUSCI_B1->I2CSA = slaveAddr;      /* setup slave address */
    EUSCI_B1->CTLW0 |= 0x0010;        /* enable transmitter */
    EUSCI_B1->CTLW0 |= 0x0002;        /* generate START and send slave address */
    while((EUSCI_B1->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B1->TXBUF = memAddr;        /* send memory address to slave */
    while(!(EUSCI_B1->IFG & 2));      /* wait till last transmit is done */
    EUSCI_B1->CTLW0 &= ~0x0010;       /* enable receiver */
    EUSCI_B1->CTLW0 |= 0x0002;        /* generate RESTART and send slave address */
    while(EUSCI_B1->CTLW0 & 2);       /* wait till RESTART is finished */

    /* receive data one byte at a time */
    do {
        if (byteCount == 1)     /* when only one byte of data is left */
            EUSCI_B1->CTLW0 |= 0x0004; /* setup to send STOP after the last byte is received */
        while(!(EUSCI_B1->IFG & 1));  /* wait till data is received */
        data[memAddr++] = EUSCI_B1->RXBUF;    /* read the received data */
        byteCount--;
    } while (byteCount);

    while(EUSCI_B1->CTLW0 & 4) ;      /* wait until STOP is sent */

    return 0;                   /* no error */
}

void manageData(void){
    data[0] = secManage();
    data[1] = minManage();
    data[2] = hrManage();
    data[3] = dayManage();
    data[4] = dateManage();
    data[5] = monthManage();
    data[6] = yearManage();
}

uint8_t secManage(void){
    if(sec > 10){
        ten = sec / 10; //Determines how many 10 seconds are there
        //e.g 55 sec contains 5 10sec and 5 individual sec (individual sec get truncated from calculation)
    }
    ind = sec % 10; //Determines how many individual seconds there are

    result = (ten << 4) | ind;
    return result;
}

uint8_t minManage(void){
    if(min > 10){
        ten = min / 10; //Determines how many 10 minutes are there
        //e.g 55 min contains 5 10min and 5 individual min (individual min get truncated from calculation)
    }
    ind = min % 10; //Determines how many individual seconds there are

    result = (ten << 4) | ind;
    return result;
}

uint8_t hrManage(void){
    //Manage 12hr/24hr setup
    if(format == 1){ // 12hr format
        result = 0b01;

        if(period == 0){
            result = (result << 1) | 0; // AM was selected
        }else{
            result = (result << 1) | 1; // PM was selected
        }

        if(hour > 10){
            ten = hour / 10; //Determines how many 10 minutes are there
        }else{
            ten = 0;
        }

        ind = hour % 10; //Determines how many individual seconds there are

        result = (result << 1) | ten;

        result = (result << 4) | ind;

    }else if(format == 0){ //24hr format
        result = 0b00;
        twnt = hour / 20; //Determines how many 10 minutes are there
        ten = hour / 10; //Determines how many 10 minutes are there
        result = (result << 1) | twnt;
        result = (result << 1) | ten;
        ind = hour % 10; //Determines how many individual seconds there are

        result = (result << 4) | ind;
    }
    return result;
}

uint8_t dayManage(void){
    result = (0b00000 << 1) | day;
    return result;
}

uint8_t dateManage(void){
    result = 0b00;
    ten = date / 10; //Determines how many 10 minutes are there
    result = (result << 2) | ten;
    ind = date % 10;
    result = (result << 4) | ind;
    return result;
}

uint8_t monthManage(void){
    if(century == 1){
        result = 4;
    }else{
        result = 0;
    }

    ten = month / 10; //Determines how many 10 minutes are there
    result = (result << 1) | ten;
    ind = month % 10;
    result = (result << 4) | ind;
    return result;
}

uint8_t yearManage(void){
    year -= 2000;
    ten = year / 10; //Determines how many 10 minutes are there
    result = (result << 4) | ten;
    ind = year % 10;
    result = (result << 4) | ind;
    return result;
}

void interpret(void){
    secInt();
    minInt();
    hrInt();
    dayInt();
    dateInt();
    monthInt();
    yearInt();
    tempInt();
}

void secInt(void){
    ten = 0;
    ind = 0;
    ten = (timeDateReadback[0] >> 4);
    ind = (timeDateReadback[0] &=~ 0xF0);
    secRead = (ten * 10) + ind;
}

void minInt(void){
    ten = 0;
    ind = 0;
    ten = (timeDateReadback[1] >> 4);
    ind = (timeDateReadback[1] &=~ 0xF0);
    minRead = (ten * 10) + ind;
}

void hrInt(void){
    format = timeDateReadback[2];
    twnt = timeDateReadback[2];
    ten = timeDateReadback[2];
    ind = timeDateReadback[2];

    format = (format >> 6);

    twnt = (twnt &=~ 0b11011111);
    twnt = (twnt >> 5);

    ten = (ten &=~ 0b11101111);
    ten = (ten >> 4);

    ind = (ind &=~ 0b11110000);

    if(format == 0){ //24hr format selected
        hrRead = (twnt * 20) + ind;
    }else{           //12hr format selected
        if(twnt == 0){
            *periodPrint = "AM";
        }else{
            *periodPrint = "PM";
        }
        hrRead = (ten * 10) + ind;
    }

}

void dayInt(void){
    ten = timeDateReadback[3];
    ind = timeDateReadback[3];
    ten = (ten >> 4);
    ind = (ind &=~ 0xF0);
    dayRead = (ten * 10) + ind;

    if(dayRead == 1){
        *dayPrint = "Monday";
    }else if(dayRead == 2){
        *dayPrint = "Tuesday";
    }else if(dayRead == 3){
        *dayPrint = "Wednesday";
    }else if(dayRead == 4){
        *dayPrint = "Thursday";
    }else if(dayRead == 5){
        *dayPrint = "Friday";
    }else if(dayRead == 6){
        *dayPrint = "Saturday";
    }else if(dayRead == 7){
        *dayPrint = "Sunday";
    }
}

void dateInt(void){
    ten = timeDateReadback[4];
    ind = timeDateReadback[4];
    ten = (ten >> 4);
    ind = (ind &=~ 0xF0);
    dateRead = (ten * 10) + ind;
}

void monthInt(void){
    ten = 0;
    ind = 0;
    century = 0;
    ten = timeDateReadback[5];
    ten = (ten &=~ 0b11101111);
    ten = (ten >> 4);
    ind = timeDateReadback[5];
    ind = (ind &=~ 0b11110000);

    monthRead = (ten * 10) + ind;

    if(monthRead == 1){
        *monthPrint = "January";
    }else if(monthRead == 2){
        *monthPrint = "February";
    }else if(monthRead == 3){
        *monthPrint = "March";
    }else if(monthRead == 4){
        *monthPrint = "April";
    }else if(monthRead == 5){
        *monthPrint = "May";
    }else if(monthRead == 6){
        *monthPrint = "June";
    }else if(monthRead == 7){
        *monthPrint = "July";
    }else if(monthRead == 8){
        *monthPrint = "August";
    }else if(monthRead == 9){
        *monthPrint = "September";
    }else if(monthRead == 10){
        *monthPrint = "October";
    }else if(monthRead == 11){
        *monthPrint = "November";
    }else if(monthRead == 12){
        *monthPrint = "December";
    }
}

void yearInt(void){
    ten = timeDateReadback[6];
    ind = timeDateReadback[6];
    ten = (ten >> 4);
    ind = (ind &=~ 0xF0);
    yrRead = 2000 + (century * 100) + (ten * 10) + ind;
}

void tempInt(void){
    dec = timeDateReadback[17];
    qrtr = timeDateReadback[18];

    qrtr = (qrtr >> 6);

    temp = dec + (0.25 * qrtr);
}

void formatData(int count){
    hold = ((monthRead << 21) | (dateRead << 16) | (hrRead << 12) | (minRead << 6) | secRead);
    dataEEPROM[count] = (0xFF000000 & hold) >> 24;
    dataEEPROM[(count + 1)] = (0xFF0000 & hold) >> 16;
    dataEEPROM[(count + 2)] = (0xFF00 & hold) >> 8;
    dataEEPROM[(count + 3)] = (0xFF & hold);
}

void EUSCIB0_IRQHandler(void){
//    uint32_t status = EUSCI_B0->IFG;             // Get EUSCI_B0 interrupt flag
//    if(status & EUSCI_B_IFG_TXIFG0){             // Check if transmit interrupt occurs
//        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));  /* wait till it's ready to transmit */
//        EUSCI_B0->TXBUF = dataEEPROM[lastAdd++];        // Load current TXData value to transmit buffer
//        EUSCI_B0->IE &=~ EUSCI_B_IE_TXIE0;       // Disable EUSCI_B0 TX interrupt
//    }
    EUSCI_B0->IFG &=~ EUSCI_B_IFG_TXIFG0;        // Clear EUSCI_B0 TX inter. flag
}

void PORT3_IRQHandler(void){
    uint32_t status = BTN_PORT->IFG;
    if (status & RECORD_BTN){
        if (CHECK(RECORD_BTN)){
            I2CB1_burstRead(SLAVE_ADDR, 0, 6, timeDateReadback);
            interpret();
            formatData(lastAdd);
            I2CB1_burstWrite(EEPROM_ADD, lastAdd, 4, dataEEPROM);
            P1->OUT ^= BIT0;

            if (lastAdd < 16){
                lastAdd += 4;
            }else{
                lastAdd = 0;
            }
        }
    }
    if (status & READ_BTN){
        if (CHECK(READ_BTN)){
            I2CB1_burstRead(EEPROM_ADD, check, 4, readEEPROM);
            P1->OUT ^= BIT0;

            if (check < 16){
                check += 4;
            }else{
                check = 0;
            }
        }
    }
    BTN_PORT->IFG &= ~ BTNS;
}
