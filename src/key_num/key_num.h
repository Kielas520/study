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

// 定义消抖阈值：2 * 10ms = 20ms
// 如果觉得按键太灵敏（有误触），把这个值改大，比如 4
#define KEY_DEBOUNCE_TICKS  2

typedef struct {
    uint8_t on_off;
    uint8_t dec;
    uint8_t inc;
} Key_Value;

typedef struct {
    GPIO_TypeDef* port;     // GPIO端口
    uint16_t pin;           // GPIO引脚
    uint8_t id;             // 键值ID (1:Switch, 2:Dec, 3:Inc)
    uint8_t count;          // 消抖计数器
    uint8_t lock;           // 按键锁 (0:未锁, 1:已锁)
} Key_Config_t;

// 1. 定义按键列表 (在这里添加新按键，无需修改逻辑代码)
static Key_Config_t Key_List[] = {
    {GPIO_SWITCH_GPIO_Port, GPIO_SWITCH_Pin, 1, 0, 0}, // Switch -> ID 1
    {GPIO_DEC_GPIO_Port,    GPIO_DEC_Pin,    2, 0, 0}, // Dec    -> ID 2
    {GPIO_INC_GPIO_Port,    GPIO_INC_Pin,    3, 0, 0}, // Inc    -> ID 3
};

void Process_Key_Num(Key_Value *Key_Data, uint8_t *Key_Val);
void Key_Timer_Scan(uint8_t *Key_Val); // 供定时器调用的扫描函数

#ifdef __cplusplus
}
#endif

#endif
