/*
 * 7Segment.c
 *
 *  Created on: Oct 24, 2021
 *      Author: renzogarzamotta
 */

#include "msp.h"
#include "systick.h"
#include "7Segment.h"

void setupSeg(void){    //Set up all modules using the write function
    uint8_t i;

    for(i = 0; i < UNIT_COUNT; i++){
        write(i, DECODE_MODE_ADD, DECODE_DIG_7);
    }

    for(i = 0; i < UNIT_COUNT; i++){
        write(i, INTENSITY_ADD, INT_17_32);
    }

    for(i = 0; i < UNIT_COUNT; i++){
        write(i, SCAN_LIMIT_ADD, SCAN_0_7);
    }

    for(i = 0; i < UNIT_COUNT; i++){
        write(i, SHUTDOWN_ADD, NORMAL_OPERATION);
    }

    for(i = 0; i < UNIT_COUNT; i++){
        write(i, DISPLAY_TEST_ADD, NORMAL_OP_MODE);
    }
}

void segmentInit(void){ //Initialize the EUSCI Module
    EUSCI_A1->CTLW0 |=  EUSCI_A_CTLW0_SWRST;    //Set UCSWRST

    EUSCI_A1->CTLW0 |= (EUSCI_A_CTLW0_CKPL|     //Clock Polarity
                        EUSCI_A_CTLW0_MSB|      //Most Significant Bit First
                        EUSCI_A_CTLW0_MST|      //Master Mode
                        EUSCI_A_CTLW0_SYNC|     //Synchronous
                        EUSCI_A_CTLW0_SSEL__SMCLK);     //Source Submaster Clock

    EUSCI_A1->BRW = 5;

    SEG_PORT->SEL0 |= SEG_SPI;      //Initialize SPI module as primary function
    SEG_PORT->SEL1 &-~ SEG_SPI;

    SEG_PORT->SEL0 &=~ SEG_CS;      //Set up GPIO for Chip Select Pin
    SEG_PORT->SEL1 &=~ SEG_CS;
    SEG_PORT->DIR |= SEG_CS;        //CS pin as output
    SEG_PORT->OUT |= SEG_CS;

    EUSCI_A1->CTLW0 &=~  EUSCI_A_CTLW0_SWRST;//Set UCSWRST
    setupSeg();
}

void writeSPI(uint8_t address, uint8_t data){   //Send two bytes of data to the shift buffer.
    while(!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG));    // wait until EUSCI_B3->TXBUF empty
    EUSCI_A1->TXBUF = address;                        // data out
    while(!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG));    // wait until EUSCI_B3->TXBUF empty
    EUSCI_A1->TXBUF = data;                        // data out
    while(!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG));    // wait until EUSCI_B3->TXBUF empty
}

//Count starts from last module connected (0) to first module (n)
void write(uint8_t unit, uint8_t address, uint8_t data){
    uint8_t i;
    SEG_PORT->OUT &=~ SEG_CS;    //Drive CS low
    for(i = 0; i < UNIT_COUNT; i++){    //Cycle through the modules
        if(i == unit){                  //If the module to send data to is in the queue
            writeSPI(address, data);    //Send data
        }else{
            writeSPI(NO_OP_ADD,0x00);        //Else, send a no-op
        }
    }
    SEG_PORT->OUT |= SEG_CS;    //Drive CS High
}

void encodeDigits(uint8_t unit, uint32_t num){
//    uint16_t temp;
//    uint8_t thousands, hundreds, tens, ones;
//    temp = num;
//    thousands = (((temp / 10) / 100) % 100);    //Extract the thousands place
//    temp = num;
//    hundreds = ((temp / 10) / 10) % 10;         //Extract the hundreds place
//    temp = num;
//    tens = ((temp / 10) / 1) % 10;              //Extract the tens place
//    temp = num;
//    ones = ((temp / 1) / 1) % 10;               //Extract the ones place
//
//    if(section == 0){
//        write(unit, DIG_0_ADD, ones);  //Display the ones
//        write(unit, DIG_1_ADD, tens);  //Display the tens
//        write(unit, DIG_2_ADD, hundreds);  //Display the hundreds
//        write(unit, DIG_3_ADD, thousands);  //Display the thousands
//    }else{
//        write(unit, DIG_4_ADD, ones);  //Display the ones
//        write(unit, DIG_5_ADD, tens);  //Display the tens
//        write(unit, DIG_6_ADD, hundreds);  //Display the hundreds
//        write(unit, DIG_7_ADD, thousands);  //Display the thousands
//    }
    uint32_t temp;
    uint8_t dig[8];

    temp = num;
    dig[7] = temp / 10000000;
    temp = num;
    dig[6] = (temp % 10000000) / 1000000;
    temp = num;
    dig[5] = (temp % 1000000) / 100000;
    temp = num;
    dig[4] = (temp % 100000) / 10000;
    temp = num;
    dig[3] = (temp % 10000) / 1000;
    temp = num;
    dig[2] = (temp % 1000) / 100;
    temp = num;
    dig[1] = (temp % 100) / 10;
    temp = num;
    dig[0] = (temp % 10) / 1;

    write(unit, DIG_0_ADD, dig[0]);
    write(unit, DIG_1_ADD, dig[1]);
    write(unit, DIG_2_ADD, dig[2]);
    write(unit, DIG_3_ADD, dig[3]);
    write(unit, DIG_4_ADD, dig[4]);
    write(unit, DIG_5_ADD, dig[5]);
    write(unit, DIG_6_ADD, dig[6]);
    write(unit, DIG_7_ADD, dig[7]);
}

void clearDisplay(uint8_t unit){    //Clear all digits in given module
    write(unit, DIG_0_ADD, BLANK);
    write(unit, DIG_1_ADD, BLANK);
    write(unit, DIG_2_ADD, BLANK);
    write(unit, DIG_3_ADD, BLANK);
    write(unit, DIG_4_ADD, BLANK);
    write(unit, DIG_5_ADD, BLANK);
    write(unit, DIG_6_ADD, BLANK);
    write(unit, DIG_7_ADD, BLANK);
}

void clearAll(void){    //Clear all digits in all modules
    uint8_t i;
    for(i = 0; i < UNIT_COUNT; i++){
        clearDisplay(i);
    }
}

void sendAddSPI(uint8_t address){
    SEG_PORT->OUT &=~ SEG_CS;    //Drive CS low
    usDelay(200);
    while(!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG));    // wait until EUSCI_B3->TXBUF empty
    EUSCI_A1->TXBUF = address;                        // data out
}

void sendDataSPI(uint8_t data){
    while(!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG));    // wait until EUSCI_B3->TXBUF empty
    EUSCI_A1->TXBUF = data;                        // data out
    while(!(EUSCI_A1->IFG & EUSCI_A_IFG_TXIFG));    // wait until EUSCI_B3->TXBUF empty
    usDelay(200);
    SEG_PORT->OUT |= SEG_CS;    //Drive CS High
}
