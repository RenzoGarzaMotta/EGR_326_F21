/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 2 - Sequencing colors of an RGB LED using a timer interrupt and pushbutton
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
 *  This part controls the RGB LED sequence using a timer peripheral in the MSP432.
 *  -Use the three on-board RGB LEDs with one external pushbutton.
 *  -The pushbutton should be triggered by interrupts.
 *  -On RESET, the LEDs should be OFF.
 *  -When the pushbutton is press and held down the RGB LED should follow the sequence
 *   RED->GREEN->BLUE with a 1sec On-Time for each color (use SysTick).
 *  -This sequence should repeat as long as the pushbutton is depressed. When the
 *   pushbutton is released the sequencing pauses with the current LED remaining ON.
 *  -Upon the next press of the pushbutton, the LED sequence will start up again in
 *   reverse order and continue for as long as the pushbutton is held down.
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
