/*Lab 9 - Speed Sensing using Hall efect Sensors
 *Part 2 - Unipolar Stepper Speed Control
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
 *  This part will use the MSP432 to control the speed of a unipolar
 *  motor through a Serial interface with the console.
 */

#include "msp.h"
#include "stdio.h"
#include "systick.h"
#include "unipolarMotor.h"

void promptUser(void);

extern uint16_t speed;  //Declare an external variable from a library.
short selection;


void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	uint8_t print = 1;  //Set print flag as default

	sysTickSetup();     //Initialize SysTick timer
	initializeUnipolarMotor();  //Initialize Unipolar Motor

	while(1){
//	    if(print){          //Check if user prompt has been printed to the console
//	        promptUser();   //Print prompt for user selection
//	        CW(66);         //Spin the Unipolar motor 66 steps clockwise
//	        print = 1;      //Set print flag to print prompt when motor rotation is finished
//	    }
	    CW(1000);
	}
}

void promptUser(void){
    printf("Please enter a speed\n");
    printf("0 -> 500 msec/step\n");     //Select "0" for a 500 msec/step speed (slow)
    printf("1 -> 400 msec/step\n");     //Select "1" for a 400 msec/step speed
    printf("2 -> 300 msec/step\n");     //Select "2" for a 300 msec/step speed
    printf("3 -> 200 msec/step\n");     //Select "3" for a 200 msec/step speed
    printf("4 -> 100 msec/step\n");     //Select "4" for a 100 msec/step speed
    printf("5 ->  50 msec/step\n");     //Select "5" for a  50 msec/step speed (fast)
    scanf("%hd", &selection);           //Scan input from the user

    switch(selection){                  //Determine what selection was made by the user
        case 0:
            speed = 500;                 //Set the speed delay to 50ms / step
            break;
        case 1:
            speed = 400;                 //Set the speed delay to 40ms / step
            break;
        case 2:
            speed = 300;                 //Set the speed delay to 30ms / step
            break;
        case 3:
            speed = 200;                 //Set the speed delay to 20ms / step
            break;
        case 4:
            speed = 100;                 //Set the speed delay to 10ms / step
            break;
        case 5:
            speed = 50;                  //Set the speed delay to 5ms / step
            break;
    }
}
