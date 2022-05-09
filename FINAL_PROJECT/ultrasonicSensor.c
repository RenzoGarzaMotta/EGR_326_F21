#include "msp.h"
#include "systick.h"
#include "ultrasonicSensor.h"

uint16_t ultrasonicPeriod = 1000;

void sampleDistance(void){
    setTrigger();           //Send a trigger signal to the US sensor
}

void setTrigger(void){
    US_PORT->OUT &=~ TRIGGER;    //Set Trigger pin to send signal to US Sensor
    usDelay(10);                 //Delay On-time for 10 us as per US Sensor Datasheet
    US_PORT->OUT |= TRIGGER;     //Clear Trigger pin to end trigger signal
}

void setupUltrasonicSensor(void){
    //Set up P2.5 Timer A
    TIMER_A3->CCTL[1] = (TIMER_A_CCTLN_CM__BOTH|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source
}

void gpioUltrasonicSetup(void){
    //Set up 2.5 as TimerA instances to measure the time it take the signal to return to the sensor
    ECHO_PORT->SEL0 |= ECHO;
    ECHO_PORT->SEL1 &=~ ECHO;
    ECHO_PORT->DIR &=~ ECHO;

    //Set up Ultrasonic Sensor trigger pin
    US_PORT->SEL0 &=~ TRIGGER;  //Set Pin 2.5 as GPIO to connect to Ultrasonic Sensor
    US_PORT->SEL1 &=~ TRIGGER;
    US_PORT->DIR |= TRIGGER;    //Set pin connected to Ultrasonic Sensor Trigger as output
    US_PORT->OUT |= TRIGGER;   //Set trigger pin to low to avoid accidental sampling
}
