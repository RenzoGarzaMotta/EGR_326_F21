#ifndef TIMER32_H_
#define TIMER32_H_

#define STEP_DELAY  3

#define BI_PORT    P5
#define IN1        BIT0     //Coil A'
#define IN2        BIT1     //Coil A
#define IN3        BIT2     //Coil B'
#define IN4        BIT3     //Coil B
#define BI_SEQ_SIZE   4
#define BI_MOTOR   (IN1|IN2|IN3|IN4)

#define UNI_PORT    P4
#define INT_1       BIT0
#define INT_2       BIT1
#define INT_3       BIT2
#define INT_4       BIT3
#define UNI_SEQ_SIZE    8
#define UNI_MOTOR   (INT_1|INT_2|INT_3|INT_4)

#define BI_PORT_2    P4
#define IN1_2        BIT0     //Coil A'
#define IN2_2        BIT1     //Coil A
#define IN3_2        BIT2     //Coil B'
#define IN4_2        BIT3     //Coil B
#define BI_SEQ_SIZE_2   4
#define BI_MOTOR_2   (IN1_2|IN2_2|IN3_2|IN4_2)

void timer32Setup1(void);
void timer32Setup2(void);
void initializeBipolarMotor(void);
void initializeUnipolarMotor(void);
void updateLocation(uint16_t loc);
void updateTachometer(uint16_t loc);

#endif /* TIMER32_H_ */
