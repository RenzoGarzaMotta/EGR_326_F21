/*Lab 4 - Interfacing a Keypad and Rotary Encoder with the MSP432
 *Part 1 - Displaying the key pressed on the CCS console
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
 *  This part will use the MSP432 serial port to send the key press information collected by the program
 *  to the CCS console window for display. This program also accommodates for bounce in the keypad buttons.
 *  The program will account for keyboard presses being held down.
 *
 *  -Connect your keypad to the MSP432 I/O port pins as designed in the prelab exercise
 *  -The program prompts the user to press a key on the keypad
 *  -Read the keypad and print the number (or symbol) of the  key that was pressed to the CCS
 */

#include "msp.h"
#include "macros.h"
#include "keypad.h"
#include "systick.h"

void gpioSetup(void);       //gpioSetup Function definition

extern char keyPress;       //Declare an external global variable
char val;                   //val is a variable that stores keypad pressed values.

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	sysTickSetup();         //Initialize SysTick Timer for delays
	gpioSetup();            //Call gpioSetup function to set up all GPIO pins
	keypadInit();           //Initialize the Keypad in Port 4

	while(1){
	    if(readKeypad()){       //Continuously check for keypad entries
	        val = keyPress;     //Store data entered into the keypad
            printKeys(val);     //Print keypad values to the console
	    }
	}
}

void gpioSetup(void){
    P2->SEL0 &=~ BIT0;  //Setup LED pin 2.0 (RED)
    P2->SEL1 &=~ BIT0;
    P2->DIR |= BIT0;    //Set P2.0 LED as an output
    P2->OUT &=~ BIT0;   //Initialize the LED as off
}
