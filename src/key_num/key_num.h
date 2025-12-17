#ifndef KEY_NUM_H
#define KEY_NUM_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "gpio.h"

#define GPIO_SWITCH_PORT GPIOB
#define GPIO_DEC_PORT GPIOB
#define GPIO_INC_PORT GPIOB
// 计算按键数量
#define KEY_COUNT (sizeof(Key_List) / sizeof(Key_List[0]))

// 定义消抖时间 (单位: ms)
// 建议设置在 50ms - 200ms 之间。
// 因为是中断触发，设大一点（比如200ms）可以非常有效地防止连点。
#define KEY_DEBOUNCE_TIME  200

typedef struct {
    uint8_t on_off;
    uint8_t dec;
    uint8_t inc;
} Key_Value;

typedef struct {
    GPIO_TypeDef* port;     // GPIO端口
    uint16_t pin;           // GPIO引脚
    uint8_t id;             // 键值ID
    uint32_t last_tick;     // [新增] 记录上一次有效按下的时间戳 (HAL_GetTick)
} Key_Config_t;

// [注意] 去掉了 count 和 lock 初始值，改为 last_tick 初始为 0
static Key_Config_t Key_List[] = {
    {GPIO_SWITCH_GPIO_Port, GPIO_SWITCH_Pin, 1, 0},
    {GPIO_DEC_GPIO_Port,    GPIO_DEC_Pin,    2, 0},
    {GPIO_INC_GPIO_Port,    GPIO_INC_Pin,    3, 0},
};

void Process_Key_Num(Key_Value *Key_Data, uint8_t *Key_Val);

// 修改函数原型，我们需要知道是哪个引脚触发了中断
void Key_EXTI_Scan(uint16_t GPIO_Pin, uint8_t *Key_Val);

#ifdef __cplusplus
}
#endif

#endif
