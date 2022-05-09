//EGR326 Final Project
//
//Authors:
//  Cianna Janicke
//  Renzo Garza Motta
//
//Instructor:
//  Dr. Nabeeh Kanlaft
//

#include "msp.h"
#include "RTC.h"        //Include Real-Time-Clock Library
#include "macros.h"     //Include Library with macros used across this project
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "systick.h"            //Include library to set up systick timer
#include "ST7735.h"             //Include library to run ST7735 LCD
#include "timer32.h"            //Include library to set up Timer32 to control project components
#include "menus.h"              //Include library to set up and manage menus used in the project
#include "ADC.h"                //Include library to set up on-board ADC
#include "timerA.h"             //Include library to set up timerA
#include "ultrasonicSensor.h"   //Include library to set up ultrasonic sensor HC-SR04
#include "EEPROM.h"             //Include library to set up and communicate with EEPROM
#include "7Segment.h"           //Include library to set up and control 7-segment array display

//Function prototypes
void initialization(void);
void encoderInitialization(void);
void encoderControl(struct menu passedMenu);
uint8_t debounceEncoder(void);
uint8_t debounceEncoderRotation(void);
void resetVariables(void);

//Enumerate the temperature Units available
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

uint16_t yearScroll = 2021;
uint8_t scrolling, hourScroll, minuteScroll, meridiemScroll, dayScroll = 1, dateScroll = 1, monthScroll = 1;
uint8_t updateHour, updateMinutes, updateMeridiem, updateDay, updateDate, updateMonth, updateYear;
uint8_t cancelA1;
uint8_t scrollA1Date, scrollA1Hour, scrollA1Minute, scrollA1Second, scrollA1Meridiem;
uint8_t updateA1Date, updateA1Hour, updateA1Minute, updateA1Second, updateA1Meridiem;
uint8_t cancelA2;
uint8_t scrollA2Date, scrollA2Hour, scrollA2Minute, scrollA2Second, scrollA2Meridiem;
uint8_t updateA2Date, updateA2Hour, updateA2Minute, updateA2Meridiem;
uint8_t scrollAppearance = Dark, scrollBrightness = 105, scrollTextColor = White, scrollBGColor = Black;
uint8_t updateTempUnits, updateSpeedUnits, updateAppearance, updateBrightness, updateTextColor, updateBGColor;
uint8_t tempUnitsScroll, speedUnitsScroll, appearanceScroll, brightnessScroll, textColorScroll, bgColorScroll;
uint8_t setTimeFlag;
uint8_t tempSelection = Celsius, currentTempSelection, speedUnitSelection = KmPerHour, currentSpeedUnitSelection, appearance = Dark;
uint8_t brightness, textColorSelected = White, bgColorSelected = Black;

float speed = 0;

uint8_t refresh = 1;
uint8_t btn1Flag = 0;   //Flag to indicate button1 has been pressed
uint8_t CWflag = 0, CCWflag = 0;    //Flag to indicate if a CW or CCW rotation has taken place
uint8_t currentSelection = 1, selectedOption, menuState = mainScreenState;
int encoderCount;
uint8_t interruptCount;
int CW = 0, CCW = 0;    //These counters keep track of rotation count in the CW or CCW direction
extern uint16_t textColor;  //ST7735_WHITE
extern uint16_t highlightedColor;
extern uint16_t bgColor;
extern uint8_t brightnessLoad;
int lastDT;
int currentDT;

extern uint8_t alarm1Flag;
extern uint8_t alarm2Flag;
extern uint8_t customFlag;
extern uint8_t readData[19];
extern uint8_t sendData[19];
extern char dayString[10];
extern char monthString[5];
extern char meridiemString[5];

extern uint16_t ultrasonicPeriod;
uint32_t currentEdge, lastEdge, deltaCycles;
double distCentimeter, distMilli, rollingValues[8], sum, rollingAverage, inchAverage;
uint8_t x, y;

uint32_t hallLastEdge, hallCurrentEdge, hallDeltaCycles;

double rots, micro, sum, temp;
int rotCount;

double hallRollingValues[8], hallSum, rpmRollingAverage;

char scrollDayString[10];
char scrollMonthString[10];
float tempToDisplay;

float radius = 0.0002130682, circumference, odometerRemainder, countForOdometer;
int oldOdometer;
uint8_t engaged, lastSeconds, lastSecBlink, tooClose;

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	circumference = 2 * PI * radius;    //Determine the circumference of theoretical wheel

	initialization();   //initialize all necessary pins

	NVIC->ISER[0] = 1 << ((WDT_A_IRQn) & 31);   //Enable Watchdog interrupt
	NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31);     //Enable TimerA0.N interrupt
	NVIC->ISER[0] = 1 << (TA3_N_IRQn & 31);     //Enable TimerA3.N interrupt
	NVIC->ISER[0] = 1 << (T32_INT1_IRQn & 31);  //Enable Timer32_INT1 interrupt
	NVIC->ISER[0] = 1 << (T32_INT2_IRQn & 31);  //Enable Timer32_INT2 interrupt
    NVIC->ISER[1] = 1 << (PORT2_IRQn & 31);     //Enable Port2 interrupt
    __enable_irq();                             //Enable all interrupts

	ST7735_FillScreen(bgColor);    //Clear the screen by filling the screen with an all-black screen.

