#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

#define US_PORT         P2
#define ECHO_PORT       P10
#define TRIGGER         BIT0
#define ECHO            BIT5
#define US_SENSOR       (TRIGGER|ECHO)

void gpioUltrasonicSetup(void);
void setTrigger(void);
void sampleDistance(void);
void setupUltrasonicSensor(void);

#endif /* ULTRASONICSENSOR_H_ */
