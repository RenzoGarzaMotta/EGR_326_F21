/*
 * RTC.c
 *
 *  Created on: Oct 6, 2021
 *      Author: renzogarzamotta
 */
#include "msp.h"
#include "macros.h"
#include "RTC.h"

void setupI2C(void){
    //Set up I2C protocol
    //P6.4 SDA
    //P6.5 CLK
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         //Set Software reset

    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_MST|          //I2C Mode
                        EUSCI_B_CTLW0_MODE_3|       //Master Mode
                        EUSCI_B_CTLW0_SSEL__SMCLK); //SMCLK

    I2C_PORT->SEL0 |=   I2C;
    I2C_PORT->SEL1 &=~  I2C;

    EUSCI_B0->BRW = UCBRx;                          //Set Bit clock prescaler
    EUSCI_B0->CTLW0 &=~ EUSCI_B_CTLW0_SWRST;        //Clear software reset
}

void burstWrite(uint8_t slaveAddress, uint8_t memoryAddress, uint16_t byteCount, uint8_t* data){
    int i = 0;

    if(byteCount > 0){                                  //Only get data is there is more data to read
        EUSCI_B0->I2CSA = slaveAddress;                 //Setup slave address
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;            //Enable transmitter
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;         //Generate START and send slave address
        while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);   //Wait until slave address is sent
        EUSCI_B0->TXBUF = memoryAddress;                //Send memory address to slave

        //Send data one byte at a time
        do {
            while(!(EUSCI_B0->IFG & EUSCI_B_CTLW0_TXSTT));  //Wait until module is ready to transmit
            EUSCI_B0->TXBUF = data[i++];                    //Place data in buffer
            byteCount--;                                    //Count down byteCount
        } while (byteCount > 0);

        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));   //Wait until last transmit is done
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;         //Setup to send STOP after the last byte is received
        while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP) ;  //Wait until STOP is sent
    }
}

void burstRead(uint8_t slaveAddress, uint8_t memoryAddress, uint16_t byteCount, uint8_t* data){
    int i = 0;
//    if(byteCount > 0){                                  //Only get data is there is more data to read
        EUSCI_B0->I2CSA = slaveAddress;                 //Setup slave address
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;            //Enable transmitter
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;         //Generate START and send slave address
        while((EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT)); //Wait until slave address is sent
        EUSCI_B0->TXBUF = memoryAddress;                //Send memory address to slave


        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));   //Wait until last transmit is done
        EUSCI_B0->CTLW0 &=~ EUSCI_B_CTLW0_TR;           //Enable receiver
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;         //Generate RESTART and send slave address
        while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);   //Wait till RESTART is finished

        //Receive data one byte at a time
        do {
            if (byteCount == 1)                             //When only one byte of data is left
                EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;     //Setup to send STOP after the last byte is received

            while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0));   //Wait until data is received
            data[i++] = EUSCI_B0->RXBUF;                    //Store received data in array
            byteCount--;                                    //Count down byteCount
        }while (byteCount);                                 //Cycle until byteCount is 0

        while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP) ;      //Wait until STOP is sent
//    }
}

void encode(uint8_t* data){

}
void encodeSeconds(uint8_t* data){
    seconds = data[0];
}
void encodeMinutes(uint8_t* data){
    minutes = data[1];
}
void encodeHours(uint8_t* data){
    hours = data[2];
}
void encodeDay(uint8_t* data){
    day = data[3];
}
void encodeDate(uint8_t* data){
    date = data[4];
}
void encodeMonth(uint8_t* data){
    month = data[5];
}
void encodeYear(uint8_t* data){
    year = data[6];
}
void encodeSecondsA1(uint8_t* data){
    secondsA1 = data[7];
}
void encodeMinutesA1(uint8_t* data){
    minutesA1 = data[8];
}
void encodeHoursA1(uint8_t* data){
    hoursA1 = data[9];
}
void encodeDayA1(uint8_t* data){
    dayA1 = data[10];
}
void encodeDateA1(uint8_t* data){
    dateA1 = data[10];
}
void encodeMinutesA2(uint8_t* data){
    minutesA2 = data[11];
}
void encodeHoursA2(uint8_t* data){
    hoursA2 = data[12];
}
void encodeDayA2(uint8_t* data){
    dayA2 = data[13];
}
void encodeDateA2(uint8_t* data){
    dateA2 = data[13];
}
void encodeControl(uint8_t* data){
    control = data[14];
}
void encodeStatus(uint8_t* data){
    status = data[15];
}
void encodeAgingOffset(uint8_t* data){
    agingOffset = data[16];
}
void encodeTempMSB(uint8_t* data){
    tempMSB = data[17];
}
void encodeTempLSB(uint8_t* data){
    tempLSB = data[18];
}




