#include "control.h"

#define MAX_SHIFT 180
#define MIN_SHIFT -180

uint16_t periodValue = 0;
bool power_positive = true;


/**
  * @brief  DAB control.
  *
  * @param  None
  *
  * @retval None
  */
void control(void){


}

/**
  * @brief  Initialization and start of the control PWM's and Timers.
  *
  * @param  None
  *
  * @retval None
  */
void control_init(void){

	// CLK @ 280MHz -> f = 50 KHz & Duty = 50% -> default values(center-aligned mode 1)
	uint16_t prescaler1  = 1,counterPeriod1  = 1399;
	uint16_t prescaler2  = 1,counterPeriod2  = 1399;
	uint16_t prescaler3  = 1,counterPeriod3  = 1399;
	uint16_t prescaler4  = 1,counterPeriod4  = 1399;
	uint16_t prescaler5  = 1,counterPeriod5  = 1399;
	uint16_t prescaler8  = 1,counterPeriod8  = 1399;
	uint16_t prescaler15 = 1,counterPeriod15 = 1399;

	  tim1_init(prescaler1,counterPeriod1);
	  tim2_init(prescaler2,counterPeriod2);
	  tim3_init(prescaler3,counterPeriod3);
	  tim4_init(prescaler4,counterPeriod4);
	  tim5_init(prescaler5,counterPeriod5);
	  tim8_init(prescaler8,counterPeriod8);
	  tim15_init(prescaler15,counterPeriod15);

	  set_duty(1, 50);
	  set_duty(2, 50);
	  set_duty(3, 50);
	  set_duty(4, 50);

	  shift_phase(1, 0);
	  shift_phase(2, 45);
	  shift_phase(3, 0);

	  tim_pwm_start();
}


/**
  * @brief  Stops the PWM generation.
  *
  * @param  None
  *
  * @retval None
  */
