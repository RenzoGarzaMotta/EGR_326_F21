/*
 * menus.c
 *
 *  Created on: Nov 25, 2021
 *      Author: renzogarzamotta
 */
#include "menus.h"
#include "RTC.h"

uint16_t textColor = ST7735_WHITE;  //ST7735_WHITE
uint16_t highlightedColor;
uint16_t bgColor;
uint8_t alarm1Flag;
uint8_t alarm2Flag;
uint8_t customFlag;
uint8_t digitFlag, prevDigitFlag;


float kmPerHour, milesPerHour, metersPerSec, knot, feetPerSec;
float speedDisp;
uint16_t threeDig, twoDig;
extern float speed;
extern float tempToDisplay;
extern char tempUnit[5];
extern char speedUnit[5];
extern uint8_t speedUnitSelection;
extern uint8_t appearance;

extern uint16_t textColor;  //ST7735_WHITE
extern uint16_t highlightedColor;
extern uint16_t bgColor;

extern uint8_t textColorSelected;
extern uint8_t bgColorSelected;

extern uint8_t menuState;

void mainScreenSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option1.text, "%02d:%02d:%02d", hour, minute, second);
    strcpy(mainScreen.option1.text, "11:39:39"); //PLACEHOLDER
    mainScreen.option1.selectable = 0;
    mainScreen.option1.x = 0;
    mainScreen.option1.y = 0;
    mainScreen.option1.size = 1;

    strcpy(mainScreen.option2.text, "AM");
    mainScreen.option2.selectable = 0;
    mainScreen.option2.x = 9;
    mainScreen.option2.y = 0;
    mainScreen.option2.size = 1;

    strcpy(mainScreen.option3.text, "*");
    mainScreen.option3.selectable = 0;
    mainScreen.option3.x = 12;
    mainScreen.option3.y = 0;
    mainScreen.option3.size = 1;

    strcpy(mainScreen.option4.text, "*");
    mainScreen.option4.selectable = 0;
    mainScreen.option4.x = 13;
    mainScreen.option4.y = 0;
    mainScreen.option4.size = 1;

    sprintf(mainScreen.option5.text, "%c%c",DEGREE_SYM, 'C');
    mainScreen.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    mainScreen.option5.x = 16;
    mainScreen.option5.y = 0;
    mainScreen.option5.size = 1;

//    sprintf(mainScreen.title.text, "%d", speed);
    sprintf(mainScreen.title.text, "000");//PLACEHOLDER
    mainScreen.title.selectable = 0;
    //Triple Digit x = 2
    //Double Digit x = 5
    //Single Digit x = 8
    mainScreen.title.x = 2;
    mainScreen.title.y = 5;
    mainScreen.title.size = 6;

    strcpy(mainScreen.option6.text, "MPH"); //REPLACE MPH by dynamic variable
    mainScreen.option6.selectable = 0;
    //KM/H x = 7
    //MPH x = 8
    mainScreen.option6.x = 8;
    mainScreen.option6.y = 10;
    mainScreen.option6.size = 2;

    strcpy(mainScreen.option7.text, "Friday");
    mainScreen.option7.selectable = 0;
    mainScreen.option7.x = 0;
    mainScreen.option7.y = 14;
    mainScreen.option7.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(mainScreen.option8.text, "Nov.21,2021"); //PLACEHOLDER
    mainScreen.option8.selectable = 0;
    mainScreen.option8.x = 0;
    mainScreen.option8.y = 15;
    mainScreen.option8.size = 1;

    strcpy(mainScreen.option9.text, "Menu");
    mainScreen.option9.selectable = 1;
    mainScreen.option9.x = 17;
    mainScreen.option9.y = 15;
    mainScreen.option9.size = 1;

    mainScreen.itemsCount = 9;
}

void mainMenuSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option3.text, "HH:MM:SS", hour, minute, second);
    strcpy(mainMenu.option1.text, "11:39:39"); //PLACEHOLDER
    mainMenu.option1.selectable = 0;
    mainMenu.option1.x = 0;
    mainMenu.option1.y = 0;
    mainMenu.option1.size = 1;

    strcpy(mainMenu.option2.text, "AM");
    mainMenu.option2.selectable = 0;
    mainMenu.option2.x = 9;
    mainMenu.option2.y = 0;
    mainMenu.option2.size = 1;

    strcpy(mainMenu.option3.text, "*");
    mainMenu.option3.selectable = 0;
    mainMenu.option3.x = 12;
    mainMenu.option3.y = 0;
    mainMenu.option3.size = 1;

    strcpy(mainMenu.option4.text, "*");
    mainMenu.option4.selectable = 0;
    mainMenu.option4.x = 13;
    mainMenu.option4.y = 0;
    mainMenu.option4.size = 1;

    sprintf(mainMenu.option5.text, "TTT%cF",DEGREE_SYM);
    mainMenu.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    mainMenu.option5.x = 16;
    mainMenu.option5.y = 0;
    mainMenu.option5.size = 1;

    strcpy(mainMenu.title.text, "Menu");
    mainMenu.option1.selectable = 0;
    mainMenu.title.x = 6;
    mainMenu.title.y = 2;
    mainMenu.title.size = 2;

    strcpy(mainMenu.option6.text, "-Time");
    mainMenu.option6.selectable = 1;
    mainMenu.option6.x = 0;
    mainMenu.option6.y = 4;
    mainMenu.option6.size = 2;

    strcpy(mainMenu.option7.text, "-Date");
    mainMenu.option7.selectable = 1;
    mainMenu.option7.x = 0;
    mainMenu.option7.y = 6;
    mainMenu.option7.size = 2;

    strcpy(mainMenu.option8.text, "-Alarms");
    mainMenu.option8.selectable = 1;
    mainMenu.option8.x = 0;
    mainMenu.option8.y = 8;
    mainMenu.option8.size = 2;

    strcpy(mainMenu.option9.text, "-Settings");
    mainMenu.option9.selectable = 1;
    mainMenu.option9.x = 0;
    mainMenu.option9.y = 10;
    mainMenu.option9.size = 2;

    strcpy(mainMenu.option10.text, "-Testing");
    mainMenu.option10.selectable = 1;
    mainMenu.option10.x = 0;
    mainMenu.option10.y = 12;
    mainMenu.option10.size = 2;

    strcpy(mainMenu.option11.text, "Friday");
    mainMenu.option11.selectable = 0;
    mainMenu.option11.x = 0;
    mainMenu.option11.y = 14;
    mainMenu.option11.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(mainMenu.option12.text, "Nov.21,2021"); //PLACEHOLDER
    mainMenu.option12.selectable = 0;
    mainMenu.option12.x = 0;
    mainMenu.option12.y = 15;
    mainMenu.option12.size = 1;

    strcpy(mainMenu.option13.text, "Back");
    mainMenu.option13.selectable = 1;
    mainMenu.option13.x = 17;
    mainMenu.option13.y = 15;
    mainMenu.option13.size = 1;

    mainMenu.itemsCount = 13;
}

void setTimeSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option3.text, "HH:MM:SS", hour, minute, second);
    strcpy(setTimeScreen.option1.text, "11:39:39"); //PLACEHOLDER
    setTimeScreen.option1.selectable = 0;
    setTimeScreen.option1.x = 0;
    setTimeScreen.option1.y = 0;
    setTimeScreen.option1.size = 1;

    strcpy(setTimeScreen.option2.text, "AM");
    setTimeScreen.option2.selectable = 0;
    setTimeScreen.option2.x = 9;
    setTimeScreen.option2.y = 0;
    setTimeScreen.option2.size = 1;

    strcpy(setTimeScreen.option3.text, "*");
    setTimeScreen.option3.selectable = 0;
    setTimeScreen.option3.x = 12;
    setTimeScreen.option3.y = 0;
    setTimeScreen.option3.size = 1;

    strcpy(setTimeScreen.option4.text, "*");
    setTimeScreen.option4.selectable = 0;
    setTimeScreen.option4.x = 13;
    setTimeScreen.option4.y = 0;
    setTimeScreen.option4.size = 1;

    sprintf(setTimeScreen.option5.text, "TTT%cF",DEGREE_SYM);
    setTimeScreen.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    setTimeScreen.option5.x = 16;
    setTimeScreen.option5.y = 0;
    setTimeScreen.option5.size = 1;

    strcpy(setTimeScreen.title.text, "Set Time");
    setTimeScreen.option1.selectable = 0;
    setTimeScreen.title.x = 3;
    setTimeScreen.title.y = 3;
    setTimeScreen.title.size = 2;
    strcpy(setTimeScreen.option6.text, "HH");
    setTimeScreen.option6.selectable = 1;
    setTimeScreen.option6.x = 3;
    setTimeScreen.option6.y = 7;
    setTimeScreen.option6.size = 2;
    strcpy(setTimeScreen.option7.text, ":");
    setTimeScreen.option7.selectable = 0;
    setTimeScreen.option7.x = 7;
    setTimeScreen.option7.y = 7;
    setTimeScreen.option7.size = 2;
    strcpy(setTimeScreen.option8.text, "MM");
    setTimeScreen.option8.selectable = 1;
    setTimeScreen.option8.x = 9;
    setTimeScreen.option8.y = 7;
    setTimeScreen.option8.size = 2;
    strcpy(setTimeScreen.option9.text, "AM");   //AM/PM/24h
    setTimeScreen.option9.selectable = 1;
    setTimeScreen.option9.x = 15;
    setTimeScreen.option9.y = 7;
    setTimeScreen.option9.size = 2;
    strcpy(setTimeScreen.option10.text, "Set");
    setTimeScreen.option10.selectable = 1;
    setTimeScreen.option10.x = 9;
    setTimeScreen.option10.y = 11;
    setTimeScreen.option10.size = 1;

    strcpy(setTimeScreen.option11.text, "Friday");
    setTimeScreen.option11.selectable = 0;
    setTimeScreen.option11.x = 0;
    setTimeScreen.option11.y = 14;
    setTimeScreen.option11.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(setTimeScreen.option12.text, "Nov.21,2021"); //PLACEHOLDER
    setTimeScreen.option12.selectable = 0;
    setTimeScreen.option12.x = 0;
    setTimeScreen.option12.y = 15;
    setTimeScreen.option12.size = 1;

    strcpy(setTimeScreen.option13.text, "Back");
    setTimeScreen.option13.selectable = 1;
    setTimeScreen.option13.x = 17;
    setTimeScreen.option13.y = 15;
    setTimeScreen.option13.size = 1;

    setTimeScreen.itemsCount = 13;
}

void setDateSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option3.text, "HH:MM:SS", hour, minute, second);
    strcpy(setDateScreen.option1.text, "11:39:39"); //PLACEHOLDER
    setDateScreen.option1.selectable = 0;
    setDateScreen.option1.x = 0;
    setDateScreen.option1.y = 0;
    setDateScreen.option1.size = 1;

    strcpy(setDateScreen.option2.text, "AM");
    setDateScreen.option2.selectable = 0;
    setDateScreen.option2.x = 9;
    setDateScreen.option2.y = 0;
    setDateScreen.option2.size = 1;

    strcpy(setDateScreen.option3.text, "*");
    setDateScreen.option3.selectable = 0;
    setDateScreen.option3.x = 12;
    setDateScreen.option3.y = 0;
    setDateScreen.option3.size = 1;

    strcpy(setDateScreen.option4.text, "*");
    setDateScreen.option4.selectable = 0;
    setDateScreen.option4.x = 13;
    setDateScreen.option4.y = 0;
    setDateScreen.option4.size = 1;

    sprintf(setDateScreen.option5.text, "TTT%cF",DEGREE_SYM);
    setDateScreen.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    setDateScreen.option5.x = 16;
    setDateScreen.option5.y = 0;
    setDateScreen.option5.size = 1;

    strcpy(setDateScreen.title.text, "Set Date");
    setDateScreen.title.selectable = 0;
    setDateScreen.title.x = 3;
    setDateScreen.title.y = 2;
    setDateScreen.title.size = 2;

    strcpy(setDateScreen.option6.text, "Day:");
    setDateScreen.option6.selectable = 0;
    setDateScreen.option6.x = 4;
    setDateScreen.option6.y = 5;
    setDateScreen.option6.size = 1;

    strcpy(setDateScreen.option7.text, " ");
    setDateScreen.option7.selectable = 1;
    setDateScreen.option7.x = 11;
    setDateScreen.option7.y = 5;
    setDateScreen.option7.size = 1;

    strcpy(setDateScreen.option8.text, "Date:");
    setDateScreen.option8.selectable = 0;
    setDateScreen.option8.x = 4;
    setDateScreen.option8.y = 7;
    setDateScreen.option8.size = 1;

    strcpy(setDateScreen.option9.text, " ");
    setDateScreen.option9.selectable = 1;
    setDateScreen.option9.x = 11;
    setDateScreen.option9.y = 7;
    setDateScreen.option9.size = 1;

    strcpy(setDateScreen.option10.text, "Month:");   //AM/PM/24h
    setDateScreen.option10.selectable = 0;
    setDateScreen.option10.x = 4;
    setDateScreen.option10.y = 9;
    setDateScreen.option10.size = 1;

    strcpy(setDateScreen.option11.text, " ");   //AM/PM/24h
    setDateScreen.option11.selectable = 1;
    setDateScreen.option11.x = 11;
    setDateScreen.option11.y = 9;
    setDateScreen.option11.size = 1;

    strcpy(setDateScreen.option12.text, "Year:");
    setDateScreen.option12.selectable = 0;
    setDateScreen.option12.x = 4;
    setDateScreen.option12.y = 11;
    setDateScreen.option12.size = 1;

    strcpy(setDateScreen.option13.text, " ");
    setDateScreen.option13.selectable = 1;
    setDateScreen.option13.x = 11;
    setDateScreen.option13.y = 11;
    setDateScreen.option13.size = 1;

    strcpy(setDateScreen.option14.text, "Set");
    setDateScreen.option14.selectable = 1;
    setDateScreen.option14.x = 9;
    setDateScreen.option14.y = 13;
    setDateScreen.option14.size = 1;

    strcpy(setDateScreen.option15.text, "Friday");
    setDateScreen.option15.selectable = 0;
    setDateScreen.option15.x = 0;
    setDateScreen.option15.y = 14;
    setDateScreen.option15.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(setDateScreen.option16.text, "Nov.21,2021"); //PLACEHOLDER
    setDateScreen.option16.selectable = 0;
    setDateScreen.option16.x = 0;
    setDateScreen.option16.y = 15;
    setDateScreen.option16.size = 1;

    strcpy(setDateScreen.option17.text, "Back");
    setDateScreen.option17.selectable = 1;
    setDateScreen.option17.x = 17;
    setDateScreen.option17.y = 15;
    setDateScreen.option17.size = 1;

    setDateScreen.itemsCount = 17;
}

void setAlarmSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option3.text, "HH:MM:SS", hour, minute, second);
    strcpy(setAlarmsScreen.option1.text, "11:39:39"); //PLACEHOLDER
    setAlarmsScreen.option1.selectable = 0;
    setAlarmsScreen.option1.x = 0;
    setAlarmsScreen.option1.y = 0;
    setAlarmsScreen.option1.size = 1;

    strcpy(setAlarmsScreen.option2.text, "AM");
    setAlarmsScreen.option2.selectable = 0;
    setAlarmsScreen.option2.x = 9;
    setAlarmsScreen.option2.y = 0;
    setAlarmsScreen.option2.size = 1;

    strcpy(setAlarmsScreen.option3.text, "*");
    setAlarmsScreen.option3.selectable = 0;
    setAlarmsScreen.option3.x = 12;
    setAlarmsScreen.option3.y = 0;
    setAlarmsScreen.option3.size = 1;

    strcpy(setAlarmsScreen.option4.text, "*");
    setAlarmsScreen.option4.selectable = 0;
    setAlarmsScreen.option4.x = 13;
    setAlarmsScreen.option4.y = 0;
    setAlarmsScreen.option4.size = 1;

    sprintf(setAlarmsScreen.option5.text, "TTT%cF",DEGREE_SYM);
    setAlarmsScreen.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    setAlarmsScreen.option5.x = 16;
    setAlarmsScreen.option5.y = 0;
    setAlarmsScreen.option5.size = 1;

    strcpy(setAlarmsScreen.title.text, "Set Alarm");
    setAlarmsScreen.title.selectable = 0;
    setAlarmsScreen.title.x = 2;
    setAlarmsScreen.title.y = 2;
    setAlarmsScreen.title.size = 2;

    strcpy(setAlarmsScreen.option6.text, "Alarm #1:");
    setAlarmsScreen.option6.selectable = 0;
    setAlarmsScreen.option6.x = 0;
    setAlarmsScreen.option6.y = 5;
    setAlarmsScreen.option6.size = 1;

    strcpy(setAlarmsScreen.option7.text, "Date");
    setAlarmsScreen.option7.selectable = 1;
    setAlarmsScreen.option7.x = 3;
    setAlarmsScreen.option7.y = 6;
    setAlarmsScreen.option7.size = 1;

    strcpy(setAlarmsScreen.option8.text, "HH");
    setAlarmsScreen.option8.selectable = 1;
    setAlarmsScreen.option8.x = 8;
    setAlarmsScreen.option8.y = 6;
    setAlarmsScreen.option8.size = 1;

    strcpy(setAlarmsScreen.option9.text, ":");
    setAlarmsScreen.option9.selectable = 0;
    setAlarmsScreen.option9.x = 10;
    setAlarmsScreen.option9.y = 6;
    setAlarmsScreen.option9.size = 1;

    strcpy(setAlarmsScreen.option10.text, "MM");
    setAlarmsScreen.option10.selectable = 1;
    setAlarmsScreen.option10.x = 11;
    setAlarmsScreen.option10.y = 6;
    setAlarmsScreen.option10.size = 1;

    strcpy(setAlarmsScreen.option11.text, ":");
    setAlarmsScreen.option11.selectable = 0;
    setAlarmsScreen.option11.x = 13;
    setAlarmsScreen.option11.y = 6;
    setAlarmsScreen.option11.size = 1;

    strcpy(setAlarmsScreen.option12.text, "SS");
    setAlarmsScreen.option12.selectable = 1;
    setAlarmsScreen.option12.x = 14;
    setAlarmsScreen.option12.y = 6;
    setAlarmsScreen.option12.size = 1;

    strcpy(setAlarmsScreen.option13.text, "AM");
    setAlarmsScreen.option13.selectable = 1;
    setAlarmsScreen.option13.x = 17;
    setAlarmsScreen.option13.y = 6;
    setAlarmsScreen.option13.size = 1;
//
    strcpy(setAlarmsScreen.option14.text, "Set");
    setAlarmsScreen.option14.selectable = 1;
    setAlarmsScreen.option14.x = 6;
    setAlarmsScreen.option14.y = 7;
    setAlarmsScreen.option14.size = 1;
//
    strcpy(setAlarmsScreen.option15.text, "Cancel");
    setAlarmsScreen.option15.selectable = 1;
    setAlarmsScreen.option15.x = 10;
    setAlarmsScreen.option15.y = 7;
    setAlarmsScreen.option15.size = 1;

    strcpy(setAlarmsScreen.option16.text, "Alarm #2:");   //AM/PM/24h
    setAlarmsScreen.option16.selectable = 0;
    setAlarmsScreen.option16.x = 0;
    setAlarmsScreen.option16.y = 10;
    setAlarmsScreen.option16.size = 1;

    strcpy(setAlarmsScreen.option17.text, "Date");
    setAlarmsScreen.option17.selectable = 1;
    setAlarmsScreen.option17.x = 3;
    setAlarmsScreen.option17.y = 11;
    setAlarmsScreen.option17.size = 1;

    strcpy(setAlarmsScreen.option18.text, "HH");
    setAlarmsScreen.option18.selectable = 1;
    setAlarmsScreen.option18.x = 8;
    setAlarmsScreen.option18.y = 11;
    setAlarmsScreen.option18.size = 1;

    strcpy(setAlarmsScreen.option19.text, ":");
    setAlarmsScreen.option19.selectable = 0;
    setAlarmsScreen.option19.x = 10;
    setAlarmsScreen.option19.y = 11;
    setAlarmsScreen.option19.size = 1;

    strcpy(setAlarmsScreen.option20.text, "MM");
    setAlarmsScreen.option20.selectable = 1;
    setAlarmsScreen.option20.x = 11;
    setAlarmsScreen.option20.y = 11;
    setAlarmsScreen.option20.size = 1;

    strcpy(setAlarmsScreen.option21.text, "AM");
    setAlarmsScreen.option21.selectable = 1;
    setAlarmsScreen.option21.x = 14;
    setAlarmsScreen.option21.y = 11;
    setAlarmsScreen.option21.size = 1;

    strcpy(setAlarmsScreen.option22.text, "Set");
    setAlarmsScreen.option22.selectable = 1;
    setAlarmsScreen.option22.x = 6;
    setAlarmsScreen.option22.y = 12;
    setAlarmsScreen.option22.size = 1;

    strcpy(setAlarmsScreen.option23.text, "Cancel");
    setAlarmsScreen.option23.selectable = 1;
    setAlarmsScreen.option23.x = 10;
    setAlarmsScreen.option23.y = 12;
    setAlarmsScreen.option23.size = 1;

    strcpy(setAlarmsScreen.option24.text, "Friday");
    setAlarmsScreen.option24.selectable = 0;
    setAlarmsScreen.option24.x = 0;
    setAlarmsScreen.option24.y = 14;
    setAlarmsScreen.option24.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(setAlarmsScreen.option25.text, "Nov.21,2021"); //PLACEHOLDER
    setAlarmsScreen.option25.selectable = 0;
    setAlarmsScreen.option25.x = 0;
    setAlarmsScreen.option25.y = 15;
    setAlarmsScreen.option25.size = 1;

    strcpy(setAlarmsScreen.option26.text, "Back");
    setAlarmsScreen.option26.selectable = 1;
    setAlarmsScreen.option26.x = 17;
    setAlarmsScreen.option26.y = 15;
    setAlarmsScreen.option26.size = 1;

    setAlarmsScreen.itemsCount = 26;
}

void setPreferencesSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option3.text, "HH:MM:SS", hour, minute, second);
    strcpy(setPreferencesScreen.option1.text, "11:39:39"); //PLACEHOLDER
    setPreferencesScreen.option1.selectable = 0;
    setPreferencesScreen.option1.x = 0;
    setPreferencesScreen.option1.y = 0;
    setPreferencesScreen.option1.size = 1;

    strcpy(setPreferencesScreen.option2.text, "AM");
    setPreferencesScreen.option2.selectable = 0;
    setPreferencesScreen.option2.x = 9;
    setPreferencesScreen.option2.y = 0;
    setPreferencesScreen.option2.size = 1;

    strcpy(setPreferencesScreen.option3.text, "*");
    setPreferencesScreen.option3.selectable = 0;
    setPreferencesScreen.option3.x = 12;
    setPreferencesScreen.option3.y = 0;
    setPreferencesScreen.option3.size = 1;

    strcpy(setPreferencesScreen.option4.text, "*");
    setPreferencesScreen.option4.selectable = 0;
    setPreferencesScreen.option4.x = 13;
    setPreferencesScreen.option4.y = 0;
    setPreferencesScreen.option4.size = 1;

    sprintf(setPreferencesScreen.option5.text, "%c%c",DEGREE_SYM, 'C');
    setPreferencesScreen.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    setPreferencesScreen.option5.x = 16;
    setPreferencesScreen.option5.y = 0;
    setPreferencesScreen.option5.size = 1;

    strcpy(setPreferencesScreen.title.text, "Settings");
    setPreferencesScreen.title.selectable = 0;
    setPreferencesScreen.title.x = 3;
    setPreferencesScreen.title.y = 2;
    setPreferencesScreen.title.size = 2;

    strcpy(setPreferencesScreen.option6.text, "Units:");   //AM/PM/24h
    setPreferencesScreen.option6.selectable = 0;
    setPreferencesScreen.option6.x = 0;
    setPreferencesScreen.option6.y = 4;
    setPreferencesScreen.option6.size = 1;

    strcpy(setPreferencesScreen.option7.text, "Temp:");
    setPreferencesScreen.option7.selectable = 0;
    setPreferencesScreen.option7.x = 1;
    setPreferencesScreen.option7.y = 5;
    setPreferencesScreen.option7.size = 1;

//    sprintf(setPreferencesScreen.option8.text, "TTT%cF",DEGREE_SYM);
    strcpy(tempUnit, "C");
    sprintf(setPreferencesScreen.option8.text, "%c%s", DEGREE_SYM, tempUnit);
    setPreferencesScreen.option8.selectable = 1;
    setPreferencesScreen.option8.x = 7;
    setPreferencesScreen.option8.y = 5;
    setPreferencesScreen.option8.size = 1;

    strcpy(setPreferencesScreen.option9.text, "Speed:");   //AM/PM/24h
    setPreferencesScreen.option9.selectable = 0;
    setPreferencesScreen.option9.x = 1;
    setPreferencesScreen.option9.y = 6;
    setPreferencesScreen.option9.size = 1;

    strcpy(setPreferencesScreen.option10.text, "KM/H");
    setPreferencesScreen.option10.selectable = 1;
    setPreferencesScreen.option10.x = 8;
    setPreferencesScreen.option10.y = 6;
    setPreferencesScreen.option10.size = 1;

    strcpy(setPreferencesScreen.option11.text, "Screen:");   //AM/PM/24h
    setPreferencesScreen.option11.selectable = 0;
    setPreferencesScreen.option11.x = 0;
    setPreferencesScreen.option11.y = 7;
    setPreferencesScreen.option11.size = 1;

    strcpy(setPreferencesScreen.option12.text, "Appearance:");
    setPreferencesScreen.option12.selectable = 0;
    setPreferencesScreen.option12.x = 1;
    setPreferencesScreen.option12.y = 8;
    setPreferencesScreen.option12.size = 1;

    strcpy(setPreferencesScreen.option13.text, "Dark");   //AM/PM/24h
    setPreferencesScreen.option13.selectable = 1;
    setPreferencesScreen.option13.x = 13;
    setPreferencesScreen.option13.y = 8;
    setPreferencesScreen.option13.size = 1;

    strcpy(setPreferencesScreen.option14.text, "Brightness:");
    setPreferencesScreen.option14.selectable = 0;
    setPreferencesScreen.option14.x = 1;
    setPreferencesScreen.option14.y = 9;
    setPreferencesScreen.option14.size = 1;

    strcpy(setPreferencesScreen.option15.text, "Auto");   //Auto/0%-100%
    setPreferencesScreen.option15.selectable = 1;
    setPreferencesScreen.option15.x = 13;
    setPreferencesScreen.option15.y = 9;
    setPreferencesScreen.option15.size = 1;

    strcpy(setPreferencesScreen.option16.text, "Text Color:");
    setPreferencesScreen.option16.selectable = 0;
    setPreferencesScreen.option16.x = 1;
    setPreferencesScreen.option16.y = 10;
    setPreferencesScreen.option16.size = 1;

    strcpy(setPreferencesScreen.option17.text, "White");   //Show color in the text
    setPreferencesScreen.option17.selectable = 1;
    setPreferencesScreen.option17.x = 13;
    setPreferencesScreen.option17.y = 10;
    setPreferencesScreen.option17.size = 1;

    strcpy(setPreferencesScreen.option18.text, "BG Color:");
    setPreferencesScreen.option18.selectable = 0;
    setPreferencesScreen.option18.x = 1;
    setPreferencesScreen.option18.y = 11;
    setPreferencesScreen.option18.size = 1;

    strcpy(setPreferencesScreen.option19.text, "Black");
    setPreferencesScreen.option19.selectable = 1;
    setPreferencesScreen.option19.x = 11;
    setPreferencesScreen.option19.y = 11;
    setPreferencesScreen.option19.size = 1;

    strcpy(setPreferencesScreen.option20.text, "Set");   //Show color in the text
    setPreferencesScreen.option20.selectable = 1;
    setPreferencesScreen.option20.x = 9;
    setPreferencesScreen.option20.y = 13;
    setPreferencesScreen.option20.size = 1;

    strcpy(setPreferencesScreen.option21.text, "Friday");
    setPreferencesScreen.option21.selectable = 0;
    setPreferencesScreen.option21.x = 0;
    setPreferencesScreen.option21.y = 14;
    setPreferencesScreen.option21.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(setPreferencesScreen.option22.text, "Nov.21,2021"); //PLACEHOLDER
    setPreferencesScreen.option22.selectable = 0;
    setPreferencesScreen.option22.x = 0;
    setPreferencesScreen.option22.y = 15;
    setPreferencesScreen.option22.size = 1;

    strcpy(setPreferencesScreen.option23.text, "Back");
    setPreferencesScreen.option23.selectable = 1;
    setPreferencesScreen.option23.x = 17;
    setPreferencesScreen.option23.y = 15;
    setPreferencesScreen.option23.size = 1;

    setPreferencesScreen.itemsCount = 23;
}

