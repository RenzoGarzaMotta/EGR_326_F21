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
#include "msp.h"
#include "systick.h"
#include "unipolarMotor.h"

//uint8_t in_1 [] = {1, 0, 0, 0};
//uint8_t in_2 [] = {0, 1, 0, 0};
//uint8_t in_3 [] = {0, 0, 1, 0};
//uint8_t in_4 [] = {0, 0, 0, 1};

//uint8_t in_1 [] = {1, 0, 0, 1};
//uint8_t in_2 [] = {1, 1, 0, 0};
//uint8_t in_3 [] = {0, 1, 1, 0};
//uint8_t in_4 [] = {0, 0, 1, 1};

uint8_t in_1 [] = {1, 0, 0, 0, 0, 0, 1, 1};
uint8_t in_2 [] = {1, 1, 1, 0, 0, 0, 0, 0};
uint8_t in_3 [] = {0, 0, 1, 1, 1, 0, 0, 0};
uint8_t in_4 [] = {0, 0, 0, 0, 1, 1, 1, 0};

uint16_t speed;

void initializeUnipolarMotor(void){
    UNI_PORT->SEL0 &=~ UNI_MOTOR; //Initialize GPIO
    UNI_PORT->SEL1 &=~ UNI_MOTOR;
    UNI_PORT->DIR  |= UNI_MOTOR;  //Set GPIO as output
    UNI_PORT->OUT  &=~ UNI_MOTOR; //Turn off output
}

void CCW(uint16_t steps){
    uint16_t i;

    for(i = 0; i < steps; i++){
        UNI_PORT->OUT  &=~ UNI_MOTOR; //Turn off output
        UNI_PORT->OUT  |= (in_1[i%SEQ_SIZE] << 4 & INT_1); //Set output of A'
        UNI_PORT->OUT  |= (in_2[i%SEQ_SIZE] << 5 & INT_2);  //Set output of A
        UNI_PORT->OUT  |= (in_3[i%SEQ_SIZE] << 6 & INT_3); //Set output of B'
        UNI_PORT->OUT  |= (in_4[i%SEQ_SIZE] << 7 & INT_4); //Set output of B
        msDelay(speed);
    }
    UNI_PORT->OUT  &=~ UNI_MOTOR; //Turn off output
}

void CW(uint16_t steps){
    uint16_t i;

    for(i = 0; i < steps; i++){
        UNI_PORT->OUT  &=~ UNI_MOTOR; //Turn off output
        UNI_PORT->OUT  |= (in_1[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] << 4 & INT_1);
        UNI_PORT->OUT  |= (in_2[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] << 5 & INT_2);
        UNI_PORT->OUT  |= (in_3[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] << 6 & INT_3);
        UNI_PORT->OUT  |= (in_4[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] << 7 & INT_4);
        msDelay(speed);
    }
    UNI_PORT->OUT  &=~ UNI_MOTOR; //Turn off output
}
