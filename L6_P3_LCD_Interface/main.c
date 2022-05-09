/*Lab 6 - Interfacing a Graphic LCD to the MSP432 MCU
 *Part 3 - Updating characters at different places on the LCD
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
 *  This part of the laboratory activity produces the splash screen as in part II in
 *  addition to writing a function that counts from 0 to 9 with a 1sec delay in between
 *  counts which moves the cursor to different corners in the screen for each count.
 *  When the count reaches 9, the program will wait 3sec and repeat the sequence.
 */

#include "msp.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "images.h"
#include "systick.h"
#include "ST7735.h"

extern const unsigned short Flux1[];
extern const unsigned short Flux2[];
extern const unsigned short Flux3[];
extern const unsigned short Flux4[];

extern const unsigned short CR[];

extern const unsigned short ezgif_frame_001[];
extern const unsigned short ezgif_frame_002[];
extern const unsigned short ezgif_frame_003[];
extern const unsigned short ezgif_frame_004[];
extern const unsigned short ezgif_frame_005[];
extern const unsigned short ezgif_frame_006[];
extern const unsigned short ezgif_frame_007[];
extern const unsigned short ezgif_frame_008[];
extern const unsigned short ezgif_frame_009[];
extern const unsigned short ezgif_frame_010[];

extern const unsigned short ezgif_frame_011[];
extern const unsigned short ezgif_frame_012[];
extern const unsigned short ezgif_frame_013[];
extern const unsigned short ezgif_frame_014[];
extern const unsigned short ezgif_frame_015[];
extern const unsigned short ezgif_frame_016[];
extern const unsigned short ezgif_frame_017[];
extern const unsigned short ezgif_frame_018[];
extern const unsigned short ezgif_frame_019[];
extern const unsigned short ezgif_frame_020[];

extern const unsigned short ezgif_frame_021[];
extern const unsigned short ezgif_frame_022[];
extern const unsigned short ezgif_frame_023[];
extern const unsigned short ezgif_frame_024[];
extern const unsigned short ezgif_frame_025[];
extern const unsigned short ezgif_frame_026[];
extern const unsigned short ezgif_frame_027[];
extern const unsigned short ezgif_frame_028[];
extern const unsigned short ezgif_frame_029[];
extern const unsigned short ezgif_frame_030[];

extern const unsigned short ezgif_frame_031[];
extern const unsigned short ezgif_frame_032[];
extern const unsigned short ezgif_frame_033[];
extern const unsigned short ezgif_frame_034[];
extern const unsigned short ezgif_frame_035[];
extern const unsigned short ezgif_frame_036[];
extern const unsigned short ezgif_frame_037[];
extern const unsigned short ezgif_frame_038[];
extern const unsigned short ezgif_frame_039[];
extern const unsigned short ezgif_frame_040[];

extern const unsigned short ezgif_frame_041[];
extern const unsigned short ezgif_frame_042[];
extern const unsigned short ezgif_frame_043[];
extern const unsigned short ezgif_frame_044[];
extern const unsigned short ezgif_frame_045[];
extern const unsigned short ezgif_frame_046[];
extern const unsigned short ezgif_frame_047[];
extern const unsigned short ezgif_frame_048[];
extern const unsigned short ezgif_frame_049[];
extern const unsigned short ezgif_frame_050[];

extern const unsigned short ezgif_frame_051[];
extern const unsigned short ezgif_frame_052[];
extern const unsigned short ezgif_frame_053[];
extern const unsigned short ezgif_frame_054[];
extern const unsigned short ezgif_frame_055[];
extern const unsigned short ezgif_frame_056[];
extern const unsigned short ezgif_frame_057[];
extern const unsigned short ezgif_frame_058[];
extern const unsigned short ezgif_frame_059[];
extern const unsigned short ezgif_frame_060[];

extern const unsigned short ezgif_frame_061[];
extern const unsigned short ezgif_frame_062[];
extern const unsigned short ezgif_frame_063[];
extern const unsigned short ezgif_frame_064[];
extern const unsigned short ezgif_frame_065[];
extern const unsigned short ezgif_frame_066[];
extern const unsigned short ezgif_frame_067[];
extern const unsigned short ezgif_frame_068[];
extern const unsigned short ezgif_frame_069[];
extern const unsigned short ezgif_frame_070[];

extern const unsigned short ezgif_frame_071[];
extern const unsigned short ezgif_frame_072[];
extern const unsigned short ezgif_frame_073[];
extern const unsigned short ezgif_frame_074[];
extern const unsigned short ezgif_frame_075[];
extern const unsigned short ezgif_frame_076[];
extern const unsigned short ezgif_frame_077[];
extern const unsigned short ezgif_frame_078[];
extern const unsigned short ezgif_frame_079[];
extern const unsigned short ezgif_frame_080[];