void decode(uint8_t* data){
    decodeSeconds(data);
    decodeMinutes(data);
    decodeHours(data);
    decodeDay(data);
    decodeDate(data);
    decodeMonth(data);
    decodeYear(data);
    decodeSecondsA1(data);
    decodeMinutesA1(data);
    decodeHoursA1(data);
    decodeDayA1(data);
    decodeDateA1(data);
    decodeMinutesA2(data);
    decodeHoursA2(data);
    decodeDayA2(data);
    decodeDateA2(data);
    decodeControl(data);
    decodeStatus(data);
    decodeAgingOffset(data);
    decodeTempMSB(data);
    decodeTempLSB(data);
}

void decodeSeconds(uint8_t* data){//DONE
    uint8_t tens, ones, temp;
    temp = data[0];
    tens = temp >> 4;    //Shift 0x00 register to get 4 MSB
    ones = temp &=~ 0xF0;  //Clear upper nibble

    seconds = (tens * 10) + (ones * 1);
}

void decodeMinutes(uint8_t* data){//DONE
    uint8_t tens, ones, temp;
    temp = data[1];
    tens = temp >> 4;    //Shift 0x00 register to get 4 MSB
    ones = temp &=~ 0xF0;  //Clear upper nibble

    minutes = (tens * 10) + (ones * 1);
}

void decodeHours(uint8_t* data){//DONE
    uint8_t twenty, tens, ones, temp;
    //re-collect data from data array since clearing bits is a destructive operation
    temp = data[2];
    clockSetting = (temp &=~ 0b10111111) >> 6; //Determine if clock is in 12hr or 24hr mode

    if(clockSetting == 1){          //12 Hour (1)
        temp = data[2];
        meridiem = (temp &=~ 0b11011111) >> 5;
        if(meridiem == 0){          //AM
            //PRINT AM
        }else if (meridiem == 1){    //PM
            //PRINT PM
        }
        temp = data[2];
        tens = (temp &=~ 0b11101111) >> 4;  //Shift data to get 10 Hour bit
        temp = data[2];
        ones = (temp &=~ 0xF0);  //Shift data to get Hour bit
        hours = (tens * 10) + (ones * 1);
    }else if(clockSetting == 0){    //24 Hour (0)
        temp = data[2];
        twenty = (temp &=~ 0b11011111) >> 5;  //Shift data to get 20 Hour bit
        temp = data[2];
        tens = (temp &=~ 0b11101111) >> 4;  //Shift data to get 10 Hour bit
        temp = data[2];
        ones = (temp &=~ 0xF0);  //Shift data to get Hour bit
        hours = (twenty * 20) + (tens * 10) + (ones * 1);
    }
}

void decodeDay(uint8_t* data){//DONE
    day = data[3];
}

void decodeDate(uint8_t* data){//DONE
    uint8_t tens, ones, temp;

    temp = data[4];
    tens = temp >> 4;    //Shift 0x00 register to get 4 MSB
    temp = data[4];
    ones = temp &=~ 0xF0;  //Clear upper nibble

    date = (tens * 10) + (ones * 1);
}

void decodeMonth(uint8_t* data){//DONE
    uint8_t tens, ones, temp;

    temp = data[5];
    century = (temp &=~ 0b01111111) >> 7;

    temp = data[5];
    tens = (temp &=~ 0b11101111) >> 4;    //Shift 0x00 register to get 4 MSB
    temp = data[5];
    ones = (temp &=~ 0xF0);  //Clear upper nibble

    month = (tens * 10) + (ones * 1);
}

void decodeYear(uint8_t* data){//DONE
    uint8_t tens, ones, temp;

    temp = data[6];
    tens = temp >> 4;    //Shift 0x00 register to get 4 MSB
    temp = data[6];
    ones = temp &=~ 0xF0;  //Clear upper nibble

    year = (tens * 10) + (ones * 1);
}

