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

#ifndef UNIPOLARMOTOR_H_
#define UNIPOLARMOTOR_H_

#include "msp.h"

#define UNI_PORT    P2
#define INT_1       BIT4
#define INT_2       BIT5
#define INT_3       BIT6
#define INT_4       BIT7
#define SEQ_SIZE    8
#define UNI_MOTOR   (INT_1|INT_2|INT_3|INT_4)
//#define DELAY       4

void initializeUnipolarMotor(void);
void CCW(uint16_t steps);
void CW(uint16_t steps);

#endif /* UNIPOLARMOTOR_H_ */
