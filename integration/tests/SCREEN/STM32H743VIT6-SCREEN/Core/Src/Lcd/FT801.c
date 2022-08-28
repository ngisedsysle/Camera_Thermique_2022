#include "Lcd/FT801.h"

void FT801cmdWrite(SPI_HandleTypeDef *hspi, unsigned char ftCommand) {
	uint8_t tx[3] = { ftCommand, 0x00, 0x00 };

	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, 3, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);
}
void FT801memWrite8(SPI_HandleTypeDef *hspi, unsigned long ftAddress, unsigned char ftData8)
{
	uint8_t tx[4];

	tx[0] = (uint8_t) (ftAddress >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (ftAddress >> 8);
	tx[2] = (uint8_t) (ftAddress);
	tx[3] = (uint8_t) (ftData8);


	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, 4, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);
}

void FT801memWrite16(SPI_HandleTypeDef *hspi, unsigned long ftAddress, unsigned int ftData16)
{
	uint8_t tx[5];

	tx[0] = (uint8_t) (ftAddress >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (ftAddress >> 8);
	tx[2] = (uint8_t) (ftAddress);
	tx[3] = (uint8_t) (ftData16);
	tx[4] = (uint8_t) (ftData16 >> 8);


	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, 5, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);
}
void FT801memWrite32(SPI_HandleTypeDef *hspi, unsigned long ftAddress, unsigned long ftData32)
{
	uint8_t tx[7];

	tx[0] = (uint8_t) (ftAddress >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (ftAddress >> 8);
	tx[2] = (uint8_t) (ftAddress);
	tx[3] = (uint8_t) (ftData32);
	tx[4] = (uint8_t) (ftData32 >> 8);
	tx[5] = (uint8_t) (ftData32 >> 16);
	tx[6] = (uint8_t) (ftData32 >> 24);


	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, 7, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);
}
uint32_t FT801memWriteRAM32(SPI_HandleTypeDef *hspi, unsigned long startAddr, uint16_t *pData, int size)
{
	unsigned long addr = startAddr;
	uint16_t tx1_size = 0xFFFF;						// 3 + 0xFF - 3
	uint16_t tx2_size = tx1_size;
	uint16_t tx3_size = 3 + size - 2*65532;
	uint8_t tx[tx1_size];
	tx[0] = (uint8_t) (addr >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (addr >> 8);
	tx[2] = (uint8_t) (addr);
	for(int i = 3; i < tx1_size; i+=2, pData++){
		tx[i] = (uint8_t) (*pData);
		tx[i+1] = (uint8_t)(*pData >> 8);
	}
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, tx1_size, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

	addr += (tx1_size-1);
	pData++;
	tx[0] = (uint8_t) (addr >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (addr >> 8);
	tx[2] = (uint8_t) (addr);
	for(int i = 3; i < tx2_size; i+=2, pData++){
		tx[i] = (uint8_t) (*pData);
		tx[i+1] = (uint8_t)(*pData >> 8);
	}
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, tx2_size, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

	addr += (tx2_size-1);
	pData++;
	tx[0] = (uint8_t) (addr >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (addr >> 8);
	tx[2] = (uint8_t) (addr);
	for(int i = 3; i < tx3_size; i+=2, pData++){
		tx[i] = (uint8_t) (*pData);
		tx[i+1] = (uint8_t)(*pData >> 8);
	}
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, tx3_size, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

	return addr + size;
}
uint32_t FT801memWriteRAM8(SPI_HandleTypeDef *hspi, unsigned long startAddr, uint16_t *pData, int size)
{
	unsigned long addr = startAddr;
	uint16_t tx_size = size + 3;
	if(tx_size > 0xFFFF){    // size > 0xFFFF - address size

		return startAddr;
	}
	uint8_t tx[tx_size];
	tx[0] = (uint8_t) (addr >> 16) | MEM_WRITE;
	tx[1] = (uint8_t) (addr >> 8);
	tx[2] = (uint8_t) (addr);
	for(int i = 3; i < tx_size; i+=2, pData++){
		tx[i] = (uint8_t)(*pData);
		tx[i+1] = (uint8_t)(*pData >> 8);
	}
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, tx, tx_size, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

	return addr + size;
}
uint8_t FT801memRead8(SPI_HandleTypeDef *hspi, unsigned long ftAddress) {

	uint8_t tx[5] = { 0 };
	uint8_t rx[5] = { 0 };

	tx[0] = (uint8_t) (ftAddress >> 16);
	tx[1] = (uint8_t) (ftAddress >> 8);
	tx[2] = (uint8_t) (ftAddress);

	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, tx, rx, 5, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

	return rx[4];
}

uint16_t FT801memRead16(SPI_HandleTypeDef *hspi, unsigned long ftAddress)
{

	uint8_t tx[6] = { 0 };
	uint8_t rx[6] = { 0 };

	tx[0] = (uint8_t) (ftAddress >> 16);
	tx[1] = (uint8_t) (ftAddress >> 8);
	tx[2] = (uint8_t) (ftAddress);

	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, tx, rx, 6, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

  return (rx[5] << 8 | rx[4]);
}

uint32_t FT801memRead32(SPI_HandleTypeDef *hspi, unsigned long ftAddress)
{

	uint8_t tx[8] = { 0 };
	uint8_t rx[8] = { 0 };

	tx[0] = (uint8_t) (ftAddress >> 16);
	tx[1] = (uint8_t) (ftAddress >> 8);
	tx[2] = (uint8_t) (ftAddress);

	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, tx, rx, 8, 0xffffff);
	HAL_GPIO_WritePin(GPIOB, FT801_CS_N, GPIO_PIN_SET);

  return (rx[7] << 24 | rx[6] << 16 | rx[5] << 8 | rx[4]);
}
