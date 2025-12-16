#ifndef OLED_H
#define OLED_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "i2c.h"
#include "gpio.h"

#define OLED_ADDR       0x78 // I2C地址
#define OLED_CMD_MODE   0x00 // 命令模式
#define OLED_DATA_MODE  0x40 // 数据模式

// 这是一个字模数组的片段
// 格式：const unsigned char F8X16[]
const unsigned char F8X16[] = {
    // ... 前面是其他字符 ...

    // --- 这里是字符 'A' 的数据 (索引位置通常是 'A' - ' ' = 33) ---

    // 上半身 (Page 0 的 8 列)
    0x00, 0xE0, 0x1C, 0x02, 0x02, 0x1C, 0xE0, 0x00,

    // 下半身 (Page 1 的 8 列)
    0x00, 0x0F, 0x08, 0x08, 0x08, 0x08, 0x0F, 0x00,

    // ... 后面是字符 'B' ...
};

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Full(void);

#ifdef __cplusplus
}
#endif

#endif
