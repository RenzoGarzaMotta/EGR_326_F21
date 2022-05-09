/*Lab 4 - Interfacing a Keypad and Rotary Encoder with the MSP432
 *Part 3 - Merging the keypad and rotary encoder
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
 *  This part requires the user to enter a value using the keypad and then turning the rotary
 *  encoder until the absolution number of pulses (abosilute value of CW - CCW) exceeds the
 *  keypad value. If this occurs, the RED led should be illuminated. Pressing the pushbutton
 *  switch resets the count and turns off the LED.
 *
 *  -Prompt the user for a pulse range
 *  -Monitor the rotary encoder as in part II to keep track of number of counts (CW and CCW)
 *  -Illuminate a RED LED if the CW count is met (but not exceeded)
 *  -Illuminate a BLUE LED if the CCW count is met (but not exceeded)
 *  -Nothing then will happen until the user presses the pushbutton switch to reset the count and turn off the LED.
 *  -The program should return to step one of part 3 upon the button being pressed.
 *
 *Connection Map:
 * KEYPAD             MSP432
 *  ROW0---------------P4.0
 *  ROW1---------------P4.1
 *  ROW2---------------P4.2
 *  ROW3---------------P4.3
 *  COL0---------------P4.4
 *  COL1---------------P4.5
 *  COL2---------------P4.6
 *
 *Relevant ASCII Values:
 * Decimal         Char
 *   35--------------#
 *   42--------------*
 *   48--------------0
 *   49--------------1
 *   50--------------2
 *   51--------------3
 *   52--------------4
 *   53--------------5
 *   54--------------6
 *   55--------------7
 *   56--------------8
 *
 *
 *   |       FRONT      |
 *   |   ______         |
 *   |  |      |        |
 *   |_GND_5V_SW_DT_CLK_|
 *      |  |  |  |  |
 *      |  |  |  |  |____P2.5 (2N7000 Level shifter 5V to 3.3V)
 *      |  |  |  |_______P2.6 (2N7000 Level shifter 5V to 3.3V)
 *      |  |  |__________P2.4 (Internally pulled-up)
 *      |  |_____________5V
 *      |________________GND
 */

#include "msp.h"
#include <stdio.h>
#include "macros.h"
#include "keypad.h"
#include "systick.h"

void gpioSetup(void);           //gpioSetup Function definition
void setupTimerA(void);         //setupTimerA Function definition
uint8_t debounceBtn1(void);     //Encoder button debounce Function definition

extern char keyPress;           //Declare an external global variable
char val;                       //val is a variable that stores keypad pressed values.
uint8_t btn1Flag = 0;           //Flag to indicate button1 has been pressed
uint8_t CWflag, CCWflag;        //Flag to indicate if a CW or CCW rotation has taken place
int CW, CCW;                    //These counters keep track of rotation count in the CW or CCW direction
int clkState, DTState;          //These variables were use to view CLK and DT state during debugger session
uint8_t prompt = 1;             //Flag to indicate if prompt to the user needs to be printed to the console.

void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	sysTickSetup(); //Set up SysTick Timer to use Delays
    gpioSetup();    //Set up project GPIO
    keypadInit();   //Initialize the Keypad in Port 4
    setupTimerA();  //Set up Rotary Encoder Timer A

    NVIC->ISER[0] = 1 << (TA0_N_IRQn & 31);         //Set up TimerA0.N interrupt
    NVIC->ISER[1] = 1 << (PORT2_IRQn & 31);         //Set interrupts for Port 2
    __enable_irq();                                 //Enable all interrupts


    while(1){
        if(prompt){
            printf("\nPlease enter a rotary encoder pulse limit\n");    //Prompt user to enter a pulse limit
            fflush(stdout);     //Flush bus buffer
            prompt = 0;         //Clear prompting flag to prevent printing multiple prompts
        }
        if(readKeypad()){       //Continuously check for keypad entries
            val = keyPress;     //Store data entered into the keypad
            printKeys(val);     //Print keypad values to the console
        }

        if(btn1Flag){           //Check if the Encoder button has been pressed
            LED_PORT->OUT &=~ (RED_LED|BLUE_LED);   //Clear all LEDs
            CW = 0;             //Reset CW counter variable
            CCW = 0;            //Reset CCW counter variable
            val = 0;            //Reset keypad input variable
            prompt = 1;         //Set flag to prompt user for another pulse limit
            btn1Flag = 0;       //Clear encoder button flag
        }

        if(CWflag){             //Check if a CW rotation was detected
            CW++;               //Increase the CW click counter

            if((val - 48) ==  CW){          //Check if CW rotations match the pulse limit
                LED_PORT->OUT |= RED_LED;   //If CW rotations and pulse limit match, turn on the RED LED
            }else if ((val - 48) <  CW){    //Else, check if the CW rotations is greater than the pulse limit
                LED_PORT->OUT &=~ RED_LED;  //If CW is greater than the pulse limit, turn the LED off
            }

            CWflag = 0;                     //Clear the CW flag
        }
        if(CCWflag){            //Check if a CW rotation was detected
            CCW++;              //Increase the CW click counter

            if((val - 48) ==  CCW){         //Check if CCW rotations match the pulse limit
                LED_PORT->OUT |= BLUE_LED;  //If CCW rotations and pulse limit match, turn on the RED LED
            }else if ((val - 48) <  CCW){   //Else, check if the CCW rotations is greater than the pulse limit
                LED_PORT->OUT &=~ BLUE_LED; //If CCW is greater than the pulse limit, turn the LED off
            }

            CCWflag = 0;                    //Clear the CCW flag
        }
    }
}

