/*
 * timer.c
 *
 *  Created on: Mar 29, 2023
 *      Author: TESTER
 */
#include <stdio.h>
#include "timer.h"
#include "tim.h"

void change_timer2(int periodo){

		float periodo_aux  = (float)periodo/1000;
		periodo_desejado = periodo_aux;

		prescaler = ceil(clock_frequency * periodo_desejado / max_preload);
		preload = round(clock_frequency * periodo_desejado / prescaler);

		__HAL_TIM_SET_AUTORELOAD(&htim2, preload);
		__HAL_TIM_SET_PRESCALER(&htim2, prescaler);

		//htim2.Instance->ARR = preload;
		//htim2.Instance->PSC = prescaler;

}

void change_timer5(int periodo){

		float periodo_aux  = (float)periodo/1000;
		periodo_desejado = periodo_aux;

		prescaler = ceil(clock_frequency * periodo_desejado / max_preload);
		preload = round(clock_frequency * periodo_desejado / prescaler);

		__HAL_TIM_SET_AUTORELOAD(&htim5, preload);
		__HAL_TIM_SET_PRESCALER(&htim5, prescaler);

		//htim2.Instance->ARR = preload;
		//htim2.Instance->PSC = prescaler;

}

