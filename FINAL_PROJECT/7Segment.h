/*
 * 7Segment.h
 *
 *  Created on: Oct 24, 2021
 *      Author: renzogarzamotta
 */

/*
 *
 *         __A__
 *       /      /
 *    F /      / B
 *     /__G__ /
 *    /      /
 * E /      / C
 *  /_____ /
 *     D
 *
 *  CLK -
 *  MISO -
 *  MOSI -
 *
 *NO PULLUP RESISTOR NEEDED
 */

#define SEG_PORT           P2
#define SEG_CLK            BIT1
#define SEG_CS             BIT2
#define SEG_MOSI           BIT3
#define SEG_SPI            (SEG_MOSI|SEG_CLK)
#define UNIT_COUNT         1

//Addresses Map
#define NO_OP_ADD           0x00
#define DIG_0_ADD           0x01
#define DIG_1_ADD           0x02
#define DIG_2_ADD           0x03
#define DIG_3_ADD           0x04
#define DIG_4_ADD           0x05
#define DIG_5_ADD           0x06
#define DIG_6_ADD           0x07
#define DIG_7_ADD           0x08
#define DECODE_MODE_ADD     0x09
#define INTENSITY_ADD       0x0A
#define SCAN_LIMIT_ADD      0x0B
#define SHUTDOWN_ADD        0x0C
#define DISPLAY_TEST_ADD    0x0F

//Shutdown Register Format  (Address = 0x0C)
#define SHUTDOWN_MODE       0x00
#define NORMAL_OPERATION    0x01

//Decode-Mode Register Examples  (Address = 0x09)
#define NO_DECODE           0x00
#define DECODE_DIG_0        0x01
#define DECODE_DIG_1        0x03
#define DECODE_DIG_2        0x07
#define DECODE_DIG_3        0x0F
#define DECODE_DIG_4        0x1F
#define DECODE_DIG_5        0x3F
#define DECODE_DIG_6        0x7F
#define DECODE_DIG_7        0xFF

//Code B Font
#define CHAR_0              0x00
#define CHAR_1              0x01
#define CHAR_2              0x02
#define CHAR_3              0x03
#define CHAR_4              0x04
#define CHAR_5              0x05
#define CHAR_6              0x06
#define CHAR_7              0x07
#define CHAR_8              0x08
#define CHAR_9              0x09
#define CHAR_DASH           0x0A
#define CHAR_E              0x0B
#define CHAR_H              0x0C
#define CHAR_L              0x0D
#define CHAR_P              0x0E
#define BLANK               0x0F
#define DECIMAL             0x80

//No Decode Data bits
#define SEG_A               0x01
#define SEG_B               0x02
#define SEG_C               0x04
#define SEG_D               0x08
#define SEG_E               0x10
#define SEG_F               0x20
#define SEG_G               0x40
#define SEG_DP              0x80

//Intensity Register Format  (Address = 0x0A)
#define INT_1_32            0x00
#define INT_3_32            0x01
#define INT_5_32            0x02
#define INT_7_32            0x03
#define INT_9_32            0x04
#define INT_11_32           0x05
#define INT_13_32           0x06
#define INT_15_32           0x07
#define INT_17_32           0x08
#define INT_19_32           0x09
#define INT_21_32           0x0A
#define INT_23_32           0x0B
#define INT_25_32           0x0C
#define INT_27_32           0x0D
#define INT_29_32           0x0E
#define INT_31_32           0x0F

//Scan-Limit Register Format  (Address = 0x0B)
#define SCAN_0              0x00
#define SCAN_0_1            0x01
#define SCAN_0_2            0x02
#define SCAN_0_3            0x03
#define SCAN_0_4            0x04
#define SCAN_0_5            0x05
#define SCAN_0_6            0x06
#define SCAN_0_7            0x07

//Display-Test Register Format  (Address = 0x0F)
#define NORMAL_OP_MODE      0x00
#define DISP_TEST_MODE      0x01

void segmentInit(void);
void writeSPI(uint8_t address, uint8_t data);
void write(uint8_t unit, uint8_t address, uint8_t data);
void clearDisplay(uint8_t unit);
void encodeDigits(uint8_t unit, uint32_t num);
void clearAll(void);
void sendAddSPI(uint8_t address);
void sendDataSPI(uint8_t data);

//#endif /* 7SEGMENT_H_ */
