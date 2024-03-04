#ifndef MONITOR_H_
#define MONITOR_H_


//--------- GLOBAL VARIABLES --------- in definitions.h
// Discrete Inputs/Outputs:
extern uint8_t OUT1, OUT2, OUT3, OUT4;

extern uint8_t IN1, IN2, IN3, IN4;

extern uint8_t LED1, LED2, LED3, LED4;

extern uint8_t RESET1, RESET2;

extern uint8_t RESET_FEEDBACK1, RESET_FEEDBACK2;

extern uint8_t FAULT1, FAULT2;

extern uint8_t EN1, EN2, EN3, EN4, EN5, EN6, EN7, EN8;

/* This routine sets various flags based on measured values,
 * discrete data (driver status, input status)
 *
 */

void monitor(void){

			//READ INPUTS

	IN1 = HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin);
	IN2 = HAL_GPIO_ReadPin(IN2_GPIO_Port, IN2_Pin);
	IN3 = HAL_GPIO_ReadPin(IN3_GPIO_Port, IN3_Pin);
	IN4 = HAL_GPIO_ReadPin(IN4_GPIO_Port, IN4_Pin);

	RESET_FEEDBACK1 = HAL_GPIO_ReadPin(RESET_FB1_GPIO_Port, RESET_FB1_Pin);
	RESET_FEEDBACK2 = HAL_GPIO_ReadPin(RESET_FB2_GPIO_Port, RESET_FB2_Pin);

	FAULT1 = HAL_GPIO_ReadPin(FAULT1_GPIO_Port, FAULT1_Pin);
	FAULT2 = HAL_GPIO_ReadPin(FAULT2_GPIO_Port, FAULT2_Pin);

				//WRITE OUTPUTS

	//OUTPUT BUS
	HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, OUT1);
	HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, OUT2);
	HAL_GPIO_WritePin(OUT3_GPIO_Port, OUT3_Pin, OUT3);
	HAL_GPIO_WritePin(OUT4_GPIO_Port, OUT4_Pin, OUT4);

	//LEDS BUS
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, LED1);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, LED2);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, LED3);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, LED4);

	//RESET BUS
	HAL_GPIO_WritePin(RESET1_GPIO_Port, RESET1_Pin, RESET1);
	HAL_GPIO_WritePin(RESET2_GPIO_Port, RESET2_Pin, RESET2);

	//ENABLE BUS
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, EN1);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, EN2);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, EN3);
	HAL_GPIO_WritePin(EN4_GPIO_Port, EN4_Pin, EN4);
	HAL_GPIO_WritePin(EN5_GPIO_Port, EN5_Pin, EN5);
	HAL_GPIO_WritePin(EN6_GPIO_Port, EN6_Pin, EN6);
	HAL_GPIO_WritePin(EN7_GPIO_Port, EN7_Pin, EN7);
	HAL_GPIO_WritePin(EN8_GPIO_Port, EN8_Pin, EN8);
}

void monitor_init(void){

	//INPUTS
	IN1    = 0, IN2    = 0, IN3 = 0,  IN4   = 0;
	FAULT1 = 0, FAULT2 = 0;
	RESET_FEEDBACK1    = 0, RESET_FEEDBACK2 = 0;

	//OUTPUTS
	RESET1 = 0, RESET2 = 0;
	OUT1   = 0, OUT2   = 0, OUT3 = 0, OUT4 = 0;
	LED1   = 0, LED2   = 0, LED3 = 0, LED4 = 0;
	EN1    = 0, EN2    = 0, EN3  = 0, EN4  = 0, EN5 = 0, EN6 = 0, EN7 = 0, EN8 = 0;

}

#endif
