/*
 * mode.c
 *
 *  Created on: 17 mai 2022
 *      Author: hulot
 */

#include "mode.h"

LCD lcd;

void MODE_init(Mode mode, UART_HandleTypeDef *ptrhuart1, SPI_HandleTypeDef *ptrhspi2 )
{
	LCD_Init(&lcd, ptrhspi2);
	HAL_Delay(20);
}
void MODE_touchInterrupt(void)
{
	LCD_readTouch(&lcd);

}


