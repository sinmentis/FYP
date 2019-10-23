/* USER CODE BEGIN Header */
/*
 * BUTTONS AND SWITCHES
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

//Timing magic numbers
#define MAX_TICKER_VALUE 	4294967295
#define LED_SCALER 			10000

//Switch/button status
#define HIGH				2
#define LOW					1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart8;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t ticker = 0;

union board_info_data board_data;
data_packet info_out;

union data_in data;

// State arrays
uint8_t newState[15] = {0};
uint8_t oldState[15] = {0};


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
  MX_USART3_UART_Init();
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_UART_Receive_IT(&huart8, data.info_in_buffer, 3);

	  // Assign board info
	  board_data.board_info_raw.board_type_1 = HAL_GPIO_ReadPin(GPIOD, board_type_1_Pin) ? 0b1 : 0b0;
	  board_data.board_info_raw.board_type_2 = HAL_GPIO_ReadPin(GPIOD, board_type_2_Pin) ? 0b1 : 0b0;
	  board_data.board_info_raw.board_type_3 = HAL_GPIO_ReadPin(GPIOC, board_type_3_Pin) ? 0b1 : 0b0;
	  board_data.board_info_raw.board_type_4 = HAL_GPIO_ReadPin(GPIOC, board_type_4_Pin) ? 0b1 : 0b0;

	  board_data.board_info_raw.board_num_1 = HAL_GPIO_ReadPin(GPIOD, board_num_1_Pin) ? 0b1 : 0b0;
	  board_data.board_info_raw.board_num_2 = HAL_GPIO_ReadPin(GPIOD, board_num_2_Pin) ? 0b1 : 0b0;
	  board_data.board_info_raw.board_num_3 = HAL_GPIO_ReadPin(GPIOD, board_num_3_Pin) ? 0b1 : 0b0;
	  board_data.board_info_raw.board_num_4 = HAL_GPIO_ReadPin(GPIOD, board_num_4_Pin) ? 0b1 : 0b0;

	  info_out.board_info = board_data.board_info_char;

	  //Status LED
	  ticker++;
	  if (ticker % LED_SCALER == 0)  HAL_GPIO_TogglePin(status_led_GPIO_Port, status_led_Pin);
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  HAL_GPIO_WritePin(GPIOF, con4_1_Pin|con4_2_Pin|con3_2_Pin|d2_Pin 
                          |d3_Pin|d4_Pin|d5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(d9_GPIO_Port, d9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(d8_GPIO_Port, d8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, status_led_Pin|con4_3_Pin|test3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : d6_Pin d7_Pin test2_Pin test1_Pin */
  GPIO_InitStruct.Pin = d6_Pin|d7_Pin|test2_Pin|test1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : con4_1_Pin con4_2_Pin con3_2_Pin d2_Pin 
                           d3_Pin d4_Pin d5_Pin */
  GPIO_InitStruct.Pin = con4_1_Pin|con4_2_Pin|con3_2_Pin|d2_Pin 
                          |d3_Pin|d4_Pin|d5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : con3_1_Pin */
  GPIO_InitStruct.Pin = con3_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(con3_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_6_Pin con2_5_Pin con2_4_Pin con2_3_Pin 
                           con2_2_Pin con2_1_Pin */
  GPIO_InitStruct.Pin = con2_6_Pin|con2_5_Pin|con2_4_Pin|con2_3_Pin 
                          |con2_2_Pin|con2_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_7_Pin con2_8_Pin con2_9_Pin con2_10_Pin */
  GPIO_InitStruct.Pin = con2_7_Pin|con2_8_Pin|con2_9_Pin|con2_10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : con4_9_Pin con4_8_Pin */
  GPIO_InitStruct.Pin = con4_9_Pin|con4_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : d9_Pin */
  GPIO_InitStruct.Pin = d9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(d9_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : d8_Pin */
  GPIO_InitStruct.Pin = d8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(d8_GPIO_Port, &GPIO_InitStruct);

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

  /*Configure GPIO pins : status_led_Pin con4_3_Pin test3_Pin */
  GPIO_InitStruct.Pin = status_led_Pin|con4_3_Pin|test3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : con4_5_Pin con4_6_Pin */
  GPIO_InitStruct.Pin = con4_5_Pin|con4_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
		case con2_1_Pin:
			newState[0] = HAL_GPIO_ReadPin(GPIOC, con2_1_Pin);
			if (oldState[0] != newState[0])
			{
				oldState[0] = newState[0];
				info_out.board_add  =  con2_1_Pin_HWAdd;
				info_out.state  = (newState[0]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_2_Pin:
			newState[1] = HAL_GPIO_ReadPin(GPIOC, con2_2_Pin);
			if (oldState[1] != newState[1])
			{
				oldState[1] = newState[1];
				info_out.board_add  =  con2_2_Pin_HWAdd;
				info_out.state  = (newState[1]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_3_Pin:
			newState[2] = HAL_GPIO_ReadPin(GPIOC, con2_3_Pin);
			if (oldState[2] != newState[2])
			{
				oldState[2] = newState[2];
				info_out.board_add  =  con2_3_Pin_HWAdd;
				info_out.state  = (newState[2]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_4_Pin:
			newState[3] = HAL_GPIO_ReadPin(GPIOC, con2_4_Pin);
			if (oldState[3] != newState[3])
			{
				oldState[3] = newState[3];
				info_out.board_add  =  con2_4_Pin_HWAdd;
				info_out.state  = (newState[3]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_5_Pin:
			newState[4] = HAL_GPIO_ReadPin(GPIOC, con2_5_Pin);
			if (oldState[4] != newState[4])
			{
				oldState[4] = newState[4];
				info_out.board_add  =  con2_5_Pin_HWAdd;
				info_out.state  = (newState[4]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_6_Pin:
			newState[5] = HAL_GPIO_ReadPin(GPIOC, con2_6_Pin);
			if (oldState[5] != newState[5])
			{
				oldState[5] = newState[5];
				info_out.board_add  =  con2_6_Pin_HWAdd;
				info_out.state  = (newState[5]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_7_Pin:
			newState[6] = HAL_GPIO_ReadPin(GPIOD, con2_7_Pin);
			if (oldState[6] != newState[6])
			{
				oldState[6] = newState[6];
				info_out.board_add  =  con2_7_Pin_HWAdd;
				info_out.state  = (newState[6]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_8_Pin:
			newState[7] = HAL_GPIO_ReadPin(GPIOD, con2_8_Pin);
			if (oldState[7] != newState[7])
			{
				oldState[7] = newState[7];
				info_out.board_add  =  con2_8_Pin_HWAdd;
				info_out.state  = (newState[7]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_9_Pin:
			newState[8] = HAL_GPIO_ReadPin(GPIOD, con2_9_Pin);
			if (oldState[8] != newState[8])
			{
				oldState[8] = newState[8];
				info_out.board_add  =  con2_9_Pin_HWAdd;
				info_out.state  = (newState[8]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con2_10_Pin:
			newState[9] = HAL_GPIO_ReadPin(GPIOD, con2_10_Pin);
			if (oldState[9] != newState[9])
			{
				oldState[9] = newState[9];
				info_out.board_add  =  con2_10_Pin_HWAdd;
				info_out.state  = (newState[9]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con3_1_Pin:
		newState[10] = HAL_GPIO_ReadPin(GPIOF, con3_1_Pin);
		if (oldState[10] != newState[10])
		{
			oldState[10] = newState[10];
			info_out.board_add  =  con3_1_Pin_HWAdd;
			info_out.state  = (newState[10]) ? LOW : HIGH;
			HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
		}
		break;


		/*case con4_5_Pin:
			newState[12] = HAL_GPIO_ReadPin(GPIOB, con4_5_Pin);
			if (oldState[12] != newState[12])
			{
				oldState[12] = newState[12];
				info_out.board_add  =  con4_5_Pin_HWAdd;
				info_out.state  = (newState[12]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con4_6_Pin:
			newState[13] = HAL_GPIO_ReadPin(GPIOB, con4_6_Pin);
			if (oldState[13] != newState[13])
			{
				oldState[13] = newState[13];
				info_out.board_add  =  con4_6_Pin_HWAdd;
				info_out.state  = (newState[13]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;

		case con4_9_Pin:
			newState[14] = HAL_GPIO_ReadPin(GPIOC, con4_9_Pin);
			if (oldState[14] != newState[14])
			{
				oldState[14] = newState[14];
				info_out.board_add  =  con4_9_Pin_HWAdd;
				info_out.state  = (newState[14]) ? LOW : HIGH;
				HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			}
			break;*/

	}


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