extern const unsigned short ezgif_frame_081[];
extern const unsigned short ezgif_frame_082[];
extern const unsigned short ezgif_frame_083[];
extern const unsigned short ezgif_frame_084[];
extern const unsigned short ezgif_frame_085[];
extern const unsigned short ezgif_frame_086[];
extern const unsigned short ezgif_frame_087[];
extern const unsigned short ezgif_frame_088[];
extern const unsigned short ezgif_frame_089[];
extern const unsigned short ezgif_frame_090[];

extern const unsigned short ezgif_frame_091[];
extern const unsigned short ezgif_frame_092[];
extern const unsigned short ezgif_frame_093[];
extern const unsigned short ezgif_frame_094[];
extern const unsigned short ezgif_frame_095[];
extern const unsigned short ezgif_frame_096[];
extern const unsigned short ezgif_frame_097[];
extern const unsigned short ezgif_frame_098[];
extern const unsigned short ezgif_frame_099[];
extern const unsigned short ezgif_frame_100[];

extern const unsigned short ezgif_frame_101[];
extern const unsigned short ezgif_frame_102[];
extern const unsigned short ezgif_frame_103[];
extern const unsigned short ezgif_frame_104[];
extern const unsigned short ezgif_frame_105[];
extern const unsigned short ezgif_frame_106[];
extern const unsigned short ezgif_frame_107[];
extern const unsigned short ezgif_frame_108[];
extern const unsigned short ezgif_frame_109[];
extern const unsigned short ezgif_frame_120[];

extern const unsigned short ezgif_frame_121[];
extern const unsigned short ezgif_frame_122[];
extern const unsigned short ezgif_frame_123[];
extern const unsigned short ezgif_frame_124[];
extern const unsigned short ezgif_frame_125[];
extern const unsigned short ezgif_frame_126[];
extern const unsigned short ezgif_frame_127[];
extern const unsigned short ezgif_frame_128[];
extern const unsigned short ezgif_frame_129[];
extern const unsigned short ezgif_frame_130[];

extern const unsigned short ezgif_frame_131[];
extern const unsigned short ezgif_frame_132[];
extern const unsigned short ezgif_frame_133[];
extern const unsigned short ezgif_frame_134[];
extern const unsigned short ezgif_frame_135[];



void main (void){
    int j = 0;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    ST7735_InitR(INITR_REDTAB); //Initialize the ST7735 as Red Tab for better pixel fit.
    sysTickSetup(); //Initialize systick library with enhances Systick Library

    do{
        //Puzzle Piece Gif
        //Can't animate the entire gif at full resolution due to memory issues, but images can be accessed in
        //SirWrenderGif Folder.
        ST7735_DrawBitmap(21, 160, ezgif_frame_001, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_004, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_007, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_010, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_013, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_016, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_019, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_022, 86, 153);
        msDelay(350);
        ST7735_DrawBitmap(21, 160, ezgif_frame_025, 86, 153);
        msDelay(350);
        j++;
    }while(j  < 50);


//    //Animate Flux Capacitor GIF -- Uncomment to enable
//    for(j = 0; j < 10; j++){
//        for(i = 0; i < 4; i++){
//            switch(i){
//                case 1:
//                    ST7735_DrawBitmap(10, 160, Flux1, 107, 160);
//                    break;
//                case 2:
//                    ST7735_DrawBitmap(10, 160, Flux2, 107, 160);
//                    break;
//                case 3:
//                    ST7735_DrawBitmap(10, 160, Flux3, 107, 160);
//                    break;
//                case 4:
//                    ST7735_DrawBitmap(10, 160, Flux4, 107, 160);
//                    break;
//            };
//            msDelay(350);
//        }
//    }

//    ST7735_FillScreen(ST7735_BLACK);    //Clear the screen by filling the screen with an all-black screen.

    while(1){
//        for(x = 0; x < 20; x += 19){    //Cycle through x-axis corners in the LCD screen
//            for(y = 0; y < 15; y += 14){    //Cycle through y-axis corners in the LCD screen
//                for(i = 0; i <= count; i++){    //Keep track of the second count
//                    sprintf(str, "%d",i);       //Store count into a string for print to screen
//                    ST7735_FillScreen(ST7735_BLACK);    //Clear the screen by filling the screen with an all-black screen.
//                    ST7735_DrawString(x, y, 2, str, ST7735_WHITE);  //Print count variable at determined (x,y) location
//                    msDelay(1000);
//                }
//            }
//        }
//        for(i = 0; i <= count; i++){    //Keep track of the second count
//            sprintf(str, "%d",i);       //Store count into a string for print to screen
//            ST7735_FillScreen(ST7735_BLACK);    //Clear the screen by filling the screen with an all-black screen.
//            ST7735_DrawString(10, 7, 2, str, ST7735_WHITE);  //Print count variable at determined (x,y) location
//            msDelay(1000);
//        }
//        ST7735_FillScreen(ST7735_BLACK);    //Clear the screen by filling the screen with an all-black screen.
//        ST7735_DrawBitmap(5, 160, CR, 117, 160);
//        msDelay(5000);  //Only use 2s delay because a 1s delay is already being use before it. This is done to achieve a 3s delay
    }
}
