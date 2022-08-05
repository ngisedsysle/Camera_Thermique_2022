#include "Lepton/lepton.h"

static int new_frame = 0;
static int lost_packet_counter = 0;
static int need_resync = 0;

Lepton_HandleTypeDef LEPTON_Init(Lepton *This, SPI_HandleTypeDef *hspi, I2C_HandleTypeDef *hi2c){
	This->hspi = hspi;
	This->hi2c = hi2c;
	This->status_reg = 0;
	This->response_code = 0;
	This->ptrRx = 0;
	This->boot = 0;
	This->ready = 0;
	This->packet_number = 0;
	This->frame_complete = 0;
	This->temp = 0;
	for(int i = 0; i < VOSPI_PACKETTED_FRAME_SIZE; i++){
		This->vospi_packet[i] = 0;
	}
	for(int i = 0; i < LEPTON_N_DATA_REGISTER - 1; i++)
	{
		This->ptrxData[i] = 0;
	}
	LEPTON_wakeup(This);
	LEPTON_I2C_waitUntilBoot(This, 10);
	LEPTON_I2C_waitUntilReady(This,100);
	LEPTON_I2C_defaultImageProcessing(This,100);


}
void LEPTON_wakeup(Lepton *This){
	HAL_GPIO_WritePin(GPIOE, LEPTON_CS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, LEPTON_RESET_L, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, LEPTON_PWDN_L, GPIO_PIN_SET);
	HAL_Delay(1000);

}
Lepton_HandleTypeDef LEPTON_I2C_waitUntilBoot(Lepton *This, uint32_t timeout){
	uint32_t tmp_timeout = timeout;
	while(tmp_timeout){
		LEPTON_I2C_readStatus(This,tmp_timeout);
		if(This->boot)
		{
			return LEPTON_OK;
		}
		else
		{
			HAL_Delay(1000);
			tmp_timeout--;
		}
	}
	return LEPTON_ERROR;
}
Lepton_HandleTypeDef LEPTON_I2C_waitUntilReady(Lepton *This, uint32_t timeout){
	uint32_t tmp_timeout = timeout;
	uint32_t command_answer = 0;
	while(tmp_timeout)
	{
		if(LEPTON_I2C_command(This, SYS, LEPTON_SYS_FFC_STATUS,GET, 2, NULL, tmp_timeout) == LEPTON_COMM_OK)
		{
			command_answer = This->ptrxData[0] << 8 | This->ptrxData[1]  ;
			if(command_answer == LEPTON_SYS_STATUS_READY)
			{
				return LEPTON_OK;
			}
		}
		HAL_Delay(20);
		tmp_timeout--;
	}
	return LEPTON_NOT_READY;
}
void LEPTON_SPI_SPIReadPacket(Lepton *This)
{
	HAL_GPIO_WritePin(GPIOE, LEPTON_CS, GPIO_PIN_RESET);
	HAL_SPI_Receive(This->hspi, &(This->vospi_packet[This->packet_offset]), VOSPI_PACKET_SIZE, 16);
	HAL_GPIO_WritePin(GPIOE, LEPTON_CS, GPIO_PIN_SET);

}
void LEPTON_resync(Lepton *This)
{
	// HAL_Delay(200);
	lost_packet_counter = 0;
}
void LEPTON_SPI_storePacket(Lepton *This)
{
	if((This->vospi_packet[This->packet_offset + LEPTON_FRAME_DISCARD_BYTE ]&0xF) != 0x0F)			//discard data
	{
		if(This->vospi_packet[This->packet_offset + LEPTON_FRAME_NUMBER_BYTE] == 0)
		{
			new_frame = 1;
		}
		This->packet_number = This->vospi_packet[This->packet_offset + LEPTON_FRAME_NUMBER_BYTE];
		if(This->packet_number < LEPTON_FRAME_HEIGHT)
		{
			lost_packet_counter = 0;
		}
		else
		{
			lost_packet_counter++;
		}
		if( This->packet_number == 0x3B && new_frame)
		{
			This->frame_complete = 1;
			new_frame = 0;
		}
		This->packet_offset += VOSPI_PACKET_SIZE;	// update the number of bytes written
	}
	else if(new_frame){
		lost_packet_counter+= This->packet_number;
		new_frame = 0;
	}
	else
	{
		This->packet_offset = 0;
	}
	lost_packet_counter++;

	if(lost_packet_counter > VOSPI_MAX_DISCARD_FRAMES)
	{
		LEPTON_resync(This);
	}

}



