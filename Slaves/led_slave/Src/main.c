/* USER CODE BEGIN Header */
/**
/*
 * LEDS
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PIN_DEFS.h"
#include "STRUCTS.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define HIGH 				2
#define LOW 				1

#define MAX_TICKER_VALUE 	4294967295
#define LED_SCALER			100000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart8;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t ticker = 0;
union data_in data;




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART8)
	{

		if (data.info_in.board_add == con2_4_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET);
		}

		if (data.info_in.board_add == con2_5_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_RESET);
		}

		if (data.info_in.board_add == con2_6_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_RESET);
		}

		if (data.info_in.board_add == con2_7_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET);
		}

		if (data.info_in.board_add == con2_8_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOD, con2_8_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOD, con2_8_Pin, GPIO_PIN_RESET);
		}

		if (data.info_in.board_add == con2_9_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOD, con2_9_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOD, con2_9_Pin, GPIO_PIN_RESET);
		}

		if (data.info_in.board_add == con2_10_HWAdd)
		{
			if (data.info_in.state == HIGH) HAL_GPIO_WritePin(GPIOD, con2_10_Pin, GPIO_PIN_SET);
			if (data.info_in.state == LOW) HAL_GPIO_WritePin(GPIOD, con2_10_Pin, GPIO_PIN_RESET);
		}

	}

	else {
		__NOP();
	}

}


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
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // UART RX interrupts
	  HAL_UART_Receive_IT(&huart8, data.info_in_buffer, 3);

	  //Status LED
	  ticker++;
	  if (ticker % LED_SCALER == 0) HAL_GPIO_TogglePin(status_led_GPIO_Port, status_led_Pin);

	  if (ticker == MAX_TICKER_VALUE) ticker = 0;



	  //Dip switches linked to LEDs
	  (HAL_GPIO_ReadPin(GPIOD, board_type_1_Pin) == 0) ? HAL_GPIO_WritePin(GPIOE, d6_Pin, GPIO_PIN_RESET) :
			  	  HAL_GPIO_WritePin(GPIOE, d6_Pin, GPIO_PIN_SET);

	  (HAL_GPIO_ReadPin(GPIOD, board_type_2_Pin) == 0) ? HAL_GPIO_WritePin(GPIOE, d7_Pin, GPIO_PIN_RESET) :
		  	  HAL_GPIO_WritePin(GPIOE, d7_Pin, GPIO_PIN_SET);

	  (HAL_GPIO_ReadPin(GPIOC, board_type_3_Pin) == 0) ? HAL_GPIO_WritePin(GPIOA, d8_Pin, GPIO_PIN_RESET) :
		  	  HAL_GPIO_WritePin(GPIOA, d8_Pin, GPIO_PIN_SET);

	  (HAL_GPIO_ReadPin(GPIOC, board_type_4_Pin) == 0) ? HAL_GPIO_WritePin(GPIOC, d9_Pin, GPIO_PIN_RESET) :
		  	  HAL_GPIO_WritePin(GPIOC, d9_Pin, GPIO_PIN_SET);



	  (HAL_GPIO_ReadPin(GPIOD, board_num_1_Pin) == 0) ? HAL_GPIO_WritePin(GPIOF, d5_Pin, GPIO_PIN_RESET) :
		  	  HAL_GPIO_WritePin(GPIOF, d5_Pin, GPIO_PIN_SET);

	  (HAL_GPIO_ReadPin(GPIOD, board_num_2_Pin) == 0) ? HAL_GPIO_WritePin(GPIOF, d4_Pin, GPIO_PIN_RESET) :
		  	  HAL_GPIO_WritePin(GPIOF, d4_Pin, GPIO_PIN_SET);

	  (HAL_GPIO_ReadPin(GPIOD, board_num_3_Pin) == 0) ? HAL_GPIO_WritePin(GPIOF, d3_Pin, GPIO_PIN_RESET) :
		  	  HAL_GPIO_WritePin(GPIOF, d3_Pin, GPIO_PIN_SET);

	  (HAL_GPIO_ReadPin(GPIOD, board_num_4_Pin) == 0) ? HAL_GPIO_WritePin(GPIOF, d2_Pin, GPIO_PIN_RESET) :
	  		  HAL_GPIO_WritePin(GPIOF, d2_Pin, GPIO_PIN_SET);


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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 9600;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, d6_Pin|d7_Pin|test2_Pin|test1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, con3_7_Pin|con3_5_Pin|con3_3_Pin|con3_1_Pin 
                          |d2_Pin|d3_Pin|d4_Pin|d5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, con2_6_Pin|con2_5_Pin|con2_4_Pin|con2_3_Pin 
                          |con2_2_Pin|con2_1_Pin|d8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, con2_7_Pin|con2_8_Pin|con2_9_Pin|con2_10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(d9_GPIO_Port, d9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, status_led_Pin|test3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : d6_Pin d7_Pin test2_Pin test1_Pin */
  GPIO_InitStruct.Pin = d6_Pin|d7_Pin|test2_Pin|test1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : con3_7_Pin con3_5_Pin con3_3_Pin con3_1_Pin 
                           d2_Pin d3_Pin d4_Pin d5_Pin */
  GPIO_InitStruct.Pin = con3_7_Pin|con3_5_Pin|con3_3_Pin|con3_1_Pin 
                          |d2_Pin|d3_Pin|d4_Pin|d5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_6_Pin con2_5_Pin con2_4_Pin */
  GPIO_InitStruct.Pin = con2_6_Pin|con2_5_Pin|con2_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_3_Pin con2_2_Pin con2_1_Pin d8_Pin */
  GPIO_InitStruct.Pin = con2_3_Pin|con2_2_Pin|con2_1_Pin|d8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : uart_tx_Pin uat_rx_Pin */
  GPIO_InitStruct.Pin = uart_tx_Pin|uat_rx_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_7_Pin con2_8_Pin con2_9_Pin con2_10_Pin */
  GPIO_InitStruct.Pin = con2_7_Pin|con2_8_Pin|con2_9_Pin|con2_10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : d9_Pin */
  GPIO_InitStruct.Pin = d9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(d9_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : board_type_4_Pin board_type_3_Pin */
  GPIO_InitStruct.Pin = board_type_4_Pin|board_type_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : board_type_2_Pin board_type_1_Pin board_num_4_Pin board_num_3_Pin 
                           board_num_2_Pin board_num_1_Pin */
  GPIO_InitStruct.Pin = board_type_2_Pin|board_type_1_Pin|board_num_4_Pin|board_num_3_Pin 
                          |board_num_2_Pin|board_num_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : status_led_Pin test3_Pin */
  GPIO_InitStruct.Pin = status_led_Pin|test3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
