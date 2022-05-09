/*
 * RTC.h
 *
 *  Created on: Oct 6, 2021
 *      Author: renzogarzamotta
 */

#ifndef RTC_H_
#define RTC_H_

#define MSP432_ADDRESS  0x0001
#define RTC_ADDRESS     0x68
#define READ            0x01
#define WRITE           0x00
#define UCBRx           8      //120 //30 //8

void setupI2C(void);
void burstWrite(uint8_t slaveAddress, uint8_t memoryAddress,uint16_t byteCount, uint8_t* data);
void burstRead(uint8_t slaveAddress, uint8_t memoryAddress,uint16_t byteCount, uint8_t* data);

void encode();
void encodeSeconds(uint8_t* data);
void encodeMinutes(uint8_t* data);
void encodeHours(uint8_t* data);
void encodeDay(uint8_t* data);
void encodeDate(uint8_t* data);
void encodeMonth(uint8_t* data);
void encodeYear(uint8_t* data);
void encodeSecondsA1(uint8_t* data);
void encodeMinutesA1(uint8_t* data);
void encodeHoursA1(uint8_t* data);
void encodeDayDateA1(uint8_t* data);
void encodeMinutesA2(uint8_t* data);
void encodeHoursA2(uint8_t* data);
void encodeDayDateA2(uint8_t* data);
void encodeControl(uint8_t* data);
void encodeStatus(uint8_t* data);
void encodeAgingOffset(uint8_t* data);

void decode(uint8_t* data);
void decodeSeconds(uint8_t* data);
void decodeMinutes(uint8_t* data);
void decodeHours(uint8_t* data);
void decodeDay(uint8_t* data);
void decodeDate(uint8_t* data);
void decodeMonth(uint8_t* data);
void decodeYear(uint8_t* data);
void decodeSecondsA1(uint8_t* data);
void decodeMinutesA1(uint8_t* data);
void decodeHoursA1(uint8_t* data);
void decodeDayDateA1(uint8_t* data);
//void decodeDateA1(uint8_t* data);
void decodeMinutesA2(uint8_t* data);
void decodeHoursA2(uint8_t* data);
void decodeDayDateA2(uint8_t* data);
//void decodeDateA2(uint8_t* data);
void decodeControl(uint8_t* data);
void decodeStatus(uint8_t* data);
void decodeAgingOffset(uint8_t* data);
void decodeTemp(uint8_t* data);

uint8_t readData[19];
uint8_t sendData[19];

//DATA TO DECODE READ FROM THE RTC
uint8_t seconds;   //0
uint8_t minutes;   //1
uint8_t hours;   //2
uint8_t clockSetting;
uint8_t meridiem;

uint8_t day;   //3
uint8_t date;   //4
uint8_t month;   //5
uint8_t century;
uint8_t year;   //6

uint8_t bitA1M1;
uint8_t bitA1M2;
uint8_t bitA1M3;
uint8_t bitA1M4;
uint8_t secondsA1;   //7
uint8_t minutesA1;   //8
uint8_t clockSettingA1;
uint8_t meridiemA1;
uint8_t hoursA1;   //9
uint8_t dayDateSelectBitA1;
uint8_t dayA1;   //10a
uint8_t dateA1;   //10b


uint8_t bitA2M2;
uint8_t bitA2M3;
uint8_t bitA2M4;
uint8_t minutesA2;   //11
uint8_t clockSettingA2;
uint8_t meridiemA2;
uint8_t hoursA2;   //12
uint8_t dayDateSelectBitA2;
uint8_t dayA2;    //13a
uint8_t dateA2;   //13b

uint8_t EOSC;
uint8_t BBSQW;
uint8_t CONV;
uint8_t RS2;
uint8_t RS1;
uint8_t INTCN;
uint8_t A2IE;
uint8_t A1IE;

uint8_t OSF;
uint8_t EN32kHz;
uint8_t BSY;
uint8_t A2F;
uint8_t A1F;

uint8_t sign;
uint8_t agingOffset;
uint8_t tempSign;

//DATA TO ENCODE TO SEND TO RTC MODULE
uint8_t newSeconds;   //0
uint8_t newMinutes;   //1
uint8_t newHours;   //2
uint8_t newClockSetting;
uint8_t newMeridiem;

uint8_t newDay;   //3
uint8_t newDate;   //4
uint8_t newMonth;   //5
uint8_t newCentury;
uint8_t newYear;   //6

uint8_t newBitA1M1;
uint8_t newBitA1M2;
uint8_t newBitA1M3;
uint8_t newBitA1M4;
uint8_t newSecondsA1;   //7
uint8_t newMinutesA1;   //8
uint8_t newClockSettingA1;
uint8_t newMeridiemA1;
uint8_t newHoursA1;   //9
uint8_t newDayDateSelectBitA1;
uint8_t newDayA1;   //10a
uint8_t newDateA1;   //10b


uint8_t newBitA2M2;
uint8_t newBitA2M3;
uint8_t newBitA2M4;
uint8_t newMinutesA2;   //11
uint8_t newClockSettingA2;
uint8_t newMeridiemA2;
uint8_t newHoursA2;   //12
uint8_t newDayDateSelectBitA2;
uint8_t newDayA2;    //13a
uint8_t newDateA2;   //13b

uint8_t newEOSC;
uint8_t newBBSQW;
uint8_t newCONV;
uint8_t newRS2;
uint8_t newRS1;
uint8_t newINTCN;
uint8_t newA2IE;
uint8_t newA1IE;

uint8_t newOSF;
uint8_t newEN32kHz;
uint8_t newBSY;
uint8_t newA2F;
uint8_t newA1F;

uint8_t newSign;
uint8_t newAgingOffset;

float tempMSB;   //17
float tempLSB;   //18
float tempDec;
float temp;

//uint8_t defaultData[19] = {0};

#endif /* RTC_H_ */
