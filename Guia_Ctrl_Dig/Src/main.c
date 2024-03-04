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
#include <string.h>
#include <stdbool.h>
#include "parsing.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char buffer[255];		//buffer will store message received by UART
char *part1, *part2, *part3, *part4;
int part_2 = 0, part_3 = 0, part_4 = 0;
int num_spaces = 0;
int inc_pos = 0, sampling_number, position_number;;
float periodo_desejado;
float pos_rad = 0;
int U_sat_a = 6;
int U_sat_b = -6;
float Kp, Ki, Kd;
float y = 0, e = 0, u_d = 0, u = 0, e_ant = 0, y_ant = 0, u_d_ant = 0, sum_e_bkp = 0, sum_e = 0;
float yr = 0, a = 0, Kd_h = 0, Kp_h = 0, Ki_h = 0;
int periodo_amostragem_pid = 0;
volatile float speed;
int voltage_fw = 0,voltage_rv = 0, value = 0, counter = 0, inc_speed = 0;
bool forward = true;
bool flag_geral = false;
static int indice = 0;
int buffer_swv[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIMER_MAX_VALUE 540
#define PI 3.141592
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
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
void reset_position(){
    inc_pos = 0;
}

void control_system(int dig){


	switch(dig){
		case 0:
			//MODO DE RESET
			printf("0 \r\n");
			break;
		case 1:
			//MODO MANUAL
			printf("1 \r\n");
			break;
		case 2:
			//MODO DE ENSAIO EXPERIMENTAL
			printf("2 \r\n");
			value = 75;
			normalize_voltage(value);
			sampling_number = 6;
			position_number = 6;
			HAL_Delay(3000);
			value = 0;
			normalize_voltage(value);
			HAL_Delay(3000);
			break;
		case 3:
			//MODO AUTOMATICO
			printf("3 \r\n");
			break;

		default:
			printf("Valor inserido invalido! \r\n");
			break;
	}
}

void enable_system(int dig){

	if(dig == 0){
		  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
		  HAL_NVIC_DisableIRQ(TIM2_IRQn);
		  HAL_NVIC_DisableIRQ(TIM3_IRQn);
		  HAL_NVIC_DisableIRQ(TIM4_IRQn);
		  HAL_NVIC_DisableIRQ(TIM5_IRQn);
		  flag_geral = false;

	}
	else if(dig == 1){
		  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		  HAL_NVIC_EnableIRQ(TIM2_IRQn);
		  HAL_NVIC_EnableIRQ(TIM3_IRQn);
		  HAL_NVIC_EnableIRQ(TIM4_IRQn);
		  HAL_NVIC_EnableIRQ(TIM5_IRQn);
		  flag_geral = true;
	}


}

void RPM_speed(){
	float rpm_speed = ((speed * 60) / 960);
	printf("speed: %f \n\r", speed);
	printf("A velocidade do motor e: %f RPM \r\n", rpm_speed);
}

void speed_calculator(){
	float speed_aux = (((inc_speed) * 2 * PI) / 960);
	speed = speed_aux/periodo_desejado;
	inc_speed = 0;
}

void define_sampling(int sampling_number_uart){
	  sampling_number = sampling_number_uart;
	  HAL_TIM_Base_Start_IT(&htim3);
}

void define_timer_value(int timer_value){
	change_timer2(timer_value);
	printf("Atualizou periodo de amostragem do timer2 para: %f \n\r", periodo_desejado);
	HAL_TIM_Base_Start_IT(&htim2);
}

void normalize_voltage(int value){

    //0-100 -> FW
    //-100-0 -> RV

    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_OC_InitTypeDef sConfigOC_RV = {0};

    if(value < 0){
        forward = false;
    } else {
        forward = true;
    }


     if (forward == true){

    	 value = 100 - value;

         voltage_fw = ((value) * TIMER_MAX_VALUE); //53999 -> 100% (TIMER_MAX_VALUE)
         voltage_rv = 0;

         HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
         HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);

         sConfigOC.OCMode = TIM_OCMODE_PWM1;
         sConfigOC.Pulse = voltage_fw;
         sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
         sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

         HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);
         HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

     }

     else if (forward == false){

    	 value = 100 + value;

         voltage_rv = ((abs(value)) * TIMER_MAX_VALUE); //50000 -> 100%
         voltage_fw = 0;

         HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
         HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);

         sConfigOC_RV.OCMode = TIM_OCMODE_PWM1;
         sConfigOC_RV.Pulse = voltage_rv;
         sConfigOC_RV.OCPolarity = TIM_OCPOLARITY_HIGH;
         sConfigOC_RV.OCFastMode = TIM_OCFAST_DISABLE;

         HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC_RV, TIM_CHANNEL_4);
         HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

     }
}

