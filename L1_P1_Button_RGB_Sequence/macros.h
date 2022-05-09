/*
 * macros.h
 *
 *  Created on: Sep 8, 2021
 *      Author: renzogarzamotta
 */

#ifndef MACROS_H_
#define MACROS_H_

#define LED_PORT        P2              //Port used for the LEDs
#define LED_RED         BIT0            //Red LED bit
#define LED_GREEN       BIT1            //Green LED bit
#define LED_BLUE        BIT2            //Blue LED bit
#define LEDS            (LED_RED|LED_GREEN|LED_BLUE)

#define BTN_PORT        P4              //Button port
#define BTN1            BIT3            //button1 bit


#endif /* MACROS_H_ */
