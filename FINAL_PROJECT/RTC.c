#include <stdint.h>
#include "msp.h"
#include "systick.h"
#include "string.h"
#include "macros.h"
#include "RTC.h"

//Indicate available temperature units
enum tempUnits{
    Celsius,
    Fahrenheit,
    Kelvin,
    Rankine,
    Delisle,
    Newton,
    Reaumur,
    Romer
};

extern uint8_t tempSelection;
extern float tempToDisplay;
extern float tempDisp;

void setupI2C(void){
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         //Set Software reset

    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_MST|          //I2C Mode
                        EUSCI_B_CTLW0_MODE_3|       //Master Mode
                        EUSCI_B_CTLW0_SSEL__SMCLK); //SMCLK

    I2C_PORT->SEL0 |=   I2C;    //Set secondary function in I2C pins
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
    if(byteCount > 0){                                  //Only get data is there is more data to read
        EUSCI_B0->I2CSA = slaveAddress;                 //Setup slave address
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;            //Enable transmitter
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;         //Generate START and send slave address
        while((EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT)); //Wait until slave address is sent
        EUSCI_B0->TXBUF = memoryAddress;                //Send memory address to slave


        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));   //Wait until last transmit is done

        __delay_cycles(60000);

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
    }
}

void encode(uint8_t* data){
    encodeSeconds(data);
    encodeMinutes(data);
    encodeHours(data);
    encodeDay(data);
    encodeDate(data);
    encodeMonth(data);
    encodeYear(data);
    encodeSecondsA1(data);
    encodeMinutesA1(data);
    encodeHoursA1(data);
    encodeDayDateA1(data);
    encodeMinutesA2(data);
    encodeHoursA2(data);
    encodeDayDateA2(data);
    encodeControl(data);
    encodeStatus(data);
    encodeAgingOffset(data);
}

void encodeSeconds(uint8_t* data){//DONE
    uint8_t tens, ones;
    tens = newSeconds  / 10;    //Get the 10s value from new data
    ones = newSeconds % 10;     //Get the 1s value from new data
    data[0] = (tens << 4) | ones;   //Store data in array for RTC
}

void encodeMinutes(uint8_t* data){//DONE
    uint8_t tens, ones;
    tens = newMinutes  / 10;    //Get the 10s value from new data
    ones = newMinutes % 10;     //Get the 1s value from new data
    data[1] = (tens << 4) | ones;   //Store data in array for RTC
}

void encodeHours(uint8_t* data){
    uint8_t twenty, tens, ones;

    if(newClockSetting == 1){//12hr Clock
        tens = newHours  / 10;  //Get the 10s value from new data
        ones = newHours  % 10;  //Get the 1s value from new data
        if(newMeridiem == 1){//PM
            //Store data in array for RTC
            data[2] = (newClockSetting << 6) | (newMeridiem << 5) | (tens << 4) | (ones);
        }else if(newMeridiem == 0){//AM
            //Store data in array for RTC
            data[2] = (newClockSetting << 6) | (newMeridiem << 5) | (tens << 4) | (ones);
        }
    }else if(newClockSetting == 0){//24hr Clock
        if(newHours > 20){  //Get the 20s value from new data
            twenty = 1;
            newHours -= 20;
        }else{
            twenty = 0;
        }
        tens = newHours  / 10;  //Get the 10s value from new data
        ones = newHours  % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[2] = (newClockSetting << 6) | (twenty << 5) | (tens << 4) | (ones);
    }
}

void encodeDay(uint8_t* data){
    data[3] = newDay;   //Store data in array for RTC
}

void encodeDate(uint8_t* data){
    uint8_t tens, ones;
    tens = newDate  / 10;  //Get the 10s value from new data
    ones = newDate % 10;  //Get the 1s value from new data
    data[4] = (tens << 4) | ones;   //Store data in array for RTC
}

void encodeMonth(uint8_t* data){
    uint8_t tens, ones;
    tens = newMonth / 10;  //Get the 10s value from new data
    ones = newMonth % 10;  //Get the 1s value from new data
    data[5] = (century << 7) | (tens << 4) | (ones);   //Store data in array for RTC
}

