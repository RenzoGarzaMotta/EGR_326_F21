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

#ifndef LEDS_H_
#define LEDS_H_

void turnOn(uint8_t led);
void turnOff(uint8_t led);
void toggleLED(uint8_t led);
void sequenceControl(void);
void reverseSequenceControl(void);

enum leds{red, green, blue};    //Enumerate leds

#endif /* LEDS_H_ */
