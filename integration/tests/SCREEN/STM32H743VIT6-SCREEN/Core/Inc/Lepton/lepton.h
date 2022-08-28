#ifndef __LEPTON_H_
#define __LEPTON_H_

#include "stm32h7xx_hal.h"
#include "lepton_sys.h"
#include "lepton_agc.h"
#include "lepton_vid.h"
#include "lepton_oem.h"
#include "lepton_rad.h"

#define LEPTON_MAX_TEMP		35315
#define LEPTON_MIN_TEMP		23315

#define LEPTON_RAD_TEMPERATURE_SCALE_FACTOR         0.01         // All temperatures in degrees C are scaled by 100 1.20 is 120
#define KELVIN_OFFSET								273.15

#define LEPTON_RESET_L	GPIO_PIN_5
#define LEPTON_PWDN_L	GPIO_PIN_0
#define LEPTON_CS		GPIO_PIN_1

#define LEPTON_DEVICE_ADDR	0x2A


//command operations
#define GET 0x00
#define SET 0x01
#define RUN 0x02


#define LEPTON_REG_POWER_ON			0x0000
#define LEPTON_REG_STATUS			0x0002
#define LEPTON_REG_COMMAND			0x0004
#define LEPTON_REG_DATA_LENGTH		0x0006
#define LEPTON_REG_DATA				0x0008

#define LEPTON_MASK_BUSY			0x0001
#define LEPTON_MASK_BOOT			0x0004
#define LEPTON_MASK_RESPONSE_CODE	0x00FF

#define LEPTON_N_DATA_REGISTER		16
#define LEPTON_N_DATA_BUFFER		2
#define LEPTON_DATA_BUFFER_SIZE		1024

#define LEPTON_FRAME_HEIGHT			60
#define LEPTON_FRAME_WIDTH			80
#define LEPTON_FRAME_SIZE 			9600

#define VOSPI_PACKET_SIZE			164
#define VOSPI_FRAME_SIZE			4800
#define VOSPI_PACKETTED_FRAME_SIZE	9840
#define VOSPI_MAX_DISCARD_FRAMES	100

#define LEPTON_FRAME_NUMBER_BYTE	1
#define LEPTON_FRAME_DISCARD_BYTE	0

typedef struct{
	SPI_HandleTypeDef *hspi;
	I2C_HandleTypeDef *hi2c;
	uint16_t status_reg;
	uint8_t	response_code;
	uint16_t ptrxData[LEPTON_N_DATA_REGISTER];
	uint16_t ptrRx;
	uint16_t ptrTx;
	uint8_t boot;
	uint8_t ready;
	uint8_t vospi_packet[VOSPI_PACKETTED_FRAME_SIZE];
	uint8_t packet_number;
	uint16_t packet_offset;
//	uint16_t last_crc;
//	uint8_t new_frame;
//	uint8_t frame_number;
//	uint8_t lost_frame_counter;
	uint8_t frame_complete;
	uint16_t temp;
//	uint8_t need_resync;
}Lepton;


