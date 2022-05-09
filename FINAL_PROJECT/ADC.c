#include "msp.h"
#include "ADC.h"

void setupADC(void){
    PHOTORES_PORT->SEL1 |= PHOTORES;             /* Configure P5.5 for A0 */
    PHOTORES_PORT->SEL0 |= PHOTORES;


    ADC14->CTL0 =  ADC14_CTL0_ON;    /* power on and disabled during configuration */

    ADC14->CTL0 |= (ADC14_CTL0_SHT0__32 | ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHP);

    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;

    ADC14->MCTL[0] = ADC14_MCTLN_INCH_0;

    ADC14->CTL1 |= (ADC14_CTL1_CSTARTADD_MASK < ADC14_MCTLN_INCH_0);

    ADC14->CTL0 |= ADC14_CTL0_ENC;             /* enable ADC after configuration*/
}


void readAmbientLighting(void){
    ADC14->CTL0 |= ADC14_CTL0_SC;         /* start a conversion */
    while (!ADC14->IFGR0);    /* wait till conversion complete */
    ambientLight = ADC14->MEM[0];   /* read conversion result */
}
