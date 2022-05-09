/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 2 - Sequencing colors of an RGB LED using a timer interrupt and pushbutton
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
 *  This part controls the RGB LED sequence using a timer peripheral in the MSP432.
 *  -Use the three on-board RGB LEDs with one external pushbutton.
 *  -The pushbutton should be triggered by interrupts.
 *  -On RESET, the LEDs should be OFF.
 *  -When the pushbutton is press and held down the RGB LED should follow the sequence
 *   RED->GREEN->BLUE with a 1sec On-Time for each color (use SysTick).
 *  -This sequence should repeat as long as the pushbutton is depressed. When the
 *   pushbutton is released the sequencing pauses with the current LED remaining ON.
 *  -Upon the next press of the pushbutton, the LED sequence will start up again in
 *   reverse order and continue for as long as the pushbutton is held down.
 */

#ifndef MACROS_H_
#define MACROS_H_

#define LED_PORT        P4              //Port used for the LEDs
#define LED_RED         BIT0            //Red LED bit
#define LED_GREEN       BIT1            //Green LED bit
#define LED_BLUE        BIT2            //Blue LED bit
#define LEDS            (LED_RED|LED_GREEN|LED_BLUE)

#define BTN_PORT        P4              //Button port
#define BTN1            BIT3            //button1 bit
#define BTN2            BIT4            //button2 bit
#define BTNS            (BTN1|BTN2)


#endif /* MACROS_H_ */
