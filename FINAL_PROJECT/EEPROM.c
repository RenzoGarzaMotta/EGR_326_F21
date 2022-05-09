/*
 * EEPROM.c
 *
 *  Created on: Dec 6, 2021
 *      Author: renzogarzamotta
 */
#include "msp.h"
#include "systick.h"
#include "macros.h"
#include "RTC.h"
#include "EEPROM.h"



int I2C1_Write(int slaveAddr, unsigned char memAddr, unsigned char data){
    EUSCI_B0->I2CSA = slaveAddr;    /* setup slave address */
    EUSCI_B0->CTLW0 |= 0x0010;      /* enable transmitter */
    EUSCI_B0->CTLW0 |= 0x0002;      /* generate START and send slave address */
    while((EUSCI_B0->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B0->TXBUF = memAddr;      /* send memory address to slave */
    while(!(EUSCI_B0->IFG & 2));    /* wait till it's ready to transmit */
    EUSCI_B0->TXBUF = data;         /* send data to slave */
    while(!(EUSCI_B0->IFG & 2));    /* wait till last transmit is done */
    EUSCI_B0->CTLW0 |= 0x0004;      /* send STOP */
    while(EUSCI_B0->CTLW0 & 4) ;    /* wait until STOP is sent */

    __delay_cycles(200000);         //Delay to allow for data to be fully written

    return 0;                       /* no error */
}

int I2C1_Read(int slaveAddr, unsigned char memAddr, unsigned char* data){
    EUSCI_B0->I2CSA = slaveAddr;    /* setup slave address */
    EUSCI_B0->CTLW0 |= 0x0010;      /* enable transmitter */
    EUSCI_B0->CTLW0 |= 0x0002;      /* generate START and send slave address */
    while((EUSCI_B0->CTLW0 & 2));   /* wait until slave address is sent */
    EUSCI_B0->TXBUF = memAddr;      /* send memory address to slave */
    while(!(EUSCI_B0->IFG & 2));    /* wait till it's ready to transmit */
    EUSCI_B0->CTLW0 &= ~0x0010;     /* enable receiver */
    EUSCI_B0->CTLW0 |= 0x0002;      /* generate RESTART and send slave address */
    while(EUSCI_B0->CTLW0 & 2);     /* wait till restart is finished */
    EUSCI_B0->CTLW0 |= 0x0004;      /* setup to send STOP after the byte is received */
    while(!(EUSCI_B0->IFG & 1));    /* wait till data is received */

    __delay_cycles(48000);          //Delay to allow for proper read function.

    *data = EUSCI_B0->RXBUF;        /* read the received data */
    while(EUSCI_B0->CTLW0 & 4) ;    /* wait until STOP is sent */
    return 0;                       /* no error */
}

void writeEEPROM(void){
    encodeEEPROM(sendEEPROMData);   //Encode data to write to EEPROM

    I2C1_Write(EEPROM_ADDRESS, 0, sendEEPROMData[0]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 1, sendEEPROMData[1]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 2, sendEEPROMData[2]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 3, sendEEPROMData[3]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 4, sendEEPROMData[4]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 5, sendEEPROMData[5]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 6, sendEEPROMData[6]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 7, sendEEPROMData[7]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 8, sendEEPROMData[8]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 9, sendEEPROMData[9]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 10, sendEEPROMData[10]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 11, sendEEPROMData[11]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 12, sendEEPROMData[12]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 13, sendEEPROMData[13]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 14, sendEEPROMData[14]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 15, sendEEPROMData[15]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 16, sendEEPROMData[16]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 17, sendEEPROMData[17]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 18, sendEEPROMData[18]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 19, sendEEPROMData[19]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 20, sendEEPROMData[20]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 21, sendEEPROMData[21]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 22, sendEEPROMData[22]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 23, sendEEPROMData[23]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 24, sendEEPROMData[24]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 25, sendEEPROMData[25]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 26, sendEEPROMData[26]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 27, sendEEPROMData[27]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 28, sendEEPROMData[28]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 29, sendEEPROMData[29]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 30, sendEEPROMData[30]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 31, sendEEPROMData[31]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 32, sendEEPROMData[32]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 33, sendEEPROMData[33]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 34, sendEEPROMData[34]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 35, sendEEPROMData[35]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 36, sendEEPROMData[36]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 37, sendEEPROMData[37]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 38, sendEEPROMData[38]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 39, sendEEPROMData[39]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 40, sendEEPROMData[40]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 41, sendEEPROMData[41]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 42, sendEEPROMData[42]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 43, sendEEPROMData[43]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 44, sendEEPROMData[44]);   //Write to EEMPROM one byte at a time

    I2C1_Write(EEPROM_ADDRESS, 45, sendEEPROMData[45]);   //Write to EEMPROM one byte at a time
}

void encodeEEPROM(uint8_t* data){
    encodeEntry1(data);
    encodeEntry2(data);
    encodeEntry3(data);
    encodeEntry4(data);
    encodeEntry5(data);
    encodeEntry6(data);
    encodeSettings(data);
    encodeOdometer(data);
}

void encodeEntry1(uint8_t* data){
    data[0] = newEntry1;    //store data with minimal encoding
    data[1] = newMonth1;    //store data with minimal encoding
    data[2] = newDay1;      //store data with minimal encoding
    data[3] = newHour1;     //store data with minimal encoding
    data[4] = newMinute1;   //store data with minimal encoding
    data[5] = newSecond1;   //store data with minimal encoding
}

void encodeEntry2(uint8_t* data){
    data[6] = newEntry2;    //store data with minimal encoding
    data[7] = newMonth2;    //store data with minimal encoding
    data[8] = newDay2;      //store data with minimal encoding
    data[9] = newHour2;     //store data with minimal encoding
    data[10] = newMinute2;  //store data with minimal encoding
    data[11] = newSecond2;  //store data with minimal encoding
}

void encodeEntry3(uint8_t* data){
    data[12] = newEntry3;   //store data with minimal encoding
    data[13] = newMonth3;   //store data with minimal encoding
    data[14] = newDay3;     //store data with minimal encoding
    data[15] = newHour3;    //store data with minimal encoding
    data[16] = newMinute3;  //store data with minimal encoding
    data[17] = newSecond3;  //store data with minimal encoding
}

void encodeEntry4(uint8_t* data){
    data[18] = newEntry4;   //store data with minimal encoding
    data[19] = newMonth4;   //store data with minimal encoding
    data[20] = newDay4;     //store data with minimal encoding
    data[21] = newHour4;    //store data with minimal encoding
    data[22] = newMinute4;  //store data with minimal encoding
    data[23] = newSecond4;  //store data with minimal encoding
}

void encodeEntry5(uint8_t* data){
    data[24] = newEntry5;   //store data with minimal encoding
    data[25] = newMonth5;   //store data with minimal encoding
    data[26] = newDay5;     //store data with minimal encoding
    data[27] = newHour5;    //store data with minimal encoding
    data[28] = newMinute5;  //store data with minimal encoding
    data[29] = newSecond5;  //store data with minimal encoding
}

void encodeEntry6(uint8_t* data){
    data[30] = newEntry6;   //store data with minimal encoding
    data[31] = newMonth6;   //store data with minimal encoding
    data[32] = newDay6;     //store data with minimal encoding
    data[33] = newHour6;    //store data with minimal encoding
    data[34] = newMinute6;  //store data with minimal encoding
    data[35] = newSecond6;  //store data with minimal encoding
}

void encodeSettings(uint8_t* data){
    data[36] = newTempUnits;    //store data with minimal encoding
    data[37] = newSpeedUnits;   //store data with minimal encoding
    data[38] = newAppearance;   //store data with minimal encoding
    data[39] = newBrightness;   //store data with minimal encoding
    data[40] = newTextColor;    //store data with minimal encoding
    data[41] = newBgColor;      //store data with minimal encoding
}

void encodeOdometer(uint8_t* data){
    uint8_t i;
    uint8_t bit[32];
    uint8_t byte1, byte2, byte3, byte4;
    uint32_t mask = 1;

    //Decode odometer value into a series of 1 and 0 values to re-encode into 4 bytes of data
    for(i = 0; i < 32; i++){
        if((newOdometer & mask) == 0){
            bit[i] = 0;
        }else{
            bit[i] = 1;
        }
        mask = mask << 1;
    }

    //Most Significant byte
    byte4 = 0 << 7;
    byte4 = byte4 | (0 << 6);
    byte4 = byte4 | (0 << 5);
    byte4 = byte4 | (0 << 4);
    byte4 = byte4 | (0 << 3);
    byte4 = byte4 | (bit[26] << 2); //Store most signiticant bit
    byte4 = byte4 | (bit[25] << 1);
    byte4 = byte4 | bit[24];

//    printf("%d\n", byte4); //Used for testing

    byte3 = bit[23] << 7;
    byte3 = byte3 | (bit[22] << 6);
    byte3 = byte3 | (bit[21] << 5);
    byte3 = byte3 | (bit[20] << 4);
    byte3 = byte3 | (bit[19] << 3);
    byte3 = byte3 | (bit[18] << 2);
    byte3 = byte3 | (bit[17] << 1);
    byte3 = byte3 | bit[16];

//    printf("%d\n", byte3); //Used for testing

    byte2 = bit[15] << 7;
    byte2 = byte2 | (bit[14] << 6);
    byte2 = byte2 | (bit[13] << 5);
    byte2 = byte2 | (bit[12] << 4);
    byte2 = byte2 | (bit[11] << 3);
    byte2 = byte2 | (bit[10] << 2);
    byte2 = byte2 | (bit[9] << 1);
    byte2 = byte2 | bit[8];

//    printf("%d\n", byte2); //Used for testing

    byte1 = bit[7] << 7;
    byte1 = byte1 | (bit[6] << 6);
    byte1 = byte1 | (bit[5] << 5);
    byte1 = byte1 | (bit[4] << 4);
    byte1 = byte1 | (bit[3] << 3);
    byte1 = byte1 | (bit[2] << 2);
    byte1 = byte1 | (bit[1] << 1);
    byte1 = byte1 | bit[0]; //store least significant bit

//    printf("%d\n", byte1); //Used for testing

    data[42] = byte4;   //Store the odometer in 4 byte of EEPROM data
    data[43] = byte3;
    data[44] = byte2;
    data[45] = byte1;
}

void readEEPROM(void){

    I2C1_Read(EEPROM_ADDRESS, 0, &readEEPROMData[0]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 1, &readEEPROMData[1]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 2, &readEEPROMData[2]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 3, &readEEPROMData[3]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 4, &readEEPROMData[4]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 5, &readEEPROMData[5]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 6, &readEEPROMData[6]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 7, &readEEPROMData[7]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 8, &readEEPROMData[8]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 9, &readEEPROMData[9]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 10, &readEEPROMData[10]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 11, &readEEPROMData[11]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 12, &readEEPROMData[12]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 13, &readEEPROMData[13]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 14, &readEEPROMData[14]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 15, &readEEPROMData[15]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 16, &readEEPROMData[16]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 17, &readEEPROMData[17]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 18, &readEEPROMData[18]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 19, &readEEPROMData[19]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 20, &readEEPROMData[20]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 21, &readEEPROMData[21]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 22, &readEEPROMData[22]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 23, &readEEPROMData[23]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 24, &readEEPROMData[24]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 25, &readEEPROMData[25]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 26, &readEEPROMData[26]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 27, &readEEPROMData[27]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 28, &readEEPROMData[28]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 29, &readEEPROMData[29]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 30, &readEEPROMData[30]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 31, &readEEPROMData[31]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 32, &readEEPROMData[32]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 33, &readEEPROMData[33]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 34, &readEEPROMData[34]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 35, &readEEPROMData[35]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 36, &readEEPROMData[36]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 37, &readEEPROMData[37]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 38, &readEEPROMData[38]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 39, &readEEPROMData[39]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 40, &readEEPROMData[40]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 41, &readEEPROMData[41]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 42, &readEEPROMData[42]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 43, &readEEPROMData[43]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 44, &readEEPROMData[44]);   //Read from EEMPROM one byte at a time

    I2C1_Read(EEPROM_ADDRESS, 45, &readEEPROMData[45]);   //Read from EEMPROM one byte at a time


    decodeEEPROM(readEEPROMData);   //Decode data read from EEPROM
}

void decodeEEPROM(uint8_t* data){
    decodeEntry1(data);
    decodeEntry2(data);
    decodeEntry3(data);
    decodeEntry4(data);
    decodeEntry5(data);
    decodeEntry6(data);
    decodeSettings(data);
    decodeOdometer(data);
}

void decodeEntry1(uint8_t* data){
    entry1 = data[0];   //Read data with minimal decoding
    month1 = data[1];   //Read data with minimal decoding
    day1 = data[2];     //Read data with minimal decoding
    hour1 = data[3];    //Read data with minimal decoding
    minute1 = data[4];  //Read data with minimal decoding
    second1 = data[5];  //Read data with minimal decoding
}

void decodeEntry2(uint8_t* data){
    entry2 = data[6];   //Read data with minimal decoding
    month2 = data[7];   //Read data with minimal decoding
    day2 = data[8];     //Read data with minimal decoding
    hour2 = data[9];    //Read data with minimal decoding
    minute2 = data[10]; //Read data with minimal decoding
    second2 = data[11]; //Read data with minimal decoding
}

void decodeEntry3(uint8_t* data){
    entry3 = data[12];  //Read data with minimal decoding
    month3 = data[13];  //Read data with minimal decoding
    day3 = data[14];    //Read data with minimal decoding
    hour3 = data[15];   //Read data with minimal decoding
    minute3 = data[16]; //Read data with minimal decoding
    second3 = data[17]; //Read data with minimal decoding
}

void decodeEntry4(uint8_t* data){
    entry4 = data[18];  //Read data with minimal decoding
    month4 = data[19];  //Read data with minimal decoding
    day4 = data[20];    //Read data with minimal decoding
    hour4 = data[21];   //Read data with minimal decoding
    minute4 = data[22]; //Read data with minimal decoding
    second4 = data[23]; //Read data with minimal decoding
}

void decodeEntry5(uint8_t* data){
    entry5 = data[24];  //Read data with minimal decoding
    month5 = data[25];  //Read data with minimal decoding
    day5 = data[26];    //Read data with minimal decoding
    hour5 = data[27];   //Read data with minimal decoding
    minute5 = data[28]; //Read data with minimal decoding
    second5 = data[29]; //Read data with minimal decoding
}

void decodeEntry6(uint8_t* data){
    entry6 = data[30];  //Read data with minimal decoding
    month6 = data[31];  //Read data with minimal decoding
    day6 = data[32];    //Read data with minimal decoding
    hour6 = data[33];   //Read data with minimal decoding
    minute6 = data[34]; //Read data with minimal decoding
    second6 = data[35]; //Read data with minimal decoding
}

void decodeSettings(uint8_t* data){
    tempUnitsEEPROM = data[36];     //Read data with minimal decoding
    speedUnitsEEPROM = data[37];    //Read data with minimal decoding
    appearanceEEPROM = data[38];    //Read data with minimal decoding
    brightnessEEPROM = data[39];    //Read data with minimal decoding
    textColorEEPROM = data[40];     //Read data with minimal decoding
    bgColorEEPROM = data[41];       //Read data with minimal decoding
}

void decodeOdometer(uint8_t* data){
    uint8_t temp;
    uint8_t bit[32];


    temp = data[42];
    bit[26] = (temp &=~ 0b11111011) >> 2;
    temp = data[42];
    bit[25] = (temp &=~ 0b11111101) >> 1;
    temp = data[42];
    bit[24] = (temp &=~ 0b11111110);

    temp = data[43];
    bit[23] = (temp &=~ 0b01111111) >> 7;
    temp = data[43];
    bit[22] = (temp &=~ 0b10111111) >> 6;
    temp = data[43];
    bit[21] = (temp &=~ 0b11011111) >> 5;
    temp = data[43];
    bit[20] = (temp &=~ 0b11101111) >> 4;
    temp = data[43];
    bit[19] = (temp &=~ 0b11110111) >> 3;
    temp = data[43];
    bit[18] = (temp &=~ 0b11111011) >> 2;
    temp = data[43];
    bit[17] = (temp &=~ 0b11111101) >> 1;
    temp = data[43];
    bit[16] = (temp &=~ 0b11111110);

    temp = data[44];
    bit[15] = (temp &=~ 0b01111111) >> 7;
    temp = data[44];
    bit[14] = (temp &=~ 0b10111111) >> 6;
    temp = data[44];
    bit[13] = (temp &=~ 0b11011111) >> 5;
    temp = data[44];
    bit[12] = (temp &=~ 0b11101111) >> 4;
    temp = data[44];
    bit[11] = (temp &=~ 0b11110111) >> 3;
    temp = data[44];
    bit[10] = (temp &=~ 0b11111011) >> 2;
    temp = data[44];
    bit[9] =  (temp &=~ 0b11111101) >> 1;
    temp = data[44];
    bit[8] =  (temp &=~ 0b11111110);

    temp = data[45];
    bit[7] =  (temp &=~ 0b01111111) >> 7;
    temp = data[45];
    bit[6] =  (temp &=~ 0b10111111) >> 6;
    temp = data[45];
    bit[5] =  (temp &=~ 0b11011111) >> 5;
    temp = data[45];
    bit[4] =  (temp &=~ 0b11101111) >> 4;
    temp = data[45];
    bit[3] =  (temp &=~ 0b11110111) >> 3;
    temp = data[45];
    bit[2] =  (temp &=~ 0b11111011) >> 2;
    temp = data[45];
    bit[1] =  (temp &=~ 0b11111101) >> 1;
    temp = data[45];
    bit[0] =  (temp &=~ 0b11111110);


    //Re-encode bits into decimal value into integer variable odometer.
    odometer = (bit[26] * 67108864) + (bit[25] * 33554432) + (bit[24] * 16777216) + (bit[23] * 8388608) + (bit[22] * 4194304) + (bit[21] * 2097152) + (bit[20] * 1048576) + (bit[19] * 524288) + (bit[18] * 262144) + (bit[17] * 131072) + (bit[16] * 65536) + (bit[15] * 32768) + (bit[14] * 16384) + (bit[13] * 8192) + (bit[12] * 4096) + (bit[11] * 2048) + (bit[10] * 1024) + (bit[9] * 512) + (bit[8] * 256) + (bit[7] * 128) + (bit[6] * 64) + (bit[5] * 32) + (bit[4] * 16) + (bit[3] * 8) + (bit[2] * 4) + (bit[1] * 2) + (bit[0] * 1);
}

void testData(void){
    //Dummy data used to test EEPROM functionality.

    newEntry1 = 1;
    newMonth1 = 2;
    newDay1 = 3;
    newHour1 = 4;
    newMinute1 = 5;
    newSecond1 = 6;

    newEntry2 = 12;
    newMonth2 = 11;
    newDay2 = 10;
    newHour2 = 9;
    newMinute2 = 8;
    newSecond2 = 7;

    newEntry3 = 13;
    newMonth3 = 14;
    newDay3 = 15;
    newHour3 = 16;
    newMinute3 = 17;
    newSecond3 = 18;

    newEntry4 = 24;
    newMonth4 = 23;
    newDay4 = 22;
    newHour4 = 21;
    newMinute4 = 20;
    newSecond4 = 19;

    newEntry5 = 25;
    newMonth5 = 26;
    newDay5 = 27;
    newHour5 = 28;
    newMinute5 = 29;
    newSecond5 = 30;

    newEntry6 = 36;
    newMonth6 = 35;
    newDay6 = 34;
    newHour6 = 33;
    newMinute6 = 32;
    newSecond6 = 31;

    newTempUnits = 0;
    newSpeedUnits = 1;
    newAppearance = 1;
    newBrightness = 100;
    newTextColor = 0;
    newBgColor = 0;

    newOdometer = 11111111;
}
