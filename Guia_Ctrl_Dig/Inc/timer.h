/*
 * timer.h
 *
 *  Created on: Mar 29, 2023
 *      Author: TESTER
 */

#ifndef TIMER_H_
#define TIMER_H_

float frequencia_desejada, prescaler, preload;
extern float periodo_desejado;
long int clock_frequency = 108000000;
float max_preload = 65535;

void change_timer2(int periodo);
void change_timer5(int periodo);

#endif /* SRC_TIMER_H_ */
