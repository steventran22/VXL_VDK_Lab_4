/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_DEBUG_TIMER		0
#define FIRST_DEL_TIMER		1
#define SECOND_DEL_TIMER	2
#define FIRST_ADD_TIMER		3
#define SECOND_ADD_TIMER	4

#define LED_DEBUG_DUR		1000
#define FIRST_DEL_DUR		10000
#define SECOND_DEL_DUR		20000
#define FIRST_ADD_DUR		30000
#define SECOND_ADD_DUR		40000


//#define tc1
//#define tc2
//#define tc3
#define tc4
//#define tc5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_IWDG_Init(void);
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
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
  display7SEGError(DISABLE_7SEG);

  SCH_Init();
  timerSet(LED_DEBUG_TIMER, LED_DEBUG_DUR);

#ifdef tc1
  uint32_t id1 = SCH_Add_Task(task1, 0, 1000);
  uint32_t id2 = SCH_Add_Task(task2, 0, 3000);
  uint32_t id3 = SCH_Add_Task(task3, 0, 5000);

#endif

#ifdef tc2
  uint32_t id1 = SCH_Add_Task(task1, 1000, 1000);
  uint32_t id2 = SCH_Add_Task(task2, 6000, 6000);
  uint32_t id3 = SCH_Add_Task(task3, 2000, 2000);
  uint32_t id4 = SCH_Add_Task(task4, 3000, 3000);
  uint32_t id5 = SCH_Add_Task(task5, 8000, 8000);

  timerSet(FIRST_DEL_TIMER, FIRST_DEL_DUR);
  timerSet(SECOND_DEL_TIMER, SECOND_DEL_DUR);
#endif

#ifdef tc3
  uint32_t id1 = SCH_Add_Task(task1, 500, 1000);
  uint32_t id2 = SCH_Add_Task(task2, 2000, 6000);
  uint32_t id3 = SCH_Add_Task(task3, 1000, 2000);
  uint32_t id4 = SCH_Add_Task(task4, 0, 3000);
  uint32_t id5 = SCH_Add_Task(task5, 3000, 8000);

  timerSet(FIRST_DEL_TIMER, FIRST_DEL_DUR);
  timerSet(SECOND_DEL_TIMER, SECOND_DEL_DUR);
  timerSet(FIRST_ADD_TIMER, FIRST_ADD_DUR);
  timerSet(SECOND_ADD_TIMER, SECOND_ADD_DUR);
#endif

#ifdef tc4
  uint32_t id1 = SCH_Add_Task(task1, 0, 1000);
  uint32_t id2 = SCH_Add_Task(task2, 2000, 2000);
  uint32_t id3 = SCH_Add_Task(task3, 0, 3000);
  uint32_t id4 = SCH_Add_Task(task4, 1000, 5000);
  uint32_t id5 = SCH_Add_Task(task5, 8000, 8000);
  SCH_Add_Task(task_7seg, 0, 250);
  SCH_Add_Task(task_incre_num, 1000, 1000);

  timerSet(FIRST_DEL_TIMER, FIRST_DEL_DUR);
  timerSet(SECOND_DEL_TIMER, SECOND_DEL_DUR);
  timerSet(FIRST_ADD_TIMER, FIRST_ADD_DUR);
  timerSet(SECOND_ADD_TIMER, SECOND_ADD_DUR);
#endif

#ifdef tc5

