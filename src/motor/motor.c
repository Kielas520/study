#include "motor.h"
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

void Motor_stop(Motor_t *motor){
    __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel1, 0);
    __HAL_TIM_SET_COMPARE(motor->mtim, motor->channel2, 0);
}

/**
 * @brief  根据 Key_Value 结构体控制电机
 * @param  motor: 电机对象句柄
 * @param  key_val: 按键状态结构体指针
 */
void Motor_Key_Control(Motor_t *motor, Key_Value *key_val)
{
    // ----------------------
    // 1. 处理减速指令 (dec)
    // ----------------------
    if (key_val->dec == 1)
    {
        motor->speed -= 2;

        // 限制最小速度 (假设最大反转是 -100)
        if(motor->speed > -12 && motor->speed <= 12){
            motor->speed = -12;
        }
        if (motor->speed < -MOTOR_MAX_SPEED + 22)
            motor->speed = -MOTOR_MAX_SPEED + 22;

        // 【关键】消费掉这个指令，防止一次按键无限减速
        key_val->dec = 0;
    }

    // ----------------------
    // 2. 处理加速指令 (inc)
    // ----------------------
    else if (key_val->inc == 1)
    {
        motor->speed += 2;
        if (motor->speed < 12 && motor->speed > -12){
            motor->speed = 12;
        }
        // 限制最大速度
        if (motor->speed > MOTOR_MAX_SPEED - 22)
            motor->speed = MOTOR_MAX_SPEED - 22;

        // 【关键】消费掉这个指令
        key_val->inc = 0;
    }

    // ----------------------
    // 3. 执行启停控制 (on_off)
    // ----------------------
    if (key_val->on_off == 0)
    {
        // 状态为 0，强制停车
        Motor_stop(motor);
    }
    else // on_off == 1
    {
        // 状态为 1，运行电机

        // 防呆设计：如果刚启动且速度是0，给个默认速度
        if (motor->speed == 0) motor->speed = 0
        ;

        // 执行当前设定的速度
        Motor_run(motor, (float)motor->speed);
    }
}
