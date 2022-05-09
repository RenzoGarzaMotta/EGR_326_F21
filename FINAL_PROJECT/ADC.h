#ifndef ADC_H_
#define ADC_H_

#define PHOTORES_PORT   P5
#define PHOTORES        BIT5

void setupADC(void);
void readAmbientLighting(void);

int ambientLight;

#endif /* ADC_H_ */