void decodeSecondsA1(uint8_t* data){//DONE
    uint8_t tens, ones, temp;

    temp = data[7];
    bitA1M1 = (temp &=~ 0b01111111) >> 7;

    temp = data[7];
    tens = (temp &=~ 0b10000000) >> 4;    //Shift 0x00 register to get 4 MSB
    temp = data[7];
    ones = temp &=~ 0xF0;  //Clear upper nibble

    secondsA1 = (tens * 10) + (ones * 1);
}

void decodeMinutesA1(uint8_t* data){//DONE
    uint8_t tens, ones, temp;

    temp = data[8];
    bitA1M2 = (temp &=~ 0b01111111) >> 7;

    temp = data[8];
    tens = (temp &=~ 0b10000000) >> 4;    //Shift 0x00 register to get 4 MSB
    temp = data[8];
    ones = temp &=~ 0xF0;  //Clear upper nibble

    minutesA1 = (tens * 10) + (ones * 1);
}

void decodeHoursA1(uint8_t* data){//DONE
    uint8_t twenty, tens, ones, temp;
    //re-collect data from data array since clearing bits is a destructive operation
    temp = data[9];
    bitA1M3 = (temp &=~ 0b01111111) >> 7;
    temp = data[9];
    clockSettingA1 = (temp &=~ 0b10111111) >> 6; //Determine if clock is in 12hr or 24hr mode

    if(clockSettingA1 == 1){          //12 Hour (1)
        temp = data[9];
        meridiemA1 = (temp &=~ 0b11011111) >> 5;
        if(meridiemA1 == 0){          //AM
            //PRINT AM
        }else if (meridiemA1 == 1){    //PM
            //PRINT PM
        }
        temp = data[9];
        tens = (temp &=~ 0b11101111) >> 4;  //Shift data to get 10 Hour bit
        temp = data[9];
        ones = (temp &=~ 0xF0);  //Shift data to get Hour bit
        hoursA1 = (tens * 10) + (ones * 1);
    }else if(clockSettingA1 == 0){    //24 Hour (0)
        temp = data[9];
        twenty = (temp &=~ 0b11011111) >> 5;  //Shift data to get 20 Hour bit
        temp = data[9];
        tens = (temp &=~ 0b11101111) >> 4;  //Shift data to get 10 Hour bit
        temp = data[9];
        ones = (temp &=~ 0xF0);  //Shift data to get Hour bit
        hoursA1 = (twenty * 20) + (tens * 10) + (ones * 1);
    }
}

void decodeDayA1(uint8_t* data){//NOT USED

    dayA1 = data[10];
}

void decodeDateA1(uint8_t* data){//NOT USED
//    uint8_t tens, ones, temp;

    dateA1 = data[10];
}

void decodeMinutesA2(uint8_t* data){//DONE
    uint8_t tens, ones, temp;

    temp = data[11];
    bitA2M2 = (temp &=~ 0b01111111) >> 7;

    temp = data[11];
    tens = (temp &=~ 0b10000000) >> 4;    //Shift 0x00 register to get 4 MSB
    temp = data[11];
    ones = temp &=~ 0xF0;  //Clear upper nibble

    minutesA2 = (tens * 10) + (ones * 1);
}

void decodeHoursA2(uint8_t* data){//NOT USED


    hoursA2 = data[12];
}

void decodeDayA2(uint8_t* data){//NOT USED


    dayA2 = data[13];
}

void decodeDateA2(uint8_t* data){//NOT USED


    dateA2 = data[13];
}

void decodeControl(uint8_t* data){//NOT USED


    control = data[14];
}

void decodeStatus(uint8_t* data){//NOT USED


    status = data[15];
}

void decodeAgingOffset(uint8_t* data){//NOT USED


    agingOffset = data[16];
}

void decodeTempMSB(uint8_t* data){//DONE
    uint8_t temp;
    temp = data[17];
    tempMSB = (temp &=~ 0x80);
}

void decodeTempLSB(uint8_t* data){//DONE
    uint8_t temp;
    temp = data[18];
    tempLSB = (temp >> 6) * 0.25;
}