#endif


  while (1)
  {
	  if (timerFlag(LED_DEBUG_TIMER)){
		  timerSet(LED_DEBUG_TIMER, LED_DEBUG_DUR);
		  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
	  }

#ifdef tc1
#endif

#ifdef tc2
	  if (timerFlag(FIRST_DEL_TIMER)){
		  timerSet(FIRST_DEL_TIMER, DISABLE_TIMER);
		  SCH_Delete_Task(id1);
		  SCH_Delete_Task(id2);
		  SCH_Delete_Task(id3);
	  }
	  if (timerFlag(SECOND_DEL_TIMER)){
		  timerSet(SECOND_DEL_TIMER, DISABLE_TIMER);
		  SCH_Delete_Task(id4);
		  SCH_Delete_Task(id5);
	  }
#endif

#ifdef tc3
	  if (timerFlag(FIRST_DEL_TIMER)){
		  timerSet(FIRST_DEL_TIMER, DISABLE_TIMER);
		  SCH_Delete_Task(id1);
		  SCH_Delete_Task(id2);
		  SCH_Delete_Task(id3);
	  }
	  if (timerFlag(SECOND_DEL_TIMER)){
		  timerSet(SECOND_DEL_TIMER, DISABLE_TIMER);
		  SCH_Delete_Task(id4);
		  SCH_Delete_Task(id5);
	  }
	  if (timerFlag(FIRST_ADD_TIMER)){
		  timerSet(FIRST_ADD_TIMER, DISABLE_TIMER);
		  SCH_Add_Task(task1, 500, 1000);
		  SCH_Add_Task(task2, 2000, 6000);
		  SCH_Add_Task(task3, 1000, 2000);
	  }
	  if (timerFlag(SECOND_ADD_TIMER)){
		  timerSet(SECOND_ADD_TIMER, DISABLE_TIMER);
		  SCH_Add_Task(task4, 0, 3000);
		  SCH_Add_Task(task5, 3000, 8000);
	  }
#endif

#ifdef tc4
	  if (timerFlag(FIRST_DEL_TIMER)){
		  timerSet(FIRST_DEL_TIMER, DISABLE_TIMER);
		  SCH_Delete_Task(id1);
		  SCH_Delete_Task(id2);
	  }
	  if (timerFlag(SECOND_DEL_TIMER)){
		  timerSet(SECOND_DEL_TIMER, DISABLE_TIMER);
		  SCH_Delete_Task(id3);
		  SCH_Delete_Task(id4);
		  SCH_Delete_Task(id5);
	  }
	  if (timerFlag(FIRST_ADD_TIMER)){
		  timerSet(FIRST_ADD_TIMER, DISABLE_TIMER);
		  SCH_Add_Task(task1, 0, 1000);
		  SCH_Add_Task(task2, 20, 2000);
	  }
	  if (timerFlag(SECOND_ADD_TIMER)){
		  timerSet(SECOND_ADD_TIMER, DISABLE_TIMER);
		  SCH_Add_Task(task3, 0, 3000);
		  SCH_Add_Task(task4, 1000, 5000);
		  SCH_Add_Task(task5, 8000, 8000);
	  }
#endif

#ifdef tc5

#endif

	  SCH_Dispatch_Tasks();
	  SCH_Report_Status();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED0_Pin|LED1_Pin|LED2_Pin|LED3_Pin
                          |LED4_Pin|LED5_Pin|BTN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, A_Pin|B_Pin|C_Pin|ERROR_B_Pin
                          |ERROR_C_Pin|ERROR_D_Pin|ERROR_E_Pin|ERROR_F_Pin
                          |ERROR_G_Pin|D_Pin|E_Pin|F_Pin
                          |G_Pin|EN1_Pin|EN2_Pin|ERROR_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED0_Pin LED1_Pin LED2_Pin LED3_Pin
                           LED4_Pin LED5_Pin BTN_LED_Pin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin|LED2_Pin|LED3_Pin
                          |LED4_Pin|LED5_Pin|BTN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN1_Pin BTN2_Pin BTN3_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin|BTN2_Pin|BTN3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : A_Pin B_Pin C_Pin ERROR_B_Pin
                           ERROR_C_Pin ERROR_D_Pin ERROR_E_Pin ERROR_F_Pin
                           ERROR_G_Pin D_Pin E_Pin F_Pin
                           G_Pin EN1_Pin EN2_Pin ERROR_A_Pin */
  GPIO_InitStruct.Pin = A_Pin|B_Pin|C_Pin|ERROR_B_Pin
                          |ERROR_C_Pin|ERROR_D_Pin|ERROR_E_Pin|ERROR_F_Pin
                          |ERROR_G_Pin|D_Pin|E_Pin|F_Pin
                          |G_Pin|EN1_Pin|EN2_Pin|ERROR_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	timerRun();
	SCH_Update();
}

/**
  * @brief My wrapper function that calls HAL_IWDG_Refresh to feed the watchdog.
  * @param None
  * @retval None
  */
void Watchdog_Refresh ( void ) {
	HAL_IWDG_Refresh(&hiwdg) ;
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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
