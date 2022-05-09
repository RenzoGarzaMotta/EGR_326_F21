
#ifndef KEYPAD_H
#define KEYPAD_H
/*------------PREPROCESSOR DEFINITION-------------*/
#include "msp.h"

/*----------------MACRO DEFINITION----------------*/
#define KEYPAD_PORT             P4
#define ROW0                    BIT0
#define ROW1                    BIT1
#define ROW2                    BIT2
#define ROW3                    BIT3
#define COL0                    BIT4
#define COL1                    BIT5
#define COL2                    BIT6

#define ROWS                    (ROW0|ROW1|ROW2|ROW3)
#define COLS                    (COL0|COL1|COL2)

#define UNPRESSED               0x0F
#define PRESSED                 !UNPRESSED

#define BUTTON_ONE              0x0E
#define BUTTON_TWO              0x0E
#define BUTTON_THREE            0x0E
#define BUTTON_FOUR             0x0D
#define BUTTON_FIVE             0x0D
#define BUTTON_SIX              0x0D
#define BUTTON_SEVEN            0x0B
#define BUTTON_EIGHT            0x0B
#define BUTTON_NINE             0x0B
#define BUTTON_ZERO             0x07
#define STAR                    0x07
#define POUND                   0x07
#define CLEAR_PORT              0x00

/*--------------PROTOTYPE DEFINITION--------------*/
void KEYPAD_INIT(void);
int READ_KEYPAD(void);
void PRINT_KEYS(char val);
void INPUT(char input[]);
void INPUT_TROUBLESHOOT(void);
void confirm_PIN(void);
void PIN_VALIDATED(char PIN2[]);

#endif //KEYPAD_H
