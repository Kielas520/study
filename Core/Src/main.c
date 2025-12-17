/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "oled.h"
#include "mpu6050.h"
#include "key_num.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint32_t oled_tick = 0; // [新增] 用来记录OLED刷新时间的静态变量
static Motor_t M0;
MPU6050_t MPU6050;
uint8_t Key_Val;
Key_Value Key_Ctrl = {0, 0, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C2_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim4);
  OLED_Init();       // 1. 初始化屏幕
  OLED_Clear();      // 2. 上电清屏（重要，防止花屏）
  while (MPU6050_Init(&hi2c1) == 1);
  Motor_init(&M0);   // 3. 电机初始化

  // --- 1. 静态UI绘制 (只画一次，防止屏幕闪烁) ---
  OLED_ShowString(1, 1, "Spd:");   // 第1行显示速度
  OLED_ShowString(2, 1, "Run:");   // 第2行显示开关状态
  OLED_ShowString(3, 1, "Dec:");   // 第3行显示减速键状态
  OLED_ShowString(3, 9, "Inc:");   // 第3行显示加速键状态

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Process_Key_Num(&Key_Ctrl, &Key_Val); // 1. 把按键转换为结构体里的 0/1 指令

    MPU6050_Read_All(&hi2c1, &MPU6050);

    // --- 2. 动态刷新区域 (显示当前状态) ---
    // 注意：必须放在 Motor_Key_Control 之前！
    // 否则 dec/inc 被执行完就变成 0 了，屏幕上看不见变化。

    // 显示速度 (3位长度 + 符号位)
    OLED_ShowSignedNum(1, 5, M0.speed, 3);

    // 显示 ON/OFF 状态
    if (Key_Ctrl.on_off == 1)
    {
        OLED_ShowString(2, 5, "ON ");
    }
    else
    {
        OLED_ShowString(2, 5, "OFF");
    }

    // 显示按键指令状态 (你会看到按下时数字短暂变 1)
    OLED_ShowNum(3, 5, Key_Ctrl.dec, 1);
    OLED_ShowNum(3, 13, Key_Ctrl.inc, 1);
    // [修改] 使用 HAL_GetTick 实现每 200ms 刷新一次 MPU6050 数据
    if (HAL_GetTick() - oled_tick >= 20)
    {
        oled_tick = HAL_GetTick(); // 更新时间戳

        // 把这行代码放在这里，它就只会在每200ms执行一次
        OLED_ShowSignedNum(4, 6, MPU6050.Gz, 8);

    }


    // --- 3. 执行电机控制 ---
    // 这一步会读取 Key_Ctrl，执行加速/减速，然后把 dec/inc 归零
    Motor_Key_Control(&M0, &Key_Ctrl);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
