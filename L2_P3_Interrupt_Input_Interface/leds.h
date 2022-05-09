/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 3 - Controlling the LED lighting sequence using a timer interrupt and two pushbutton interrupts
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
 *  This part will use a second pushbutton in combination with the first to flash the selected LED on
 *  and off in 0.5 second intervals. Both the SysTick and each pushbutton  should be triggered using
 *  interrupts.
 *  -Modify Part II code so that the first pushbutton will select the LED colot. The second pushbutton
 *   is then used to turn that color LED on and off. (Both buttons should be triggered by interrupts.)
 *  -Selectr a color LED by sequencing through the colors with the first pushbutton. When the second
 *   pushbutton is pressed and held, the selected colot LED flashed on and off every 0.5s. When released,
 *   the LED stays lit with the last selected color.
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