void testingSetup(void){
    //This function serves to set up this menu.

//    sprintf(mainScreen.option3.text, "HH:MM:SS", hour, minute, second);
    strcpy(testingScreen.option1.text, "11:39:39"); //PLACEHOLDER
    testingScreen.option1.selectable = 0;
    testingScreen.option1.x = 0;
    testingScreen.option1.y = 0;
    testingScreen.option1.size = 1;

    strcpy(testingScreen.option2.text, "AM");
    testingScreen.option2.selectable = 0;
    testingScreen.option2.x = 9;
    testingScreen.option2.y = 0;
    testingScreen.option2.size = 1;

    strcpy(testingScreen.option3.text, "*");
    testingScreen.option3.selectable = 0;
    testingScreen.option3.x = 12;
    testingScreen.option3.y = 0;
    testingScreen.option3.size = 1;

    strcpy(testingScreen.option4.text, "*");
    testingScreen.option4.selectable = 0;
    testingScreen.option4.x = 13;
    testingScreen.option4.y = 0;
    testingScreen.option4.size = 1;

    sprintf(testingScreen.option5.text, "TTT%cF",DEGREE_SYM);
    testingScreen.option5.selectable = 0;
    //Triple Digits x = 16
    //Double Digits x = 17
    //Single Digits x = 18
    testingScreen.option5.x = 16;
    testingScreen.option5.y = 0;
    testingScreen.option5.size = 1;

    strcpy(testingScreen.title.text, "Testing");
    testingScreen.title.selectable = 0;
    testingScreen.title.x = 2;
    testingScreen.title.y = 2;
    testingScreen.title.size = 2;

    strcpy(testingScreen.option6.text, "-Emergency Logs");
    testingScreen.option6.selectable = 1;
    testingScreen.option6.x = 0;
    testingScreen.option6.y = 4;
    testingScreen.option6.size = 1;

    strcpy(testingScreen.option7.text, "-Motors Test");
    testingScreen.option7.selectable = 1;
    testingScreen.option7.x = 0;
    testingScreen.option7.y = 5;
    testingScreen.option7.size = 1;

    strcpy(testingScreen.option8.text, "-LEDs Test");   //AM/PM/24h
    testingScreen.option8.selectable = 1;
    testingScreen.option8.x = 0;
    testingScreen.option8.y = 6;
    testingScreen.option8.size = 1;

    strcpy(testingScreen.option9.text, "-WDT Test");
    testingScreen.option9.selectable = 1;
    testingScreen.option9.x = 0;
    testingScreen.option9.y = 7;
    testingScreen.option9.size = 1;

    strcpy(testingScreen.option10.text, "-7-Seg Array Test");
    testingScreen.option10.selectable = 1;
    testingScreen.option10.x = 0;
    testingScreen.option10.y = 8;
    testingScreen.option10.size = 1;

    strcpy(testingScreen.option11.text, "-Ultrasonic Test");
    testingScreen.option11.selectable = 1;
    testingScreen.option11.x = 0;
    testingScreen.option11.y = 9;
    testingScreen.option11.size = 1;

    strcpy(testingScreen.option12.text, "-Speaker Test");   //AM/PM/24h
    testingScreen.option12.selectable = 1;
    testingScreen.option12.x = 0;
    testingScreen.option12.y = 10;
    testingScreen.option12.size = 1;

    strcpy(testingScreen.option13.text, "-Encoder Test");   //AM/PM/24h
    testingScreen.option13.selectable = 1;
    testingScreen.option13.x = 0;
    testingScreen.option13.y = 11;
    testingScreen.option13.size = 1;

    strcpy(testingScreen.option14.text, "Friday");
    testingScreen.option14.selectable = 0;
    testingScreen.option14.x = 0;
    testingScreen.option14.y = 14;
    testingScreen.option14.size = 1;

//    sprintf(mainScreen.option1.text, "%c %c, %c", Month, Day, Year);
    strcpy(testingScreen.option15.text, "Nov.21,2021"); //PLACEHOLDER
    testingScreen.option15.selectable = 0;
    testingScreen.option15.x = 0;
    testingScreen.option15.y = 15;
    testingScreen.option15.size = 1;

    strcpy(testingScreen.option16.text, "Back");
    testingScreen.option16.selectable = 1;
    testingScreen.option16.x = 17;
    testingScreen.option16.y = 15;
    testingScreen.option16.size = 1;

    testingScreen.itemsCount = 16;
}