//	testData();     //Used for EEPROM Testing
//	writeEEPROM();  //Used for EEPROM Testing

	readEEPROM();   //Read data stored in EEPROM

	//Store pre-set settings
	oldOdometer = odometer;         //Store odometer value stored in EEPROM from previous session
	tempSelection = tempUnitsEEPROM;    //Get temperature units presets
    speedUnitSelection = speedUnitsEEPROM;  //Get speed units presets
    appearance = appearanceEEPROM;  //Get appearance presets

    switch(appearance){ //Set up appropriate system appearance from stored presets
        case Light:
            customFlag = 0;
            textColor = ST7735_BLACK;
            highlightedColor = ~textColor;
            bgColor = ~textColor;
            break;
        case Dark:
            customFlag = 0;
            textColor = ST7735_WHITE;
            highlightedColor = ~textColor;
            bgColor = ~textColor;
            break;
        case Custom:
            customFlag = 1;

            textColorSelected = textColorEEPROM;
            bgColorSelected = bgColorEEPROM;
            break;
        default:
            appearance = Dark;
            break;
    }
    brightnessLoad = brightnessEEPROM;  //Set birghtness presets
    lastSeconds = seconds;              //Store latest seconds gotten from the RTC for future checks

	while(1){   //Enter while(1) loop
	    if(lastSeconds > seconds){  //Check if there was a seconds overflow (59s -> 0s) in the RTC
	        if((lastSeconds + 2) == (seconds + 59)){    //Check if two seconds have elapsed since the last check
                engaged = 0;    //If two or more seconds have elapsed, reset engaged flag which resets rpmRollingAverage reading.
                lastSeconds = seconds;  //Store most current time in seconds
            }
	    }else{  //Check if no overflow has happened
	        if((lastSeconds + 2) == seconds){   //Check if two seconds have elapsed since the last check
                engaged = 0;    //If two or more seconds have elapsed, reset engaged flag which resets rpmRollingAverage reading.
                lastSeconds = seconds;      //Store most current time in seconds
            }
	    }

	    if(lastSecBlink != seconds){    //Check if at least one second has elapsed for turn signals and prox alert LED blinking
	        TURNSIG_PORT->OUT ^= TURNSIG;   //Toggle turning signal every second
	        if(tooClose){   //Check if the proximity sensor detects an object within 8 inches.
	            PROXBLINK_PORT->OUT ^= PROXBLINK;   //Toggle prox sensor alert every second
	        }else{
	            PROXBLINK_PORT->OUT &=~ PROXBLINK;  //Turn off prox sensor alert every second
	        }
	        lastSecBlink = seconds; //Store latest second
	    }

	    if(!engaged){   //Check state of engaged flag
	        rpmRollingAverage = 0;  //if the hall effect hasn't detected movement in 2 seconds, reset speed.
	    }

	    msDelay(60);    //Set up a 60ms delay for ultrasonic sensor.
	    updateMenuData();   //Update the menu data
	    sampleDistance();   //Sample distance from HC-SR04
	    readAmbientLighting();  //Read ambient lighting from internal ADC from photoresistor
        brightnessControl();    //Adjust PWM signal to reflect ambient reading.


        //Check for RPMs rolling average to adjust analog needle
        //locations require adjustment for physical print
        if((0 <= rpmRollingAverage) && (rpmRollingAverage < 10)){
            rpmRollingAverage -= 0;
            updateTachometer(66 + (rpmRollingAverage * 7)); //0
        }else if((10 <= rpmRollingAverage) && (rpmRollingAverage < 20)){
            rpmRollingAverage -= 10;
            updateTachometer(132 + (rpmRollingAverage * 7)); //10
        }else if((20 <= rpmRollingAverage) && (rpmRollingAverage < 30)){
            rpmRollingAverage -= 20;
            updateTachometer(180 + (rpmRollingAverage * 7)); //20
        }else if((30 <= rpmRollingAverage) && (rpmRollingAverage < 40)){
            rpmRollingAverage -= 30;
            updateTachometer(228 + (rpmRollingAverage * 7)); //30
        }else if((40 <= rpmRollingAverage) && (rpmRollingAverage < 50)){
            rpmRollingAverage -= 40;
            updateTachometer(276 + (rpmRollingAverage * 7)); //40
        }else if((50 <= rpmRollingAverage) && (rpmRollingAverage < 60)){
            rpmRollingAverage -= 50;
            updateTachometer(335 + (rpmRollingAverage * 7)); //50
        }else if((60 <= rpmRollingAverage) && (rpmRollingAverage < 70)){
            rpmRollingAverage -= 60;
            updateTachometer(383 + (rpmRollingAverage * 7)); //60
        }else if((70 <= rpmRollingAverage) && (rpmRollingAverage < 80)){
            rpmRollingAverage -= 70;
            updateTachometer(431 + (rpmRollingAverage * 7)); //70
        }else if((80 <= rpmRollingAverage) && (rpmRollingAverage < 90)){
            rpmRollingAverage -= 80;
            updateTachometer(479 + (rpmRollingAverage * 7)); //80
        }else if(90 <= rpmRollingAverage){
            updateTachometer(527); //90
        }

        //Calculate the speed from a given rpm
        speed = (circumference * rpmRollingAverage * 60 * 50);   //mi * (revolutions/1min) * (60min / 1 hr)

        //Check for speed's rolling average to adjust analog needle
        //locations require adjustment for physical print
        if((0 <= speed) && (speed < 10)){
            speed -= 0;
            updateLocation(45 + (speed * 5)); //0
        }else if((10 <= speed) && (speed < 20)){
            speed -= 10;
            updateLocation(90 + (speed * 5)); //10
        }else if((20 <= speed) && (speed < 30)){
            speed -= 20;
            updateLocation(135 + (speed * 5)); //20
        }else if((30 <= speed) && (speed < 40)){
            speed -= 30;
            updateLocation(185 + (speed * 5)); //30
        }else if((40 <= speed) && (speed < 50)){
            speed -= 40;
            updateLocation(235 + (speed * 5)); //40
        }else if((50 <= speed) && (speed < 60)){
            speed -= 50;
            updateLocation(280 + (speed * 5)); //50
        }else if((60 <= speed) && (speed < 70)){
            speed -= 60;
            updateLocation(345 + (speed * 5)); //60
        }else if((70 <= speed) && (speed < 80)){
            speed -= 70;
            updateLocation(405 + (speed * 5)); //70
        }else if((80 <= speed) && (speed < 90)){
            speed -= 80;
            updateLocation(460 + (speed * 5)); //80
        }else if((90 <= speed) && (speed < 100)){
            speed -= 90;
            updateLocation(515 + (speed * 5)); //80
        }else if((100 <= speed) && (speed < 110)){
            speed -= 100;
            updateLocation(570 + (speed * 5)); //80
        }else if(110 <= speed){
            updateLocation(615); //80
        }

        //Calculate estimate distance traveled since last power-on session to update odometer value
        odometerRemainder += (countForOdometer * circumference);
        countForOdometer = 0;//reset hall-effect counter for odometer

        if((int)odometerRemainder == 1){//Check if the system has traveled an accumulated mile.
            readEEPROM();   //Read current system presets

            //Keep entries unchanged
            newEntry1 = entry1;
            newMonth1 = month1;
            newDay1 = day1;
            newHour1 = hour1;
            newMinute1 = minute1;
            newSecond1 = second1;

            newEntry2 = entry2;
            newMonth2 = month2;
            newDay2 = day2;
            newHour2 = hour2;
            newMinute2 = minute2;
            newSecond2 = second2;

            newEntry3 = entry3;
            newMonth3 = month3;
            newDay3 = day3;
            newHour3 = hour3;
            newMinute3 = minute3;
            newSecond3 = second3;

            newEntry4 = entry4;
            newMonth4 = month4;
            newDay4 = day4;
            newHour4 = hour4;
            newMinute4 = minute4;
            newSecond4 = second4;

            newEntry5 = entry5;
            newMonth5 = month5;
            newDay5 = day5;
            newHour5 = hour5;
            newMinute5 = minute5;
            newSecond5 = second5;

            newEntry6 = entry6;
            newMonth6 = month6;
            newDay6 = day6;
            newHour6 = hour6;
            newMinute6 = minute6;
            newSecond6 = second6;

            //(Working notes)Make 0 the reset value. If value is 0, scrolling hasn't happened. Otherwise, store value in EEPROM.

            newTempUnits = currentTempSelection;
            newSpeedUnits = currentSpeedUnitSelection;
            newAppearance = scrollAppearance;
            newBrightness = scrollBrightness;
            newTextColor = scrollTextColor;
            newBgColor = scrollBGColor;

            newOdometer = odometer + 1; //Update odometer value

            writeEEPROM();  //Write updated data to EEPROM

            odometer += 1;  //update array display

            odometerRemainder = 0;  //Reset travel accumulated mile
        }

        encodeDigits(0, odometer);  //Update odometer value in 7-seg display

        //if refresh flag is set, this indicates a change in menu state (UI)
	    if(refresh){    //Check if refresh flag has been set
	        ST7735_FillScreen(bgColor);    //Clear the screen by filling the screen with an all-black screen.
	        refresh = 0;    //Once the screen has been clear, reset flag to allow for updated menu to be printed
	    }
        switch(menuState){  //check for the menuState
            case mainScreenState:   //Check if mainScreenState is the current state selected
                encoderControl(mainScreen); //interpret current state encoder input
                highlightOption(mainScreen, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 9: //Menu
                        menuState = mainMenuState;  //Move to next menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            case mainMenuState:   //Check if mainMenuState is the current state selected
                encoderControl(mainMenu); //interpret current state encoder input
                highlightOption(mainMenu, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 6:     //Time
                        menuState = setTimeState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 7:     //Date
                        menuState = setDateState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 8:     //Alarms
                        menuState = setAlarmState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 9:     //Settings
                        menuState = setPrefState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 10:    //Testing
                        menuState = testingState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 13:    //Back
                        menuState = mainScreenState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            case setTimeState:  //Check if setTimeState is the current state selected
                encoderControl(setTimeScreen);  //interpret current state encoder input
                highlightOption(setTimeScreen, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 6:     //HH
                        //UPDATE HOUR #
                        if(updateHour == 0){    //check and toggle scrolling flag
                            updateHour = 1;
                            scrolling = 1;
                        }
                        break;
                    case 8:     //MM
                        //UPDATE MIN #
                        if(updateMinutes == 0){    //check and toggle scrolling flag
                            updateMinutes = 1;
                            scrolling = 1;
                        }
                        break;
                    case 9:     //AM/PM
                        //UPDATE MERIDIAM
                        if(updateMeridiem == 0){    //check and toggle scrolling flag
                            updateMeridiem = 1;
                            scrolling = 1;
                        }
                        break;
                    case 10:    //Set updated data
                        //SET FLAG
                        newHours = hourScroll;
                        newMinutes = minuteScroll;
                        newSeconds = 0;
                        switch(meridiemScroll){
                            case 0: //AM
                                newMeridiem = 0;
                                newClockSetting = 1;
                                break;
                            case 1: //PM
                                newMeridiem = 1;
                                newClockSetting = 1;
                                break;
                            case 2: //24hr
                                newClockSetting = 0;
                                break;
                            default:
                                break;
                        }

                        hourScroll = 0;
                        minuteScroll = 0;
                        meridiemScroll = 0;

                        encode(sendData);   //encode data for RTC
                        burstWrite(RTC_ADDRESS, 0x00, 3, sendData); //Write data to RTC
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 13:    //Back
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            case setDateState:  //Check if setDateState is the current state selected
                encoderControl(setDateScreen);  //interpret current state encoder input
                highlightOption(setDateScreen, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 7:     //Day Scroll
                        //UPDATE Day of the week
                        if(updateDay == 0){    //check and toggle scrolling flag
                            updateDay = 1;
                            scrolling = 1;
                        }
                        break;
                    case 9:     //Date Scroll
                        //UPDATE Date #
                        if(updateDate == 0){    //check and toggle scrolling flag
                            if(monthScroll == 0){
                                monthScroll = month;
                            }else{
                                monthScroll = 1;
                            }
                            updateDate = 1;
                            scrolling = 1;
                        }
                        break;
                    case 11:     //Month Scroll
                        //UPDATE Month #
                        if(updateMonth == 0){    //check and toggle scrolling flag
                            updateMonth = 1;
                            scrolling = 1;
                        }
                        break;
                    case 13:    //Year Scroll
                        //UPDATE Year #
                        if(updateYear == 0){    //check and toggle scrolling flag
                            updateYear = 1;
                            scrolling = 1;
                        }
                        break;
                    case 14:    //Set updated data
                        newSeconds = seconds;
                        newMinutes = minutes;
                        newHours = hours;
                        newDay = dayScroll;
                        newDate = dateScroll;
                        newMonth = monthScroll;
                        newYear = yearScroll;

                        dayScroll = 0;
                        dateScroll = 0;
                        monthScroll = 0;
                        yearScroll = 0;

                        encode(sendData);   //encode data for RTC
                        burstWrite(RTC_ADDRESS, 0, 7, sendData); //Write data to RTC
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 17:    //Back
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            case setAlarmState:  //Check if setAlarmState is the current state selected
                encoderControl(setAlarmsScreen);    //interpret current state encoder input
                highlightOption(setAlarmsScreen, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 7:     //Date Scroll
                        if(updateA1Date == 0){    //check and toggle scrolling flag
                            scrollA1Date = 1;
                            sprintf(setAlarmsScreen.option7.text, "    ", scrollA1Date);
                            ST7735_DrawString(setAlarmsScreen.option7.x,  setAlarmsScreen.option7.y,  setAlarmsScreen.option7.size,  setAlarmsScreen.option7.text, textColor, bgColor, 0);
                            updateA1Date = 1;
                            scrolling = 1;
                        }
                        break;
                    case 8:     //Hour Scroll
                        if(updateA1Hour == 0){    //check and toggle scrolling flag
                            updateA1Hour = 1;
                            scrolling = 1;
                        }
                        break;
                    case 10:     //Minute Scroll
                        if(updateA1Minute == 0){    //check and toggle scrolling flag
                            updateA1Minute = 1;
                            scrolling = 1;
                        }
                        break;
                    case 12:    //Second Scroll
                        if(updateA1Second == 0){    //check and toggle scrolling flag
                            updateA1Second = 1;
                            scrolling = 1;
                        }
                        break;
                    case 13:    //Meridiam Scroll
                        if(updateA1Meridiem == 0){    //check and toggle scrolling flag
                            updateA1Meridiem = 1;
                            scrolling = 1;
                        }
                        break;
                    case 14:    //Set selected data for alarm 1
                        alarm1Flag = 1;

                        newSeconds = seconds;   //0
                        newMinutes = minutes;   //1
                        newHours = hours;   //2
                        newClockSetting = clockSetting;
                        newMeridiem = meridiem;

                        newDay = day;   //3
                        newDate = date;   //4
                        newMonth = month;   //5
                        newCentury = century;
                        newYear = year;   //6

                        newBitA1M1 = bitA1M1;
                        newBitA1M2 = bitA1M2;
                        newBitA1M3 = bitA1M3;
                        newBitA1M4 = bitA1M4;

                        if(scrollA1Second != 0){
                            newBitA1M1 = 1;
                        }
                        if(scrollA1Minute != 0){
                            newBitA1M2 = 1;
                        }
                        if(scrollA1Hour != 0){
                            newBitA1M3 = 1;
                        }
                        if(scrollA1Date != 0){
                            newBitA1M4 = 1;
                        }
                        newSecondsA1 = scrollA1Second;   //7
                        newMinutesA1 = scrollA1Minute;   //8

                        switch(scrollA1Meridiem){
                            case 0: //AM
                                newMeridiemA1 = 0;
                                newClockSettingA1 = 1;
                                break;
                            case 1: //PM
                                newMeridiemA1 = 1;
                                newClockSettingA1 = 1;
                                break;
                            case 2: //24hr
                                newClockSettingA1 = 0;
                                break;
                            default:
                                break;
                        }
                        newHoursA1 = scrollA1Hour;   //9
                        newDayDateSelectBitA1 = 0;
                        newDateA1 = scrollA1Date;   //10b

                        newINTCN = 1;
                        newA1IE = 1;
                        newA2IE = A2IE;

                        scrollA1Date = 0;
                        scrollA1Hour = 0;
                        scrollA1Minute = 0;
                        scrollA1Second = 0;
                        scrollA1Meridiem = 0;

                        encode(sendData);  //encode data for RTC
                        burstWrite(RTC_ADDRESS, 0, 11, sendData); //Write data to RTC
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 15:    //Cancel current edits
                        alarm1Flag = 0;

                        if(scrollA1Second != 0){
                            scrollA1Second = 0;
                        }else if(secondsA1 != 0){
                            cancelA1 = 1;
                            newSecondsA1 = 0;
                        }
                        if(scrollA1Minute != 0){
                            scrollA1Minute = 0;
                        }else if(minutesA1 != 0){
                            cancelA1 = 1;
                            newMinutesA1 = 0;
                        }
                        if(scrollA1Hour != 0){
                            scrollA1Hour = 0;
                        }else if(hoursA1 != 0){
                            cancelA1 = 1;
                            newHoursA1 = 0;
                        }
                        if(scrollA1Date != 0){
                            scrollA1Date = 0;
                        }else if(dateA1 != 0){
                            cancelA1 = 1;
                            newDateA1 = 0;
                        }
                        if(cancelA1 == 1){
                            cancelA1 = 0;
                            newSeconds = seconds;   //0
                            newMinutes = minutes;   //1
                            newHours = hours;   //2
                            newClockSetting = clockSetting;
                            newMeridiem = meridiem;

                            newDay = day;   //3
                            newDate = date;   //4
                            newMonth = month;   //5
                            newCentury = century;
                            newYear = year;   //6

                            newClockSettingA1 = 0;
                            newMeridiemA1 = 0;

                            newBitA1M1 = 0;
                            newBitA1M2 = 0;
                            newBitA1M3 = 0;
                            newBitA1M4 = 0;
                            encode(sendData);  //encode data for RTC
                            burstWrite(RTC_ADDRESS, 0, 11, sendData); //Write data to RTC
                        }
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 17:     //Date Scroll
                        if(updateA2Date == 0){    //check and toggle scrolling flag
                            scrollA2Date = 1;
                            sprintf(setAlarmsScreen.option17.text, "    ", scrollA2Date);
                            ST7735_DrawString(setAlarmsScreen.option17.x,  setAlarmsScreen.option17.y,  setAlarmsScreen.option17.size,  setAlarmsScreen.option17.text, textColor, bgColor, 0);
                            updateA2Date = 1;
                            scrolling = 1;
                        }
                        break;
                    case 18:     //Hour Scroll
                        if(updateA2Hour == 0){    //check and toggle scrolling flag
                            updateA2Hour = 1;
                            scrolling = 1;
                        }
                        break;
                    case 20:    //Minute Scroll
                        if(updateA2Minute == 0){    //check and toggle scrolling flag
                            updateA2Minute = 1;
                            scrolling = 1;
                        }
                        break;
                    case 21:    //Meridiam Scroll
                        if(updateA2Meridiem == 0){    //check and toggle scrolling flag
                            updateA2Meridiem = 1;
                            scrolling = 1;
                        }
                        break;
                    case 22:    //Set selected data for alarm 2
                        alarm2Flag = 1;

                        newSeconds = seconds;   //0
                        newMinutes = minutes;   //1
                        newHours = hours;   //2
                        newClockSetting = clockSetting;
                        newMeridiem = meridiem;

                        newDay = day;   //3
                        newDate = date;   //4
                        newMonth = month;   //5
                        newCentury = century;
                        newYear = year;   //6

                        newBitA2M2 = bitA2M2;
                        newBitA2M3 = bitA2M3;
                        newBitA2M4 = bitA2M4;

                        if(scrollA2Minute != 0){
                            newBitA2M2 = 1;
                        }
                        if(scrollA2Hour != 0){
                            newBitA2M3 = 1;
                        }
                        if(scrollA2Date != 0){
                            newBitA2M4 = 1;
                        }
                        newMinutesA2 = scrollA2Minute;   //8

                        switch(scrollA2Meridiem){
                            case 0: //AM
                                newMeridiemA2 = 0;
                                newClockSettingA2 = 1;
                                break;
                            case 1: //PM
                                newMeridiemA2 = 1;
                                newClockSettingA2 = 1;
                                break;
                            case 2: //24hr
                                newClockSettingA2 = 0;
                                break;
                            default:
                                break;
                        }
                        newHoursA2 = scrollA2Hour;   //9
                        newDayDateSelectBitA2 = 0;
                        newDateA2 = scrollA2Date;   //10b

                        newINTCN = 1;
                        newA1IE = A1IE;
                        newA2IE = 1;

                        scrollA2Date = 0;
                        scrollA2Hour = 0;
                        scrollA2Minute = 0;
                        scrollA2Second = 0;
                        scrollA2Meridiem = 0;

                        encode(sendData);  //encode data for RTC
                        burstWrite(RTC_ADDRESS, 0, 14, sendData); //Write data to RTC
                        menuState = mainMenuState;    //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 23:    //Cancel
                        alarm2Flag = 0;

                        if(scrollA2Minute != 0){
                            scrollA2Minute = 0;
                        }else if(minutesA2 != 0){
                            cancelA2 = 1;
                            newMinutesA2 = 0;
                        }
                        if(scrollA2Hour != 0){
                            scrollA2Hour = 0;
                        }else if(hoursA2 != 0){
                            cancelA2 = 1;
                            newHoursA2 = 0;
                        }
                        if(scrollA2Date != 0){
                            scrollA2Date = 0;
                        }else if(dateA2 != 0){
                            cancelA2 = 1;
                            newDateA2 = 0;
                        }
                        if(cancelA2 == 1){
                            cancelA2 = 0;
                            newSeconds = seconds;   //0
                            newMinutes = minutes;   //1
                            newHours = hours;   //2
                            newClockSetting = clockSetting;
                            newMeridiem = meridiem;

                            newDay = day;   //3
                            newDate = date;   //4
                            newMonth = month;   //5
                            newCentury = century;
                            newYear = year;   //6

                            newClockSettingA2 = 0;
                            newMeridiemA2 = 0;

                            newBitA2M2 = 0;
                            newBitA2M3 = 0;
                            newBitA2M4 = 0;
                            encode(sendData);  //encode data for RTC
                            burstWrite(RTC_ADDRESS, 0, 14, sendData); //Write data to RTC
                        }
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 26:    //Back
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            case setPrefState:  //Check if setPrefState is the current state selected
                encoderControl(setPreferencesScreen);   //interpret current state encoder input
                highlightOption(setPreferencesScreen, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 8:     //TEMP Scroll
//                        updateTempUnits,
                        if(updateTempUnits == 0){    //check and toggle scrolling flag
                            strcpy(setPreferencesScreen.option8.text, "         ");
                            ST7735_DrawString(setPreferencesScreen.option8.x,  setPreferencesScreen.option8.y,  setPreferencesScreen.option8.size,  setPreferencesScreen.option8.text, textColor, bgColor, 0);
                            updateTempUnits = 1;
                            scrolling = 1;
                        }
                        break;
                    case 10:     //SPEED Scroll KM/H or MPH
//                        updateSpeedUnits
                        if(updateSpeedUnits == 0){    //check and toggle scrolling flag
                            strcpy(setPreferencesScreen.option10.text, "         ");
                            ST7735_DrawString(setPreferencesScreen.option10.x,  setPreferencesScreen.option10.y,  setPreferencesScreen.option10.size,  setPreferencesScreen.option10.text, textColor, bgColor, 0);
                            updateSpeedUnits = 1;
                            scrolling = 1;
                        }
                        break;
                    case 13:     //Appearance Light/Dark
//                        updateAppearance,
                        if(updateAppearance == 0){    //check and toggle scrolling flag
//                            strcpy(setPreferencesScreen.option15.text, "Auto");
                            strcpy(setPreferencesScreen.option13.text, "         ");
                            ST7735_DrawString(setPreferencesScreen.option13.x,  setPreferencesScreen.option13.y,  setPreferencesScreen.option13.size,  setPreferencesScreen.option13.text, textColor, bgColor, 0);
                            updateAppearance = 1;
                            scrolling = 1;
                        }
                        break;
                    case 15:    //Brightness
//                        updateBrightness,
                        if(updateBrightness == 0){    //check and toggle scrolling flag
                            strcpy(setPreferencesScreen.option15.text, "         ");
                            ST7735_DrawString(setPreferencesScreen.option15.x,  setPreferencesScreen.option15.y,  setPreferencesScreen.option15.size,  setPreferencesScreen.option15.text, textColor, bgColor, 0);
                            updateBrightness = 1;
                            scrolling = 1;
                        }
                        break;
                    case 17:    //Text Color
//                        updateTextColor,
                        if(updateTextColor == 0){    //check and toggle scrolling flag
                            updateTextColor = 1;
                            scrolling = 1;
                        }
                        break;
                    case 19:    //Background Color
//                        updateBGColor;
                        if(updateBGColor == 0){    //check and toggle scrolling flag
                            updateBGColor = 1;
                            scrolling = 1;
                        }
                        break;
                    case 20:    //Set
                        tempSelection = currentTempSelection;
                        speedUnitSelection = currentSpeedUnitSelection;
                        appearance = scrollAppearance;
                        textColorSelected = scrollTextColor;
                        bgColorSelected = scrollBGColor;
                        brightnessLoad = scrollBrightness;

                        readEEPROM();   //Read current entries stored

                        newEntry1 = entry1;
                        newMonth1 = month1;
                        newDay1 = day1;
                        newHour1 = hour1;
                        newMinute1 = minute1;
                        newSecond1 = second1;

                        newEntry2 = entry2;
                        newMonth2 = month2;
                        newDay2 = day2;
                        newHour2 = hour2;
                        newMinute2 = minute2;
                        newSecond2 = second2;

                        newEntry3 = entry3;
                        newMonth3 = month3;
                        newDay3 = day3;
                        newHour3 = hour3;
                        newMinute3 = minute3;
                        newSecond3 = second3;

                        newEntry4 = entry4;
                        newMonth4 = month4;
                        newDay4 = day4;
                        newHour4 = hour4;
                        newMinute4 = minute4;
                        newSecond4 = second4;

                        newEntry5 = entry5;
                        newMonth5 = month5;
                        newDay5 = day5;
                        newHour5 = hour5;
                        newMinute5 = minute5;
                        newSecond5 = second5;

                        newEntry6 = entry6;
                        newMonth6 = month6;
                        newDay6 = day6;
                        newHour6 = hour6;
                        newMinute6 = minute6;
                        newSecond6 = second6;

                        //(Working Notes) Make 0 the reset value. If value is 0, scrolling hasn't happened. Otherwise, store value in EEPROM.
                        //Update user preferences
                        newTempUnits = currentTempSelection;
                        newSpeedUnits = currentSpeedUnitSelection;
                        newAppearance = scrollAppearance;
                        newBrightness = scrollBrightness;
                        newTextColor = scrollTextColor;
                        newBgColor = scrollBGColor;

                        newOdometer = odometer; //Leave odometer value unchanged.

                        writeEEPROM();  //Store new preferences in EEPROM

                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 23:    //Back
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            case testingState:  //Check if testingState is the current state selected
                encoderControl(testingScreen);  //interpret current state encoder input
                highlightOption(testingScreen, currentSelection);  //Determine and highlight what current selected option is being selected
                switch(selectedOption){ //Check what option has been selected
                    case 6:     //Emergency Logs (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 7:     //Motors Test (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 8:     //LEDS Test (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 9:     //WDT Test
                        while(1);
                        break;
                    case 10:    //7-Seg Array Test (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 11:    //Ultrasonic Test (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 12:    //Speaker Test (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 13:    //Encoder Test (Not finished)
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    case 16:    //Back
                        menuState = mainMenuState;  //Move to selected menu state
                        resetVariables();   //Reset variables as menu state changes
                        break;
                    default:
                        break;
                }
                break;
            default:
                menuState = mainScreenState;  //Move to selected menu state
                break;
        }
        WDT_A->CTL = (WDT_A_CTL_PW|             //Enter WDT password
                      WDT_A_CTL_SSEL__ACLK |    //Select ACLK as source
                      WDT_A_CTL_CNTCL |         //Clear Timer
                      WDT_A_CTL_IS_4);          //Select 1s intervals (@ 32kHz)
	}
}



void encoderControl(struct menu passedMenu){
    if(btn1Flag){
        if(scrolling == 1){
            selectedOption = 0;
            updateYear = 0;
            updateMonth = 0;
            updateDate = 0;
            updateDay = 0;
            updateHour = 0;
            updateMinutes = 0;
            updateMeridiem = 0;
            //Alarm1
            updateA1Date = 0;
            updateA1Hour = 0;
            updateA1Minute = 0;
            updateA1Second = 0;
            updateA1Meridiem = 0;
            //Alarm2
            updateA2Date = 0;
            updateA2Hour = 0;
            updateA2Minute = 0;
            updateA2Meridiem = 0;

            //Preferences
            updateTempUnits = 0;
            updateSpeedUnits = 0;
            updateAppearance = 0;
            updateBrightness = 0;
            updateTextColor = 0;
            updateBGColor = 0;
            scrolling = 0;
        }else if(scrolling == 0){
            selectedOption = currentSelection;
        }
        btn1Flag = 0;
    }
//    if(CWflag){                                 //Check if a CW rotation was detected
//        CWflag = 0;                             //Clear flag to avoid faulty click counting
//        currentSelection++;
//        if(currentSelection >= passedMenu.itemsCount){
//            currentSelection = passedMenu.itemsCount;
//        }
//        CW++;                                   //Increase the CW click counter
//    }else if(CCWflag){                                //Check if a CCW rotation was detected
//        CCWflag = 0;                            //Clear flag to avoid faulty click counting
//        currentSelection--;
//        if(currentSelection == 1){
//            currentSelection = 1;
//        }
//        CCW++;                                  //Increase the CCW click counter
//    }
    if(scrolling){
        if(updateHour == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(meridiemScroll == 2){
                    if(hourScroll == 24){
                        hourScroll = 24;
                    }else{
                        hourScroll++;
                    }
                }else{
                    if(hourScroll >= 12){
                        hourScroll = 12;
                    }else{
                        hourScroll++;
                    }
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(hourScroll == 0){
                    hourScroll = 0;
                }else{
                    hourScroll--;
                }
            }
            sprintf(setTimeScreen.option6.text, "%02d", hourScroll);
        }else if(updateMinutes == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(minuteScroll == 59){
                    minuteScroll = 59;
                }else{
                    minuteScroll++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(minuteScroll == 0){
                    minuteScroll = 0;
                }else{
                    minuteScroll--;
                }
            }
            sprintf(setTimeScreen.option8.text, "%02d", minuteScroll);
        }else if(updateMeridiem == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(meridiemScroll == 2){
                    meridiemScroll = 2;
                }else{
                    meridiemScroll++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(meridiemScroll == 0){
                    meridiemScroll = 0;
                }else{
                    meridiemScroll--;
                }
            }

            if(meridiemScroll == 0){
                strcpy(setTimeScreen.option9.text, "AM");   //AM/PM/24h
                if(hourScroll >= 12){
                    hourScroll = 12;
                    sprintf(setTimeScreen.option6.text, "%02d", hourScroll);
                }
            }else if(meridiemScroll == 1){
                strcpy(setTimeScreen.option9.text, "PM");   //AM/PM/24h
                if(hourScroll >= 12){
                    hourScroll = 12;
                    sprintf(setTimeScreen.option6.text, "%02d", hourScroll);
                }
            }else if(meridiemScroll == 2){
                strcpy(setTimeScreen.option9.text, "24");   //AM/PM/24h
            }
        }else if(updateDay == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(dayScroll == 7){
                    dayScroll = 7;
                }else{
                    dayScroll++;
                }
                strcpy(setDateScreen.option7.text, "         ");
                ST7735_DrawString(setDateScreen.option7.x,  setDateScreen.option7.y,  setDateScreen.option7.size,  setDateScreen.option7.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(dayScroll == 1){
                    dayScroll = 1;
                }else{
                    dayScroll--;
                }
                strcpy(setDateScreen.option7.text, "         ");
                ST7735_DrawString(setDateScreen.option7.x,  setDateScreen.option7.y,  setDateScreen.option7.size,  setDateScreen.option7.text, textColor, bgColor, 0);
            }
            switch(dayScroll){
                case 1:
                    strcpy(scrollDayString, "Monday");
                    break;
                case 2:
                    strcpy(scrollDayString, "Tuesday");
                    break;
                case 3:
                    strcpy(scrollDayString, "Wednesday");
                    break;
                case 4:
                    strcpy(scrollDayString, "Thursday");
                    break;
                case 5:
                    strcpy(scrollDayString, "Friday");
                    break;
                case 6:
                    strcpy(scrollDayString, "Saturday");
                    break;
                case 7:
                    strcpy(scrollDayString, "Sunday");
                    break;
            }
            strcpy(setDateScreen.option7.text, scrollDayString);
        }else if(updateDate == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting

                switch(monthScroll){
                    case 1:     //January
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 2:     //February
                        if(dateScroll == 28){
                            dateScroll = 28;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 3:     //March
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 4:     //April
                        if(dateScroll == 30){
                            dateScroll = 30;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 5:     //May
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 6:     //June
                        if(dateScroll == 30){
                            dateScroll = 30;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 7:     //July
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 8:     //August
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 9:     //September
                        if(dateScroll == 30){
                            dateScroll = 30;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 10:    //October
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 11:    //November
                        if(dateScroll == 30){
                            dateScroll = 30;
                        }else{
                            dateScroll++;
                        }
                        break;
                    case 12:    //December
                        if(dateScroll == 31){
                            dateScroll = 31;
                        }else{
                            dateScroll++;
                        }
                        break;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(dateScroll == 1){
                    dateScroll = 1;
                }else{
                    dateScroll--;
                }
            }
            sprintf(setDateScreen.option9.text, "%d",dateScroll);
        }else if(updateMonth == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(monthScroll == 12){
                    monthScroll = 12;
                }else{
                    monthScroll++;
                }
                strcpy(setDateScreen.option11.text, "         ");
                ST7735_DrawString(setDateScreen.option11.x,  setDateScreen.option11.y,  setDateScreen.option11.size,  setDateScreen.option11.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(monthScroll == 1){
                    monthScroll = 1;
                }else{
                    monthScroll--;
                }
                strcpy(setDateScreen.option11.text, "         ");
                ST7735_DrawString(setDateScreen.option11.x,  setDateScreen.option11.y,  setDateScreen.option11.size,  setDateScreen.option11.text, textColor, bgColor, 0);
            }
            switch(monthScroll){
                case 1:
                    strcpy(scrollMonthString, "Jan.");
                    break;
                case 2:
                    strcpy(scrollMonthString, "Feb.");
                    break;
                case 3:
                    strcpy(scrollMonthString, "Mar.");
                    break;
                case 4:
                    strcpy(scrollMonthString, "Apr.");
                    break;
                case 5:
                    strcpy(scrollMonthString, "May");
                    break;
                case 6:
                    strcpy(scrollMonthString, "Jun.");
                    break;
                case 7:
                    strcpy(scrollMonthString, "Jul.");
                    break;
                case 8:
                    strcpy(scrollMonthString, "Aug.");
                    break;
                case 9:
                    strcpy(scrollMonthString, "Sep.");
                    break;
                case 10:
                    strcpy(scrollMonthString, "Oct.");
                    break;
                case 11:
                    strcpy(scrollMonthString, "Nov.");
                    break;
                case 12:
                    strcpy(scrollMonthString, "Dec.");
                    break;
                default:
                    monthScroll = 1;
                    break;
            }
            strcpy(setDateScreen.option11.text, scrollMonthString);
        }else if(updateYear == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(yearScroll >= 2999){
                    yearScroll = 2999;
                }else{
                    yearScroll++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(yearScroll == 0){
                    yearScroll = 0;
                }else{
                    yearScroll--;
                }
            }
            sprintf(setDateScreen.option13.text, "%d", yearScroll);
        }else if(updateA1Date == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Date == 31){
                    scrollA1Date = 31;
                }else{
                    scrollA1Date++;
                }
                strcpy(setAlarmsScreen.option7.text, "    ");
                ST7735_DrawString(setAlarmsScreen.option7.x,  setAlarmsScreen.option7.y,  setAlarmsScreen.option7.size,  setAlarmsScreen.option7.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Date == 1){
                    scrollA1Date = 1;
                }else{
                    scrollA1Date--;
                }
                strcpy(setAlarmsScreen.option7.text, "    ");
                ST7735_DrawString(setAlarmsScreen.option7.x,  setAlarmsScreen.option7.y,  setAlarmsScreen.option7.size,  setAlarmsScreen.option7.text, textColor, bgColor, 0);
            }
            sprintf(setAlarmsScreen.option7.text, "%d", scrollA1Date);
        }else if(updateA1Hour == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Meridiem == 2){
                    if(scrollA1Hour == 24){
                        scrollA1Hour = 24;
                    }else{
                        scrollA1Hour++;
                    }
                }else{
                    if(scrollA1Hour >= 12){
                        scrollA1Hour = 12;
                    }else{
                        scrollA1Hour++;
                    }
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Hour == 1){
                    scrollA1Hour = 1;
                }else{
                    scrollA1Hour--;
                }
            }
            sprintf(setAlarmsScreen.option8.text, "%02d", scrollA1Hour);
        }else if(updateA1Minute == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Minute == 59){
                    scrollA1Minute = 59;
                }else{
                    scrollA1Minute++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Minute == 1){
                    scrollA1Minute = 1;
                }else{
                    scrollA1Minute--;
                }
            }
            sprintf(setAlarmsScreen.option10.text, "%02d", scrollA1Minute);
        }else if(updateA1Second == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Second == 59){
                    scrollA1Second = 59;
                }else{
                    scrollA1Second++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Second == 1){
                    scrollA1Second = 1;
                }else{
                    scrollA1Second--;
                }
            }
            sprintf(setAlarmsScreen.option12.text, "%02d", scrollA1Second);
        }else if(updateA1Meridiem == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Meridiem == 2){
                    scrollA1Meridiem = 2;
                }else{
                    scrollA1Meridiem++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA1Meridiem == 0){
                    scrollA1Meridiem = 0;
                }else{
                    scrollA1Meridiem--;
                }
            }

            if(scrollA1Meridiem == 0){
                strcpy(setAlarmsScreen.option13.text, "AM");   //AM/PM/24h
                if(scrollA1Hour >= 12){
                    scrollA1Hour = 12;
                    sprintf(setAlarmsScreen.option8.text, "%02d", scrollA1Hour);
                    ST7735_DrawString(setAlarmsScreen.option8.x,  setAlarmsScreen.option8.y,  setAlarmsScreen.option8.size,  setAlarmsScreen.option8.text, textColor, bgColor, 0);
                }
            }else if(scrollA1Meridiem == 1){
                strcpy(setAlarmsScreen.option13.text, "PM");   //AM/PM/24h
                if(scrollA1Hour >= 12){
                    scrollA1Hour = 12;
                    sprintf(setAlarmsScreen.option8.text, "%02d", scrollA1Hour);
                    ST7735_DrawString(setAlarmsScreen.option8.x,  setAlarmsScreen.option8.y,  setAlarmsScreen.option8.size,  setAlarmsScreen.option8.text, textColor, bgColor, 0);
                }
            }else if(scrollA1Meridiem == 2){
                strcpy(setAlarmsScreen.option13.text, "24");   //AM/PM/24h
            }
        }else if(updateA2Date == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Date == 31){
                    scrollA2Date = 31;
                }else{
                    scrollA2Date++;
                }
                sprintf(setAlarmsScreen.option17.text, "    ", scrollA2Date);
                ST7735_DrawString(setAlarmsScreen.option17.x,  setAlarmsScreen.option17.y,  setAlarmsScreen.option17.size,  setAlarmsScreen.option17.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Date == 1){
                    scrollA2Date = 1;
                }else{
                    scrollA2Date--;
                }
                sprintf(setAlarmsScreen.option17.text, "    ", scrollA2Date);
                ST7735_DrawString(setAlarmsScreen.option17.x,  setAlarmsScreen.option17.y,  setAlarmsScreen.option17.size,  setAlarmsScreen.option17.text, textColor, bgColor, 0);
            }
            sprintf(setAlarmsScreen.option17.text, "%d", scrollA2Date);
        }else if(updateA2Hour == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Meridiem == 2){
                    if(scrollA2Hour == 24){
                        scrollA2Hour = 24;
                    }else{
                        scrollA2Hour++;
                    }
                }else{
                    if(scrollA2Hour >= 12){
                        scrollA2Hour = 12;
                    }else{
                        scrollA2Hour++;
                    }
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Hour == 1){
                    scrollA2Hour = 1;
                }else{
                    scrollA2Hour--;
                }
            }
            sprintf(setAlarmsScreen.option18.text, "%02d", scrollA2Hour);
        }else if(updateA2Minute == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Minute == 59){
                    scrollA2Minute = 59;
                }else{
                    scrollA2Minute++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Minute == 1){
                    scrollA2Minute = 1;
                }else{
                    scrollA2Minute--;
                }
            }
            sprintf(setAlarmsScreen.option20.text, "%02d", scrollA2Minute);
        }else if(updateA2Meridiem == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Meridiem == 2){
                    scrollA2Meridiem = 2;
                }else{
                    scrollA2Meridiem++;
                }
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollA2Meridiem == 0){
                    scrollA2Meridiem = 0;
                }else{
                    scrollA2Meridiem--;
                }
            }

            if(scrollA2Meridiem == 0){
                strcpy(setAlarmsScreen.option21.text, "AM");   //AM/PM/24h
                if(scrollA2Hour >= 12){
                    scrollA2Hour = 12;
                    sprintf(setAlarmsScreen.option18.text, "%02d", scrollA2Hour);
                    ST7735_DrawString(setAlarmsScreen.option18.x,  setAlarmsScreen.option18.y,  setAlarmsScreen.option18.size,  setAlarmsScreen.option18.text, textColor, bgColor, 0);
                }
            }else if(scrollA2Meridiem == 1){
                strcpy(setAlarmsScreen.option21.text, "PM");   //AM/PM/24h
                if(scrollA2Hour >= 12){
                    scrollA2Hour = 12;
                    sprintf(setAlarmsScreen.option18.text, "%02d", scrollA2Hour);
                    ST7735_DrawString(setAlarmsScreen.option18.x,  setAlarmsScreen.option18.y,  setAlarmsScreen.option18.size,  setAlarmsScreen.option18.text, textColor, bgColor, 0);
                }
            }else if(scrollA2Meridiem == 2){
                strcpy(setAlarmsScreen.option21.text, "24");   //AM/PM/24h
            }
        }else if(updateTempUnits == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(currentTempSelection == Romer){
                    currentTempSelection = Romer;
                }else{
                    currentTempSelection++;
                }
                strcpy(setPreferencesScreen.option8.text, "      ");
                ST7735_DrawString(setPreferencesScreen.option8.x,  setPreferencesScreen.option8.y,  setPreferencesScreen.option8.size,  setPreferencesScreen.option8.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(currentTempSelection == Celsius){
                    currentTempSelection = Celsius;
                }else{
                    currentTempSelection--;
                }
                strcpy(setPreferencesScreen.option8.text, "      ");
                ST7735_DrawString(setPreferencesScreen.option8.x,  setPreferencesScreen.option8.y,  setPreferencesScreen.option8.size,  setPreferencesScreen.option8.text, textColor, bgColor, 0);
            }
            switch(currentTempSelection){
                case Celsius:
//                    strcpy(tempUnit, "C");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "C");
                    break;
                case Fahrenheit:
//                    strcpy(tempUnit, "F");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "F");
                    break;
                case Kelvin:
//                    strcpy(tempUnit, "K");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "K");
                    break;
                case Rankine:
//                    strcpy(tempUnit, "R");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "R");
                    break;
                case Delisle:
//                    strcpy(tempUnit, "De");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "De");
                    break;
                case Newton:
//                    strcpy(tempUnit, "N");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "N");
                    break;
                case Reaumur:
//                    strcpy(tempUnit, "Re");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "Re");
                    break;
                case Romer:
//                    strcpy(tempUnit, "Ro");
                    sprintf(setPreferencesScreen.option8.text, "%c%s",DEGREE_SYM, "Ro");
                    break;
                default:
                    currentTempSelection = Celsius;
                    break;
            }
        }else if(updateSpeedUnits == 1){
//            KmPerHour,
//            MilesPerHour,
//            MetersPerSecond,
//            Knot,
//            FeetPerSec
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(currentSpeedUnitSelection == FeetPerSec){
                    currentSpeedUnitSelection = FeetPerSec;
                }else{
                    currentSpeedUnitSelection++;
                }
                strcpy(setPreferencesScreen.option10.text, "       ");
                ST7735_DrawString(setPreferencesScreen.option10.x,  setPreferencesScreen.option10.y,  setPreferencesScreen.option10.size,  setPreferencesScreen.option10.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(currentSpeedUnitSelection == KmPerHour){
                    currentSpeedUnitSelection = KmPerHour;
                }else{
                    currentSpeedUnitSelection--;
                }
                strcpy(setPreferencesScreen.option10.text, "       ");
                ST7735_DrawString(setPreferencesScreen.option10.x,  setPreferencesScreen.option10.y,  setPreferencesScreen.option10.size,  setPreferencesScreen.option10.text, textColor, bgColor, 0);
            }
            switch(currentSpeedUnitSelection){
                case KmPerHour:
                    strcpy(setPreferencesScreen.option10.text, "KM/H");
                    break;
                case MilesPerHour:
                    strcpy(setPreferencesScreen.option10.text, "MPH");
                    break;
                case MetersPerSecond:
                    strcpy(setPreferencesScreen.option10.text, "M/S");
                    break;
                case Knot:
                    strcpy(setPreferencesScreen.option10.text, "KNOT");
                    break;
                case FeetPerSec:
                    strcpy(setPreferencesScreen.option10.text, "FT/S");
                    break;
                default:
                    currentSpeedUnitSelection = KmPerHour;
                    break;
            }
        }else if(updateAppearance == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollAppearance == Custom){
                    scrollAppearance = Custom;
                }else{
                    scrollAppearance++;
                }
                strcpy(setPreferencesScreen.option13.text, "        ");
                ST7735_DrawString(setPreferencesScreen.option13.x,  setPreferencesScreen.option13.y,  setPreferencesScreen.option13.size,  setPreferencesScreen.option13.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollAppearance == Light){
                    scrollAppearance = Light;
                }else{
                    scrollAppearance--;
                }
                strcpy(setPreferencesScreen.option13.text, "        ");
                ST7735_DrawString(setPreferencesScreen.option13.x,  setPreferencesScreen.option13.y,  setPreferencesScreen.option13.size,  setPreferencesScreen.option13.text, textColor, bgColor, 0);
            }
