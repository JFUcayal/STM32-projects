#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "tim.h"


void tim1_init(uint16_t psc, uint16_t arr);
void tim2_init(uint16_t psc, uint16_t arr);
void tim3_init(uint16_t psc, uint16_t arr);
void tim4_init(uint16_t psc, uint16_t arr);
void tim5_init(uint16_t psc, uint16_t arr);
void tim8_init(uint16_t psc, uint16_t arr);
void tim15_init(uint16_t psc, uint16_t arr);

void tim_pwm_start();
void tim_pwm_stop();

void control_init(void);
void control(void);

void shift_phase(uint8_t pwm, int16_t phase_shift);
void set_duty(uint8_t pwm,uint8_t duty_value);
void insert_deadTime(uint8_t deadTime);

#endif
