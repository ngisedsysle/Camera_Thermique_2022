/*
 * mode.c
 *
 *  Created on: 17 mai 2022
 *      Author: hulot
 */

#include "mode.h"

Lepton lepton;


void MODE_init(Mode mode, UART_HandleTypeDef *ptrhuart, SPI_HandleTypeDef *ptrhspi, I2C_HandleTypeDef *ptrhi2c )
{
	  LEPTON_Init(&lepton,ptrhspi, ptrhi2c);
	  if(lepton.boot){
		  if(lepton.ready)
		  {
			 uint8_t msg[] = "TEST OK\r\n";
			 HAL_UART_Transmit(ptrhuart, msg, strlen(msg), 100000);
		  }
		  else{
			  uint8_t msg[] = "TEST FAIL : not ready\r\n";
			  HAL_UART_Transmit(ptrhuart, msg, strlen(msg), 100000);
		  }
	  }
	  else
	  {
		  uint8_t msg[] = "TEST FAIL : couldn't boot\r\n";
		  HAL_UART_Transmit(ptrhuart, msg, strlen(msg), 100000);
	  }
}
void MODE_run(Mode mode, UART_HandleTypeDef *ptrhuart, size_t timeout)
{
	while(timeout--){
		LEPTON_SPI_SPIReadPacket(&lepton);

		LEPTON_SPI_storePacket(&lepton);
		if(lepton.frame_complete){
			uint8_t msg[] = "TEST OK\r\n";
			HAL_UART_Transmit(ptrhuart, msg, strlen(msg), 100000);
			return ;
		}
	}
	uint8_t msg[] = "TEST FAIL\r\n";
	HAL_UART_Transmit(ptrhuart, msg, strlen(msg), 100000);

}


