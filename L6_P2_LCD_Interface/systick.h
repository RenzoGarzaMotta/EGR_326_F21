/*Lab 6 - Interfacing a Graphic LCD to the MSP432 MCU
 *Part 2 - Displaying Images on the LCD from the MCU
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
 *  This part uses the BmpConvert16.exe executable file to convert 24-bit color bitmap
 *  files into 16-bit color bitmap. This executable file only works on bitmap images of
 *  dimensions of that are less than or equal to 128x160px.
 *
 *  This part of the lab displays a splash screen by dsiplaying an image for three seconds
 *  followed by clearing the screen by filling the entire screen white or black. In addition
 *  to this, a menu is displayed (dor demonstration purposes only).
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

/*--------------------- CPU Frequency Configuration -------------------------*/
//  CPU Frequency
//     <1500000> 1.5 MHz
//     <3000000> 3 MHz
//     <12000000> 12 MHz
//     <24000000> 24 MHz
//     <48000000> 48 MHz
#define  SYS_CLK    48000000

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
