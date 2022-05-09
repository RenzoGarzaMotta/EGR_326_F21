/*Lab 2 - Input Interfacing with the MSP432 using interrupts
 *Part 1 - Controlling the blink rate of an LED using pushbutton switches
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
 *  This part will drive an external LED interfaced to a port pin on the MSP432. Also, two external
 *  pushbuttons are required to interface with other port pins on the MSP432. These pushbuttons
 *  will control the blink rate of the LED using interrupts
 *
 *  The first button will start the LED to blink at 2 sec intervals (one second on, one second off).
 *  Each press of the button will double the blink rate.
 *  The seond button will stop the LED from blinking (if the blink rate is 0.5 Hz), or reduce the
 *  blink rate by a factor of 2 if the blink rate is faster than 0.5 Hz.
 *
 *  Holding down a button will not change the blink rate more than pressing and releasing a button.
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
