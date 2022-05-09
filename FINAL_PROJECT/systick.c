#include "msp.h"
#include <math.h>
#include "systick.h"

void sysTickSetup(void){
    //Program the value in the STRVR register
    //Clear the STCVR register by writing to it with any value
    //Configure the STCSR register for the required operation.
    SysTick->CTRL = 0;              //
    SysTick->LOAD = SYS_MAX;        //Load the timer to its max of 0x00FFFFFF
    SysTick->VAL = 0;               //
    SysTick->CTRL = SYS_SETUP;      //Enable SysTick without an interrupt
}

void intSysTickSetup(void){
    //Program the value in the STRVR register
    //Clear the STCVR register by writing to it with any value
    //Configure the STCSR register for the required operation.
    int freqScalar;
    freqScalar = (SYS_CLK / 1000);
    SysTick->CTRL = 0;                       //Disable SysTick for setup
    SysTick->LOAD = (freqScalar * 1000) - 1;        //Load the timer to 500 to account for the 2Hz signal required for this lab
    SysTick->VAL = 0;                        //Any write clears the register
    SysTick->CTRL = SYS_SETUP_INT;           //Enable SysTick with an interrupt
}

void msDelay(uint16_t time){    //Function uses SysTick as a delay
    unsigned int freqScalar, sysLoad;
    unsigned int count = 0, count1 = 0, i = 0, j = 0;
    unsigned int lim;
    if(SYS_CLK == 1500000){//     <1500000> 1.5 MHz
        freqScalar = (SYS_CLK / 1000);
        SysTick->LOAD = (freqScalar * time) - 1;  //Load value to SysTick
        SysTick->VAL = 0;   //Clear SysTick value register
        while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
    }else if(SYS_CLK == 3000000){//     <3000000> 3 MHz
        freqScalar = (SYS_CLK / 1000);
        SysTick->LOAD = (freqScalar * time) - 1;  //Load value to SysTick
        SysTick->VAL = 0;   //Clear SysTick value register
        while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
    }else if(SYS_CLK == 12000000){//     <12000000> 12 MHz
        freqScalar = (SYS_CLK / 1000);
        SysTick->LOAD = (freqScalar * time) - 1;  //Load value to SysTick
        SysTick->VAL = 0;   //Clear SysTick value register
        while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
    }else if(SYS_CLK == 24000000){//     <24000000> 24 MHz
        freqScalar = (SYS_CLK / 1000);
        sysLoad = (freqScalar * time);
        lim = (sysLoad / 1000000) % 100;

        if(lim > 16){
            for(i = 0; i < (sysLoad/1000000); i++){
                count = (1 * 1000000);
                SysTick->LOAD = count;  //Load value to SysTick
                SysTick->VAL = 0;   //Clear SysTick value register
                while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
            }
        }

        count = 0;

        if(lim > 16){
            for(j = 100000; (count1 + ((sysLoad / 1000000) * 1000000)) != sysLoad ;j = j/10){
                count = 0;
                if((sysLoad / j) % 10){
                    lim = (sysLoad / j) % 10;
                    for(i = 0; i < lim; i++){
                        count = count + (1 * j);
                    }
                    count1 += count;
//                    printf("%d\n", count);
                }
                if(count > 0){
                    SysTick->LOAD = count;  //Load value to SysTick
                    SysTick->VAL = 0;   //Clear SysTick value register
                    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
                }
            }
        }
    }else if(SYS_CLK == 48000000){//     <48000000> 48 MHz
        freqScalar = (SYS_CLK / 1000);
        sysLoad = (freqScalar * time);
        lim = (sysLoad / 1000000) % 100;

        if(lim > 16){
            for(i = 0; i < (sysLoad/1000000); i++){
                count = (1 * 1000000);
                SysTick->LOAD = count;  //Load value to SysTick
                SysTick->VAL = 0;   //Clear SysTick value register
                while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
            }
        }

        count = 0;

        if(lim > 16){
            for(j = 100000; (count1 + ((sysLoad / 1000000) * 1000000)) != sysLoad ;j = j/10){
                count = 0;
                if((sysLoad / j) % 10){
                    lim = (sysLoad / j) % 10;
                    for(i = 0; i < lim; i++){
                        count = count + (1 * j);
                    }
                    count1 += count;
//                    printf("%d\n", count);
                }
                if(count > 0){
                    SysTick->LOAD = count;  //Load value to SysTick
                    SysTick->VAL = 0;   //Clear SysTick value register
                    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
                }
            }
        }
    }
}

