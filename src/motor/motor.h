#ifndef MOTOR_H
#define MOTOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include"main.h"
#include "tim.h"
#include "gpio.h"
#include "key_num.h"

#define MOTOR_MAX_SPEED 50.0
#define CMP_MAX_VAL htim2.Init.Period

typedef struct {
  TIM_HandleTypeDef *mtim;
  uint16_t channel1;
  uint16_t channel2;
  uint8_t dir;
  GPIO_TypeDef *GPIOINx;
  uint16_t in1;
  uint16_t in2;
  int speed;
}Motor_t;

void Motor_init(Motor_t *motor);
void Motor_run(Motor_t *motor, float speed);
void Motor_Key_Control(Motor_t *motor, Key_Value *key_val);

#ifdef __cplusplus
}
#endif

#endif
