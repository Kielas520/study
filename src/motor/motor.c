#include "motor.h"
#include "stm32f1xx_hal_tim.h"
#include <stdint.h>
void Motor_init(Motor_t *motor)
{
    motor->mtim = &htim2;
    motor->channel1 = TIM_CHANNEL_1;
    motor->channel2 = TIM_CHANNEL_2;
    motor->in1 = MOTOR_IN1_Pin;
    motor->in2 = MOTOR_IN2_Pin;
    motor->GPIOINx = MOTOR_IN1_GPIO_Port;
    HAL_TIM_PWM_Start(motor->mtim, motor->channel1);
    HAL_TIM_PWM_Start(motor->mtim, motor->channel2);
}

uint16_t map_speed(Motor_t *motor, float speed){ // 330RPM = 100%
    if (speed > 0){
        motor->dir = 1;
        if (speed > MOTOR_MAX_SPEED)
            speed = MOTOR_MAX_SPEED;
    }
    else if (speed <= 0){
        motor->dir = 0;
        speed = -speed;
        if (speed < -MOTOR_MAX_SPEED)
            speed = -MOTOR_MAX_SPEED;
    }

    return (uint16_t)((speed / MOTOR_MAX_SPEED) * CMP_MAX_VAL);
}

void Motor_run(Motor_t *motor, float speed){
    uint16_t cmp = map_speed(motor, speed);

    if (motor->dir == 1){
        __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel1, cmp);
        __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel2, 0);
        // HAL_GPIO_WritePin(motor->GPIOINx, motor->in1, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(motor->GPIOINx, motor->in2, GPIO_PIN_RESET);
    }
    else if(motor->dir == 0){
        __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel1, 0);
        __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel2, cmp);
        // HAL_GPIO_WritePin(motor->GPIOINx, motor->in1, GPIO_PIN_RESET);
        // HAL_GPIO_WritePin(motor->GPIOINx, motor->in2, GPIO_PIN_SET);
    }
    else {
        __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel1, 0);
        __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel2, 0);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

}
