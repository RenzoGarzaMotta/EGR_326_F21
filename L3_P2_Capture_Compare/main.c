/*Lab 3 - Capture and Compare with the MSP432
 *Part 2 - Measuring distance using the proximity sensor
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
 *  This part will use the MSP432 Timer A in Capture mode to determine the length
 *  of a pulse generated by a proximity sensor (HC-SR04).
 *  The proximity sensor must be pre-triggered in order to generate a pulse so that
 *  distance to an object can be determined.
 *
 *  -Connect the proximity sensor to the MSP432 such that:
 *      -MSP output pin is connected via a 2N7000 MOSFET to provide a 5V signal to
 *       the HC-SR04 Trigger pin.
 *      -HC-SR04 echo pin outputs a 5V signal that has to be stepped down to ~3.3V
 *       via a voltage divider to act as an input signal to the MSP432. The design
 *       used a 68kOhm and 36kOhm resistors for the voltage divider.
 *      -Additionally, Vcc = 5V and GND = 0V.
 *  -The program is to print the distance measured by the proximity sensor every
 *   2 sec to the console using printf() -- stdio.h was included to print to console.
 */

#include "msp.h"
#include <stdio.h>
#include "macros.h"
#include "leds.h"
#include "systick.h"

uint8_t printFlag, toggleFlag;
uint16_t currentEdge, lastEdge, deltaCycles, period = 1000;
uint8_t i = 0;
double distCentimeter, distMilli, rollingValues[8] = {0}, sum = 0, rollingAverage = 0;

void gpioSetup(void);
void setTrigger(void);
void sampleDistance(void);
void setupTimerA(void);

 void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
//	intSysTickSetup();
	sysTickSetup();                                 //Set up SysTick timer
	gpioSetup();                                    //GPIO setup
	setupTimerA();                                  //Setup Timer A
	TIMER_A2->CCR[0] = period;                      //Determine the period of the Timer A clock signal
	TIMER_A2->CCR[4] = period * 0;                  //Set the duty cycle to 1/2

	NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31); //Determine interrupt vector
	__enable_irq(); //Enable all interrupts

	while(1){
        if(!toggleFlag){    //Check if toggleFlag has been set
            sysTickSetup(); //Set up SysTick Timer without an interrupt
            gpioSetup();    //Set up GPIO pins
            setupTimerA();  //Set up Timer A
            msDelay(60);    //Delay for 60ms to allow the US sensor to get ready for next pulse
        }else{
//            //Set up 5.6 as TimerA instances to measure the time it take the signal to return to the sensor
            LED_PORT->SEL0 &=~ WARN_LED;    //Set up LED pin as primary function
            LED_PORT->SEL1 &=~ WARN_LED;
            LED_PORT->DIR |= WARN_LED;      //Set as output
        }
        sampleDistance();                   //Call function to sample distance
        distanceBlinkerControl();           //Determine LED state dependent on the distance
	}
}

void TA0_N_IRQHandler(void){
    uint8_t j;
    double sigTime;
    sum = 0;
    if((TIMER_A0->CCTL[4] & TIMER_A_CCTLN_COV) == 0){
        currentEdge = TIMER_A0->CCR[4];                 //Grab timer at which interrupt was triggered from buffer
        deltaCycles = currentEdge - lastEdge;           //Determine the time difference since last occurrence
        lastEdge = currentEdge;                         //Set the new lastEdge

        //Calculate the distance from deltaTime using speed of sound at sea level.
        //T = (1/3MHz) = 0.333 (us/clock cycle)
        if(deltaCycles < 50000){
            sigTime = deltaCycles * 0.333;  //sigTime units is microseconds (us)
            distCentimeter = sigTime / 58;  //microseconds / 58 = centimeters
            distMilli = distCentimeter * 100; //centimeter * 100 = millimeters
            rollingValues[i%8] = distCentimeter;
            i++;
            for(j = 0; j < 8; j++){
                sum += rollingValues[j];
            }
            rollingAverage = sum / 8;
        }
    }


    TIMER_A0->CCTL[4] &=~ TIMER_A_CCTLN_COV;
    TIMER_A0->CCTL[4] &=~ TIMER_A_CCTLN_CCIFG;      //Clear TimerA0 interrupts
}

void sampleDistance(void){
    setTrigger();           //Send a trigger signal to the US sensor
}

void setTrigger(void){
    US_PORT->OUT &=~ TRIGGER;    //Set Trigger pin to send signal to US Sensor
    sysTickSetup();              //Initialize SysTick Timer
    usDelay(10);                 //Delay On-time for 10 us as per US Sensor Datasheet
    intSysTickSetup();           //Re-initialize SysTick Timer with an interrupt
    US_PORT->OUT |= TRIGGER;     //Clear Trigger pin to end trigger signal
}

void setupTimerA(void){
    TIMER_A2->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__UP|              //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P5.6 Timer A
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;        //Set output mode as Reset/Set

    TIMER_A0->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__CONTINUOUS|      //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P2.5 Timer A
    TIMER_A0->CCTL[4] = (TIMER_A_CCTLN_CM__BOTH|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source
}

void SysTick_Handler(void){
    LED_PORT->OUT ^= WARN_LED;      //Toggle LED when SysTick interrupt is triggered.
}

void gpioSetup(void){
    //Set up 2.5 as TimerA instances to measure the time it take the signal to return to the sensor
    US_PORT->SEL0 |= ECHO;
    US_PORT->SEL1 &=~ ECHO;
    US_PORT->DIR &=~ ECHO;

    //Set up 5.6 as TimerA instances to measure the time it take the signal to return to the sensor
    LED_PORT->SEL0 |= WARN_LED;
    LED_PORT->SEL1 &=~ WARN_LED;
    LED_PORT->DIR |= WARN_LED;

    //Set up Ultrasonic Sensor trigger pin
    US_PORT->SEL0 &=~ TRIGGER;  //Set Pin 2.5 as GPIO to connect to Ultrasonic Sensor
    US_PORT->SEL1 &=~ TRIGGER;
    US_PORT->DIR |= TRIGGER;    //Set pin connected to Ultrasonic Sensor Trigger as output
    US_PORT->OUT |= TRIGGER;   //Set trigger pin to low to avoid accidental sampling
}