void encodeYear(uint8_t* data){
    uint8_t tens, ones;
    newYear = newYear - 2000;   //Subtract 2000 from year value
    tens = newYear / 10;  //Get the 10s value from new data
    ones = newYear % 10;  //Get the 1s value from new data
    data[6] = (tens << 4) | (ones);   //Store data in array for RTC
}

void encodeSecondsA1(uint8_t* data){
    uint8_t tens, ones;
    tens = newSecondsA1 / 10;  //Get the 10s value from new data
    ones = newSecondsA1 % 10;  //Get the 1s value from new data
    data[7] = (newBitA1M1 << 7) | (tens << 4) | (ones);   //Store data in array for RTC
}

void encodeMinutesA1(uint8_t* data){
    uint8_t tens, ones;
    tens = newMinutesA1 / 10;  //Get the 10s value from new data
    ones = newMinutesA1 % 10;  //Get the 1s value from new data
    data[8] = (newBitA1M2 << 7) | (tens << 4) | (ones);   //Store data in array for RTC
}

void encodeHoursA1(uint8_t* data){
    uint8_t twenty, tens, ones;

    if(newClockSettingA1 == 1){//12hr Clock
        tens = newHoursA1  / 10;  //Get the 10s value from new data
        ones = newHoursA1  % 10;  //Get the 1s value from new data
        if(newMeridiemA1 == 1){//PM
            //Store data in array for RTC
            data[9] = (newBitA1M3 << 7) | (newClockSettingA1 << 6) | (newMeridiemA1 << 5) | (tens << 4) | (ones);
        }else if(newMeridiemA1 == 0){//AM
            //Store data in array for RTC
            data[9] = (newBitA1M3 << 7) | (newClockSettingA1 << 6) | (newMeridiemA1 << 5) | (tens << 4) | (ones);
        }
    }else if(newClockSettingA1 == 0){//24hr Clock
        if(newHoursA1 > 20){  //Get the 20s value from new data
            twenty = 1;
            newHoursA1 -= 20;
        }else{
            twenty = 0;
        }
        tens = newHoursA1  / 10;  //Get the 10s value from new data
        ones = newHoursA1  % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[9] = (newBitA1M3 << 7) | (newClockSettingA1 << 6) | (twenty << 5) | (tens << 4) | (ones);
    }
}

void encodeDayDateA1(uint8_t* data){
    uint8_t tens, ones;

    if(newDayDateSelectBitA1 == 1){ //Day Selected
        tens = newDayA1 / 10;  //Get the 10s value from new data
        ones = newDayA1 % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[10] = (newBitA1M4 << 7) | (newDayDateSelectBitA1 << 6) | (tens << 4) | (ones) ;
    }else if(newDayDateSelectBitA1 == 0){ //Date Selected
        tens = newDateA1 / 10;  //Get the 10s value from new data
        ones = newDateA1 % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[10] = (newBitA1M4 << 7) | (newDayDateSelectBitA1 << 6) | (tens << 4) | (ones) ;
    }
}

void encodeMinutesA2(uint8_t* data){
    uint8_t tens, ones;
    tens = newMinutesA2 / 10;  //Get the 10s value from new data
    ones = newMinutesA2 % 10;  //Get the 1s value from new data
    //Store data in array for RTC
    data[11] = (newBitA2M2 << 7) | (tens << 4) | (ones);
}

void encodeHoursA2(uint8_t* data){
    uint8_t twenty, tens, ones;

    if(newClockSettingA2 == 1){//12hr Clock
        tens = newHoursA2  / 10;  //Get the 10s value from new data
        ones = newHoursA2  % 10;  //Get the 1s value from new data
        if(newMeridiemA2 == 1){//PM
            //Store data in array for RTC
            data[12] = (newBitA2M3 << 7) | (newClockSettingA2 << 6) | (newMeridiemA2 << 5) | (tens << 4) | (ones);
        }else if(newMeridiemA2 == 0){//AM
            //Store data in array for RTC
            data[12] = (newBitA2M3 << 7) | (newClockSettingA2 << 6) | (newMeridiemA2 << 5) | (tens << 4) | (ones);
        }
    }else if(newClockSettingA2 == 0){//24hr Clock
        if(newHoursA2 > 20){   //Get the 20s value from new data
            twenty = 1;
            newHoursA2 -= 20;
        }else{
            twenty = 0;
        }
        tens = newHoursA2  / 10;  //Get the 10s value from new data
        ones = newHoursA2  % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[12] = (newBitA2M3 << 7) | (newClockSettingA2 << 6) | (twenty << 5) | (tens << 4) | (ones);
    }
}

