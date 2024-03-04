#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f7xx_hal.h"

#define row1 row_1_Pin
#define row1_port GPIOE
#define row2 row_2_Pin
#define row2_port GPIOE
#define row3 row_3_Pin
#define row3_port row_3_GPIO_Port
#define row4 row_4_Pin
#define row4_port GPIOE
#define col1 col_1_Pin
#define col1_port col_1_GPIO_Port
#define col2 col_2_Pin
#define col2_port GPIOG
#define col3 col_3_Pin
#define col3_port GPIOG

void CycleCol();
void read();

#endif
