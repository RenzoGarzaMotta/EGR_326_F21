/*
 * EEPROM.h
 *
 *  Created on: Dec 6, 2021
 *      Author: renzogarzamotta
 */

#ifndef EEPROM_H_
#define EEPROM_H_

//EEPROM Address
#define EEPROM_ADDRESS 0b1010000

int I2C1_Write(int slaveAddr, unsigned char memAddr, unsigned char data);

int I2C1_Read(int slaveAddr, unsigned char memAddr, unsigned char* data);

void writeEEPROM(void);

void encodeEEPROM(uint8_t* data);

void encodeEntry1(uint8_t* data);

void encodeEntry2(uint8_t* data);

void encodeEntry3(uint8_t* data);

void encodeEntry4(uint8_t* data);

void encodeEntry5(uint8_t* data);

void encodeEntry6(uint8_t* data);

void encodeSettings(uint8_t* data);

void encodeOdometer(uint8_t* data);

void readEEPROM(void);

void decodeEEPROM(uint8_t* data);

void decodeEntry1(uint8_t* data);

void decodeEntry2(uint8_t* data);

void decodeEntry3(uint8_t* data);

void decodeEntry4(uint8_t* data);

void decodeEntry5(uint8_t* data);

void decodeEntry6(uint8_t* data);

void decodeSettings(uint8_t* data);

void decodeOdometer(uint8_t* data);

void testData(void);

//Read and Write arrays
uint8_t readEEPROMData[50];
uint8_t sendEEPROMData[50];

//Write variables
int newOdometer;
uint8_t newEntry1;
uint8_t newEntry2;
uint8_t newEntry3;
uint8_t newEntry4;
uint8_t newEntry5;
uint8_t newEntry6;
uint8_t newMonth1;
uint8_t newMonth2;
uint8_t newMonth3;
uint8_t newMonth4;
uint8_t newMonth5;
uint8_t newMonth6;
uint8_t newDay1;
uint8_t newDay2;
uint8_t newDay3;
uint8_t newDay4;
uint8_t newDay5;
uint8_t newDay6;
uint8_t newHour1;
uint8_t newHour2;
uint8_t newHour3;
uint8_t newHour4;
uint8_t newHour5;
uint8_t newHour6;
uint8_t newMinute1;
uint8_t newMinute2;
uint8_t newMinute3;
uint8_t newMinute4;
uint8_t newMinute5;
uint8_t newMinute6;
uint8_t newSecond1;
uint8_t newSecond2;
uint8_t newSecond3;
uint8_t newSecond4;
uint8_t newSecond5;
uint8_t newSecond6;
uint8_t newTempUnits;
uint8_t newSpeedUnits;
uint8_t newAppearance;
uint8_t newBrightness;
uint8_t newTextColor;
uint8_t newBgColor;

//Read variables
int odometer;
uint8_t entry1;
uint8_t entry2;
uint8_t entry3;
uint8_t entry4;
uint8_t entry5;
uint8_t entry6;
uint8_t month1;
uint8_t month2;
uint8_t month3;
uint8_t month4;
uint8_t month5;
uint8_t month6;
uint8_t day1;
uint8_t day2;
uint8_t day3;
uint8_t day4;
uint8_t day5;
uint8_t day6;
uint8_t hour1;
uint8_t hour2;
uint8_t hour3;
uint8_t hour4;
uint8_t hour5;
uint8_t hour6;
uint8_t minute1;
uint8_t minute2;
uint8_t minute3;
uint8_t minute4;
uint8_t minute5;
uint8_t minute6;
uint8_t second1;
uint8_t second2;
uint8_t second3;
uint8_t second4;
uint8_t second5;
uint8_t second6;
uint8_t tempUnitsEEPROM;
uint8_t speedUnitsEEPROM;
uint8_t appearanceEEPROM;
uint8_t brightnessEEPROM;
uint8_t textColorEEPROM;
uint8_t bgColorEEPROM;

#endif /* EEPROM_H_ */
