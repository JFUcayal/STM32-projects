/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lcd.h"
#include "keyPad.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_ADDR (0x27 << 1)

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5
#define lcd_row2 0b11000000
# define lcd_row1 0b10000000

#define PASS_NUM_TRY 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart3;

extern uint8_t StateCol , flag_pressed, col, row;
extern bool flag_it;

bool flag_acess = false, flag_forbidden = false, flag_enter_pass = false, flag_delete_last_ch = false;
uint8_t index_pass=0,index_key[4]={0}, passAllowed[4]={0,1,2,3}, passAttmpt[4]={0}, passAttmpts=PASS_NUM_TRY;

bool flag_set = false, flag_recebido = false, flag_first_time = false, flag_first_time1 = false, flag_first_time2 = false;
uint32_t start_time = 0, end_time = 0, capturedValue1 = 0, capturedValue2 = 0, capturedValue3 = 0, capturedValue4 = 0, capturedValue5 = 0, capturedValue6 = 0;

char keyPad_button[12]={'0','1','2','3','4','5','6','7','8','9','*','#'};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int __io_getchar(void)
{
    uint8_t ch=0;
    __HAL_UART_CLEAR_OREFLAG(&huart3);
    HAL_UART_Receive(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

			if(flag_first_time == false){
			capturedValue1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			flag_first_time = true;
			}
			else{
				capturedValue2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				float dif_capturedValue = capturedValue2 - capturedValue1;
				double periodo = 1.0 / (108000000.0 / dif_capturedValue);
				float distance = (periodo * 34300.0f) / 2.0f;


				  if (distance < 150)					//verifica distância em centimetros
				  {
					printf("Distance1: %f \n", distance);
					printf("Barreira interdita \n\r");
				  }
				  else{
						printf("Distance1: %f \n", distance);
						printf("Barreira nao interdita \n\r");
				  }
				  flag_first_time = false;
				  __HAL_TIM_SET_COUNTER(&htim2, 0);

			}
		}

		if (htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){

			if(flag_first_time1 == false){
			capturedValue3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			flag_first_time1 = true;
			}
			else{
				capturedValue4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
				float dif_capturedValue = capturedValue4 - capturedValue3;
				double periodo = 1.0 / (108000000.0 / (dif_capturedValue * 1727));
				float distance = (periodo * 34300.0f) / 2.0f;


				  if (distance < 150)					//verifica distância em centimetros
				  {
					printf("Distance2: %f \n", distance);
					printf("Barreira interdita \n\r");
				  }
				  else{
						printf("Distance2: %f \n", distance);
						printf("Barreira nao interdita \n\r");
				  }
				  flag_first_time1 = false;
				  __HAL_TIM_SET_COUNTER(&htim4, 0);

			}
		}

		if (htim->Instance == TIM12 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){

			if(flag_first_time2 == false){
			capturedValue5 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			flag_first_time2 = true;
			}
			else{
				capturedValue6 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				float dif_capturedValue = capturedValue6 - capturedValue5;
				double periodo = 1.0 / (108000000.0 / (dif_capturedValue * 1727));
				float distance = (periodo * 34300.0f) / 2.0f;


				  if (distance < 150)					//verifica distância em centimetros
				  {
					printf("Distance3: %f \n", distance);
					printf("Barreira interdita \n\r");
				  }
				  else{
						printf("Distance3: %f \n", distance);
						printf("Barreira nao interdita \n\r");
				  }
				  flag_first_time2 = false;
				  __HAL_TIM_SET_COUNTER(&htim12, 0);

			}
		}
/*
		if(htim == &htim5){
			flag_it=1;
		}
*/
}

void reset_attmpt(){
	memset(passAttmpt, 0, sizeof(passAttmpt));
	index_pass = 0;
}

void verify_pass(){

	for(uint8_t i=0; i < 4 ;i++){
		if(passAttmpt[i] == passAllowed[i]){
			flag_acess = true;
		} else {
			printlcd(lcd_row1, "pass errada");
			flag_acess = false;
			passAttmpts--;
			reset_attmpt();
			break;
		}
	}

	if(passAttmpts == 0 && !flag_acess){
		flag_forbidden = true;
		clearlcd();
		printlcd(lcd_row1, "acesso negado");
		reset_attmpt();
	}

	if(flag_acess && flag_enter_pass){
		clearlcd();
		printlcd(lcd_row1, "acesso validado");
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
	char lcdstr[16];

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
  MX_I2C2_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
  printf("----------INIT---------\r\n");
  init();

  clearlcd();
  printlcd(lcd_row1, "Insira a pass:");

  //HAL_TIM_Base_Start_IT(&htim5);

  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);	//ECHO
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);	//ECHO
  HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_2);	//ECHO
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);		//SINAL TRIGGER

  HAL_GPIO_WritePin(col1_port, col1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(col2_port, col2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(col3_port, col3, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  while(!flag_forbidden){


		  //ler teclado
		  if(flag_it){
			  CycleCol();
			  read();
		  }

		  HAL_Delay(50);


		  switch(index_pass){
		  case 0:
			  printlcd(lcd_row2, " ");
			  break;
		  case 1:

  			  printlcd(lcd_row2, "*");

			  if(flag_delete_last_ch){
				  flag_delete_last_ch = false;
				  passAttmpt[0] = 0;
				  index_key[0] = 0;
				  index_pass--;
			  } else {

				  passAttmpt[0]=index_key[0];
			  }
			  break;
		  case 2:

			  printlcd(lcd_row2, "**");

			  if(flag_delete_last_ch){
				  flag_delete_last_ch = false;
				  passAttmpt[1] = 0;
				  index_key[1] = 0;
				  index_pass--;
			  } else {

				  passAttmpt[1]=index_key[1];
			  }
			  break;
		  case 3:

			  printlcd(lcd_row2, "***");

			  if(flag_delete_last_ch){
				  flag_delete_last_ch = false;
				  passAttmpt[2] = 0;
				  index_key[2] = 0;
				  index_pass--;
			  } else {

				  passAttmpt[2]=index_key[2];
			  }
			  break;
		  case 4:

			  printlcd(lcd_row2, "****");

			  if(flag_delete_last_ch){
				  flag_delete_last_ch = false;
				  passAttmpt[3] = 0;
				  index_key[3] = 0;
				  index_pass--;
			  } else {
				  passAttmpt[3]=index_key[3];
			  }

			  if(flag_enter_pass){				//Pressionar no # para verificar a password
				  verify_pass();
				  flag_enter_pass = false;
			  }
			  break;
		  default:
			  reset_attmpt();
			  flag_acess = false;
		  }

		  while(flag_acess){

			  //desativar pino de segurança

		  }

	  }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
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
