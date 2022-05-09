/*Lab 3 - Input Interfacing with the MSP432 using interrupts
 *Part 2 - Controlling the LED lighting sequence using a timer interrupt and two pushbutton interrupts
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
 *  This part will use a second pushbutton in combination with the first to flash the selected LED on
 *  and off in 0.5 second intervals. Both the SysTick and each pushbutton  should be triggered using
 *  interrupts.
 *  -Modify Part II code so that the first pushbutton will select the LED colot. The second pushbutton
 *   is then used to turn that color LED on and off. (Both buttons should be triggered by interrupts.)
 *  -Selectr a color LED by sequencing through the colors with the first pushbutton. When the second
 *   pushbutton is pressed and held, the selected colot LED flashed on and off every 0.5s. When released,
 *   the LED stays lit with the last selected color.
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
