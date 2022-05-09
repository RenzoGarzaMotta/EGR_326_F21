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
void encodeDayA1(uint8_t* data);
void encodeDateA1(uint8_t* data);
void encodeMinutesA2(uint8_t* data);
void encodeHoursA2(uint8_t* data);
void encodeDayA2(uint8_t* data);
void encodeDateA2(uint8_t* data);
void encodeControl(uint8_t* data);
void encodeStatus(uint8_t* data);
void encodeAgingOffset(uint8_t* data);
void encodeTempMSB(uint8_t* data);
void encodeTempLSB(uint8_t* data);

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
void decodeDayA1(uint8_t* data);
void decodeDateA1(uint8_t* data);
void decodeMinutesA2(uint8_t* data);
void decodeHoursA2(uint8_t* data);
void decodeDayA2(uint8_t* data);
void decodeDateA2(uint8_t* data);
void decodeControl(uint8_t* data);
void decodeStatus(uint8_t* data);
void decodeAgingOffset(uint8_t* data);
void decodeTempMSB(uint8_t* data);
void decodeTempLSB(uint8_t* data);

uint8_t readData[19];
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
uint8_t secondsA1;   //7
uint8_t bitA1M1;
uint8_t bitA1M2;
uint8_t bitA1M3;
uint8_t bitA1M4;
uint8_t clockSettingA1;
uint8_t meridiemA1;

uint8_t minutesA1;   //8
uint8_t hoursA1;   //9
uint8_t dayA1;   //10a
uint8_t dateA1;   //10b
uint8_t minutesA2;   //11
uint8_t bitA2M2;
uint8_t bitA2M3;
uint8_t bitA2M4;
uint8_t hoursA2;   //12
uint8_t dayA2;   //13a
uint8_t dateA2;   //13b
uint8_t control;   //14
uint8_t status;   //15
uint8_t agingOffset;   //16
float tempMSB;   //17
float tempLSB;   //18
float temp;

//uint8_t defaultData[19] = {0};

#endif /* RTC_H_ */