typedef enum{
	LEPTON_OK = 0, /* Camera ok */
	LEPTON_COMM_OK = LEPTON_OK, /* Camera comm ok (same as LEPTON_OK) */
	LEPTON_ERROR = -1, /* Camera general error */
LEPTON_NOT_READY = -2, /* Camera not ready error */
	LEPTON_RANGE_ERROR = -3, /* Camera range error */
	LEPTON_CHECKSUM_ERROR = -4, /* Camera checksum error */
	LEPTON_BAD_ARG_POINTER_ERROR = -5, /* Camera Bad argument error */
	LEPTON_DATA_SIZE_ERROR = -6, /* Camera byte count error */
	LEPTON_UNDEFINED_FUNCTION_ERROR = -7, /* Camera undefined function error */
	LEPTON_FUNCTION_NOT_SUPPORTED = -8, /* Camera function not yet supported error */
	LEPTON_DATA_OUT_OF_RANGE_ERROR = -9, /* Camera input DATA is out of valid range error */
	LEPTON_COMMAND_NOT_ALLOWED = -11, /* Camera unable to execute command due to current camera
	state */
	/* OTP access errors */
	LEPTON_OTP_WRITE_ERROR = -15, /*!< Camera OTP write error */
	LEPTON_OTP_READ_ERROR = -16, /* double bit error detected (uncorrectible) */
	LEPTON_OTP_NOT_PROGRAMMED_ERROR = -18, /* Flag read as non-zero */
	/* I2C Errors */
	LEPTON_ERROR_I2C_BUS_NOT_READY = -20, /* I2C Bus Error - Bus Not Avaialble */
	LEPTON_ERROR_I2C_BUFFER_OVERFLOW = -22, /* I2C Bus Error - Buffer Overflow */
	LEPTON_ERROR_I2C_ARBITRATION_LOST = -23, /* I2C Bus Error - Bus Arbitration Lost */
	LEPTON_ERROR_I2C_BUS_ERROR = -24, /* I2C Bus Error - General Bus Error */
	LEPTON_ERROR_I2C_NACK_RECEIVED = -25, /* I2C Bus Error - NACK Received */
	LEPTON_ERROR_I2C_FAIL = -26, /* I2C Bus Error - General Failure */
	/* Processing Errors */
	LEPTON_DIV_ZERO_ERROR = -80, /* Attempted div by zero */
	/* Comm Errors */
	LEPTON_COMM_PORT_NOT_OPEN = -101, /* Comm port not open */
	LEPTON_COMM_INVALID_PORT_ERROR = -102, /* Comm port no such port error */
	LEPTON_COMM_RANGE_ERROR = -103, /* Comm port range error */
	LEPTON_ERROR_CREATING_COMM = -104, /* Error creating comm */
	LEPTON_ERROR_STARTING_COMM = -105, /* Error starting comm */
	LEPTON_ERROR_CLOSING_COMM = -106, /* Error closing comm */
	LEPTON_COMM_CHECKSUM_ERROR = -107, /* Comm checksum error */
	LEPTON_COMM_NO_DEV = -108, /* No comm device */
	LEPTON_TIMEOUT_ERROR = -109, /* Comm timeout error */
	LEPTON_COMM_ERROR_WRITING_COMM = -110, /* Error writing comm */
	LEPTON_COMM_ERROR_READING_COMM = -111, /* Error reading comm */
	LEPTON_COMM_COUNT_ERROR = -112, /* Comm byte count error */
	/* Other Errors */
	LEPTON_OPERATION_CANCELED = -126, /* Camera operation canceled */
	LEPTON_UNDEFINED_ERROR_CODE = -127 /* Undefined error */
}Lepton_HandleTypeDef;

Lepton_HandleTypeDef LEPTON_Init(Lepton *This, SPI_HandleTypeDef *hspi, I2C_HandleTypeDef *hi2c);
Lepton_HandleTypeDef LEPTON_I2C_waitUntilReady(Lepton *This, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_waitUntilBoot(Lepton *This, uint32_t timeout);
void LEPTON_wakeup(Lepton *This);
void LEPTON_SPI_SPIReadPacket(Lepton *This);
void LEPTON_SPI_storePacket(Lepton *This);

Lepton_HandleTypeDef LEPTON_I2C_write16(Lepton *This, uint16_t register_address, uint16_t register_data, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_read16(Lepton *This, uint16_t register_address, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_readN(Lepton *This, uint16_t register_address, uint16_t dataLength, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_readStatus(Lepton *This, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_command(Lepton *This, uint8_t moduleID, uint8_t commandID, uint8_t commandType, uint16_t dataLength, uint16_t* pData, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_isReady(Lepton *This, uint32_t timeout);
Lepton_HandleTypeDef LEPTON_I2C_writeN(Lepton *This, uint16_t register_address, uint16_t dataLength, uint16_t* pData, uint32_t timeout);
void LEPTON_I2C_defaultImageProcessing(Lepton *This, uint32_t timeout);
void LEPTON_resync(Lepton *This);

#endif