void pid_sampling(int periodo){
	change_timer5(periodo);
	periodo_amostragem_pid = periodo_desejado;
	printf("Atualizou periodo de amostragem do PID para: %f \n\r", periodo_desejado);
	HAL_TIM_Base_Start_IT(&htim5);
}

void pid_define_pos(int position){
	yr = position;
}

void pid_define_prop_gain(int prop_gain){
	Kp = prop_gain;
}

void pid_define_intg_gain(int intg_gain){
	Ki = intg_gain;
}

void pid_define_dvt_gain(int dvt_gain){
	Kd = dvt_gain;
}

void pid_define_low_filter_const(int low_filter_const){
	a = low_filter_const;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin == SENSOR_A_Pin){

		inc_speed++;
		if(HAL_GPIO_ReadPin(GPIOA, SENSOR_B_Pin) == 0){		//SENTIDO HORARIO - INCREMENTA

			inc_pos++;
			if(inc_pos == 9600){
				inc_pos = 0;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOA, SENSOR_B_Pin) == 1){ 	//SENTIDO ANTI-HORARIO - DECREMENTA

			inc_pos--;
			if(inc_pos == 9600){
				inc_pos = 0;
			}
		}
	}
	  HAL_TIM_Base_Start_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim == &htim2){
		speed_calculator();
	}

	if(htim == &htim3){
		if(sampling_number != 0){
			printf("A velocidade do motor e: %f RAD/S \r\n", speed);
			sampling_number--;
		}
		if(position_number != 0){
			pos_rad = ((inc_pos * 2 * PI) / 960);
			printf ("posicao do motor: %f \n\r", pos_rad);
			position_number--;
		}
	}
	if(htim == &htim5){
		//printf("ENTROU -> TIMER5 \r\n");

		y = buffer_swv[indice];
		indice++;
		if(indice == 20){
			indice = 0;
		}
		yr = 1;
		Kp = 0;
		Ki = 20;
		Kd = 0;

		Kp_h = Kp;
		Ki_h = (Ki*periodo_desejado) / 2;
		Kd_h = Kd * (1 - a) / periodo_desejado;

		//y = pos_rad;
		e = yr - y;

		sum_e_bkp = sum_e; //in case u becomes saturated
		sum_e = sum_e + e_ant;

		u_d = Kd_h * (y - y_ant) + a * u_d_ant;
		u = Kp_h * e + Ki_h * sum_e - u_d;
		e_ant = e;
		y_ant = y;
		u_d_ant = u_d;

		if (u > U_sat_a){ //u above upper saturation

		u = U_sat_a;
		sum_e = sum_e_bkp; //sum of errors frozen,

		}//back to last value

		else if (u < U_sat_b){ //u below lower saturation
		u = U_sat_b;
		sum_e = sum_e_bkp; //sum of errors frozen,
		}//back to last value

		//output(u); //unsaturated u remains unchanged ->  faz a saída do valor do comando, para memória, um gerador de PWM

		float duty = (u * 50) / 3;
		normalize_voltage(duty);

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
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  //HAL_TIM_Base_Start_IT(&htim5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   int index = 0;

	  printf("> \n");	//command prompt


	  //will receive command from the uart and put it in the buffer
	  while(1){

		  HAL_UART_Receive(&huart3, &buffer[index], 1, HAL_MAX_DELAY);	//HAL function that receives data via UART and inserts it on the buffer

			  if(buffer[index] == 0x0D){	//carriage return detected -> clean unused of buffer
					  memset(&buffer[index], 0, sizeof(buffer) - index);
					  break;
			  }

			  if(value >= 100){
				value = 100;
			} else if (value <= -100){
				value = -100;
			}

			if(buffer[index] == '/'){
				value += 5;
				if(value > 0){
					//canal forward -> PD14
					forward = true;
				}
				normalize_voltage(value);

			} else if(buffer[index] == '\\'){
				value -= 5;
				if(value < 0){
					//canal reverse -> PD15
					forward = false;
				}
				normalize_voltage(value);
				}

			  if(buffer[index] == ' '){		//counter number of spaces
				  num_spaces++;
			  }

			  if(buffer[index] == 0x08){//BACKSPACE command -> clear the last character
				  buffer[index] = 0;
				  if(buffer[index - 1] == ' '){
					  num_spaces--;
				  }

				  buffer[index - 1] = 0;
				  index = index - 2;
			  }
				  index++;	//goes through buffer
		  }


	  	  //divide_convert(&part1,&part2,&part3,&part4);

	  	  //PARSING
	  	  decide_function(part1,part2,part3,part4,num_spaces);
	  	  num_spaces = 0;
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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* TIM4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM4_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
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
