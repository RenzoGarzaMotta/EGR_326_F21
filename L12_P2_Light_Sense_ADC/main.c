#include "msp.h"
#include "systick.h"

#define LED_PORT        P5              //Port used for the LEDs
#define WARN_LED        BIT6

#define PHOTORES_PORT   P5
#define PHOTORES        BIT5

void setupADC(void);
void readAmbientLighting(void);
void setupBrightnessControl(void);
void brightnessControl(void);

int ambientLight;
uint16_t period = 50000;             //Get period variable from main

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	sysTickSetup();
	setupBrightnessControl();
	setupADC();

	while(1){
	    readAmbientLighting();
	    brightnessControl();

	    msDelay(10);
	}
}

void setupADC(void){
    PHOTORES_PORT->SEL1 |= PHOTORES;             /* Configure P4.7 for A6 */
    PHOTORES_PORT->SEL0 |= PHOTORES;


    ADC14->CTL0 =  ADC14_CTL0_ON;    /* power on and disabled during configuration */

//    ADC14->CTL0 |= 0x04080300;    /* S/H pulse mode, sysclk, 32 sample clocks, software trigger */
    ADC14->CTL0 |= (ADC14_CTL0_SHT0__32 | ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHP);

//    ADC14->CTL1 =  0x00000020;    /* 12-bit resolution */
    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;

//    ADC14->MCTL[5] = 6;           /* A6 input, single-ended, Vref=AVCC */
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_0;

//    ADC14->CTL1 |= 0x00050000;    /* convert for mem reg 5 */

    ADC14->CTL1 |= (ADC14_CTL1_CSTARTADD_MASK < ADC14_MCTLN_INCH_0);

    ADC14->CTL0 |= ADC14_CTL0_ENC;             /* enable ADC after configuration*/
}

void readAmbientLighting(void){
    ADC14->CTL0 |= ADC14_CTL0_SC;         /* start a conversion */
    while (!ADC14->IFGR0);    /* wait till conversion complete */
    ambientLight = ADC14->MEM[0];   /* read conversion result */
}


void setupBrightnessControl(void){
    //Set up 5.6 as TimerA instances to measure the time it take the signal to return to the sensor
    LED_PORT->SEL0 |= WARN_LED;
    LED_PORT->SEL1 &=~ WARN_LED;
    LED_PORT->DIR |= WARN_LED;

    TIMER_A2->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__UP|              //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P5.6 Timer A
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;        //Set output mode as Reset/Set

    TIMER_A2->CCR[0] = period;                      //Determine the period of the Timer A clock signal
    TIMER_A2->CCR[1] = period * 0.05;                  //Set the duty cycle to 1/2
}

void brightnessControl(void){
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
