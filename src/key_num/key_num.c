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
 * @brief  通用的按键扫描核心逻辑 (放在定时器中断里调用)
 */
void Key_Timer_Scan(uint8_t *Key_Val)
{
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        // 获取当前按键的指针，方便操作
        Key_Config_t *pKey = &Key_List[i];

        // --- 核心扫描逻辑 ---

        // 1. 如果检测到低电平 (按下)
        if (HAL_GPIO_ReadPin(pKey->port, pKey->pin) == GPIO_PIN_RESET)
        {
            // 计数器未达标，继续累加
            if (pKey->count < KEY_DEBOUNCE_TICKS)
            {
                pKey->count++;
            }

            // 计数器达标 (说明按下时间足够长，消抖通过)
            if (pKey->count >= KEY_DEBOUNCE_TICKS)
            {
                if (pKey->lock == 0) // 如果没上锁
                {
                    *Key_Val = pKey->id; // 触发按键事件
                    pKey->lock = 1;     // 上锁，防止连发
                }
            }
        }
        else // 2. 如果检测到高电平 (松开)
        {
            pKey->count = 0; // 清空计数器
            pKey->lock = 0;  // 解锁，允许下一次触发
        }
    }
}


