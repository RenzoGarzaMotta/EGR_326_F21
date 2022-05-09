#include "msp.h"
#include "timer32.h"

//uint8_t inA     [] = {1,0,0,0,0,0,1,1};
//uint8_t inB     [] = {1,1,1,0,0,0,0,0};
//uint8_t inAp    [] = {0,0,1,1,1,0,0,0};
//uint8_t inBp    [] = {0,0,0,0,1,1,1,0};

uint8_t inA     [] = {1,0,0,1};
uint8_t inB     [] = {1,1,0,0};
uint8_t inAp    [] = {0,1,1,0};
uint8_t inBp    [] = {0,0,1,1};

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

uint16_t currentLocation;
uint16_t stepCount = 0, moveSteps, engageMotor;
uint8_t CWsteps, CCWsteps, i = 0;

uint16_t currentLocation2;
uint16_t stepCount2 = 0, moveSteps2, engageMotor2;
uint8_t CWsteps2, CCWsteps2, j = 0;

void timer32Setup1(void){

    TIMER32_1->CONTROL |= (TIMER32_CONTROL_ENABLE|  //Enable Timer 32
                           TIMER32_CONTROL_MODE|    //Periodic Mode
                           TIMER32_CONTROL_SIZE|    //Set 32-bit Timer
                           TIMER32_CONTROL_IE);     //Enable Interrupt

    TIMER32_1->LOAD = 1;
    TIMER32_1->BGLOAD = 48000 * 1000;

    TIMER32_1->INTCLR = 0; //Any write to the T32INTCLR1 register clears the interrupt from the counter.
}

void timer32Setup2(void){
    TIMER32_2->CONTROL |= (TIMER32_CONTROL_ENABLE|  //Enable Timer 32
                           TIMER32_CONTROL_MODE|    //Periodic Mode
                           TIMER32_CONTROL_SIZE|    //Set 32-bit Timer
                           TIMER32_CONTROL_IE);     //Enable Interrupt

    TIMER32_2->LOAD = 1;
    TIMER32_2->BGLOAD = 48000 * 1;

    TIMER32_2->INTCLR = 0; //Any write to the T32INTCLR1 register clears the interrupt from the counter.
}

void initializeBipolarMotor(void){
    BI_PORT->SEL0 &=~ BI_MOTOR; //Initialize GPIO
    BI_PORT->SEL1 &=~ BI_MOTOR;
    BI_PORT->DIR  |= BI_MOTOR;  //Set GPIO as output
    BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output


    BI_PORT_2->SEL0 &=~ BI_MOTOR_2; //Initialize GPIO
    BI_PORT_2->SEL1 &=~ BI_MOTOR_2;
    BI_PORT_2->DIR  |= BI_MOTOR_2;  //Set GPIO as output
    BI_PORT_2->OUT  &=~ BI_MOTOR_2; //Turn off output
}

void initializeUnipolarMotor(void){
    UNI_PORT->SEL0 &=~ UNI_MOTOR; //Initialize GPIO
    UNI_PORT->SEL1 &=~ UNI_MOTOR;
    UNI_PORT->DIR  |= UNI_MOTOR;  //Set GPIO as output
    UNI_PORT->OUT  &=~ UNI_MOTOR; //Turn off output
}

void updateLocation(uint16_t loc){
    uint16_t diff = 0;
    engageMotor = 1;
    if(loc < currentLocation){
        CWsteps = 0;
        CCWsteps = 1;
        diff = currentLocation - loc;
        moveSteps = diff * 1;
    }else if(loc > currentLocation){
        CWsteps = 1;
        CCWsteps = 0;
        diff = loc - currentLocation;
        moveSteps = diff * 1;
    }
    currentLocation = loc;
}

void updateTachometer(uint16_t loc){
    uint16_t diff = 0;
    engageMotor2 = 1;
    if(loc < currentLocation2){
        CWsteps2 = 0;
        CCWsteps2 = 1;
        diff = currentLocation2 - loc;
        moveSteps2 = diff * 1;
    }else if(loc > currentLocation2){
        CWsteps2 = 1;
        CCWsteps2 = 0;
        diff = loc - currentLocation2;
        moveSteps2 = diff * 1;
    }
    currentLocation2 = loc;
}

