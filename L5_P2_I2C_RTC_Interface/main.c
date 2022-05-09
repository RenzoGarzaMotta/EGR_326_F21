#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msp.h"
#include "Keypad.h"
#include "SysTick.h"

void I2C1_init(void);
int I2C1_burstWrite(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data);
int I2C1_burstRead(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data);
int I2C1_Write(int slaveAddr, unsigned char memAddr, unsigned char data);
void pin(char PIN[]);
uint8_t secManage(void);
uint8_t minManage(void);
uint8_t hrManage(void);
uint8_t dayManage(void);
uint8_t dateManage(void);
uint8_t monthManage(void);
uint8_t yearManage(void);
void manageData(void);
void secInt(void);
void minInt(void);
void hrInt(void);
void dayInt(void);
void dateInt(void);
void monthInt(void);
void yearInt(void);
void interpret(void);
void reset(void);
void tempInt(void);

#define SLAVE_ADDR  0x68        /* 1101 000.    DS1337 (RTC Slave Address)*/
#define SDA         BIT4        // Serial Data Line
#define SCL         BIT5        //Serial Clock Line
#define I2C         (SDA|SCL)   //Reference to both SDA and SCL
#define I2C_PORT    P6          //I2C Port


/*  Global Variable Definition*/
unsigned char timeDateReadback[20] = {0};
unsigned char data[18] = {0};
//unsigned char data[19] = {0x00/*Sec*/,            0x18/*Min*/,          0x72/*Hr*/,          0x03/*Day*/,           0x24/*Date*/,
//                          0x11/*Month/Century*/,  0x20/*Year*/,         0x00/*A1Sec*/,       0x00/*A1Min*/,         0x00/*A1Hr*/,
//                          0x00/*A1Day/Date*/,     0x00/*A2Min*/,        0x00/*A2Hr*/,        0x00/*A2Day/Date*/,    0x1C/*Control*/,
//                          0x8A/*Control/Status*/, 0x00/*Aging Offset*/, 0x00/*MSB of Temp*/, 0x00/*LSB of Temp*/};
extern int terminate;
extern char keyPress;
int state = 0, prnt, month, date, day, hour, period, min, sec, century;
int secRead, minRead, hrRead, dayRead, dateRead, monthRead, yrRead;
char *monthPrint[20];
char *dayPrint[20];
char *periodPrint[5];
int ten = 0, twnt = 0, ind = 0, result = 0, dataWrite = 0, year = 0, format = 1, dec, qrtr;
int conv = 1;
char PIN[4] = {0}, var[4] = {0};
float temp;

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    KEYPAD_INIT();  //Initialize Keypad
    sysTick_init(); //Initialize SysTick Timer for delay usage
    I2C1_init();    //Initialize I1C protocol

    char val;
    puts("Change the current date/time on the RTC");

