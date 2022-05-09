/*Lab 6 - Interfacing a Graphic LCD to the MSP432 MCU
 *Part 1 - Displaying Characters on the LCD from the MCU
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
 *  This part requires us to change our SysTick timer library to work properly with an MSP432
 *  running at 48MHz. Additionally, this part has a function that:
 *      -Clears the screen.
 *      -Makes the background black.
 *      -Display "EGR 326" Centered on the screen in a font size and color of our choosing.
 */

#include "msp.h"
#include "images.h"
#include "systick.h"
#include "ST7735.h"

void testRoutine(void);

extern const unsigned short Flux1[];
extern const unsigned short Flux2[];
extern const unsigned short Flux3[];
extern const unsigned short Flux4[];

void main (void){
//    int i;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    ST7735_InitR(INITR_REDTAB); //Initialize the ST7735 as Red Tab for better pixel fit.
    sysTickSetup(); //Initialize systick library with enhances Systick Library
    testRoutine();  //Call testing routine to test LCD screen

    while(1){
    }
}

void testRoutine(void){
    ST7735_FillScreen(ST7735_BLACK);    //Clear the screen by filling the screen with an all-black screen.
    ST7735_DrawString(4, 7, 2,"EGR 326", ST7735_WHITE);   //Display "EGR 326" in the middle of the screen.

}