//            Light,
//            Dark,
//            Custom
            switch(scrollAppearance){
                case Light:
                    strcpy(setPreferencesScreen.option13.text, "Light");
                    break;
                case Dark:
                    strcpy(setPreferencesScreen.option13.text, "Dark");
                    break;
                case Custom:
                    strcpy(setPreferencesScreen.option13.text, "Custom");
                    break;
                default:
                    scrollAppearance = Light;
                    break;
            }
        }else if(updateBrightness == 1){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollBrightness == 105){
                    scrollBrightness = 105;
                }else{
                    scrollBrightness+=5;
                }
                strcpy(setPreferencesScreen.option15.text, "        ");
                ST7735_DrawString(setPreferencesScreen.option15.x,  setPreferencesScreen.option15.y,  setPreferencesScreen.option15.size,  setPreferencesScreen.option15.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollBrightness == 5){
                    scrollBrightness = 5;
                }else{
                    scrollBrightness-=5;
                }
                strcpy(setPreferencesScreen.option15.text, "        ");
                ST7735_DrawString(setPreferencesScreen.option15.x,  setPreferencesScreen.option15.y,  setPreferencesScreen.option15.size,  setPreferencesScreen.option15.text, textColor, bgColor, 0);
            }
            if(scrollBrightness <= 100){
                sprintf(setPreferencesScreen.option15.text, "%d", scrollBrightness);
//                ST7735_DrawString(setPreferencesScreen.option15.x,  setPreferencesScreen.option15.y,  setPreferencesScreen.option15.size,  setPreferencesScreen.option15.text, textColor, bgColor, 0);
            }else{
                strcpy(setPreferencesScreen.option15.text, "Auto");
//                ST7735_DrawString(setPreferencesScreen.option15.x,  setPreferencesScreen.option15.y,  setPreferencesScreen.option15.size,  setPreferencesScreen.option15.text, textColor, bgColor, 0);
            }
        }else if(updateTextColor){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollTextColor == White){
                    scrollTextColor = White;
                }else{
                    scrollTextColor++;
                }
                strcpy(setPreferencesScreen.option17.text, "            ");
                ST7735_DrawString(setPreferencesScreen.option17.x,  setPreferencesScreen.option17.y,  setPreferencesScreen.option17.size,  setPreferencesScreen.option17.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollTextColor == Black){
                    scrollTextColor = Black;
                }else{
                    scrollTextColor--;
                }
                strcpy(setPreferencesScreen.option17.text, "            ");
                ST7735_DrawString(setPreferencesScreen.option17.x,  setPreferencesScreen.option17.y,  setPreferencesScreen.option17.size,  setPreferencesScreen.option17.text, textColor, bgColor, 0);
            }
            switch(scrollTextColor){
                case Black:
                    strcpy(setPreferencesScreen.option17.text, "Black");
                    break;
                case Blue:
                    strcpy(setPreferencesScreen.option17.text, "Blue");
                    break;
                case Red:
                    strcpy(setPreferencesScreen.option17.text, "Red");
                    break;
                case Green:
                    strcpy(setPreferencesScreen.option17.text, "Green");
                    break;
                case Cyan:
                    strcpy(setPreferencesScreen.option17.text, "Cyan");
                    break;
                case Magenta:
                    strcpy(setPreferencesScreen.option17.text, "Magenta"); //Maybe abbreviate
                    break;
                case Yellow:
                    strcpy(setPreferencesScreen.option17.text, "Yellow"); //Maybe abbreviate
                    break;
                case White:
                    strcpy(setPreferencesScreen.option17.text, "White");
                    break;
                default:
                    scrollTextColor = White;
                    break;
            }
        }else if(updateBGColor){
            if(CWflag){
                CWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollBGColor == White){
                    scrollBGColor = White;
                }else{
                    scrollBGColor++;
                }
                strcpy(setPreferencesScreen.option19.text, "          ");
                ST7735_DrawString(setPreferencesScreen.option19.x,  setPreferencesScreen.option19.y,  setPreferencesScreen.option19.size,  setPreferencesScreen.option19.text, textColor, bgColor, 0);
            }else if(CCWflag){
                CCWflag = 0;                             //Clear flag to avoid faulty click counting
                if(scrollBGColor == Black){
                    scrollBGColor = Black;
                }else{
                    scrollBGColor--;
                }
                strcpy(setPreferencesScreen.option19.text, "          ");
                ST7735_DrawString(setPreferencesScreen.option19.x,  setPreferencesScreen.option19.y,  setPreferencesScreen.option19.size,  setPreferencesScreen.option19.text, textColor, bgColor, 0);
            }
            switch(scrollBGColor){
                case Black:
                    strcpy(setPreferencesScreen.option19.text, "Black");
                    break;
                case Blue:
                    strcpy(setPreferencesScreen.option19.text, "Blue");
                    break;
                case Red:
                    strcpy(setPreferencesScreen.option19.text, "Red");
                    break;
                case Green:
                    strcpy(setPreferencesScreen.option19.text, "Green");
                    break;
                case Cyan:
                    strcpy(setPreferencesScreen.option19.text, "Cyan");
                    break;
                case Magenta:
                    strcpy(setPreferencesScreen.option19.text, "Magenta"); //Maybe abbreviate
                    break;
                case Yellow:
                    strcpy(setPreferencesScreen.option19.text, "Yellow"); //Maybe abbreviate
                    break;
                case White:
                    strcpy(setPreferencesScreen.option19.text, "White");
                    break;
                default:
                    scrollBGColor = White;
                    break;
            }
        }
