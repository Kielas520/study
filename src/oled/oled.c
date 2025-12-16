#include "oled.h"

// 写命令函数
void OLED_Write_Cmd(uint8_t cmd) {
    // 使用 HAL_I2C_Mem_Write 可以简化寄存器操作
    // 参数：I2C句柄, 设备地址, 寄存器地址(0x00代表命令), 寄存器地址长度, 数据指针, 数据长度, 超时
    HAL_I2C_Mem_Write(&hi2c2, OLED_ADDR, OLED_CMD_MODE, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
}

// 写数据函数
void OLED_Write_Data(uint8_t data) {
    // 寄存器地址 0x40 代表数据
    HAL_I2C_Mem_Write(&hi2c2, OLED_ADDR, OLED_DATA_MODE, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

// 清屏函数：将所有显存写0
void OLED_Clear(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLED_Write_Cmd(0xB0 + i); // 设置页地址 (Page 0~7)
        OLED_Write_Cmd(0x00);     // 设置列地址低4位
        OLED_Write_Cmd(0x10);     // 设置列地址高4位

        for (n = 0; n < 128; n++) {
            OLED_Write_Data(0x00); // 写0清空当前列
        }
    }
}

// 开启全屏显示（测试坏点用）
void OLED_Full(void) {
    uint8_t i, n;
    for (i = 0; i < 8; i++) {
        OLED_Write_Cmd(0xB0 + i);
        OLED_Write_Cmd(0x00);
        OLED_Write_Cmd(0x10);
        for (n = 0; n < 128; n++) {
            OLED_Write_Data(0xFF); // 写1点亮
        }
    }
}

void OLED_Init(void) {
    HAL_Delay(100); // 上电延时，等待屏幕内部电路稳定

    OLED_Write_Cmd(0xAE); // 关闭显示 (Display OFF)

    OLED_Write_Cmd(0x20); // 设置内存寻址模式
    OLED_Write_Cmd(0x02); // 00:水平, 01:垂直, 02:页寻址(常用)

    OLED_Write_Cmd(0xB0); // 设置页起始地址 (Page 0)
    OLED_Write_Cmd(0xC8); // 设置COM扫描方向 (C8:从下到上, C0:从上到下 - 屏幕翻转用)

    OLED_Write_Cmd(0x00); // 设置低列地址
    OLED_Write_Cmd(0x10); // 设置高列地址

    OLED_Write_Cmd(0x40); // 设置起始行地址

    OLED_Write_Cmd(0x81); // 对比度设置命令
    OLED_Write_Cmd(0xFF); // 亮度值 (0x00-0xFF)

    OLED_Write_Cmd(0xA1); // 设置段重映射 (A1:左右反置, A0:正常)
    OLED_Write_Cmd(0xA6); // 常规显示 (A6:正常, A7:反色)

    OLED_Write_Cmd(0xA8); // 设置多路复用率
    OLED_Write_Cmd(0x3F); // 1/64 duty

    OLED_Write_Cmd(0xD3); // 设置显示偏移
    OLED_Write_Cmd(0x00); // 无偏移

    OLED_Write_Cmd(0xD5); // 设置显示时钟分频/振荡器频率
    OLED_Write_Cmd(0x80);

    OLED_Write_Cmd(0xD9); // 设置预充电周期
    OLED_Write_Cmd(0xF1);

    OLED_Write_Cmd(0xDA); // 设置COM硬件引脚配置
    OLED_Write_Cmd(0x12);

    OLED_Write_Cmd(0xDB); // 设置VCOMH反压值
    OLED_Write_Cmd(0x40);

    OLED_Write_Cmd(0x8D); // 电荷泵设置 (必须开启才能点亮)
    OLED_Write_Cmd(0x14); // 开启电荷泵

    OLED_Write_Cmd(0xAF); // 开启显示 (Display ON)

    OLED_Clear(); // 初始化后清屏，防止花屏
}

// 辅助函数：定位光标
void OLED_Set_Pos(uint8_t x, uint8_t y) {
    OLED_Write_Cmd(0xB0 + y);
    OLED_Write_Cmd(((x & 0xF0) >> 4) | 0x10);
    OLED_Write_Cmd(x & 0x0F);
}

// 在指定位置显示一个字符
// x: 列坐标 (0~127), y: 页坐标 (0~7)
void OLED_ShowChar(uint8_t x, uint8_t y, char chr) {
    uint8_t c = chr - ' '; // 得到偏移值（假设字库从空格开始）
    if (x > 120) { x = 0; y++; }

    // 写上半部分 (8个像素)
    OLED_Set_Pos(x, y);
    for (uint8_t i = 0; i < 8; i++)
        OLED_Write_Data(F8X16[c * 16 + i]);

    // 写下半部分 (8个像素)
    OLED_Set_Pos(x, y + 1);
    for (uint8_t i = 0; i < 8; i++)
        OLED_Write_Data(F8X16[c * 16 + i + 8]);
}

