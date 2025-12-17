#include "key_num.h"

void Process_Key_Num(Key_Value *Key_Data, uint8_t *Key_Val) // 建议改名 Key_Data 避免变量名冲突
{
    if (*Key_Val != 0) // 检测到按键按下
    {
        switch (*Key_Val)
        {
        case 1: // Switch
            // 简洁写法：直接取反，实现 0/1 切换
            Key_Data->on_off = !Key_Data->on_off;
            break;

        case 2: // Dec
            // 这里之前的逻辑好像也是开关？通常 Dec 是减法
            // 假设你要做 0/1 切换：
            Key_Data->dec = !Key_Data->dec;
            break;

        case 3: // Inc
             Key_Data->inc = !Key_Data->inc;
            break;
        }

        // !!! 极其重要 !!!
        // 处理完必须清零，否则下一次循环进来 if (Key_Val != 0) 依然成立
        // 导致按一下变成了按无数下
        *Key_Val = 0;
    }
}

/**
 * @brief  EXTI中断专用的按键处理函数
 * @param  GPIO_Pin: 触发中断的引脚号 (从中断回调函数传入)
 * @param  Key_Val:  键值指针
 */
void Key_EXTI_Scan(uint16_t GPIO_Pin, uint8_t *Key_Val)
{
    // 遍历列表，找到是哪个按键对应的引脚触发了中断
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        Key_Config_t *pKey = &Key_List[i];

        // 1. 匹配引脚：只处理当前触发中断的那个按键
        if (pKey->pin == GPIO_Pin)
        {
            // 2. 再次确认电平（可选，增强稳定性）：
            // 确保当前确实是低电平（按下状态），防止噪声引起的误触发
            if (HAL_GPIO_ReadPin(pKey->port, pKey->pin) == GPIO_PIN_RESET)
            {
                // 3. 核心逻辑：时间差消抖
                // 获取当前系统滴答时间
                uint32_t current_tick = HAL_GetTick();

                // 如果 (当前时间 - 上次时间) > 消抖阈值，说明是一次新的有效按下
                // 注意：这里利用了无符号整数溢出回绕特性，即使 tick 溢出也能正常工作
                if ((current_tick - pKey->last_tick) > KEY_DEBOUNCE_TIME)
                {
                    *Key_Val = pKey->id;      // 输出键值
                    pKey->last_tick = current_tick; // 更新最后一次触发时间
                }
            }
            // 找到对应的按键后，直接退出循环，提高效率
            break;
        }
    }
}