void encodeDayDateA2(uint8_t* data){
    uint8_t tens, ones;

    if(newDayDateSelectBitA2 == 1){ //Day Selected
        tens = newDayA2 / 10;  //Get the 10s value from new data
        ones = newDayA2 % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[13] = (newBitA2M4 << 7) | (newDayDateSelectBitA2 << 6) | (tens << 4) | (ones) ;
    }else if(newDayDateSelectBitA2 == 0){ //Date Selected
        tens = newDateA2 / 10;  //Get the 10s value from new data
        ones = newDateA2 % 10;  //Get the 1s value from new data
        //Store data in array for RTC
        data[13] = (newBitA2M4 << 7) | (newDayDateSelectBitA2 << 6) | (tens << 4) | (ones) ;
    }
}

void encodeControl(uint8_t* data){
    //Store data in array for RTC
    data[14] = (newEOSC << 7) | (newBBSQW << 6) | (newCONV << 5) | (newRS2 << 4) | (newRS1 << 3) | (newINTCN << 2) | (newA2IE << 1) | (newA1IE);
}

void encodeStatus(uint8_t* data){
    //Store data in array for RTC
    data[15] = (newOSF << 7) | (newEN32kHz << 3) | (newBSY << 2) | (newA2F << 1) | (newA1F);
}

void encodeAgingOffset(uint8_t* data){
    //Store data in array for RTC
    data[16] = (newSign << 7) | (newAgingOffset);
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
    decodeDayDateA1(data);
    decodeMinutesA2(data);
    decodeHoursA2(data);
    decodeDayDateA2(data);
    decodeControl(data);
    decodeStatus(data);
    decodeAgingOffset(data);
    decodeTemp(data);
    stringDecode();
    tempDisp = convertTemp();
}

