
#include "msp.h"
#include "systick.h"
#include "bipolarMotor.h"

//uint8_t inA     [] = {1,0,0,0,0,0,1,1};
//uint8_t inB     [] = {1,1,1,0,0,0,0,0};
//uint8_t inAp    [] = {0,0,1,1,1,0,0,0};
//uint8_t inBp    [] = {0,0,0,0,1,1,1,0};

uint8_t inA     [] = {1,0,0,1};
uint8_t inB     [] = {1,1,0,0};
uint8_t inAp    [] = {0,1,1,0};
uint8_t inBp    [] = {0,0,1,1};

void initializeBipolarMotor(void){
    BI_PORT->SEL0 &=~ BI_MOTOR; //Initialize GPIO
    BI_PORT->SEL1 &=~ BI_MOTOR;
    BI_PORT->DIR  |= BI_MOTOR;  //Set GPIO as output
    BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
}

void CCW(uint16_t steps){
    uint16_t i;

    for(i = 0; i < steps; i++){
        BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
        BI_PORT->OUT  |= (inAp[i%SEQ_SIZE]      & IN1); //Set output of A'
        BI_PORT->OUT  |= (inA[i%SEQ_SIZE] <<1 & IN2);  //Set output of A
        BI_PORT->OUT  |= (inBp[i%SEQ_SIZE]  <<2 & IN3); //Set output of B'
        BI_PORT->OUT  |= (inB[i%SEQ_SIZE] <<3 & IN4); //Set output of B
        msDelay(5);
    }
    BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
}

void CW(uint16_t steps){
    uint16_t i;

    for(i = 0; i < steps; i++){
        BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
        BI_PORT->OUT  |= (inA[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] & IN1);
        BI_PORT->OUT  |= (inAp[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] <<1 & IN2);
        BI_PORT->OUT  |= (inB[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] <<2& IN3);
        BI_PORT->OUT  |= (inBp[(SEQ_SIZE - 1) - (i%SEQ_SIZE)] <<3& IN4);
        msDelay(5);
    }
    BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
}


