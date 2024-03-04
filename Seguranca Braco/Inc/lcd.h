/*
 * lcd.h
 *
 *  Created on: Apr 5, 2023
 *      Author:
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_

#include "stm32f7xx_hal.h"

void I2C_Scan();
void LCD_SendCommand(uint8_t lcd_addr, uint8_t cmd);
void LCD_SendData(uint8_t lcd_addr, uint8_t data);
void LCD_Init(uint8_t lcd_addr);
void LCD_SendString(uint8_t lcd_addr, char *str);
void init();
void printlcd(int row, char *str);
void clearlcd();

#endif /* SRC_LCD_H_ */
