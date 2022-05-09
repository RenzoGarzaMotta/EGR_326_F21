#ifndef TIMERA_H_
#define TIMERA_H_

#define LED_PORT        P5              //Port used for the LEDs
#define BRIGHTNESS_LED  BIT6

void setupBrightnessControl(void);
void brightnessControl(void);
void setupHallEffect(void);

#endif /* TIMERA_H_ */
