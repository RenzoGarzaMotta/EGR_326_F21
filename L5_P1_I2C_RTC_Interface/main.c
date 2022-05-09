/*Lab 5 - Interfacing with a Real Time Clock (RTC) IC Using an I2C Bus
 *Part 1 - Setting the current time on the RTC IC
 *
 *Authors:
 *  Renzo Garza Motta
 *  Cianna Janicke
 *
 *Class: EGR 326
 *Section: 903
 *
 *Instructor:
 *  Dr. Nabeeh Kandalaft
 *
 *Description:
 *  This part will connect the MSP432 I2C bus interface pins to the RTC SDA and SCL pins and send it
 *  commands over this I2C bus to set the current time and date.
 */

#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "macros.h"
#include "systick.h"
#include "keypad.h"
#include "RTC.h"

void updateStates(void);

//unsigned char readData[19] = {0};
uint8_t updateState = 0xFF;
uint8_t prnt = 1;
unsigned char sendData[25] = {
                                0b01000000,//Seconds
                                0b01011001,//Minutes
                                0b01110001,//Hours
                                0b00000110,//Day
                                0b00001000,//Date
                                0b00010000,//Month
                                0b00100001,//Year
                                0b10010000,//AlarmSec
                                0b10001001,//AlarmMin
                                0b11010001,//AlarmHours
                                0x00,//AlarmDay
                                0x00,//AlarmDate
                                0b00010001,//A2Min
                                0x00,//A2Hours
                                0x00,//A2Day
                                0x00,//A2Date
                                0x00,//Control
                                0x00,//Status
                                0x00,//AgingOffset
                                0x00,//TempMSB
                                0x00,//TempLSB
};

extern uint8_t readData[19];
extern uint8_t seconds;   //0
extern uint8_t minutes;   //1
extern uint8_t hours;   //2
extern uint8_t clockSetting;
extern uint8_t meridiem;

extern uint8_t day;   //3
extern uint8_t date;   //4
extern uint8_t month;   //5
extern uint8_t century;

extern uint8_t year;   //6
extern uint8_t secondsA1;   //7
extern uint8_t bitA1M1;
extern uint8_t bitA1M2;
extern uint8_t bitA1M3;
extern uint8_t bitA1M4;
extern uint8_t clockSettingA1;
extern uint8_t meridiemA1;

extern uint8_t minutesA1;   //8
extern uint8_t hoursA1;   //9
extern uint8_t dayA1;   //10a
extern uint8_t dateA1;   //10b
extern uint8_t minutesA2;   //11
extern uint8_t hoursA2;   //12
extern uint8_t dayA2;   //13a
extern uint8_t dateA2;   //13b
extern uint8_t control;   //14
extern uint8_t status;   //15
extern uint8_t agingOffset;   //16
extern float temp;
extern float tempMSB;   //17
extern float tempLSB;   //18

uint8_t updatedYear = 0x00;
uint8_t updatedMonth;
uint8_t updatedDate;
uint8_t updatedDay;
uint8_t updatedClockSetting;
uint8_t updatedHours;
uint8_t updatedMeridiem;
uint8_t updatedMinutes;
uint8_t updatedSeconds;

extern char keyPress;
uint16_t byteCount = 20;
uint8_t val[2] = {0};
uint8_t tempVal1, tempVal2;

enum{
    updateYear,
    updateMonth,
    updateDate,
    updateDay,
    updateClockSetting,
    updateHours,
    updateMeridiem,
    updateMinutes,
    updateSeconds
};

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	sysTickSetup();
	keypadInit();
	setupI2C();
	//WRITE DATA

	burstWrite(RTC_ADDRESS, 0x00, 19, sendData);

	msDelay(5000);

    //READ DATA
	burstRead(RTC_ADDRESS, 0x00, 19, readData);

	decode(readData);

	while(1){
	    updateStates();
//	    msDelay(250);
	}
}


void updateStates(void){
    switch(updateState){
        case updateYear:
            if(prnt == 1){
                printf("\nPlease enter year (YYYY)\n");    //Prompt user to enter a pulse limit
                fflush(stdout);     //Flush bus buffer
                prnt = 0;
            }

            if(readKeypad()){       //Continuously check for keypad entries
//                val = keyPress;     //Store data entered into the keypad
                if(keyPress == '*'){
                    tempVal1 = val[1];
                    tempVal2 = val[0];
                    updatedYear = (tempVal1 << 4);
                    updatedYear = (updatedYear | tempVal2);
                    updateState = updateMonth;
                    prnt = 1;
                }

                shiftVal(val);
                printKeys(val);     //Print keypad values to the console
            }
            break;
        case updateMonth:
            printf("\nPlease enter month (MM)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateDate;
            break;
        case updateDate:
            printf("\nPlease enter date (DD)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateDay;
            break;
        case updateDay:
            printf("\nPlease enter weekday (Sun (1) - Sat(7))\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateClockSetting;
            break;
        case updateClockSetting:
            printf("\nPlease enter 12hr(1) or 24hr(0) mode\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateHours;
            break;
        case updateHours:
            printf("\nPlease enter hour (hh)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateMeridiem;
            break;
        case updateMeridiem:
            printf("\nPlease enter AM(0) or PM(1)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateMinutes;
            break;
        case updateMinutes:
            printf("\nPlease enter minutes (mm)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateSeconds;
            break;
        case updateSeconds:
            printf("\nPlease enter seconds (ss)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateYear;
            exit (0);
            break;
        default:
            printf("\nUpdate Real-Time-Clock (RTC)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateYear;
            break;
    }
}