void T32_INT2_IRQHandler(void){
    TIMER32_2->INTCLR = 0; //Any write to the T32INTCLR1 register clears the interrupt from the counter.
    TIMER32_2->BGLOAD = 48000 * STEP_DELAY;

    if(engageMotor2){
        if(stepCount2 <= moveSteps2){
            if(CWsteps2){ //Bipolar
                BI_PORT_2->OUT  &=~ BI_MOTOR_2; //Turn off output
                BI_PORT_2->OUT  |= (inA[(BI_SEQ_SIZE_2 - 1) - (stepCount2%BI_SEQ_SIZE_2)] & IN1_2);
                BI_PORT_2->OUT  |= (inAp[(BI_SEQ_SIZE_2 - 1) - (stepCount2%BI_SEQ_SIZE_2)] <<1 & IN2_2);
                BI_PORT_2->OUT  |= (inB[(BI_SEQ_SIZE_2 - 1) - (stepCount2%BI_SEQ_SIZE_2)] <<2& IN3_2);
                BI_PORT_2->OUT  |= (inBp[(BI_SEQ_SIZE_2 - 1) - (stepCount2%BI_SEQ_SIZE_2)] <<3& IN4_2);
            }else if(CCWsteps2){ //Bipolar
                BI_PORT_2->OUT  &=~ BI_MOTOR_2; //Turn off output
                BI_PORT_2->OUT  |= (inAp[stepCount2%BI_SEQ_SIZE_2]      & IN1_2); //Set output of A'
                BI_PORT_2->OUT  |= (inA[stepCount2%BI_SEQ_SIZE_2] <<1 & IN2_2);  //Set output of A
                BI_PORT_2->OUT  |= (inBp[stepCount2%BI_SEQ_SIZE_2]  <<2 & IN3_2); //Set output of B'
                BI_PORT_2->OUT  |= (inB[stepCount2%BI_SEQ_SIZE_2] <<3 & IN4_2); //Set output of B
            }
            stepCount2++;
        }else{
            engageMotor2 = 0;
            stepCount2 = 0;
            CWsteps2 = 0;
            CCWsteps2 = 0;
        }
    }
}

void T32_INT1_IRQHandler(void){
    TIMER32_1->INTCLR = 0; //Any write to the T32INTCLR1 register clears the interrupt from the counter.
    TIMER32_1->BGLOAD = 48000 * STEP_DELAY;

    if(engageMotor){
        if(stepCount <= moveSteps){
            if(CWsteps){ //Bipolar
                BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
                BI_PORT->OUT  |= (inA[(BI_SEQ_SIZE - 1) - (stepCount%BI_SEQ_SIZE)] & IN1);
                BI_PORT->OUT  |= (inAp[(BI_SEQ_SIZE - 1) - (stepCount%BI_SEQ_SIZE)] <<1 & IN2);
                BI_PORT->OUT  |= (inB[(BI_SEQ_SIZE - 1) - (stepCount%BI_SEQ_SIZE)] <<2& IN3);
                BI_PORT->OUT  |= (inBp[(BI_SEQ_SIZE - 1) - (stepCount%BI_SEQ_SIZE)] <<3& IN4);
            }else if(CCWsteps){ //Bipolar
                BI_PORT->OUT  &=~ BI_MOTOR; //Turn off output
                BI_PORT->OUT  |= (inAp[stepCount%BI_SEQ_SIZE]      & IN1); //Set output of A'
                BI_PORT->OUT  |= (inA[stepCount%BI_SEQ_SIZE] <<1 & IN2);  //Set output of A
                BI_PORT->OUT  |= (inBp[stepCount%BI_SEQ_SIZE]  <<2 & IN3); //Set output of B'
                BI_PORT->OUT  |= (inB[stepCount%BI_SEQ_SIZE] <<3 & IN4); //Set output of B
            }
            stepCount++;
        }else{
            engageMotor = 0;
            stepCount = 0;
            CWsteps = 0;
            CCWsteps = 0;
        }
    }
}