void decodeSeconds(uint8_t* data){//DONE
    uint8_t tens, ones, temp;
    temp = data[0]; //Store data in temporary variable
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

    year = 2000 + (century * 100) + (tens * 10) + (ones * 1);
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

void decodeDayDateA1(uint8_t* data){//DONE
    uint8_t temp, tens, ones;
    temp = data[10];
    bitA1M4 = (temp &=~ 0b01111111) >> 7;

    if(dayDateSelectBitA1 == 1){   //DY
        temp = data[10];
        ones = (temp &=~ 0b11110000);
        temp = data[10];
        dayA1 = (ones * 1);
    }else if(dayDateSelectBitA1 == 0){    //DT
        temp = data[10];
        tens = (temp &=~ 0b11001111) >> 4;
        temp = data[10];
        ones = (temp &=~ 0b11110000);
        dateA1 = (tens * 10) + (ones * 1);
    }
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

void decodeHoursA2(uint8_t* data){//DONE
    uint8_t twenty, tens, ones, temp;
    //re-collect data from data array since clearing bits is a destructive operation
    temp = data[12];
    bitA2M3 = (temp &=~ 0b01111111) >> 7;
    temp = data[12];
    clockSettingA2 = (temp &=~ 0b10111111) >> 6; //Determine if clock is in 12hr or 24hr mode

    if(clockSettingA2 == 1){          //12 Hour (1)
        temp = data[12];
        meridiemA2 = (temp &=~ 0b11011111) >> 5;
        if(meridiemA2 == 0){          //AM
            //PRINT AM
        }else if (meridiemA2 == 1){    //PM
            //PRINT PM
        }
        temp = data[12];
        tens = (temp &=~ 0b11101111) >> 4;  //Shift data to get 10 Hour bit
        temp = data[12];
        ones = (temp &=~ 0xF0);  //Shift data to get Hour bit
        hoursA2 = (tens * 10) + (ones * 1);
    }else if(clockSettingA1 == 0){    //24 Hour (0)
        temp = data[12];
        twenty = (temp &=~ 0b11011111) >> 5;  //Shift data to get 20 Hour bit
        temp = data[12];
        tens = (temp &=~ 0b11101111) >> 4;  //Shift data to get 10 Hour bit
        temp = data[12];
        ones = (temp &=~ 0xF0);  //Shift data to get Hour bit
        hoursA2 = (twenty * 20) + (tens * 10) + (ones * 1);
    }
}

void decodeDayDateA2(uint8_t* data){//DONE
    uint8_t temp, tens, ones;
    temp = data[13];
    bitA2M4 = (temp &=~ 0b01111111) >> 7;

    if(dayDateSelectBitA2 == 1){   //DY
        temp = data[13];
        ones = (temp &=~ 0b11110000);
        temp = data[13];
        dayA2 = (ones * 1);
    }else if(dayDateSelectBitA2 == 0){    //DT
        temp = data[13];
        tens = (temp &=~ 0b11001111) >> 4;
        temp = data[13];
        ones = (temp &=~ 0b11110000);
        dateA2 = (tens * 10) + (ones * 1);
    }
}

void decodeControl(uint8_t* data){//DONE
    uint8_t temp;
    temp = data[14];
    EOSC =  (temp &=~ 0b01111111) >> 7;
    temp = data[14];
    BBSQW = (temp &=~ 0b10111111) >> 6;
    temp = data[14];
    CONV =  (temp &=~ 0b11011111) >> 5;
    temp = data[14];
    RS2 =   (temp &=~ 0b11101111) >> 4;
    temp = data[14];
    RS1 =   (temp &=~ 0b11110111) >> 3;
    temp = data[14];
    INTCN = (temp &=~ 0b11111011) >> 2;
    temp = data[14];
    A2IE =  (temp &=~ 0b11111101) >> 1;
    temp = data[14];
    A1IE =  (temp &=~ 0b11111110) >> 0;
}

void decodeStatus(uint8_t* data){//DONE
    uint8_t temp;
    temp = data[15];
    OSF = (temp &=~ 0b01111111) >> 7;
    temp = data[15];
    EN32kHz = (temp &=~ 0b11110111) >> 3;
    temp = data[15];
    BSY = (temp &=~ 0b11111011) >> 2;
    temp = data[15];
    A2F = (temp &=~ 0b11111101) >> 1;
    temp = data[15];
    A1F = (temp &=~ 0b11111110) >> 0;
}

void decodeAgingOffset(uint8_t* data){//DONE
    uint8_t temp;
    temp = data[16];
    sign = (temp &=~ 0b01111111) >> 7;
    temp = data[16];
    agingOffset = (temp &=~ 0b10000000);
}

void decodeTemp(uint8_t* data){//DONE
    uint8_t temp;
    temp = data[17];
    tempMSB = (temp &=~ 0b10001111) >> 4;
    temp = data[17];
    tempLSB = (temp &=~ 0b11110000);
    temp = data[18];
    tempDec = (temp &=~ 0b00111111) >> 6;
    temperature = (tempMSB * 10) + (tempLSB * 1) + (tempDec * 0.25);
}

void stringDecode(void){
    stringDecodeDay();
    stringDecodeMonth();
    stringDecodeMeridiem();
}

void stringDecodeDay(void){
    switch(day){
        case 1:
            strcpy(dayString, "Monday");
            break;
        case 2:
            strcpy(dayString, "Tuesday");
            break;
        case 3:
            strcpy(dayString, "Wednesday");
            break;
        case 4:
            strcpy(dayString, "Thursday");
            break;
        case 5:
            strcpy(dayString, "Friday");
            break;
        case 6:
            strcpy(dayString, "Saturday");
            break;
        case 7:
            strcpy(dayString, "Sunday");
            break;
        default:
            strcpy(dayString, "ERROR");
            break;
    }
}

void stringDecodeMonth(void){
    switch(month){
        case 1:
            strcpy(monthString, "Jan.");
            break;
        case 2:
            strcpy(monthString, "Feb.");
            break;
        case 3:
            strcpy(monthString, "Mar.");
            break;
        case 4:
            strcpy(monthString, "Apr.");
            break;
        case 5:
            strcpy(monthString, "May");
            break;
        case 6:
            strcpy(monthString, "Jun.");
            break;
        case 7:
            strcpy(monthString, "Jul.");
            break;
        case 8:
            strcpy(monthString, "Aug.");
            break;
        case 9:
            strcpy(monthString, "Sep.");
            break;
        case 10:
            strcpy(monthString, "Oct.");
            break;
        case 11:
            strcpy(monthString, "Nov.");
            break;
        case 12:
            strcpy(monthString, "Dec.");
            break;
        default:
            strcpy(monthString, "ERR.");
            break;
    }
}

void stringDecodeMeridiem(void){
    switch(meridiem){
        case 0: //AM
            strcpy(meridiemString, "AM");
            break;
        case 1: //PM
            strcpy(meridiemString, "PM");
            break;
        default:
            strcpy(meridiemString, "ER");
            break;
    }
}

float convertTemp(void){
    switch(tempSelection){
        case Celsius:
            celsius = temperature;
            strcpy(tempUnit, "C");
            return celsius;
        case Fahrenheit:
            fahrenheit = (temperature * (9 / 5)) + 32;
            strcpy(tempUnit, "F");
            return fahrenheit;
        case Kelvin:
            kelvin = temperature + 273.15;
            strcpy(tempUnit, "K");
            return kelvin;
        case Rankine:
            renkine = (temperature + 273.15) * (9 / 5);
            strcpy(tempUnit, "R");
            return renkine;
        case Delisle:
            delisle = (100 - temperature) * (3 / 2);
            strcpy(tempUnit, "De");
            return delisle;
        case Newton:
            newton = temperature * (33 / 100);
            strcpy(tempUnit, "N");
            return newton;
        case Reaumur:
            reaumur = ((temperature) * (4 / 5));
            strcpy(tempUnit, "Re");
            return reaumur;
        case Romer:
            romer = (temperature * (21 / 40)) + 7.5;
            strcpy(tempUnit, "Ro");
            return romer;
        default:
            tempSelection = Celsius;
            break;
    }
    return 0;
}

void loadTestData(void){
    newSeconds = 0;   //0
    newMinutes = 23;   //1
    newHours = 4;   //2
    newClockSetting = 1;
    newMeridiem = 1;

    newDay = 2;   //3
    newDate = 4;   //4
    newMonth = 7;   //5
    newCentury = 0;
    newYear = 2021;   //6

    newBitA1M1 = 0;
    newBitA1M2 = 0;
    newBitA1M3 = 0;
    newBitA1M4 = 0;
    newSecondsA1 = 0;   //7
    newMinutesA1 = 0;   //8
    newClockSettingA1 = 0;
    newMeridiemA1 = 0;
    newHoursA1 = 0;   //9
    newDayDateSelectBitA1 = 0;
    newDayA1 = 0;   //10a
    newDateA1 = 0;   //10b

    newBitA2M2 = 0;
    newBitA2M3 = 0;
    newBitA2M4 = 0;
    newMinutesA2 = 0;   //11
    newClockSettingA2 = 0;
    newMeridiemA2 = 0;
    newHoursA2 = 0;   //12
    newDayDateSelectBitA2 = 0;
    newDayA2 = 0;    //13a
    newDateA2 = 0;   //13b

    newEOSC = 1;
    newBBSQW = 0;
    newCONV = 0;
    newRS2 = 0;
    newRS1 = 0;
    newINTCN = 0;
    newA2IE = 0;
    newA1IE = 0;

    newOSF = 1;
    newEN32kHz = 0;
    newBSY = 0;
    newA2F = 0;
    newA1F = 0;

    newSign = 0;
    newAgingOffset = 0;

    encode(sendData);
    burstWrite(RTC_ADDRESS, 0x00, 19, sendData);
}