void TA0_N_IRQHandler(void){
    clkState = ENCODER_PORT->IN & CLK;                  //Store clock state when an event in TimerA happens
    DTState = ENCODER_PORT->IN & DT;                    //Store DT state when an event in TimerA happens

    if((ENCODER_PORT->IN & CLK) == 0){                  //Check if falling edge was triggered
        if((ENCODER_PORT->IN & DT) == 0x00000020){      //Check if DT is 1
            CWflag = 1;                                 //If DT is a 1 when CLK encounters a falling edge, the encoder is rotating CW
        }else if((ENCODER_PORT->IN & DT) == 0){         //Check if DT is 0
            CCWflag = 1;                                //If DT is a 0 when CLK encounters a falling edge, the encoder is rotating CCW
        }
    }else if((ENCODER_PORT->IN & CLK) == 0x00000040){   //Check if rising edge was triggered
        if((ENCODER_PORT->IN & DT) == 0x00000020){      //Check if DT is 1
            CCWflag = 1;                                //If DT is a 1 when CLK encounters a rising edge, the encoder is rotating CCW
        }else if((ENCODER_PORT->IN & DT) == 0){         //Check if DT is 0
            CWflag = 1;                                 //If DT is a 0 when CLK encounters a rising edge, the encoder is rotating CW
        }
    }

    TIMER_A0->CCTL[3] &=~ TIMER_A_CCTLN_CCIFG;          //Clear TimerA0 interrupts
}

void PORT2_IRQHandler(void){
    int i;
    if(ENCODER_PORT->IFG & SW){
        for(i = 0; i < 75; i++){        //Cycle through for-loop to check state of button bounce
            if(debounceBtn1()){         //Check that button1 has been pressed
                btn1Flag = 1;           //Set button1 flag to indicate that button1 was pressed
            }
        }
        ENCODER_PORT->IFG &=~ SW;       //Clear interrupt flag for button pin
    }
    ENCODER_PORT->IFG = 0;              //Clear the entire port interrupt flag
}

uint8_t debounceBtn1(void){         //Debounce function for button1
    static uint16_t State1 = 0;     //Current debounce status
    State1 = (State1 << 1) | (ENCODER_PORT->IN & SW)>>1 | 0x8000;     //Shift state to indicate bounce
    if(State1==0x8000)return 1;     //indicates 0 level is
    return 0;
}

void setupTimerA(void){
    ENCODER_PORT->SEL0 |= CLK;                         //Enable Secondary function in Encoder CLK pin
    ENCODER_PORT->SEL1 &=~ CLK;

    TIMER_A0->CTL |= (TIMER_A_CTL_SSEL__SMCLK|         //Timer_A clock source: SMCLK
                      TIMER_A_CTL_MC__CONTINUOUS|      //Continuous mode: Timer counts to 0FFFFh
                      TIMER_A_CTL_ID__1|               //Input Divider of /1
                      TIMER_A_CTL_CLR);                //Clear TA0R

    //Set up P2.6 Timer A
    TIMER_A0->CCTL[3] = (TIMER_A_CCTLN_CM__BOTH|       //Capture on both rising and falling edges
                         TIMER_A_CCTLN_CCIS__CCIA|     //Capture/Compare Input Select: CCIxA
                         TIMER_A_CCTLN_CCIE|           //capture/Compare Interrupt Enable
                         TIMER_A_CCTLN_CAP|            //Enable Capture mode
                         TIMER_A_CCTLN_SCS);           //Synchronize Capture source
}

void gpioSetup(void){
    LED_PORT->SEL0 &=~ (RED_LED|BLUE_LED);  //Setup LED pin 2.0 (RED) and LED pin 2.2 (BLUE)
    LED_PORT->SEL1 &=~ (RED_LED|BLUE_LED);
    LED_PORT->DIR |= (RED_LED|BLUE_LED);
    LED_PORT->OUT &=~ (RED_LED|BLUE_LED);

    ENCODER_PORT->SEL0 &=~ ENCODER;     //Set up encoder pins as GPIO
    ENCODER_PORT->SEL1 &=~ ENCODER;

    ENCODER_PORT->DIR &=~ ENCODER;      //Set up encoder pins as inputs

    ENCODER_PORT->REN |= SW;            //Enable internal resistor
    ENCODER_PORT->OUT |= SW;            //Set internal resistor to pull-up

    ENCODER_PORT->IES |= SW;            //Interrupt edge select high-to-low edge (falling edge)

    ENCODER_PORT->IE |= SW;             //Enable port interrupt

    ENCODER_PORT->IFG = 0;              //Clear interrupt flag (just in case)
}
