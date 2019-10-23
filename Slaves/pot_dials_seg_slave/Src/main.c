/* USER CODE BEGIN Header */

/*
 * OTHER SLAVE:
 *  - POTENTIOMETERS
 *  - DIALS
 *  - SEVEN SEGS
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

//Stepper motor step size
#define MAX_NUM_STEPS 		600

//Stepper motor directions
#define ANTICLOCKWISE 		0
#define CLOCKWISE 			1

// ADC buffer
#define ADC_BUFF_SIZE 		10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart8;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Received data from UART
union data_in recieved_data;

// Compiling data from dip switches
// into the data to send out
union board_info_data board_data;

// Data to send out on UART
data_packet info_out;

// Timing variables
uint8_t TEST_MODE = 0;
uint32_t ticker = 0;

// Dial 1 variables
uint32_t desiredDialState1 = 0;
uint32_t currentDialState1 = 0;
uint32_t tempDialState1 = 0;

// Dial 2 variables
uint32_t desiredDialState2 = 0;
uint32_t currentDialState2 = 0;

// Pot variables
uint32_t pot1Sum = 0;
uint8_t pot1BufferIndex = 0;
uint8_t pot1Buffer[ADC_BUFF_SIZE] = {0};
uint8_t pot1State = 0;

uint32_t pot2Sum = 0;
uint8_t pot2BufferIndex = 0;
uint8_t pot2Buffer[ADC_BUFF_SIZE] = {0};
uint8_t pot2State = 0;

// 7 Seg 1 variables
uint8_t desiredDispValue1 = 0;
uint8_t currentDispValue1 = 0;
uint8_t desiredDispValueDigits1[4] = {0};
uint8_t dispIndex1 = 0;
uint8_t tempDigits1[4] = {0};

// 7 Seg 2 variables
uint8_t desiredDispValue2 = 0;
uint32_t currentDispValue2 = 0;
uint8_t desiredDispValueDigits2[4] = {0};
uint8_t dispIndex2 = 0;
uint8_t tempDigits2[4] = {0};

// Latency testing
uint32_t start_time = 0;
uint32_t end_time = 0;
uint32_t total_time[30] = {0};
uint8_t timerIndex = 0;


// UART RX interrupt to update states
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART8) {
		if (TEST_MODE)
		{
			HAL_GPIO_WritePin(GPIOE, testPinOut_Pin, GPIO_PIN_SET);
			end_time = HAL_GetTick();
			total_time[timerIndex] = end_time - start_time;
			timerIndex++;
			end_time = 0;
			start_time = 0;
			TEST_MODE = 0;
		}

		if (recieved_data.info_in.board_add == Display1HWAdd)
			desiredDispValue1 = recieved_data.info_in.state;

		if (recieved_data.info_in.board_add == Display2HWAdd)
			desiredDispValue2 = recieved_data.info_in.state;

		if (recieved_data.info_in.board_add == Dial1HWAdd)
			desiredDialState1 = 5.3 * recieved_data.info_in.state;

		if (recieved_data.info_in.board_add == Dial2HWAdd)
			desiredDialState2 = 5.3 * recieved_data.info_in.state;
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_UART8_Init();
  /* USER CODE BEGIN 2 */

  // DMA buffer
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, 2);

  //con3: 3 - 3v3; 4 - dir; 5 - step

  // Set stepper motor power high
  HAL_GPIO_WritePin(GPIOF, con3_3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, con1_3_Pin, GPIO_PIN_SET);

  // Calibrate the stepper motors
  for (int i = 0; i < MAX_NUM_STEPS; i++)
  {
		HAL_GPIO_WritePin(GPIOF, con3_4_Pin, ANTICLOCKWISE);
		HAL_GPIO_WritePin(GPIOA, con1_4_Pin, ANTICLOCKWISE);

		HAL_GPIO_WritePin(GPIOF, con3_5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, con1_5_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOF, con3_5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, con1_5_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
  }

  for (int i = 0; i < 123; i++)
  {
		HAL_GPIO_WritePin(GPIOF, con3_4_Pin, CLOCKWISE);
		HAL_GPIO_WritePin(GPIOA, con1_4_Pin, CLOCKWISE);

		HAL_GPIO_WritePin(GPIOF, con3_5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, con1_5_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOF, con3_5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, con1_5_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
  }

  currentDialState1 = 0;
  currentDialState2 = 0;


  // Pot  1buffer
  for (; pot1BufferIndex < ADC_BUFF_SIZE; pot1BufferIndex++)
  {
	  currentPotState = ((float) adcBuffer[0] / 4095) * 100;
	  pot1Buffer[pot1BufferIndex] = currentPotState;
	  pot1Sum += currentPotState;
  }
  pot1State = pot1Sum / ADC_BUFF_SIZE;

  // Pot 2 buffer
  for (; pot2BufferIndex < ADC_BUFF_SIZE; pot2BufferIndex++)
  {
	  currentPotState = ((float) adcBuffer[1] / 4095) * 100;
  	  pot2Buffer[pot2BufferIndex] = currentPotState;
  	  pot2Sum += currentPotState;
  }
  pot2State = pot2Sum / ADC_BUFF_SIZE;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  	  // TX UART interrupt to update the displays and dials
	  	  HAL_UART_Receive_IT(&huart8, recieved_data.info_in_buffer, 3);

	  	  // Assign the board information to the packet
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
	  	  if (ticker % LED_SCALER == 0) HAL_GPIO_TogglePin(status_led_GPIO_Port, status_led_Pin);
	  	  if (ticker == MAX_TICKER_VALUE) ticker = 0;

	  	  //Dip switches linked to LEDs
	  	  //Board type
	  	  HAL_GPIO_ReadPin(GPIOD, board_type_1_Pin) ? HAL_GPIO_WritePin(GPIOE, d6_Pin, GPIO_PIN_SET) :
	  			  	  HAL_GPIO_WritePin(GPIOE, d6_Pin, GPIO_PIN_RESET);

	  	  HAL_GPIO_ReadPin(GPIOD, board_type_2_Pin) ? HAL_GPIO_WritePin(GPIOE, d7_Pin, GPIO_PIN_SET) :
	  		  	  HAL_GPIO_WritePin(GPIOE, d7_Pin, GPIO_PIN_RESET);

	  	  HAL_GPIO_ReadPin(GPIOC, board_type_3_Pin) ? HAL_GPIO_WritePin(GPIOA, d8_Pin, GPIO_PIN_SET) :
	  		  	  HAL_GPIO_WritePin(GPIOA, d8_Pin, GPIO_PIN_RESET);

	  	  HAL_GPIO_ReadPin(GPIOC, board_type_4_Pin) ? HAL_GPIO_WritePin(GPIOC, d9_Pin, GPIO_PIN_SET) :
	  		  	  HAL_GPIO_WritePin(GPIOC, d9_Pin, GPIO_PIN_RESET);

	  	  //Board number
	  	  HAL_GPIO_ReadPin(GPIOD, board_num_1_Pin) ? HAL_GPIO_WritePin(GPIOF, d5_Pin, GPIO_PIN_SET) :
	  		  	  HAL_GPIO_WritePin(GPIOF, d5_Pin, GPIO_PIN_RESET);

	  	  HAL_GPIO_ReadPin(GPIOD, board_num_2_Pin) ? HAL_GPIO_WritePin(GPIOF, d4_Pin, GPIO_PIN_SET) :
	  		  	  HAL_GPIO_WritePin(GPIOF, d4_Pin, GPIO_PIN_RESET);

	  	  HAL_GPIO_ReadPin(GPIOD, board_num_3_Pin) ? HAL_GPIO_WritePin(GPIOF, d3_Pin, GPIO_PIN_SET) :
	  		  	  HAL_GPIO_WritePin(GPIOF, d3_Pin, GPIO_PIN_RESET);

	  	  HAL_GPIO_ReadPin(GPIOD, board_num_4_Pin) ? HAL_GPIO_WritePin(GPIOF, d2_Pin, GPIO_PIN_SET) :
	  	  		  HAL_GPIO_WritePin(GPIOF, d2_Pin, GPIO_PIN_RESET);

	  	// ADC reading of both POTs - reads one per loop
		currentPotState = ((float) adcBuffer[adcIndex] / 4095) * 100;

		if (adcIndex == 0 && ((currentPotState >= prevPotState[adcIndex] + 5) || (currentPotState <= prevPotState[adcIndex] - 5)))
		{
			// Moving average
			pot1Sum -= pot1Buffer[pot1BufferIndex];
			pot1Buffer[pot1BufferIndex] = currentPotState;
			pot1Sum += currentPotState;
			pot1State = pot1Sum / ADC_BUFF_SIZE;

			if (pot1BufferIndex++ > (ADC_BUFF_SIZE - 1)) pot1BufferIndex = 0;

			// Sending packet
			info_out.board_add  =  Pot1HWAdd;
			info_out.state  = pot1State;
			HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			prevPotState[adcIndex] = pot1State;

		}

		if (adcIndex == 1 && ((currentPotState >= prevPotState[adcIndex] + 5) || (currentPotState <= prevPotState[adcIndex] - 5)))
		{
			// Moving average
			pot2Sum -= pot2Buffer[pot2BufferIndex];
			pot2Buffer[pot2BufferIndex] = currentPotState;
			pot2Sum += currentPotState;
			pot2State = pot2Sum / ADC_BUFF_SIZE;

			if (pot2BufferIndex++ > (ADC_BUFF_SIZE - 1)) pot2BufferIndex = 0;

			// Sending packet
			info_out.board_add  =  Pot2HWAdd;
			info_out.state  = pot2State;
			HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),10);
			prevPotState[adcIndex] = pot2State;
		}

		if (adcIndex++ > 1) adcIndex = 0;

	  	// Stepper motor driving 1
	  	if (abs(desiredDialState1 - currentDialState1) != 0)
	  	{
	  		if ((currentDialState1 > desiredDialState1) && currentDialState1 != 0)
			{
	  			currentDialState1--;
				HAL_GPIO_WritePin(GPIOF, con3_4_Pin, ANTICLOCKWISE);
			}
			else if ((currentDialState1 < desiredDialState1) && currentDialState1 != 600)
			{
				currentDialState1++;
				HAL_GPIO_WritePin(GPIOF, con3_4_Pin, CLOCKWISE);
			}

	  		HAL_GPIO_WritePin(GPIOF, con3_5_Pin, GPIO_PIN_RESET);
	  		HAL_Delay(1);
	  		HAL_GPIO_WritePin(GPIOF, con3_5_Pin, GPIO_PIN_SET);
	  		HAL_Delay(1);
	  	}


	  	// Stepper motor driving 2 - need to change the pins
	  	if (abs(desiredDialState2 - currentDialState2) != 0)
	  	{
	  		if ((currentDialState2 > desiredDialState2) && currentDialState2 != 0)
			{
	  			currentDialState2--;
				HAL_GPIO_WritePin(GPIOA, con1_4_Pin, ANTICLOCKWISE);
			}
			else if ((currentDialState2 < desiredDialState2) && currentDialState2 != 530)
			{
				currentDialState2++;
				HAL_GPIO_WritePin(GPIOA, con1_4_Pin, CLOCKWISE);
			}

	  		HAL_GPIO_WritePin(GPIOA, con1_5_Pin, GPIO_PIN_RESET);
	  		HAL_Delay(1);
	  		HAL_GPIO_WritePin(GPIOA, con1_5_Pin, GPIO_PIN_SET);
	  		HAL_Delay(1);
	  	}

	  	// 7 Seg #1
		if (desiredDispValue1 != currentDispValue1)
		{
			currentDispValue1 = desiredDispValue1;
			memcpy(&desiredDispValueDigits1, 0x00, sizeof(tempDigits1));
			for (int i = 0; desiredDispValue1 > 0; i++)
			{
				 desiredDispValueDigits1[i] = desiredDispValue1 % 10;
				 desiredDispValue1 -= desiredDispValueDigits1[i];
				 desiredDispValue1 /= 10;
			}
			desiredDispValue1 = currentDispValue1;
		}

		// Clear the display
		HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_SET); 	// A
		HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_SET); 	// B
		HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_SET); 	// C
		HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_SET); 	// D
		HAL_GPIO_WritePin(GPIOB, con4_5_Pin, GPIO_PIN_SET); 	// E
		HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_SET); 	// F
		HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_SET); 	// G
		HAL_GPIO_WritePin(GPIOC, con4_8_Pin, GPIO_PIN_SET); 	// DP

		HAL_GPIO_WritePin(GPIOC, con4_9_Pin, GPIO_PIN_RESET); 	// Dig 1
		HAL_GPIO_WritePin(GPIOC, con4_10_Pin, GPIO_PIN_RESET); 	// Dig 2
		HAL_GPIO_WritePin(GPIOE, con3_9_Pin, GPIO_PIN_RESET); 	// Dig 3
		HAL_GPIO_WritePin(GPIOE, con3_10_Pin, GPIO_PIN_RESET); 	// Dig 4



		// Select digit
		switch(dispIndex1)
		{
			// Dig 1
			case 0:
				HAL_GPIO_WritePin(GPIOE, con3_10_Pin, GPIO_PIN_SET); 	// Dig 4
				break;

			// Dig 2
			case 1:
				HAL_GPIO_WritePin(GPIOE, con3_9_Pin, GPIO_PIN_SET); 	// Dig 3
				break;

			// Dig 3
			case 2:
				HAL_GPIO_WritePin(GPIOC, con4_10_Pin, GPIO_PIN_SET); 	// Dig 2
				break;

			// Dig 4
			case 3:
				HAL_GPIO_WritePin(GPIOC, con4_9_Pin, GPIO_PIN_SET); 	// Dig 1
				break;

		}


		// Update the digit
		switch(desiredDispValueDigits1[dispIndex1])
		{
			case 0:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOB, con4_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_RESET); 	// F
				break;


			case 1:
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				break;

			case 2:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOB, con4_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 3:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 4:
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 5:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 6:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOB, con4_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET);	// G
				break;

			case 7:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				break;

			case 8:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOB, con4_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 9:
				HAL_GPIO_WritePin(GPIOF, con4_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOF, con4_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOB, con4_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOB, con4_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOA, con4_7_Pin, GPIO_PIN_RESET); 	// G
				break;

		}

		// Update the index
		dispIndex1++;
		if (dispIndex1 > 3) dispIndex1 = 0;


		// 7 Seg #2
		if (desiredDispValue2 != currentDispValue2)
		{
			memcpy(&desiredDispValueDigits2, 0x00, sizeof(tempDigits2));
			currentDispValue2 = desiredDispValue2;
			for (int i = 0; desiredDispValue2 > 0; i++)
			{
				desiredDispValueDigits2[i] = desiredDispValue2 % 10;
				 desiredDispValue2 -= desiredDispValueDigits2[i];
				 desiredDispValue2 /= 10;
			}

			desiredDispValue2 = currentDispValue2;
		}

		// Clear the display
		HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_SET); 	// A
		HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_SET); 	// B
		HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_SET); 	// C
		HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_SET); 	// D
		HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_SET); 	// E
		HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_SET); 	// F
		HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_SET); 	// G
		HAL_GPIO_WritePin(GPIOD, con2_8_Pin, GPIO_PIN_SET); 	// DP

		HAL_GPIO_WritePin(GPIOD, con2_9_Pin, GPIO_PIN_RESET); 	// Dig 1
		HAL_GPIO_WritePin(GPIOD, con2_10_Pin, GPIO_PIN_RESET); 	// Dig 2
		HAL_GPIO_WritePin(GPIOF, con3_1_Pin, GPIO_PIN_RESET); 	// Dig 3
		HAL_GPIO_WritePin(GPIOF, con3_2_Pin, GPIO_PIN_RESET); 	// Dig 4

		// Select digit
		switch(dispIndex2)
		{
			// Dig 1
			case 0:
				HAL_GPIO_WritePin(GPIOF, con3_2_Pin, GPIO_PIN_SET); 	// Dig 4
				break;

			// Dig 2
			case 1:
				HAL_GPIO_WritePin(GPIOF, con3_1_Pin, GPIO_PIN_SET); 	// Dig 3
				break;

			// Dig 3
			case 2:
				HAL_GPIO_WritePin(GPIOD, con2_10_Pin, GPIO_PIN_SET); 	// Dig 2
				break;

			// Dig 4
			case 3:
				HAL_GPIO_WritePin(GPIOD, con2_9_Pin, GPIO_PIN_SET); 	// Dig 1
				break;
		}

		// Update the digit
		switch(desiredDispValueDigits2[dispIndex2])
		{
			case 0:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_RESET); 	// F
				break;


			case 1:
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				break;

			case 2:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 3:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 4:
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 5:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 6:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET);	// G
				break;

			case 7:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				break;

			case 8:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_4_Pin, GPIO_PIN_RESET); 	// D
				HAL_GPIO_WritePin(GPIOC, con2_5_Pin, GPIO_PIN_RESET); 	// E
				HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET); 	// G
				break;

			case 9:
				HAL_GPIO_WritePin(GPIOC, con2_1_Pin, GPIO_PIN_RESET); 	// A
				HAL_GPIO_WritePin(GPIOC, con2_2_Pin, GPIO_PIN_RESET); 	// B
				HAL_GPIO_WritePin(GPIOC, con2_3_Pin, GPIO_PIN_RESET); 	// C
				HAL_GPIO_WritePin(GPIOC, con2_6_Pin, GPIO_PIN_RESET); 	// F
				HAL_GPIO_WritePin(GPIOD, con2_7_Pin, GPIO_PIN_RESET); 	// G
				break;

		}

		// Update the index
		dispIndex2++;
		if (dispIndex2 > 3) dispIndex2 = 0;

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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, d6_Pin|d7_Pin|con3_10_Pin|con3_9_Pin 
                          |con5_3_Pin|con5_1_Pin|con5_2_Pin|testPinOut_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, con4_1_Pin|con4_2_Pin|con3_5_Pin|con3_4_Pin 
                          |con3_3_Pin|con3_2_Pin|con3_1_Pin|d2_Pin 
                          |d3_Pin|d4_Pin|d5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, con2_6_Pin|con2_5_Pin|con2_4_Pin|con2_3_Pin 
                          |con2_2_Pin|con2_1_Pin|con4_9_Pin|con4_10_Pin 
                          |con4_8_Pin|d8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, con1_5_Pin|con1_4_Pin|con1_3_Pin|con4_7_Pin 
                          |d9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, con1_2_Pin|con1_1_Pin|status_led_Pin|con8_1_Pin 
                          |con4_3_Pin|con4_4_Pin|con4_5_Pin|con4_6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, con5_9_Pin|con8_3_Pin|con8_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, con2_7_Pin|con2_8_Pin|con2_9_Pin|con2_10_Pin 
                          |con8_10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : d6_Pin d7_Pin con3_10_Pin con3_9_Pin 
                           con5_3_Pin con5_1_Pin con5_2_Pin testPinOut_Pin */
  GPIO_InitStruct.Pin = d6_Pin|d7_Pin|con3_10_Pin|con3_9_Pin 
                          |con5_3_Pin|con5_1_Pin|con5_2_Pin|testPinOut_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : con4_1_Pin con4_2_Pin con3_5_Pin con3_4_Pin 
                           con3_3_Pin con3_2_Pin con3_1_Pin d2_Pin 
                           d3_Pin d4_Pin d5_Pin */
  GPIO_InitStruct.Pin = con4_1_Pin|con4_2_Pin|con3_5_Pin|con3_4_Pin 
                          |con3_3_Pin|con3_2_Pin|con3_1_Pin|d2_Pin 
                          |d3_Pin|d4_Pin|d5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_6_Pin con2_5_Pin con2_4_Pin con2_3_Pin 
                           con2_2_Pin con2_1_Pin */
  GPIO_InitStruct.Pin = con2_6_Pin|con2_5_Pin|con2_4_Pin|con2_3_Pin 
                          |con2_2_Pin|con2_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : con1_5_Pin con1_4_Pin con1_3_Pin con4_7_Pin 
                           d9_Pin */
  GPIO_InitStruct.Pin = con1_5_Pin|con1_4_Pin|con1_3_Pin|con4_7_Pin 
                          |d9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : con1_2_Pin con1_1_Pin status_led_Pin con8_1_Pin 
                           con4_3_Pin con4_4_Pin con4_5_Pin con4_6_Pin */
  GPIO_InitStruct.Pin = con1_2_Pin|con1_1_Pin|status_led_Pin|con8_1_Pin 
                          |con4_3_Pin|con4_4_Pin|con4_5_Pin|con4_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : con5_9_Pin con8_3_Pin con8_2_Pin */
  GPIO_InitStruct.Pin = con5_9_Pin|con8_3_Pin|con8_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_7_Pin con2_8_Pin */
  GPIO_InitStruct.Pin = con2_7_Pin|con2_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : con2_9_Pin con2_10_Pin con8_10_Pin */
  GPIO_InitStruct.Pin = con2_9_Pin|con2_10_Pin|con8_10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : con4_9_Pin con4_10_Pin con4_8_Pin d8_Pin */
  GPIO_InitStruct.Pin = con4_9_Pin|con4_10_Pin|con4_8_Pin|d8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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

  /*Configure GPIO pin : testPinIn_Pin */
  GPIO_InitStruct.Pin = testPinIn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(testPinIn_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
		case testPinIn_Pin:
			TEST_MODE = 1;
			info_out.board_add  =  100; //this needs to be recognised at the GUI as the test packet
			info_out.state  = 100;
			start_time = HAL_GetTick();
		    //HAL_Delay(10);
			HAL_UART_Transmit(&huart8,&info_out,sizeof(info_out),1000);
			break;

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
