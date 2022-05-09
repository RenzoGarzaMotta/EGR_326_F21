#include "msp.h"
#include "macros.h"
#include "ADC.h"
#include "timerA.h"

extern int ambientLight;
uint16_t period = 50000;            //Get period variable from main
uint8_t brightnessLoad = 105;

void setupBrightnessControl(void){
    //Set up 5.6 as TimerA instances to measure the time it take the signal to return to the sensor
    LED_PORT->SEL0 |= BRIGHTNESS_LED;
    LED_PORT->SEL1 &=~ BRIGHTNESS_LED;
    LED_PORT->DIR |= BRIGHTNESS_LED;

    TIMER_A2->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__UP|              //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P5.6 Timer A
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;        //Set output mode as Reset/Set

    TIMER_A2->CCR[0] = period;                         //Determine the period of the Timer A clock signal
    TIMER_A2->CCR[1] = period * 0.05;                  //Set the duty cycle to 1/2
}

void setupHallEffect(void){
    HALL_PORT->SEL0 |= HALL;
    HALL_PORT->SEL1 &=~ HALL;
    HALL_PORT->DIR &=~ HALL;

    TIMER_A3->EX0 = TIMER_A_EX0_IDEX__8;

    TIMER_A3->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__CONTINUOUS|      //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__8|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P8.2 Timer A
    TIMER_A3->CCTL[2] = (TIMER_A_CCTLN_CM__BOTH|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source
}

void brightnessControl(void){
    if(brightnessLoad  != 105){
        switch(brightnessLoad){
            case 5:
                TIMER_A2->CCR[1] = period * 0.05;
                break;
            case 10:
                TIMER_A2->CCR[1] = period * 0.10;
                break;
            case 15:
                TIMER_A2->CCR[1] = period * 0.15;
                break;
            case 20:
                TIMER_A2->CCR[1] = period * 0.20;
                break;
            case 25:
                TIMER_A2->CCR[1] = period * 0.25;
                break;
            case 30:
                TIMER_A2->CCR[1] = period * 0.30;
                break;
            case 35:
                TIMER_A2->CCR[1] = period * 0.35;
                break;
            case 40:
                TIMER_A2->CCR[1] = period * 0.40;
                break;
            case 45:
                TIMER_A2->CCR[1] = period * 0.45;
                break;
            case 50:
                TIMER_A2->CCR[1] = period * 0.50;
                break;
            case 55:
                TIMER_A2->CCR[1] = period * 0.55;
                break;
            case 60:
                TIMER_A2->CCR[1] = period * 0.60;
                break;
            case 65:
                TIMER_A2->CCR[1] = period * 0.65;
                break;
            case 70:
                TIMER_A2->CCR[1] = period * 0.70;
                break;
            case 75:
                TIMER_A2->CCR[1] = period * 0.75;
                break;
            case 80:
                TIMER_A2->CCR[1] = period * 0.80;
                break;
            case 85:
                TIMER_A2->CCR[1] = period * 0.85;
                break;
            case 90:
                TIMER_A2->CCR[1] = period * 0.90;
                break;
            case 95:
                TIMER_A2->CCR[1] = period * 0.95;
                break;
            case 100:
                TIMER_A2->CCR[1] = period;
                break;

        }
    }else if(brightnessLoad  == 105){
        if(ambientLight > 11000){
            //5% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.05;
        }else if((11000 >= ambientLight) && (ambientLight > 10450)){
            //10% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.10;
        }else if((10450 >= ambientLight) && (ambientLight > 9900)){
            //15% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.15;
        }else if((9900 >= ambientLight) && (ambientLight > 9350)){
            //20% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.20;
        }else if((9350 >= ambientLight) && (ambientLight > 8800)){
            //25% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.25;
        }else if((8800 >= ambientLight) && (ambientLight > 8250)){
            //30% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.30;
        }else if((8250 >= ambientLight) && (ambientLight > 7700)){
            //35% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.35;
        }else if((7700 >= ambientLight) && (ambientLight > 7150)){
            //40% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.40;
        }else if((7150 >= ambientLight) && (ambientLight > 6600)){
            //45% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.45;
        }else if((6600 >= ambientLight) && (ambientLight > 6050)){
            //50% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.50;
        }else if((6050 >= ambientLight) && (ambientLight > 5500)){
            //55% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.55;
        }else if((4950 >= ambientLight) && (ambientLight > 4400)){
            //60% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.60;
        }else if((4400 >= ambientLight) && (ambientLight > 3850)){
            //65% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.65;
        }else if((3850 >= ambientLight) && (ambientLight > 3300)){
            //70% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.70;
        }else if((3300 >= ambientLight) && (ambientLight > 2750)){
            //75% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.75;
        }else if((2750 >= ambientLight) && (ambientLight > 2200)){
            //80% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.80;
        }else if((2200 >= ambientLight) && (ambientLight > 1650)){
            //85% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.85;
        }else if((1650 >= ambientLight) && (ambientLight > 1100)){
            //90% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.90;
        }else if((1100 >= ambientLight) && (ambientLight > 550)){
            //95% Duty Cycle
            TIMER_A2->CCR[1] = period * 0.95;
        }else if(550 > ambientLight){
            //100% Duty Cycle
            TIMER_A2->CCR[1] = period;
        }
    }
}
