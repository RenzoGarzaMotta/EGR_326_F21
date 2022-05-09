/*Lab 4 - Interfacing a Keypad and Rotary Encoder with the MSP432
 *Part 1 - Displaying the key pressed on the CCS console
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
 *  This part will use the MSP432 serial port to send the key press information collected by the program
 *  to the CCS console window for display. This program also accommodates for bounce in the keypad buttons.
 *  The program will account for keyboard presses being held down.
 *
 *  -Connect your keypad to the MSP432 I/O port pins as designed in the prelab exercise
 *  -The program prompts the user to press a key on the keypad
 *  -Read the keypad and print the number (or symbol) of the  key that was pressed to the CCS
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define SYS_MAX         0x00FFFFFF      //Max LOAD value
#define SYS_COUNT_FG    0x00010000      //Flag sets when SysTick timer runs out
#define SYS_ENABLE      0x00000001      //SysTick Enable bit mask
#define SYS_INT         0x00000002      //SysTick Interrupt bit mask
#define SYS_CLKSRC      0x00000004      //SysTick Clock Source bit mask
#define SYS_SETUP       (SYS_ENABLE|SYS_CLKSRC)
#define SYS_SETUP_INT   (SYS_ENABLE|SYS_CLKSRC|SYS_INT)

void sysTickSetup(void);
void intSysTickSetup(void);
void msDelay(uint16_t time);
void usDelay(uint32_t time);

#endif /* SYSTICK_H_ */
