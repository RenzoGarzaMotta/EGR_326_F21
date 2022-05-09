/*
 * menus.h
 *
 *  Created on: Nov 25, 2021
 *      Author: renzogarzamotta
 */

#ifndef MENUS_H_
#define MENUS_H_

#include "msp.h"
#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "systick.h"
#include "ST7735.h"

#define DEGREE_SYM  247

//Each item within a menu has different properties
struct item{
    char text[20];      //Text property of item
    uint8_t selectable; //Selectable property of the item
    uint8_t x;          //x-location of the item
    uint8_t y;          //y-location of the item
    uint8_t size;       //size of item
};


//Each menu had different selectable options, each with different properties.
struct menu{
    uint8_t itemsCount;     //Count indicates how many items each menu is using to limit scrolling
    struct item title;      //Title item which is never selectable
    struct item option1;    //
    struct item option2;    //
    struct item option3;    //
    struct item option4;    //
    struct item option5;    //
    struct item option6;    //
    struct item option7;    //
    struct item option8;    //
    struct item option9;    //
    struct item option10;   //
    struct item option11;   //
    struct item option12;   //
    struct item option13;   //
    struct item option14;   //
    struct item option15;   //
    struct item option16;   //
    struct item option17;   //
    struct item option18;   //
    struct item option19;   //
    struct item option20;   //
    struct item option21;   //
    struct item option22;   //
    struct item option23;   //
    struct item option24;   //
    struct item option25;   //
    struct item option26;   //
    struct item option27;   //
    struct item option28;   //
    struct item option29;   //
    struct item option30;   //
};

//Indicate all menus available
enum menus {
    mainScreenState,
    mainMenuState,
    setTimeState,
    setDateState,
    setAlarmState,
    setPrefState,
    testingState
};

//Indicate speed units available
enum speedUnits{
    KmPerHour,
    MilesPerHour,
    MetersPerSecond,
    Knot,
    FeetPerSec
};

//Indicate appearance options
enum appearanceOptions{
    Light,
    Dark,
    Custom
};

//Indicate color options
enum colorOptions{
    Black,
    Blue,
    Red,
    Green,
    Cyan,
    Magenta,
    Yellow,
    White,
};

void highlightOption(struct menu passedMenu, uint8_t hlOption);
void mainScreenSetup(void);
void mainMenuSetup(void);
void setTimeSetup(void);
void setDateSetup(void);
void setAlarmSetup(void);
void setPreferencesSetup(void);
void testingSetup(void);
void updateMenuData(void);
float convertSpeed(void);
void updateColor(void);
void updateCustomColor(void);

//struct menu testMenu;
struct menu mainMenu;
struct menu mainScreen;
struct menu setTimeScreen;
struct menu setDateScreen;
struct menu setAlarmsScreen;
struct menu setPreferencesScreen;
struct menu testingScreen;

#endif /* MENUS_H_ */