//        updateA1Date, updateA1Hour, updateA1Minute, updateA1Second, updateA1Meridiem;
//        scrollA1Date, scrollA1Hour, scrollA1Minute, scrollA1Second, scrollA1Meridiem;
//        scrollA2Date, scrollA2Hour, scrollA2Minute, scrollA2Second, scrollA2Meridiem;
//        updateA2Date, updateA2Hour, updateA2Minute, updateA2Second, updateA2Meridiem;
    }else{
        switch(menuState){
            case mainScreenState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=8;
                            if(currentSelection >= passedMenu.itemsCount){
                                currentSelection = passedMenu.itemsCount;
                            }
                            CW++;                                   //Increase the CW click counter
                        }
                        break;
                    case 9:
                        if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=8;
                            if(currentSelection == 1){
                                currentSelection = 1;
                            }
                            CCW++;                                  //Increase the CCW click counter
                        }
                        break;
                }
                break;
            case mainMenuState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=5;                                //Increase the CW click counter
                        }
                        break;
                    case 6:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=5;
                        }
                        break;
                    case 7:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 8:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 9:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 10:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 13:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection = passedMenu.itemsCount;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                }
                break;
            case setTimeState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=5;                                //Increase the CW click counter
                        }
                        break;
                    case 6:     //HH
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=5;
                        }
                        break;
                    case 8:     //MM
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 9:     //AM/PM
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 10:    //Set
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 13:    //Back
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection = passedMenu.itemsCount;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case setDateState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=6;                                //Increase the CW click counter
                        }
                        break;
                    case 7:     //Day Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=6;
                        }
                        break;
                    case 9:     //Date Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 11:     //Month Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 13:    //Year Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 14:    //Set
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 17:    //Back
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection = passedMenu.itemsCount;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case setAlarmState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=6;                                //Increase the CW click counter
                        }
                        break;
                    case 7:     //Date Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=6;
                        }
                        break;
                    case 8:     //Hour Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 10:     //Minute Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 12:    //Second Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 13:    //Meridiam Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 14:    //Set
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 15:    //Cancel
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 17:     //Date Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 18:     //Hour Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 20:    //Minute Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 21:    //Meridiam Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 22:    //Set
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 23:    //Cancel
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 26:    //Back
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection = passedMenu.itemsCount;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case setPrefState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=7;                                //Increase the CW click counter
                        }
                        break;
                    case 8:     //TEMP Scroll
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=7;
                        }
                        break;
                    case 10:     //SPEED Scroll KM/H or MPH
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 13:     //Appearance Light/Dark
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                    case 15:    //Brightness
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 17:    //Text Color
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=2;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 19:    //Background Color
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=2;
                        }
                        break;
                    case 20:    //Set
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 23:    //Back
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection = passedMenu.itemsCount;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case testingState:
                switch(currentSelection){
                    case 1:
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=5;                                //Increase the CW click counter
                        }
                        break;
                    case 6:     //Emergency Logs
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=5;
                        }
                        break;
                    case 7:     //Motors Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 8:     //LEDS Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 9:    //WDT Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 10:    //7-Seg Array Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 11:    //Ultrasonic Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 12:    //Speaker Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection++;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 13:    //Encoder Test
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection+=3;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection--;
                        }
                        break;
                    case 16:    //Back
                        if(CWflag){                                 //Check if a CW rotation was detected
                            CWflag = 0;                             //Clear flag to avoid faulty click counting
                            currentSelection = passedMenu.itemsCount;
                        }else if(CCWflag){                                //Check if a CCW rotation was detected
                            CCWflag = 0;                            //Clear flag to avoid faulty click counting
                            currentSelection-=3;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void encoderInitialization(void){
    ENCODER_PORT->SEL0 &=~ ENCODER;     //Set up encoder pins as GPIO
    ENCODER_PORT->SEL1 &=~ ENCODER;
    ENCODER_PORT->DIR &=~ ENCODER;      //Set up encoder pins as inputs
//    ENCODER_PORT->REN |= ENCODER;            //Enable internal resistor
//    ENCODER_PORT->OUT |= ENCODER;            //Set internal resistor to pull-up
//    ENCODER_PORT->IES |= SW|CLK;            //Interrupt edge select high-to-low edge (falling edge)
//    ENCODER_PORT->IE |= SW|CLK;             //Enable port interrupt
//    ENCODER_PORT->IFG = 0;              //Clear interrupt flag (just in case)
    ENCODER_PORT->REN |= SW;            //Enable internal resistor
    ENCODER_PORT->OUT |= SW;            //Set internal resistor to pull-up
    ENCODER_PORT->IES |= SW;            //Interrupt edge select high-to-low edge (falling edge)
    ENCODER_PORT->IE |= SW;             //Enable port interrupt
    ENCODER_PORT->IFG = 0;              //Clear interrupt flag (just in case)

    ENCODER_PORT->SEL0 |= DT;         //Enable Secondary function in Encoder CLK pin
    ENCODER_PORT->SEL1 &=~ DT;
    ENCODER_PORT->DIR &=~ CLK|DT;      //Set up encoder pins as inputs
    ENCODER_PORT->REN |= CLK|DT;       //Enable internal resistor
    ENCODER_PORT->OUT |= CLK|DT;       //Set internal resistor to pull-up

    TIMER_A0->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__CONTINUOUS|      //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /8
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P2.6 Timer A
    TIMER_A0->CCTL[2] = (TIMER_A_CCTLN_CM__BOTH|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source

//    TIMER_A0->EX0 = TIMER_A_EX0_IDEX__8;
}

void watchdog_init(void){
    SYSCTL->WDTRESET_CTL = 0;               //Setup WDT with a soft reset (0) instead of a hard reset (1)

    CS->KEY = CS_KEY_VAL;                   //Enter CS password
    CS->CTL1 |= (CS_CTL1_DIVA__4|           //Set ACLK source with a 1/4 clkc divider (REFOCLK / 4)
                 CS_CTL1_SELA__REFOCLK);    //Sel REFOCLK as ACLK source
    CS->KEY = 1;                            //Close CS
}

void initialization(void){

    TURNSIG_PORT->SEL0 &=~ TURNSIG; //Set up Turn signal as GPIO
    TURNSIG_PORT->SEL1 &=~ TURNSIG;
    TURNSIG_PORT->DIR |= TURNSIG;   //Set up output

    PROXBLINK_PORT->SEL0 &=~ PROXBLINK; //Set up proximity sensor Alarm LED as GPIO
    PROXBLINK_PORT->SEL1 &=~ PROXBLINK;
    PROXBLINK_PORT->DIR |= PROXBLINK;   //Set up as output
    PROXBLINK_PORT->OUT &=~ PROXBLINK;  //Initialize as off

    highlightedColor = ~textColor;  //Define the default highlight color as the inverse of the text color for maximum contrast
    bgColor = ~textColor;           //Define the background color of the system as the inverse of the test for maximumm contrast
    watchdog_init();                //Initialize the system watchdog.
    setupI2C();                     //Set up project I2C line which includes RTC module and EEPROM
    initializeBipolarMotor();       //Initialize Project Bipolar stepper motor
    initializeUnipolarMotor();      //Initialize Project Unipolar stepper motor
    gpioUltrasonicSetup();          //Initialize HC-SR04 Sensor pings used for the project
    ST7735_InitR(INITR_REDTAB);     //Initialize the ST7735 as Red Tab for better pixel fit.
    sysTickSetup();                 //Initialize systick timer for use as delay
    segmentInit();                  //Initialize 7-Segment array to use as Odometer
    timer32Setup1();                //Initialize 1st Timer32 for interrupt-driven stepper motor control
    timer32Setup2();                //Initialize 2nd Timer32 for interrupt-driven stepper motor control
    encoderInitialization();        //Initialize rotary encoder used for user input
    mainScreenSetup();              //Set up ST7735 LCD menus used for UI
    mainMenuSetup();                //Set up ST7735 LCD menus used for UI
    setTimeSetup();                 //Set up ST7735 LCD menus used for UI
    setDateSetup();                 //Set up ST7735 LCD menus used for UI
    setAlarmSetup();                //Set up ST7735 LCD menus used for UI
    setPreferencesSetup();          //Set up ST7735 LCD menus used for UI
    testingSetup();                 //Set up ST7735 LCD menus used for UI
    setupBrightnessControl();       //Set up PWM signal used for brightness control
    setupADC();                     //Set up ADC for analog sensor control
    setupHallEffect();              //Set up Hall effect sensor for speed sensing
    setupUltrasonicSensor();        //Set up HC-SR04 Sensor pings used for the project
}

void PORT2_IRQHandler(void){
    int i;
    if(ENCODER_PORT->IFG & SW){
//        msDelay(5);
        if(ENCODER_PORT->IFG & SW){
//        if((ENCODER_PORT->IN & SW) == 0){
            for(i = 0; i < 75; i++){        //Cycle through for-loop to check state of button bounce
                if(debounceEncoder()){         //Check that button1 has been pressed
                    btn1Flag = 1;           //Set button1 flag to indicate that button1 was pressed
                    break;
                }
            }
            ENCODER_PORT->IFG &=~ SW;       //Clear interrupt flag for button pin
        }
    }
    ENCODER_PORT->IFG = 0;              //Clear the entire port interrupt flag
}

void TA0_N_IRQHandler(void){
    if((TIMER_A0->CCTL[2] & TIMER_A_CCTLN_CCIFG) != 0){
        if((ENCODER_PORT->IN & DT) == 0){
            msDelay(20);
            if((ENCODER_PORT->IN & CLK) == 0x00000040){
                encoderCount-=30;
                CWflag = 0;
                CCWflag = 1;
            }else if((ENCODER_PORT->IN & CLK) == 0){
                encoderCount+=30;
                CWflag = 1;
                CCWflag = 0;
            }
        }else if((ENCODER_PORT->IN & DT) == 0x00000020){
            msDelay(20);
            if((ENCODER_PORT->IN & CLK) == 0x00000040){
                encoderCount+=30;
                CCWflag = 0;
                CWflag = 1;
            }else if((ENCODER_PORT->IN & CLK) == 0){
                encoderCount-=30;
                CCWflag = 1;
                CWflag = 0;
            }
        }
        if(encoderCount >= 800){
            encoderCount = 800;
        }else if(encoderCount <=0){
            encoderCount = 0;
        }
    }

    TIMER_A0->CCTL[2] &=~ TIMER_A_CCTLN_CCIFG;          //Clear TimerA0 interrupts
}

void TA3_N_IRQHandler(void){
    uint8_t j;
    rots = 0;
    hallSum = 0;
    rotCount++;
    if((TIMER_A3->CCTL[2] & TIMER_A_CCTLN_COV) == 0){
        hallCurrentEdge = TIMER_A3->CCR[2];                 //Grab timer at which interrupt was triggered from buffer
//        if(hallLastEdge > hallCurrentEdge){                     //Check for overflow
//    //        currentEdge = currentEdge + 65534;     //If overflow happens, add a full period to current edge to 'extend' the values
//            hallDeltaCycles = hallCurrentEdge - hallLastEdge;       //Determine the time difference since last occurrence
//        }else{
            hallDeltaCycles = hallCurrentEdge - hallLastEdge;       //Determine the time difference since last occurrence
//        }
        hallLastEdge = hallCurrentEdge;                         //Set the new lastEdge

        //Calculate the distance from deltaTime using speed of sound at sea level.
        //T = (1/3MHz) = 0.333 (us/clock cycle)
//        4294915295
        if(hallDeltaCycles < 50000){
            micro = hallDeltaCycles * 0.3333;  //sigTime units is microseconds (us)
            if((rotCount % 2) == 0){
                countForOdometer++;
                rots = (1/ micro) * (1000) * (1000);
            }
            hallRollingValues[y%8] = rots;
            y++;
            for(j = 0; j < 8; j++){
                hallSum += hallRollingValues[j];
            }
            rpmRollingAverage = hallSum / 8;
            engaged = 1;
        }
    }else{
        hallCurrentEdge = TIMER_A3->CCR[2] + 65535;                 //Grab timer at which interrupt was triggered from buffer
//        if(hallLastEdge > hallCurrentEdge){                     //Check for overflow
//    //        currentEdge = currentEdge + 65534;     //If overflow happens, add a full period to current edge to 'extend' the values
//            hallDeltaCycles = hallCurrentEdge - hallLastEdge;       //Determine the time difference since last occurrence
//        }else{
            hallDeltaCycles = hallCurrentEdge - hallLastEdge;       //Determine the time difference since last occurrence
//        }
        hallLastEdge = hallCurrentEdge;                         //Set the new lastEdge

        //Calculate the distance from deltaTime using speed of sound at sea level.
        //T = (1/3MHz) = 0.333 (us/clock cycle)
//        4294915295
        if(hallDeltaCycles < 50000){
            micro = hallDeltaCycles * 0.3333;  //sigTime units is microseconds (us)
            if((rotCount % 2) == 0){
                countForOdometer++;
                rots = (1/ micro) * (1000) * (1000);
            }
            hallRollingValues[y%8] = rots;
            y++;
            for(j = 0; j < 8; j++){
                hallSum += hallRollingValues[j];
            }
            rpmRollingAverage = hallSum / 8;
            engaged = 1;
        }
    }

    double sigTime;
    sum = 0;

    if((TIMER_A3->CCTL[1] & TIMER_A_CCTLN_COV) == 0){
        currentEdge = TIMER_A3->CCR[1];                 //Grab timer at which interrupt was triggered from buffer
        deltaCycles = currentEdge - lastEdge;           //Determine the time difference since last occurrence
        lastEdge = currentEdge;                         //Set the new lastEdge

        //Calculate the distance from deltaTime using speed of sound at sea level.
        //T = (1/3MHz) = 0.333 (us/clock cycle)
        if(deltaCycles < 50000){
            sigTime = deltaCycles * 0.3333;  //sigTime units is microseconds (us)
            distCentimeter = sigTime / 58.0;  //microseconds / 58 = centimeters
            distMilli = distCentimeter * 100.0; //centimeter * 100 = millimeters
            rollingValues[x%8] = distCentimeter;
            x++;
            for(j = 0; j < 8; j++){
                sum += rollingValues[j];
            }
            rollingAverage = sum / 8;
        }
    }else{
        currentEdge = TIMER_A3->CCR[1] + 65535;                 //Grab timer at which interrupt was triggered from buffer
        deltaCycles = currentEdge - lastEdge;           //Determine the time difference since last occurrence
        lastEdge = currentEdge;                         //Set the new lastEdge

        //Calculate the distance from deltaTime using speed of sound at sea level.
        //T = (1/3MHz) = 0.333 (us/clock cycle)
        if(deltaCycles < 50000){
            sigTime = deltaCycles * 0.3333;  //sigTime units is microseconds (us)
            distCentimeter = sigTime / 58.0;  //microseconds / 58 = centimeters
            distMilli = distCentimeter * 100.0; //centimeter * 100 = millimeters
            rollingValues[x%8] = distCentimeter;
            x++;
            for(j = 0; j < 8; j++){
                sum += rollingValues[j];
            }
            rollingAverage = sum / 8;


        }
    }

    inchAverage = rollingAverage / 2.54;

    if(inchAverage < 8){
        tooClose = 1;
    }else{
        tooClose = 0;
    }

    TIMER_A3->CCTL[1] &=~ TIMER_A_CCTLN_COV;
    TIMER_A3->CCTL[1] &=~ TIMER_A_CCTLN_CCIFG;  //Clear interrupt flag
    TIMER_A3->CCTL[2] &=~ TIMER_A_CCTLN_COV;
    TIMER_A3->CCTL[2] &=~ TIMER_A_CCTLN_CCIFG;      //Clear TimerA0 interrupts
}

uint8_t debounceEncoder(void){         //Debounce function for button1
    static uint16_t State1 = 0;     //Current debounce status
    State1 = (State1 << 1) | (ENCODER_PORT->IN & SW)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State1==0x8000)return 1;     //indicates 0 level is
    return 0;
}

void resetVariables(void){
    selectedOption = 0;
    currentSelection = 1;
    refresh = 1;
}
