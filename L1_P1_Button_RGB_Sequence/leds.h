/*
 * leds.h
 *
 *  Created on: Sep 8, 2021
 *      Author: renzogarzamotta
 */

#ifndef LEDS_H_
#define LEDS_H_

void turnOn(uint8_t led);
void turnOff(uint8_t led);
void toggleLED(uint8_t led);
void sequenceControl(void);

enum leds{red, green, blue};    //Enumerate leds

#endif /* LEDS_H_ */