void highlightOption(struct menu passedMenu, uint8_t hlOption){
    uint16_t hlStat1 = textColor;
    uint16_t hlStat2 = textColor;
    uint16_t hlStat3 = textColor;
    uint16_t hlStat4 = textColor;
    uint16_t hlStat5 = textColor;
    uint16_t hlStat6 = textColor;
    uint16_t hlStat7 = textColor;
    uint16_t hlStat8 = textColor;
    uint16_t hlStat9 = textColor;
    uint16_t hlStat10 = textColor;
    uint16_t hlStat11 = textColor;
    uint16_t hlStat12 = textColor;
    uint16_t hlStat13 = textColor;
    uint16_t hlStat14 = textColor;
    uint16_t hlStat15 = textColor;
    uint16_t hlStat16 = textColor;
    uint16_t hlStat17 = textColor;
    uint16_t hlStat18 = textColor;
    uint16_t hlStat19 = textColor;
    uint16_t hlStat20 = textColor;
    uint16_t hlStat21 = textColor;
    uint16_t hlStat22 = textColor;
    uint16_t hlStat23 = textColor;
    uint16_t hlStat24 = textColor;
    uint16_t hlStat25 = textColor;
    uint16_t hlStat26 = textColor;
    uint16_t hlStat27 = textColor;
    uint16_t hlStat28 = textColor;
    uint16_t hlStat29 = textColor;
    uint16_t hlStat30 = textColor;

    uint8_t invertStat1 = 0;
    uint8_t invertStat2 = 0;
    uint8_t invertStat3 = 0;
    uint8_t invertStat4 = 0;
    uint8_t invertStat5 = 0;
    uint8_t invertStat6 = 0;
    uint8_t invertStat7 = 0;
    uint8_t invertStat8 = 0;
    uint8_t invertStat9 = 0;
    uint8_t invertStat10 = 0;
    uint8_t invertStat11 = 0;
    uint8_t invertStat12 = 0;
    uint8_t invertStat13 = 0;
    uint8_t invertStat14 = 0;
    uint8_t invertStat15 = 0;
    uint8_t invertStat16 = 0;
    uint8_t invertStat17 = 0;
    uint8_t invertStat18 = 0;
    uint8_t invertStat19 = 0;
    uint8_t invertStat20 = 0;
    uint8_t invertStat21 = 0;
    uint8_t invertStat22 = 0;
    uint8_t invertStat23 = 0;
    uint8_t invertStat24 = 0;
    uint8_t invertStat25 = 0;
    uint8_t invertStat26 = 0;
    uint8_t invertStat27 = 0;
    uint8_t invertStat28 = 0;
    uint8_t invertStat29 = 0;
    uint8_t invertStat30 = 0;

    switch(hlOption){   //Check what option was selected
        case 1:
            if(passedMenu.option1.selectable == 1){ //Check if option is selectable
                hlStat1 = highlightedColor; //Define what the highlighted color is
                invertStat1 = 1;
            }
            break;
        case 2:
            if(passedMenu.option2.selectable == 1){ //Check if option is selectable
                hlStat2 = highlightedColor; //Define what the highlighted color is
                invertStat2 = 1;
            }
            break;
        case 3:
            if(passedMenu.option3.selectable == 1){ //Check if option is selectable
                hlStat3 = highlightedColor; //Define what the highlighted color is
                invertStat3 = 1;
            }
            break;
        case 4:
            if(passedMenu.option4.selectable == 1){ //Check if option is selectable
                hlStat4 = highlightedColor; //Define what the highlighted color is
                invertStat4 = 1;
            }
            break;
        case 5:
            if(passedMenu.option5.selectable == 1){ //Check if option is selectable
                hlStat5 = highlightedColor; //Define what the highlighted color is
                invertStat5 = 1;
            }
            break;
        case 6:
            if(passedMenu.option6.selectable == 1){ //Check if option is selectable
                hlStat6 = highlightedColor; //Define what the highlighted color is
                invertStat6 = 1;
            }
            break;
        case 7:
            if(passedMenu.option7.selectable == 1){ //Check if option is selectable
                hlStat7 = highlightedColor; //Define what the highlighted color is
                invertStat7 = 1;
            }
            break;
        case 8:
            if(passedMenu.option8.selectable == 1){ //Check if option is selectable
                hlStat8 = highlightedColor; //Define what the highlighted color is
                invertStat8 = 1;
            }
            break;
        case 9:
            if(passedMenu.option9.selectable == 1){ //Check if option is selectable
                hlStat9 = highlightedColor; //Define what the highlighted color is
                invertStat9 = 1;
            }
            break;
        case 10:
            if(passedMenu.option10.selectable == 1){ //Check if option is selectable
                hlStat10 = highlightedColor; //Define what the highlighted color is
                invertStat10 = 1;
            }
            break;
        case 11:
            if(passedMenu.option11.selectable == 1){ //Check if option is selectable
                hlStat11 = highlightedColor; //Define what the highlighted color is
                invertStat11 = 1;
            }
            break;
        case 12:
            if(passedMenu.option12.selectable == 1){ //Check if option is selectable
                hlStat12 = highlightedColor; //Define what the highlighted color is
                invertStat12 = 1;
            }
            break;
        case 13:
            if(passedMenu.option13.selectable == 1){ //Check if option is selectable
                hlStat13 = highlightedColor; //Define what the highlighted color is
                invertStat13 = 1;
            }
            break;
        case 14:
            if(passedMenu.option14.selectable == 1){ //Check if option is selectable
                hlStat14 = highlightedColor; //Define what the highlighted color is
                invertStat14 = 1;
            }
            break;
        case 15:
            if(passedMenu.option15.selectable == 1){ //Check if option is selectable
                hlStat15 = highlightedColor; //Define what the highlighted color is
                invertStat15 = 1;
            }
            break;
        case 16:
            if(passedMenu.option16.selectable == 1){ //Check if option is selectable
                hlStat16 = highlightedColor; //Define what the highlighted color is
                invertStat16 = 1;
            }
            break;
        case 17:
            if(passedMenu.option17.selectable == 1){ //Check if option is selectable
                hlStat17 = highlightedColor; //Define what the highlighted color is
                invertStat17 = 1;
            }
            break;
        case 18:
            if(passedMenu.option18.selectable == 1){ //Check if option is selectable
                hlStat18 = highlightedColor; //Define what the highlighted color is
                invertStat18 = 1;
            }
            break;
        case 19:
            if(passedMenu.option19.selectable == 1){ //Check if option is selectable
                hlStat19 = highlightedColor; //Define what the highlighted color is
                invertStat19 = 1;
            }
            break;
        case 20:
            if(passedMenu.option20.selectable == 1){ //Check if option is selectable
                hlStat20 = highlightedColor; //Define what the highlighted color is
                invertStat20 = 1;
            }
            break;
        case 21:
            if(passedMenu.option21.selectable == 1){ //Check if option is selectable
                hlStat21 = highlightedColor; //Define what the highlighted color is
                invertStat21 = 1;
            }
            break;
        case 22:
            if(passedMenu.option22.selectable == 1){ //Check if option is selectable
                hlStat22 = highlightedColor; //Define what the highlighted color is
                invertStat22 = 1;
            }
            break;
        case 23:
            if(passedMenu.option23.selectable == 1){ //Check if option is selectable
                hlStat23 = highlightedColor; //Define what the highlighted color is
                invertStat23 = 1;
            }
            break;
        case 24:
            if(passedMenu.option24.selectable == 1){ //Check if option is selectable
                hlStat24 = highlightedColor; //Define what the highlighted color is
                invertStat24 = 1;
            }
            break;
        case 25:
            if(passedMenu.option25.selectable == 1){ //Check if option is selectable
                hlStat25 = highlightedColor; //Define what the highlighted color is
                invertStat25 = 1;
            }
            break;
        case 26:
            if(passedMenu.option26.selectable == 1){ //Check if option is selectable
                hlStat26 = highlightedColor; //Define what the highlighted color is
                invertStat26 = 1;
            }
            break;
        case 27:
            if(passedMenu.option27.selectable == 1){ //Check if option is selectable
                hlStat27 = highlightedColor; //Define what the highlighted color is
                invertStat27 = 1;
            }
            break;
        case 28:
            if(passedMenu.option28.selectable == 1){ //Check if option is selectable
                hlStat28 = highlightedColor; //Define what the highlighted color is
                invertStat28 = 1;
            }
            break;
        case 29:
            if(passedMenu.option29.selectable == 1){ //Check if option is selectable
                hlStat29 = highlightedColor; //Define what the highlighted color is
                invertStat29 = 1;
            }
            break;
        case 30:
            if(passedMenu.option30.selectable == 1){ //Check if option is selectable
                hlStat30 = highlightedColor; //Define what the highlighted color is
                invertStat30 = 1;
            }
            break;
        default:
            hlStat1 = textColor;
            hlStat2 = textColor;
            hlStat3 = textColor;
            hlStat4 = textColor;
            hlStat5 = textColor;
            hlStat6 = textColor;
            hlStat7 = textColor;
            hlStat8 = textColor;
            hlStat9 = textColor;
            hlStat10 = textColor;
            hlStat11 = textColor;
            hlStat12 = textColor;
            hlStat13 = textColor;
            hlStat14 = textColor;
            hlStat15 = textColor;
            hlStat16 = textColor;
            hlStat17 = textColor;
            hlStat18 = textColor;
            hlStat19 = textColor;
            hlStat20 = textColor;
            hlStat21 = textColor;
            hlStat22 = textColor;
            hlStat23 = textColor;
            hlStat24 = textColor;
            hlStat25 = textColor;
            hlStat26 = textColor;
            hlStat27 = textColor;
            hlStat28 = textColor;
            hlStat29 = textColor;
            hlStat30 = textColor;

            invertStat1 = 0;
            invertStat2 = 0;
            invertStat3 = 0;
            invertStat4 = 0;
            invertStat5 = 0;
            invertStat6 = 0;
            invertStat7 = 0;
            invertStat8 = 0;
            invertStat9 = 0;
            invertStat10 = 0;
            invertStat11 = 0;
            invertStat12 = 0;
            invertStat13 = 0;
            invertStat14 = 0;
            invertStat15 = 0;
            invertStat16 = 0;
            invertStat17 = 0;
            invertStat18 = 0;
            invertStat19 = 0;
            invertStat20 = 0;
            invertStat21 = 0;
            invertStat22 = 0;
            invertStat23 = 0;
            invertStat24 = 0;
            invertStat25 = 0;
            invertStat26 = 0;
            invertStat27 = 0;
            invertStat28 = 0;
            invertStat29 = 0;
            invertStat30 = 0;
            break;
    }


    //Print the passed menu with the selected properties
    ST7735_DrawString(passedMenu.title.x  ,  passedMenu.title.y  ,  passedMenu.title.size  ,  passedMenu.title.text  , textColor, bgColor, 0);
    ST7735_DrawString(passedMenu.option1.x,  passedMenu.option1.y,  passedMenu.option1.size,  passedMenu.option1.text, hlStat1, bgColor, invertStat1);
    ST7735_DrawString(passedMenu.option2.x,  passedMenu.option2.y,  passedMenu.option2.size,  passedMenu.option2.text, hlStat2, bgColor, invertStat2);

    if(A1F){//If Alarm flag goes off, change the color of the alarm indicator.
        ST7735_DrawString(passedMenu.option3.x,  passedMenu.option3.y,  passedMenu.option3.size,  passedMenu.option3.text, ST7735_RED, bgColor, invertStat3);
    }else{
        ST7735_DrawString(passedMenu.option3.x,  passedMenu.option3.y,  passedMenu.option3.size,  passedMenu.option3.text, ST7735_WHITE, bgColor, invertStat3);
    }

    if(A2F){
        ST7735_DrawString(passedMenu.option4.x,  passedMenu.option4.y,  passedMenu.option4.size,  passedMenu.option4.text, ST7735_RED, bgColor, invertStat4);
    }else{
        ST7735_DrawString(passedMenu.option4.x,  passedMenu.option4.y,  passedMenu.option4.size,  passedMenu.option4.text, ST7735_WHITE, bgColor, invertStat4);
    }


    ST7735_DrawString(passedMenu.option4.x,  passedMenu.option4.y,  passedMenu.option4.size,  passedMenu.option4.text, hlStat4, bgColor, invertStat4);
    ST7735_DrawString(passedMenu.option5.x,  passedMenu.option5.y,  passedMenu.option5.size,  passedMenu.option5.text, hlStat5, bgColor, invertStat5);
    ST7735_DrawString(passedMenu.option6.x,  passedMenu.option6.y,  passedMenu.option6.size,  passedMenu.option6.text, hlStat6, bgColor, invertStat6);
    ST7735_DrawString(passedMenu.option7.x,  passedMenu.option7.y,  passedMenu.option7.size,  passedMenu.option7.text, hlStat7, bgColor, invertStat7);
    ST7735_DrawString(passedMenu.option8.x,  passedMenu.option8.y,  passedMenu.option8.size,  passedMenu.option8.text, hlStat8, bgColor, invertStat8);
    ST7735_DrawString(passedMenu.option9.x,  passedMenu.option9.y,  passedMenu.option9.size,  passedMenu.option9.text, hlStat9, bgColor, invertStat9);
    ST7735_DrawString(passedMenu.option10.x, passedMenu.option10.y, passedMenu.option10.size, passedMenu.option10.text, hlStat10, bgColor, invertStat10);
    ST7735_DrawString(passedMenu.option11.x, passedMenu.option11.y, passedMenu.option11.size, passedMenu.option11.text, hlStat11, bgColor, invertStat11);
    ST7735_DrawString(passedMenu.option12.x, passedMenu.option12.y, passedMenu.option12.size, passedMenu.option12.text, hlStat12, bgColor, invertStat12);
    ST7735_DrawString(passedMenu.option13.x, passedMenu.option13.y, passedMenu.option13.size, passedMenu.option13.text, hlStat13, bgColor, invertStat13);
    ST7735_DrawString(passedMenu.option14.x, passedMenu.option14.y, passedMenu.option14.size, passedMenu.option14.text, hlStat14, bgColor, invertStat14);
    ST7735_DrawString(passedMenu.option15.x, passedMenu.option15.y, passedMenu.option15.size, passedMenu.option15.text, hlStat15, bgColor, invertStat15);
    ST7735_DrawString(passedMenu.option16.x, passedMenu.option16.y, passedMenu.option16.size, passedMenu.option16.text, hlStat16, bgColor, invertStat16);
    ST7735_DrawString(passedMenu.option17.x, passedMenu.option17.y, passedMenu.option17.size, passedMenu.option17.text, hlStat17, bgColor, invertStat17);
    ST7735_DrawString(passedMenu.option18.x, passedMenu.option18.y, passedMenu.option18.size, passedMenu.option18.text, hlStat18, bgColor, invertStat18);
    ST7735_DrawString(passedMenu.option19.x, passedMenu.option19.y, passedMenu.option19.size, passedMenu.option19.text, hlStat19, bgColor, invertStat19);
    ST7735_DrawString(passedMenu.option20.x, passedMenu.option20.y, passedMenu.option20.size, passedMenu.option20.text, hlStat20, bgColor, invertStat20);
    ST7735_DrawString(passedMenu.option21.x, passedMenu.option21.y, passedMenu.option21.size, passedMenu.option21.text, hlStat21, bgColor, invertStat21);
    ST7735_DrawString(passedMenu.option22.x, passedMenu.option22.y, passedMenu.option22.size, passedMenu.option22.text, hlStat22, bgColor, invertStat22);
    ST7735_DrawString(passedMenu.option23.x, passedMenu.option23.y, passedMenu.option23.size, passedMenu.option23.text, hlStat23, bgColor, invertStat23);
    ST7735_DrawString(passedMenu.option24.x, passedMenu.option24.y, passedMenu.option24.size, passedMenu.option24.text, hlStat24, bgColor, invertStat24);
    ST7735_DrawString(passedMenu.option25.x, passedMenu.option25.y, passedMenu.option25.size, passedMenu.option25.text, hlStat25, bgColor, invertStat25);
    ST7735_DrawString(passedMenu.option26.x, passedMenu.option26.y, passedMenu.option26.size, passedMenu.option26.text, hlStat26, bgColor, invertStat26);
    ST7735_DrawString(passedMenu.option27.x, passedMenu.option27.y, passedMenu.option27.size, passedMenu.option27.text, hlStat27, bgColor, invertStat27);
    ST7735_DrawString(passedMenu.option28.x, passedMenu.option28.y, passedMenu.option28.size, passedMenu.option28.text, hlStat28, bgColor, invertStat28);
    ST7735_DrawString(passedMenu.option29.x, passedMenu.option29.y, passedMenu.option29.size, passedMenu.option29.text, hlStat29, bgColor, invertStat29);
    ST7735_DrawString(passedMenu.option30.x, passedMenu.option30.y, passedMenu.option30.size, passedMenu.option30.text, hlStat30, bgColor, invertStat30);
    ST7735_DrawFastHLine(0, 10, 128, textColor);
}

