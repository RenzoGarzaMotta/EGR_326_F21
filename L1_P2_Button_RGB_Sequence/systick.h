/*
 * systick.h
 *
 *  Created on: Sep 8, 2021
 *      Author: renzogarzamotta
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
void usDelay(uint16_t time);

#endif /* SYSTICK_H_ */
