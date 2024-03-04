#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "keyPad.h"

uint8_t StateCol=1, flag_pressed=0, col=0, row=0;
bool flag_it = false;
extern uint8_t index_key[4], index_pass;
extern bool flag_enter_pass, flag_delete_last_ch;

void CycleCol(){

    if(StateCol==1)
    {
          HAL_GPIO_WritePin(col3_port, col3, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(col1_port, col1, GPIO_PIN_SET);
          HAL_Delay(50);
    }
    else if(StateCol==2)
    {
          HAL_GPIO_WritePin(col1_port, col1, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(col2_port, col2, GPIO_PIN_SET);
          HAL_Delay(50);
    }
    else if(StateCol==3)
    {
          HAL_GPIO_WritePin(col2_port, col2, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(col3_port, col3, GPIO_PIN_SET);
          HAL_Delay(50);
    }
}

void read(){
    HAL_GPIO_WritePin(col1_port, col1, GPIO_PIN_SET);

    if(HAL_GPIO_ReadPin(row1_port, row1)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=1;
			index_pass++;
    	}
    	printf("BOTAO CARREGADO: 1\r\n");
    }
    else if(HAL_GPIO_ReadPin(row2_port, row2)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
    		index_key[index_pass]=4;
    		index_pass++;
    	}
    	printf("BOTAO CARREGADO: 4\r\n");
    }
    else if(HAL_GPIO_ReadPin(row3_port, row3)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=7;
			index_pass++;
    	}
        printf("BOTAO CARREGADO: 7\r\n");
    }
    else if(HAL_GPIO_ReadPin(row4_port, row4)==GPIO_PIN_SET)
    {
    	flag_delete_last_ch = true;
        printf("BOTAO CARREGADO: *\r\n");
    }

    HAL_GPIO_WritePin(col_1_GPIO_Port, col_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(col_2_GPIO_Port, col_2_Pin, GPIO_PIN_SET);

    if(HAL_GPIO_ReadPin(row_1_GPIO_Port, row_1_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=2;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 2\r\n");
    }
    else if(HAL_GPIO_ReadPin(row_2_GPIO_Port, row_2_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=5;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 5\r\n");
    }
    else if(HAL_GPIO_ReadPin(row_3_GPIO_Port, row_3_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=8;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 8\r\n");
    }
    else if(HAL_GPIO_ReadPin(row_4_GPIO_Port, row_4_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=0;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 0\r\n");
    }

    HAL_GPIO_WritePin(col_2_GPIO_Port, col_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(col_3_GPIO_Port, col_3_Pin, GPIO_PIN_SET);

    if(HAL_GPIO_ReadPin(row_1_GPIO_Port, row_1_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=3;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 3\r\n");
    }
    else if(HAL_GPIO_ReadPin(row_2_GPIO_Port, row_2_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=6;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 6\r\n");
    }
    else if(HAL_GPIO_ReadPin(row_3_GPIO_Port, row_3_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass < 4){
			index_key[index_pass]=9;
			index_pass++;
		}
        printf("BOTAO CARREGADO: 9\r\n");
    }
    else if(HAL_GPIO_ReadPin(row_4_GPIO_Port, row_4_Pin)==GPIO_PIN_SET)
    {
    	if(index_pass == 4){
    		flag_enter_pass = true;
    	}

        printf("BOTAO CARREGADO: #\r\n");
    }

    HAL_GPIO_WritePin(col_3_GPIO_Port, col_3_Pin, GPIO_PIN_RESET);
}