void updateMenuData(void){

    burstRead(RTC_ADDRESS, 0x00, 19, readData); //Read RTC data to update system
    decode(readData); //Decode read RTC data
    updateColor();  //Update system text and background color

    temperature = convertTemp();    //Convert C temp to selected units
    speedDisp = convertSpeed();     //Convert mph to selected units

    //Update mainScreen
    sprintf(mainScreen.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);    //Store decoded time  into formatted string
    strcpy(mainScreen.option2.text, meridiemString);    //Store AM/PM  to display on screen
    if(alarm1Flag){ //Print Alarm indicator if alarm is set
        strcpy(mainScreen.option3.text, "*");
    }else{
        strcpy(mainScreen.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(mainScreen.option4.text, "*");
    }else{
        strcpy(mainScreen.option4.text, " ");
    }
    sprintf(mainScreen.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);  //Store converted temperature and degree into formatted string
    sprintf(mainScreen.title.text, "%d", (int)speedDisp);//Store system speed into formatted string
    threeDig = speedDisp / 100; //Determine if the speed is a three-digit number
    twoDig = speedDisp / 10;    //Determine if the speed is a two-digit number

    if(threeDig){       //Triple Digit Speed
        mainScreen.title.x = 2; //Change print location to accomodate 3 digit speed
        digitFlag = 3;
    }else if(twoDig){   //Double Digit Speed
        mainScreen.title.x = 5; //Change print location to accomodate 2 digit speed
        digitFlag = 2;
    }else{              //Single Digit Speed
        mainScreen.title.x = 8; //Change print location to accomodate 1 digit speed
        digitFlag = 1;
    }

    if(prevDigitFlag > digitFlag){  //Check speed length flag.
        if(menuState == mainScreenState){ //Check if the current menu state is mainScreenState
            //If there is a change in speed digits, clear the current speed on display.
            ST7735_DrawString(0,  mainScreen.title.y,  mainScreen.title.size,  "      ", textColor, bgColor, 0);
        }
    }

    prevDigitFlag = digitFlag;  //Keep track of last speed digit

    strcpy(mainScreen.option6.text, speedUnit); //update selected speed units
    strcpy(mainScreen.option7.text, dayString); //update day string
    sprintf(mainScreen.option8.text, "%s%d,%d", monthString, date, year);   //Update formatted date string

    //Update mainMenu
    sprintf(mainMenu.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);
    strcpy(mainMenu.option2.text, meridiemString);
    if(alarm1Flag){
        strcpy(mainMenu.option3.text, "*");
    }else{
        strcpy(mainMenu.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(mainMenu.option4.text, "*");
    }else{
        strcpy(mainMenu.option4.text, " ");
    }
    sprintf(mainMenu.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);
    strcpy(mainMenu.option11.text, dayString);
    sprintf(mainMenu.option12.text, "%s%d,%d", monthString, date, year);

    //Update setTime
    sprintf(setTimeScreen.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);
    strcpy(setTimeScreen.option2.text, meridiemString);
    if(alarm1Flag){
        strcpy(setTimeScreen.option3.text, "*");
    }else{
        strcpy(setTimeScreen.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(setTimeScreen.option4.text, "*");
    }else{
        strcpy(setTimeScreen.option4.text, " ");
    }
    sprintf(setTimeScreen.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);
    strcpy(setTimeScreen.option11.text, dayString);
    sprintf(setTimeScreen.option12.text, "%s%d,%d", monthString, date, year);

    //Update setDate
    sprintf(setDateScreen.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);
    strcpy(setDateScreen.option2.text, meridiemString);
    if(alarm1Flag){
        strcpy(setDateScreen.option3.text, "*");
    }else{
        strcpy(setDateScreen.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(setDateScreen.option4.text, "*");
    }else{
        strcpy(setDateScreen.option4.text, " ");
    }
    sprintf(setDateScreen.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);
    strcpy(setDateScreen.option15.text, dayString);
    sprintf(setDateScreen.option16.text, "%s%d,%d", monthString, date, year);

    //Update setAlarm
    sprintf(setAlarmsScreen.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);
    strcpy(setAlarmsScreen.option2.text, meridiemString);
    if(alarm1Flag){
        strcpy(setAlarmsScreen.option3.text, "*");
    }else{
        strcpy(setAlarmsScreen.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(setAlarmsScreen.option4.text, "*");
    }else{
        strcpy(setAlarmsScreen.option4.text, " ");
    }
    sprintf(setAlarmsScreen.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);
    strcpy(setAlarmsScreen.option24.text, dayString);
    sprintf(setAlarmsScreen.option25.text, "%s%d,%d", monthString, date, year);

    //Update setPreferences
    sprintf(setPreferencesScreen.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);
    strcpy(setPreferencesScreen.option2.text, meridiemString);
    if(alarm1Flag){
        strcpy(setPreferencesScreen.option3.text, "*");
    }else{
        strcpy(setPreferencesScreen.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(setPreferencesScreen.option4.text, "*");
    }else{
        strcpy(setPreferencesScreen.option4.text, " ");
    }
    sprintf(setPreferencesScreen.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);
    strcpy(setPreferencesScreen.option21.text, dayString);
    sprintf(setPreferencesScreen.option22.text, "%s%d,%d", monthString, date, year);

    //Update Testing
    sprintf(testingScreen.option1.text, "%02d:%02d:%02d", hours, minutes, seconds);
    strcpy(testingScreen.option2.text, meridiemString);
    if(alarm1Flag){
        strcpy(testingScreen.option3.text, "*");
    }else{
        strcpy(testingScreen.option3.text, " ");
    }
    if(alarm2Flag){
        strcpy(testingScreen.option4.text, "*");
    }else{
        strcpy(testingScreen.option4.text, " ");
    }
    sprintf(testingScreen.option5.text, "%d%c%s",(int)temperature, DEGREE_SYM, tempUnit);
    strcpy(testingScreen.option14.text, dayString);
    sprintf(testingScreen.option15.text, "%s%d,%d", monthString, date, year);
}

float convertSpeed(void){
    switch(speedUnitSelection){ //Check what speed has been selected by the user
        case KmPerHour: //Convert to km/h from mph
            kmPerHour = speed * 1.60934;
            strcpy(speedUnit, "KM/H");
            return kmPerHour;
        case MilesPerHour: //Convert to mph from mph
            milesPerHour = speed;
            strcpy(speedUnit, "MPH");
            return milesPerHour;
        case MetersPerSecond: //Convert to m/s from mph
            metersPerSec = speed / 2.237;
            strcpy(speedUnit, "M/S");
            return metersPerSec;
        case Knot: //Convert to knots from mph
            knot = speed * 1.151;
            strcpy(speedUnit, "KNOT");
            return knot;
        case FeetPerSec: //Convert to ft/s from mph
            feetPerSec = speed * 1.467;
            strcpy(speedUnit, "FT/S");
            return feetPerSec;
        default:
            speedUnitSelection = MilesPerHour;
            break;
    }
    return 0;
}

void updateColor(void){
    switch(appearance){ //Check what appearance has been selected by the user
        case Light: //Light mode selected
            customFlag = 0;
            textColor = ST7735_BLACK;
            highlightedColor = ~textColor;
            bgColor = ~textColor;
            break;
        case Dark:  //Dark mode selected
            customFlag = 0;
            textColor = ST7735_WHITE;
            highlightedColor = ~textColor;
            bgColor = ~textColor;
            break;
        case Custom: //Custome mode selected
            customFlag = 1;
            updateCustomColor();    //Update text and background color to reflect user selections
            break;
        default:
            appearance = Dark;  //Use dark mode as default mode
            break;
    }
}

void updateCustomColor(void){
    switch(textColorSelected){  //Check what color user has selected
        case Black:
            textColor = ST7735_BLACK;
            break;
        case Blue:
            textColor = ST7735_BLUE;
            break;
        case Red:
            textColor = ST7735_RED;
            break;
        case Green:
            textColor = ST7735_GREEN;
            break;
        case Cyan:
            textColor = ST7735_CYAN;
            break;
        case Magenta:
            textColor = ST7735_MAGENTA;
            break;
        case Yellow:
            textColor = ST7735_YELLOW;
            break;
        case White:
            textColor = ST7735_WHITE;
            break;
        default:
            textColorSelected = White;
            break;
    }
    if(textColorSelected == bgColorSelected){   //Make sure that user cannot select the same background and text color to avoid navigation issues
        highlightedColor = ~textColor;  //If text and background color are the same, set the background color to be the inverse of the text color
        bgColor = ~textColor;
    }else{
        switch(bgColorSelected){
            case Black:
                highlightedColor = ST7735_BLACK;
                bgColor = ST7735_BLACK;
                break;
            case Blue:
                highlightedColor = ST7735_BLUE;
                bgColor = ST7735_BLUE;
                break;
            case Red:
                highlightedColor = ST7735_RED;
                bgColor = ST7735_RED;
                break;
            case Green:
                highlightedColor = ST7735_GREEN;
                bgColor = ST7735_GREEN;
                break;
            case Cyan:
                highlightedColor = ST7735_CYAN;
                bgColor = ST7735_CYAN;
                break;
            case Magenta:
                highlightedColor = ST7735_MAGENTA;
                bgColor = ST7735_MAGENTA;
                break;
            case Yellow:
                highlightedColor = ST7735_YELLOW;
                bgColor = ST7735_YELLOW;
                break;
            case White:
                highlightedColor = ST7735_WHITE;
                bgColor = ST7735_WHITE;
                break;
            default:
                bgColorSelected = White;
                break;
        }
    }
}
