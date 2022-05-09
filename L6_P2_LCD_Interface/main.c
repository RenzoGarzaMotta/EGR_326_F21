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

#include "msp.h"
#include "images.h"
#include "systick.h"
#include "ST7735.h"

extern const unsigned short Flux1[];
extern const unsigned short Flux2[];
extern const unsigned short Flux3[];
extern const unsigned short Flux4[];

void main (void){
    int i, j;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    ST7735_InitR(INITR_REDTAB); //Initialize the ST7735 as Red Tab for better pixel fit.
    sysTickSetup(); //Initialize systick library with enhances Systick Library


    //Animate Flux Capacitor GIF -- Uncomment to enable
    for(j = 0; j < 10; j++){
        for(i = 0; i < 4; i++){
            switch(i){
                case 1:
                    ST7735_DrawBitmap(10, 160, Flux1, 107, 160);
                    break;
                case 2:
                    ST7735_DrawBitmap(10, 160, Flux2, 107, 160);
                    break;
                case 3:
                    ST7735_DrawBitmap(10, 160, Flux3, 107, 160);
                    break;
                case 4:
                    ST7735_DrawBitmap(10, 160, Flux4, 107, 160);
                    break;
            };
            msDelay(50);
        }
    }

    ST7735_FillScreen(ST7735_BLACK);    //Clear the screen by filling the screen with an all-black screen.
    ST7735_DrawString(6, 1, 2,"Menu", ST7735_WHITE);   //Display "Item #1" in the menu
    ST7735_DrawString(2, 4, 2,"*Item #1", ST7735_WHITE);   //Display "Item #1" in the menu
    ST7735_DrawString(2, 6, 2,"*Item #2", ST7735_WHITE);   //Display "Item #2" in the menu
    ST7735_DrawString(2, 8, 2,"*Item #3", ST7735_WHITE);   //Display "Item #3" in the menu
    ST7735_DrawString(2, 10, 2,"*Item #4", ST7735_WHITE);   //Display "Item #4" in the menu

    while(1){

    }
}


