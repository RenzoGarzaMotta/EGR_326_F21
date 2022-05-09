/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 1 - Controlling the blink rate of an LED using pushbutton switches
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
 *  This part will drive an external LED interfaced to a port pin on the MSP432. Also, two external
 *  pushbuttons are required to interface with other port pins on the MSP432. These pushbuttons
 *  will control the blink rate of the LED using interrupts
 *
 *  The first button will start the LED to blink at 2 sec intervals (one second on, one second off).
 *  Each press of the button will double the blink rate.
 *  The seond button will stop the LED from blinking (if the blink rate is 0.5 Hz), or reduce the
 *  blink rate by a factor of 2 if the blink rate is faster than 0.5 Hz.
 *
 *  Holding down a button will not change the blink rate more than pressing and releasing a button.
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
