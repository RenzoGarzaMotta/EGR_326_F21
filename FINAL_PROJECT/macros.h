 /*
 * macros.h
 *
 *  Created on: Sep 8, 2021
 *      Author: renzogarzamotta
 */

#ifndef MACROS_H_
#define MACROS_H_

//#define US_PORT         P2
//#define TRIGGER         BIT4
//#define ECHO            BIT5
//#define US_SENSOR       (TRIGGER|ECHO)


//Rotary Encoder
#define ENCODER_PORT    P2
#define CLK             BIT6    //0x40
#define DT              BIT5    //0x20
#define SW              BIT4    //0x10
#define ENCODER         (CLK|DT|SW)

//Buttons Definitions
#define BTN_PORT    P1
#define BTN1        BIT1
#define BTN2        BIT4
#define BTNS        (BTN1|BTN2)

//I2C Definitions
#define I2C_PORT    P1
#define I2C_SCL     BIT7
#define I2C_SDA     BIT6
#define I2C         (I2C_SCL|I2C_SDA)

//Hall Effect Definitions
#define HALL_PORT   P8      //Define Hall Effect Sensor Port
#define HALL        BIT2    //Define Hall Effect Sensor Pin

//Constant definition
#define PI          3.14159265359

//LED signals Definitions
#define TURNSIG_PORT    P6
#define TURNSIG         BIT6
#define PROXBLINK_PORT  P6
#define PROXBLINK       BIT7

#endif /* MACROS_H_ */