void usDelay(uint32_t time){    //Function uses SysTick as a delay
    unsigned int freqScalar, sysLoad;
    unsigned int count = 0, count1 = 0, i = 0, j = 0;
    unsigned int lim;
    if(SYS_CLK == 1500000){//     <1500000> 1.5 MHz
        freqScalar = (SYS_CLK / 1000000);
        SysTick->LOAD = (freqScalar * time) - 1;  //Load value to SysTick
        SysTick->VAL = 0;   //Clear SysTick value register
        while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
    }else if(SYS_CLK == 3000000){//     <3000000> 3 MHz
        freqScalar = (SYS_CLK / 1000000);
        SysTick->LOAD = (freqScalar * time) - 1;  //Load value to SysTick
        SysTick->VAL = 0;   //Clear SysTick value register
        while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
    }else if(SYS_CLK == 12000000){//     <12000000> 12 MHz
        freqScalar = (SYS_CLK / 1000000);
        SysTick->LOAD = (freqScalar * time) - 1;  //Load value to SysTick
        SysTick->VAL = 0;   //Clear SysTick value register
        while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
    }else if(SYS_CLK == 24000000){//     <24000000> 24 MHz
        freqScalar = (SYS_CLK / 1000000);
        sysLoad = (freqScalar * time);
        lim = (sysLoad / 1000000) % 100;

        if(lim > 16){
            for(i = 0; i < (sysLoad/1000000); i++){
                count = (1 * 1000000);
                SysTick->LOAD = count;  //Load value to SysTick
                SysTick->VAL = 0;   //Clear SysTick value register
                while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
            }
        }

        count = 0;

        if(lim > 16){
            for(j = 100000; (count1 + ((sysLoad / 1000000) * 1000000)) != sysLoad ;j = j/10){
                count = 0;
                if((sysLoad / j) % 10){
                    lim = (sysLoad / j) % 10;
                    for(i = 0; i < lim; i++){
                        count = count + (1 * j);
                    }
                    count1 += count;
//                    printf("%d\n", count);
                }
                if(count > 0){
                    SysTick->LOAD = count;  //Load value to SysTick
                    SysTick->VAL = 0;   //Clear SysTick value register
                    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
                }
            }
        }
    }else if(SYS_CLK == 48000000){//     <48000000> 48 MHz
        freqScalar = (SYS_CLK / 1000000);
        sysLoad = (freqScalar * time);
        lim = (sysLoad / 1000000) % 100;

        if(lim > 16){
            for(i = 0; i < (sysLoad/1000000); i++){
                count = (1 * 1000000);
                SysTick->LOAD = count;  //Load value to SysTick
                SysTick->VAL = 0;   //Clear SysTick value register
                while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
            }
        }

        count = 0;

        if(lim > 16){
            for(j = 100000; (count1 + ((sysLoad / 1000000) * 1000000)) != sysLoad ;j = j/10){
                count = 0;
                if((sysLoad / j) % 10){
                    lim = (sysLoad / j) % 10;
                    for(i = 0; i < lim; i++){
                        count = count + (1 * j);
                    }
                    count1 += count;
//                    printf("%d\n", count);
                }
                if(count > 0){
                    SysTick->LOAD = count;  //Load value to SysTick
                    SysTick->VAL = 0;   //Clear SysTick value register
                    while((SysTick->CTRL & SYS_COUNT_FG) == 0); //Wait until SysTick timer is done counting
                }
            }
        }
    }
}