void tim_pwm_stop(){
		//MASTER REF
		HAL_TIM_PWM_Stop   (&htim1,TIM_CHANNEL_1);
		HAL_TIM_OC_Stop    (&htim1,TIM_CHANNEL_2);

		//AUX1
		HAL_TIM_OC_Stop    (&htim2,TIM_CHANNEL_1);

		//AUX2
		HAL_TIM_OC_Stop    (&htim4,TIM_CHANNEL_1);

		//PWM1/2
		HAL_TIM_PWM_Stop   (&htim3,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop   (&htim3,TIM_CHANNEL_3);
		HAL_TIM_OC_Stop    (&htim3,TIM_CHANNEL_2);

		//PWM3/4
		HAL_TIM_PWM_Stop	(&htim15,TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Stop(&htim15,TIM_CHANNEL_1);

		//PWM5/6
		HAL_TIM_PWM_Stop   (&htim5,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop   (&htim5,TIM_CHANNEL_4);
		HAL_TIM_OC_Stop    (&htim5,TIM_CHANNEL_2);

		//PWM7/8
		HAL_TIM_PWM_Stop   (&htim8,TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Stop(&htim8,TIM_CHANNEL_2);
}

/**
  * @brief  Starts the PWM generation.
  *
  * @param  None
  *
  * @retval None
  */
void tim_pwm_start(){

		//MASTER REF
		HAL_TIM_PWM_Start   (&htim1,TIM_CHANNEL_1);
		HAL_TIM_OC_Start    (&htim1,TIM_CHANNEL_2);

		//AUX1
		HAL_TIM_OC_Start	(&htim2,TIM_CHANNEL_1);

		//AUX2
		HAL_TIM_OC_Start    (&htim4,TIM_CHANNEL_1);

		//PWM1/2
		HAL_TIM_PWM_Start   (&htim3,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start	(&htim3,TIM_CHANNEL_3);
		HAL_TIM_OC_Start    (&htim3,TIM_CHANNEL_2);

		//PWM3/4
		HAL_TIM_PWM_Start	(&htim15,TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);

		//PWM5/6
		HAL_TIM_PWM_Start   (&htim5,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start   (&htim5,TIM_CHANNEL_4);
		HAL_TIM_OC_Start    (&htim5,TIM_CHANNEL_2);

		//PWM7/8
		HAL_TIM_PWM_Start	(&htim8,TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Start(&htim8,TIM_CHANNEL_2);
}


/**
  * @brief  Initialization of TIM1 and the AUX Timers.
  *
  * @param  Prescaler(PSC)
  * @param  Period(ARR)
  *
  * @note   Sets the frequency of the timer depending on
  * 		  SYSCLK frequency and BUS(APBx) prescaler.
  * 		  e.g. -> SYSCLK @ 280MHz -> APB2 Timer Clocks @ 140MHz (PSC = 1/2) -> freq = 50KHz
  *
  * @retval None
  */
void tim1_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD (&htim1,period);
	__HAL_TIM_SET_PRESCALER  (&htim1,prescaler);

	  //DUTY CYCLE - MASTER REF
	  TIM1  -> CCR1 = 699;

	  //PHASE-SHIFT - MASTER REF
	  TIM1 -> CCR2 = 1;
}

void tim2_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD(&htim2,period);
	__HAL_TIM_SET_PRESCALER(&htim2,prescaler);
}

void tim3_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD(&htim3,period);
	__HAL_TIM_SET_PRESCALER(&htim3,prescaler);
}

void tim4_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD(&htim4,period);
	__HAL_TIM_SET_PRESCALER(&htim4,prescaler);
}

void tim5_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD(&htim5,period);
	__HAL_TIM_SET_PRESCALER(&htim5,prescaler);
}

void tim8_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD(&htim8,period);
	__HAL_TIM_SET_PRESCALER(&htim8,prescaler);
}

void tim15_init(uint16_t prescaler, uint16_t period){
	__HAL_TIM_SET_AUTORELOAD(&htim15,period);
	__HAL_TIM_SET_PRESCALER(&htim15,prescaler);
}


/**
  * @brief  Configures the duty cycles.
  *
  * @param  Complementary PWM waves to set.
  * @param  Duty value in percentage (0-100%)
  *
  * @note   Can be adjusted "on the fly" doesn't need stoppage of PWM
  *
  * @retval None
  */
void set_duty(uint8_t pwm,uint8_t duty_value){

	uint16_t duty = 0;

	switch(pwm){
		//PWM1 + PWM2
		case 1:
			periodValue = TIM3 -> ARR;
			duty = (periodValue * duty_value)/100;

			TIM3  -> CCR1 = duty;
			TIM3  -> CCR3 = duty;
			break;
		//PWM3 + PWM4
		case 2:
			periodValue = TIM15 -> ARR;
			duty = (periodValue * duty_value)/100;

			TIM15 -> CCR1 = duty;
			break;
		//PWM5 + PWM6
		case 3:
			periodValue = TIM5 -> ARR;
			duty = (periodValue * duty_value)/100;

			TIM5 -> CCR1 = duty;
			TIM5  -> CCR4 = duty;
			break;
		//PWM7 + PWM8
		case 4:
			periodValue = TIM8 -> ARR;
			duty = (periodValue * duty_value)/100;

			TIM8  -> CCR2 = duty;
			break;
		//ERROR
		default:
			break;
	}
}


/**
  * @brief  Configures the shift fase in degrees.
  *
  * @param  PWM wave to configure.
  * @param  Phase-shift value in degrees from 0-180º
  *
  * @note	__HAL_TIM_SET_COMPARE cant be loaded with a value of 0
  * 			if value == 0 -> the pwm's will not be aligned
  *
  * @retval None
  */
void shift_phase(uint8_t pwm, int16_t phase_shift){

	uint16_t x_degrees = 0;

	//Number validations
	if(phase_shift > MAX_SHIFT){
		phase_shift = MAX_SHIFT;
	}
	else if(phase_shift < MIN_SHIFT){
		phase_shift = MIN_SHIFT;
	}
	else if(phase_shift == 0){
		phase_shift = 1;
	}
	else if(phase_shift > 0){
		power_positive = true;
	}
	else if(phase_shift < 0){
		power_positive = false;
	}

	phase_shift = abs(phase_shift);

	switch(pwm){
		//PHASE-SHIFT BETWEEN PWM1/2 & PWM5/6 -> POSSIBILITY OF POSITIVE/NEGATIVE PHASE SHIFT
		case 1:
			if(power_positive){
				periodValue = TIM4 -> ARR;
				x_degrees = ((phase_shift * periodValue)/180);

				TIM2 -> CCR1 = 1;
				TIM4 -> CCR1 = x_degrees;

			} else {
				periodValue = TIM2 -> ARR;
				x_degrees = ((phase_shift * periodValue)/180);

				TIM4 -> CCR1 = 1;
				TIM2 -> CCR1 = x_degrees;
			}
			break;
		//PHASE-SHIFT BETWEEN PWM1/2 & PWM3/4
		case 2:
			periodValue = TIM3 -> ARR;
			x_degrees = ((phase_shift * periodValue)/180);

			TIM3 -> CCR2 = x_degrees;
			break;

		//PHASE-SHIFT BETWEEN PWM5/6 & PWM7/8
		case 3:
			periodValue = TIM5 -> ARR;
			x_degrees = ((phase_shift * periodValue)/180);

			TIM5 -> CCR2 = x_degrees;
			break;

		default:
			break;
	}
}

/**
  * @brief  Configures the dead-time for TIM1, TIM8 & TIM15. -> ***  NOT USED  ***
  *
  * @param  Dead-time value
  *
  * @note   Changing the dead-time through the struct might need stoppage of PWM
  * 		(see the possibility to change directly through DTG[7:0] of TIMx_BDTR register)
  * 		It only works on timers that have the DTG byte -> advanced timers
  *
  * @retval None
  */
void insert_deadTime(uint8_t deadTime){

	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	//DEAD TIME INSERTION -> TIM1 + TIM8 + TIM15

	  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	  sBreakDeadTimeConfig.DeadTime = deadTime;
	  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	  sBreakDeadTimeConfig.BreakFilter = 0;
	  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	  sBreakDeadTimeConfig.Break2Filter = 0;
	  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

		if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
		{
			Error_Handler();
		}
		if (HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sBreakDeadTimeConfig) != HAL_OK)
		{
			Error_Handler();
		}
}