//    I2C1_burstRead(SLAVE_ADDR, 0, 19, timeDateReadback);
//    interpret();

    while(1){
        switch(state){
            case 0: // Collect Year data
                if(prnt == 0){
                    puts("Please enter Year (YYYY)");   // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                     // READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    year = atoi(PIN);                   // Retrieve keypad buffer data
                    reset();
                    state++;                            // Increment state
                }

                val = keyPress;
                if(val && (keyPress != '#')){           //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 1:
                if(prnt == 0){
                    puts("Please enter Month (MM)"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                     // READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    month = atoi(PIN);                   // Retrieve keypad buffer data
                    reset();
                    state++;                            // Increment state
                }

                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 2:
                if(prnt == 0){
                    puts("Please enter Date(DD)"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    date = atoi(PIN);
                    reset();
                    state++;                            // Increment state
                }

                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 3:
                if(prnt == 0){
                    puts("Please enter Day(W)"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    PIN[2] = '0';
                    day = atoi(PIN);
                    reset();
                    state++;                            // Increment state
                }

                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 4:
                if(prnt == 0){
                    puts("Please enter hour format (12h(1)/24h(0))"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    PIN[2] = '0';
                    format = atoi(PIN);
                    reset();
                    state++;                            // Increment state
                }
                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 5:
                if(prnt == 0){
                    puts("Please enter Hour(hh)"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    hour = atoi(PIN);
                    reset();
                    state++;
                }
                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 6:
                if(prnt == 0){
                    puts("Please enter Minute(mm)"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    min = atoi(PIN);
                    reset();
                    state++;
                }
                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 7:
                if(prnt == 0){
                    puts("Please enter Seconds(ss)"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    sec = atoi(PIN);
                    reset();
                    if(format == 0){ //24hr format selected
                        state += 2;
                    }else{           //12hr format selected
                        state++;
                    }
                }
                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 8:
                if(prnt == 0){
                    puts("Please enter (AM(0)/PM(1))"); // Display data request to user
                    prnt = 1;
                }

                if(terminate != 1){                             //READ KEYPAD AS LONG AS # HASN'T BEEN PRESSED
                    READ_KEYPAD();
                }else{
                    PIN[0] = '0';
                    PIN[1] = '0';
                    PIN[2] = '0';
                    period = atoi(PIN);
                    reset();
                    state++;
                }
                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                break;
            case 9:
                manageData();
                I2C1_burstWrite(SLAVE_ADDR, 0, 7, data);
                PIN[0] = '0';
                PIN[1] = '0';
                PIN[2] = '0';
                PIN[3] = '0';
                state++;
                break;
            case 10:
                if(prnt == 0){
                    puts("1 - Date");   // Display data request to user
                    puts("2 - Time");   // Display data request to user
                    puts("3 - Temp");   // Display data request to user
                    puts("Press \'*\' to confirm");   // Display data request to user
                    prnt = 1;
                }
                READ_KEYPAD();
                val = keyPress;
                if(val && (keyPress != '#')){                   //ONLY PRINT ALLOWED INPUT
                    PRINT_KEYS(val);
                    pin(PIN);                                   //SHIFT ALLOCATION OF PIN VALUES IN ARRAY
                }
                if((keyPress == '*') /*(PIN[2] == '*')*/ && (PIN[3] == '1')){
                    I2C1_burstRead(SLAVE_ADDR, 0, 19, timeDateReadback);
//                    for(i = 0; i <= 19; i++){
//                        printf("\n%d - 0x%x", i, timeDateReadback[i]);
//                    }
                    interpret();
                    puts("\nDate:");
                    printf("%s, %s %d 20%d\n", *dayPrint, *monthPrint, date, year);
                    reset();
                    PIN[0] = '0';
                    PIN[1] = '0';
                    PIN[2] = '0';
                    PIN[3] = '0';
                } else if((keyPress == '*') /*(PIN[2] == '*')*/ && (PIN[3] == '2')){
                    I2C1_burstRead(SLAVE_ADDR, 0, 19, timeDateReadback);
                    interpret();
                    puts("\nTime:");
                    if(format == 0){ //24hr format selected
                        printf("%d:%d:%d\n", hrRead, minRead, secRead);
                    }else{           //12hr format selected
                        printf("%d:%d:%d %s\n", hrRead, minRead, secRead, *periodPrint);
                    }
                    reset();
                    PIN[0] = '0';
                    PIN[1] = '0';
                    PIN[2] = '0';
                    PIN[3] = '0';
                } else if((keyPress == '*') /*(PIN[2] == '*')*/ && (PIN[3] == '3')){
                    I2C1_burstRead(SLAVE_ADDR, 0, 19, timeDateReadback);
                    interpret();
                    puts("\nTemperature:");
                    printf("%g C\n", temp);
                    reset();
                    PIN[0] = '0';
                    PIN[1] = '0';
                    PIN[2] = '0';
                    PIN[3] = '0';
                }
                break;
        }
        //msDelay(50);
    }
}

void pin(char PIN[]){
    if(keyPress != '*'){                                //SHIFT PIN VALUES FROM ONE INDEX TO THE ONE PREVIOUS
        PIN[0] = PIN[1];
        PIN[1] = PIN[2];
        PIN[2] = PIN[3];
        PIN[3] = keyPress;
    }
}

/* configure UCB1 as I2C */
void I2C1_init(void) {
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;       /* disable UCB1 during config */
    EUSCI_B1->CTLW0 = 0x0F81;   /* // 0x0F81 // 7-bit slave addr, master, I2C, synch mode, use SMCLK */
    EUSCI_B1->BRW = 30;         /* set clock prescaler 3MHz / 30 = 100kHz */
    I2C_PORT->SEL0 |= I2C;           /* P6.5, P6.4 for UCB1 */
    I2C_PORT->SEL1 &=~ I2C;
    EUSCI_B1->CTLW0 &= ~1;      /* enable UCB1 after config */
}

/* Write a single byte at memAddr
 * write: S-(slaveAddr+w)-ACK-memAddr-ACK-data-ACK-P
 */
int I2C1_Write(int slaveAddr, unsigned char memAddr, unsigned char data) {
    EUSCI_B1->I2CSA = slaveAddr;    /* setup slave address */
    EUSCI_B1->CTLW0 |= 0x0010;      /* enable transmitter */
    EUSCI_B1->CTLW0 |= 0x0002;      /* generate START and send slave address */
    while((EUSCI_B1->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B1->TXBUF = memAddr;      /* send memory address to slave */
    while(!(EUSCI_B1->IFG & 2));    /* wait till it's ready to transmit */
    EUSCI_B1->TXBUF = data;         /* send data to slave */
    while(!(EUSCI_B1->IFG & 2));    /* wait till last transmit is done */
    EUSCI_B1->CTLW0 |= 0x0004;      /* send STOP */
    while(EUSCI_B1->CTLW0 & 4) ;    /* wait until STOP is sent */
    return 0;                       /* no error */
}

int I2C1_burstWrite(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data){
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
        EUSCI_B1->TXBUF = *data++;    /* send data to slave */
        byteCount--;
     } while (byteCount > 0);

    while(!(EUSCI_B1->IFG & 2));      /* wait till last transmit is done */
    EUSCI_B1->CTLW0 |= 0x0004;        /* send STOP */
    while(EUSCI_B1->CTLW0 & 4) ;      /* wait until STOP is sent */

    return 0;                   /* no error */
}

int I2C1_burstRead(int slaveAddr, unsigned char memAddr, int byteCount, unsigned char* data) {
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
        *data++ = EUSCI_B1->RXBUF;    /* read the received data */
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

void reset(void){
    PIN[0] = '0';
    PIN[1] = '0';
    PIN[2] = '0';
    PIN[3] = '0';
    puts("\n");
    prnt = 0;                           // Reset data request print flag
    terminate = 0;                      // Reset keypad input flag
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
