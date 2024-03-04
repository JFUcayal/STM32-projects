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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define COLOR 12500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool flag_color = false, flag_s3 = false, flag_send_counter = false, flag_S2 = false, flag_sensor = false;
uint32_t counter = 0;
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
#define DEBOUNCE_DELAY 200 // Atraso em milissegundos
uint32_t lastDebounceTime = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin == S1_Pin){
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);		//SENSOR DE INICIO -> ATIVA MOTOR
	}

	else if(GPIO_Pin == S2_Pin){
		uint32_t currentMillis = HAL_GetTick();
		if (flag_S2 == false){

		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);		//CHEGA AO MEIO -> PARA MOTOR E ATIVA TIMER DE 0,5S

		EXTI->IMR |= EXTI_IMR_MR7;
		counter = 0;
		HAL_TIM_Base_Start_IT(&htim2);
		flag_S2 = true;
		flag_sensor = true;
			}
		}
	else if(GPIO_Pin == S3_Pin){
		if(flag_sensor == true){

			flag_sensor = false;
			flag_S2 = false;

		if(counter >= COLOR){		//É VERDE
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);	  //PARO MOTOR

			printf("E DA COR \r\n");
			//deixa a 1 enquanto a peça estiver no sensor 3
			HAL_GPIO_WritePin(S_Braco_GPIO_Port, S_Braco_Pin, GPIO_PIN_SET);	//ENVIA SINAL AO BRAÇO COMO PEÇA CHEGOU
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == 0);
			HAL_GPIO_WritePin(S_Braco_GPIO_Port, S_Braco_Pin, GPIO_PIN_RESET);
		}
		else{																//NÃO É VERDE
			flag_s3 = true;
			HAL_TIM_Base_Start_IT(&htim2);									//MOTOR TRABALHA MAIS 0,5S
		}
	}
	}
	else if(GPIO_Pin == S_Cores_Pin){
		counter++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim2){
		if(flag_s3 == true){		//SE NÃO É VERDE NO S3 -> PASSOU 0,5S -> PARA MOTOR
			HAL_TIM_Base_Stop_IT(&htim2);
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
			flag_s3 = false;
			printf("NAO E DA COR \r\n");
		}
		else{						//PASSOU 0,5S DO QUE PEÇA EST�? NO SENSOR DO MEIO
			  EXTI->IMR &= ~(EXTI_IMR_MR7);
			  flag_send_counter = true;
			  HAL_TIM_Base_Stop_IT(&htim2);		//PARA TIMER
			  flag_color = false;
			  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);		//VOLTA A ARRANCAR MOTOR
		}
	}
}


void delay_ms(uint32_t milliseconds) {
    // Este valor pode variar com base na velocidade de execução do seu sistema
    // e é necessário ajustá-lo para obter um atraso aproximado de 10 ms.
    uint32_t iterations_per_ms = 1000;

    uint32_t iterations = milliseconds * iterations_per_ms;

    for (uint32_t i = 0; i < iterations; i++) {
        // Faz alguma operação envolvendo a variável 'i' (que não altera o resultado final)
        uint32_t temp = i * 2;
        temp /= 3;
        temp += 1;
        temp -= 1;
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
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
  HAL_GPIO_WritePin(S_Braco_GPIO_Port, S_Braco_Pin, GPIO_PIN_RESET);
  flag_S2 = false;
  EXTI->IMR &= ~(EXTI_IMR_MR7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("ready \r\n");
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(flag_send_counter == true){
		  flag_send_counter = false;
		  printf("counter: %d \n\r", counter);
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
