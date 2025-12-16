#ifndef OLED_H
#define OLED_H
#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "gpio.h"

#define OLED_ADDR       0x78 // I2C地址
#define OLED_CMD_MODE   0x00 // 命令模式
#define OLED_DATA_MODE  0x40 // 数据模式
#define OLEDIIC &hi2c2 // I2C句柄

void OLED_Power_On();
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#ifdef __cplusplus
}
#endif

#endif