Lepton_HandleTypeDef LEPTON_I2C_readStatus(Lepton *This, uint32_t timeout){
	Lepton_HandleTypeDef ret = LEPTON_I2C_read16(This, LEPTON_REG_STATUS, timeout);
	if(ret != LEPTON_OK)
	{
		return ret;
	}
	This->status_reg = This->ptrRx;
	This->boot = ((This->ptrRx >> 8 ) & LEPTON_MASK_BOOT) >> 2;
	This->ready = (~This->ptrRx >> 8 ) & LEPTON_MASK_BUSY;
	This->response_code = (uint8_t)This->ptrRx;

	return LEPTON_OK;
}
Lepton_HandleTypeDef LEPTON_I2C_read16(Lepton *This, uint16_t register_address, uint32_t timeout) {
	uint32_t tmp_timeout = timeout;
	while(HAL_I2C_Mem_Read(This->hi2c, LEPTON_DEVICE_ADDR << 1, register_address, I2C_MEMADD_SIZE_16BIT, &This->ptrRx, 2, 15000) != HAL_OK)
	{
		if(tmp_timeout)
		{
			uint32_t err = HAL_I2C_GetError(This->hi2c);
			tmp_timeout--;
		}
		else{
			return LEPTON_OTP_READ_ERROR;			//0xFFFF
		}
	}
	while (HAL_I2C_GetState(This->hi2c) != HAL_I2C_STATE_READY){
		if(tmp_timeout)
		{
			tmp_timeout--;
		}
		else
		{
			return LEPTON_ERROR_I2C_BUS_NOT_READY;
		}
	}
	//DBG_LOG("I2C READ16 : Slave = 0X%X, TX = 0x%X, RX = 0x%X%X\r\n", LEPTON_DEVICE_ADDR, register_address, This->ptrRx[1], *This->ptrRx);
	return LEPTON_OK;
}
Lepton_HandleTypeDef LEPTON_I2C_write16(Lepton *This, uint16_t register_address, uint16_t register_data, uint32_t timeout){

	uint32_t tmp_timeout = timeout;
	uint16_t tx = (register_data >> 8) | (register_data << 8);	//MSB first
	while(HAL_I2C_Mem_Write(This->hi2c, LEPTON_DEVICE_ADDR << 1, register_address, I2C_MEMADD_SIZE_16BIT, (uint8_t*)(&tx), 2, 15000) != HAL_OK)
	{
		if(tmp_timeout)
		{
			uint32_t err = HAL_I2C_GetError(This->hi2c);
			timeout--;
		}
		else
		{
			return LEPTON_OTP_WRITE_ERROR;
		}
	}
	while (HAL_I2C_GetState(This->hi2c) != HAL_I2C_STATE_READY)
	{
		if(tmp_timeout)
		{
			tmp_timeout--;
		}
		else
		{
			return LEPTON_ERROR_I2C_BUS_NOT_READY;
		}
	}
	 return LEPTON_OK;

}
Lepton_HandleTypeDef LEPTON_I2C_command(Lepton *This, uint8_t moduleID, uint8_t commandID, uint8_t commandType, uint16_t dataLength, uint16_t *pData, uint32_t timeout){
	uint32_t tmp_timeout = timeout;
	while(tmp_timeout)
	{
		//read status register to check the cam is ready
		if(LEPTON_I2C_isReady(This,tmp_timeout) != LEPTON_OK)
		{
			return LEPTON_NOT_READY;
		}
		//set the number of uint16_t to read/write
		if(LEPTON_I2C_write16(This, LEPTON_REG_DATA_LENGTH, dataLength, tmp_timeout) != LEPTON_OK)
		{
			return LEPTON_OTP_WRITE_ERROR;
		}
		if(commandType == SET){
			if(LEPTON_I2C_writeN(This,LEPTON_REG_DATA, dataLength, pData,tmp_timeout) != LEPTON_OK)
			{
				return LEPTON_OTP_WRITE_ERROR;
			}
		}

		uint8_t protectionBit = (moduleID == OEM ) || (moduleID == RAD ) ? 1 : 0 ;
		uint16_t command = (protectionBit << 14 | moduleID << 8) | (commandID | commandType) ;
		if(LEPTON_I2C_write16(This,LEPTON_REG_COMMAND, command, tmp_timeout) != LEPTON_OK)
		{
			return LEPTON_OTP_WRITE_ERROR;
		}
		if (LEPTON_I2C_isReady(This,tmp_timeout) != LEPTON_OK)
		{
			return LEPTON_NOT_READY;
		}
		if(commandType != RUN)
		{
			if(LEPTON_I2C_readN(This,LEPTON_REG_DATA,dataLength,tmp_timeout) != LEPTON_OK)
			{
				return LEPTON_OTP_READ_ERROR;
			}
		}
		// TODO : CRC check if available
		return LEPTON_COMM_OK;
	}
	return LEPTON_TIMEOUT_ERROR;
}
Lepton_HandleTypeDef LEPTON_I2C_isReady(Lepton *This, uint32_t timeout)
{
	Lepton_HandleTypeDef reading_error = LEPTON_I2C_readStatus(This, 10);
	if(reading_error != LEPTON_OK)
	{
		return reading_error;
	}
	else if(!(This->ready))
	{
		return LEPTON_NOT_READY;
	}
	else
	{
		return LEPTON_OK;
	}
}
Lepton_HandleTypeDef LEPTON_I2C_readN(Lepton *This, uint16_t register_address, uint16_t dataLength, uint32_t timeout) {
	uint32_t tmp_timeout = timeout;
	// set base address register current location to LEPTON_REG_DATA before reading command answer
	uint16_t tx = register_address >> 8 | register_address << 8;		//MSB first
	while(HAL_I2C_Master_Transmit(This->hi2c, LEPTON_DEVICE_ADDR << 1, (uint8_t*)(&tx), 2, 15000) != HAL_OK)
	{
		if(tmp_timeout)
		{
			uint32_t err = HAL_I2C_GetError(This->hi2c);
			tmp_timeout--;
		}
		else{
			return LEPTON_OTP_READ_ERROR;			//0xFFFF
		}
	}
	while (HAL_I2C_GetState(This->hi2c) != HAL_I2C_STATE_READY){
		if(tmp_timeout)
		{
			tmp_timeout--;
		}
		else
		{
			return LEPTON_ERROR_I2C_BUS_NOT_READY;
		}
	}
	while(HAL_I2C_Master_Receive(This->hi2c, LEPTON_DEVICE_ADDR << 1, (uint8_t*)(&This->ptrxData), dataLength << 1, 15000) != HAL_OK)
		{
		if(tmp_timeout)
		{
			uint32_t err = HAL_I2C_GetError(This->hi2c);
			tmp_timeout--;
		}
		else{
			return LEPTON_OTP_READ_ERROR;			//0xFFFF
		}
	}
	while (HAL_I2C_GetState(This->hi2c) != HAL_I2C_STATE_READY){
		if(tmp_timeout)
		{
			tmp_timeout--;
		}
		else
		{
			return LEPTON_ERROR_I2C_BUS_NOT_READY;
		}
	}
	// data are encoded in little endian...since lepton only accepts msb, we need to change the order manually (as far as i know)

	for(int i = 0; i < dataLength ; i++){
		uint16_t tmp_data;
		tmp_data = This->ptrxData[i];
		This->ptrxData[i] = ((0xFF00 & tmp_data) >> 8) | ((tmp_data & 0x00FF) << 8) ;
	}
	return LEPTON_OK;
}
void LEPTON_I2C_defaultImageProcessing(Lepton *This, uint32_t timeout){
	//AGC processing
	uint16_t set_data[2] = {0};
	set_data[0] = 0x0000;
	set_data[1] = (uint16_t)LEP_AGC_ENABLE;
	LEPTON_I2C_command(This, AGC, LEP_CID_AGC_ENABLE_STATE, SET, 2, set_data, timeout);
	set_data[1] = LEP_AGC_HEQ;
	LEPTON_I2C_command(This, AGC, LEP_CID_AGC_POLICY, SET, 2, set_data, timeout);
	set_data[1] = LEP_AGC_CALC_ENABLE ;
	LEPTON_I2C_command(This, AGC, LEP_CID_AGC_CALC_ENABLE_STATE, SET, 2, set_data, timeout);
	LEPTON_I2C_command(This, SYS, LEPTON_SYS_FPA_TEMPERATURE_KELVIN, GET, 1, NULL, timeout);
	This->temp = This->ptrxData[0];

	//////////		 GET images/sec		////////////
//	command(This, SYS, LEPTON_SYS_EXECTUE_FRAME_AVERAGE, RUN, 0, NULL, timeout);
//	command(This, SYS, LEPTON_SYS_NUM_FRAMES_TO_AVERAGE, GET, 1, NULL, timeout);
//	DBG_LOG("SYS average frames/sec = %d\r\n", This->ptrxData[0] );
}
Lepton_HandleTypeDef LEPTON_I2C_writeN(Lepton *This, uint16_t register_address, uint16_t dataLength, uint16_t* pData, uint32_t timeout) {
	uint32_t tmp_timeout = timeout;
	// set base address register current location to LEPTON_REG_DATA before reading command answer
	uint16_t tx_size = 1 + dataLength;
	uint16_t tx[tx_size];
	tx[0] = register_address >> 8 | register_address << 8;		//MSB first
	for(int i = 1; i < tx_size; i++){
		tx[i] = pData[i-1] >> 8 | pData[i-1] << 8;
	}
	while(HAL_I2C_Master_Transmit(This->hi2c, LEPTON_DEVICE_ADDR << 1, (uint8_t*)(&tx), 2*tx_size, 15000) != HAL_OK)
	{
		if(tmp_timeout)
		{
			uint32_t err = HAL_I2C_GetError(This->hi2c);
			tmp_timeout--;
		}
		else{
			return LEPTON_OTP_READ_ERROR;			//0xFFFF
		}
	}
	while (HAL_I2C_GetState(This->hi2c) != HAL_I2C_STATE_READY){
		if(tmp_timeout)
		{
			tmp_timeout--;
		}
		else
		{
			return LEPTON_ERROR_I2C_BUS_NOT_READY;
		}
	}
	return LEPTON_OK;
}

