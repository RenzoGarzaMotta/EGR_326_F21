/*
 * bipolarMotor.h
 *
 *  Created on: Oct 21, 2021
 *      Author: renzogarzamotta
 */

#ifndef BIPOLARMOTOR_H_
#define BIPOLARMOTOR_H_

#define BI_PORT    P4
#define IN1        BIT0     //Coil A'
#define IN2        BIT1     //Coil A
#define IN3        BIT2     //Coil B'
#define IN4        BIT3     //Coil B
#define SEQ_SIZE   4
#define BI_MOTOR   (IN1|IN2|IN3|IN4)

void initializeBipolarMotor(void);
void CCW(uint16_t steps);
void CW(uint16_t steps);

#endif /* BIPOLARMOTOR_H_ */
