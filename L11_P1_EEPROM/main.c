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
#include "RTC.h"

void updateStates(void);
void formatData(int count);
void updateEEPROM(void);
uint8_t debounceBtn1(void);
uint8_t debounceBtn2(void);


//unsigned char readData[19] = {0};
uint8_t updateState = 0xFF;
uint8_t prnt = 1, lastAdd = 0;
uint8_t var1 = 0, var2 = 0, check;

uint8_t dataEEPROM[20] = {0};
uint8_t readEEPROM[20] = {0};

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
int hold;

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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    sysTickSetup();
    setupI2C();
    //WRITE DATA

    P1->SEL0 &=~ BIT0;
    P1->SEL1 &=~ BIT0;
    P1->DIR |= BIT0;
    P1->OUT &=~ BIT0;

    BTN_PORT->SEL0 &=~ BTNS;
    BTN_PORT->SEL1 &=~ BTNS;    //Set pins as GPIO
    BTN_PORT->DIR &=~ BTNS;     //Set buttons as inputs
    BTN_PORT->REN |= BTNS;      //Enable internal pull-up/down resistor
    BTN_PORT->OUT |= BTNS;      //Enable internal pull-up resistors
    BTN_PORT->IE |= BTNS;       //Enable interrupts
    BTN_PORT->IES |= BTNS;      //Select High-To-Low Interrupt edge
    BTN_PORT->IFG = 0;

    NVIC->ISER[1] = 1 << (PORT1_IRQn & 31);
    __enable_irq();
//    burstWrite(RTC_ADDRESS, 0x00, 19, sendData);
//
//    msDelay(5000);

    //READ DATA
    burstRead(RTC_ADDRESS, 0x00, 19, readData);

    decode(readData);

    burstWrite(EEPROM_ADDRESS, 0x00, 19, sendData);

    while(1){
        if(var1){
            burstRead(RTC_ADDRESS, 0x00, 19, readData);
            decode(readData);

            burstRead(EEPROM_ADDRESS, check, 4, readEEPROM);
            if (check < 16){
                check += 4;
            }else{
                check = 0;
            }

            var1 = 0;
        }
        if(var2){
            updateEEPROM();
            P1->OUT ^= BIT0;
            var2 = 0;
        }
//        updateStates();

        //READ DATA
//        burstRead(RTC_ADDRESS, 0x00, 19, readData);
//
//        decode(readData);
//        msDelay(1000);

    }
}

void PORT1_IRQHandler(void){
    int i;

    if(BTN_PORT->IFG & BTN1){
        for(i = 0; i < 150; i++){                   //Cycle through for-loop to check state of button bounce
            if(debounceBtn1()){
                var1 = 1;
                break;
            }
        }
    }
    if(BTN_PORT->IFG & BTN2){
        for(i = 0; i < 150; i++){                   //Cycle through for-loop to check state of button bounce
            if(debounceBtn2()){
                var2 = 1;
                break;
            }
        }
    }
    BTN_PORT->IFG = 0;
}

void formatData(int count){
    //hold = (() | (month << 21) | (date << 16) | (hours << 12) | (minutes << 6) | seconds);
    hold = ((month << 21) | (date << 16) | (hours << 12) | (minutes << 6) | seconds);
    dataEEPROM[count] = (0xFF000000 & hold) >> 24;
    dataEEPROM[(count + 1)] = (0xFF0000 & hold) >> 16;
    dataEEPROM[(count + 2)] = (0xFF00 & hold) >> 8;
    dataEEPROM[(count + 3)] = (0xFF & hold);
}

void updateEEPROM(void){
    burstRead(RTC_ADDRESS, 0x00, 19, readData);
    decode(readData);
    formatData(lastAdd);
    burstWrite(EEPROM_ADDRESS, lastAdd, 4, dataEEPROM);
//    P1->OUT ^= BIT0;

    if (lastAdd < 16){
        lastAdd += 4;
    }else{
        lastAdd = 0;
    }
}

uint8_t debounceBtn1(void){                                         //Debounce function for button1
    static uint16_t State1 = 0;                                     //Current debounce status
    State1 = (State1 << 1) | (BTN_PORT->IN & BTN1)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State1==0x8000)return 1;                                     //indicates 0 level is
    return 0;
}

uint8_t debounceBtn2(void){                                         //Debounce function for button1
    static uint16_t State2 = 0;                                     //Current debounce status
    State2 = (State2 << 1) | (BTN_PORT->IN & BTN2)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State2==0x8000)return 1;                                     //indicates 0 level is
    return 0;
}

void updateStates(void){
    switch(updateState){
        case updateYear:
            if(prnt == 1){
                printf("\nPlease enter year (YYYY)\n");    //Prompt user to enter a pulse limit
                fflush(stdout);     //Flush bus buffer
                prnt = 0;
            }

//            if(readKeypad()){       //Continuously check for keypad entries
////                val = keyPress;     //Store data entered into the keypad
//                if(keyPress == '*'){
//                    tempVal1 = val[1];
//                    tempVal2 = val[0];
//                    updatedYear = (tempVal1 << 4);
//                    updatedYear = (updatedYear | tempVal2);
//                    updateState = updateMonth;
//                    prnt = 1;
//                }
//
//                shiftVal(val);
//                printKeys(val);     //Print keypad values to the console
//            }
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
            exit(0);
            break;
        default:
            printf("\nUpdate Real-Time-Clock (RTC)\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            updateState = updateYear;
            break;
    }
}
