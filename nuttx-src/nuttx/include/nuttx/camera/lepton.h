#ifndef __INCLUDE_NUTTX_CAMERA_LEPTON_H_
#define __INCLUDE_NUTTX_CAMERA_LEPTON_H_
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "lepton_agc.h"
#include "lepton_rad.h"
#include "lepton_oem.h"
#include "lepton_sys.h"
#include "lepton_vid.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LEPTON_MAX_TEMP 35315
#define LEPTON_MIN_TEMP 23315

#define LEPTON_RAD_TEMPERATURE_SCALE_FACTOR 0.01 // All temperatures in degrees C are scaled by 100 1.20 is 120
#define KELVIN_OFFSET 273.15

#define LEPTON_DEVICE_ADDR 0x2A

// command operations
#define GET 0x00
#define SET 0x01
#define RUN 0x02

#define LEPTON_REG_POWER_ON 0x0000
#define LEPTON_REG_STATUS 0x0002
#define LEPTON_REG_COMMAND 0x0004
#define LEPTON_REG_DATA_LENGTH 0x0006
#define LEPTON_REG_DATA 0x0008

#define LEPTON_BUSY_MASK 0x0001
#define LEPTON_BOOT_MASK 0x0004
#define LEPTON_RESPONSE_CODE_MASK 0x00FF

#define LEPTON_N_DATA_REGISTER 16
#define LEPTON_N_DATA_BUFFER 2
#define LEPTON_DATA_BUFFER_SIZE 1024

#define LEPTON_FRAME_HEIGHT 60
#define LEPTON_FRAME_WIDTH 80
#define LEPTON_FRAME_SIZE 9600

#define LEPTON_PACKET_SIZE 164
#define LEPTON_LINE_SIZE 160
#define LEPTON_PACKETTED_FRAME_SIZE 9840
#define LEPTON_MAX_DISCARD_FRAMES 100

#define LEPTON_LINE_ID_BYTE 1
#define LEPTON_DISCARD_BYTE 0

struct lepton_config_s
{
	/* Device characterization */

	uint32_t spi_frequency; /* SPI frequency */
	uint32_t i2c_frequency; /* I2C frequency */
	uint8_t address;		/* 7-bit I2C address */

	/* IRQ/GPIO access callbacks.
	 *  These operations all hidden behind callbacks to isolate the LEPTON
	 *  driver from differences in GPIO interrupt handling by varying
	 *  boards and MCUs.
	 * Interrupts should be configured on the falling edge of nINT.
	 *
	 *   attach  - Attach the ADS7843E interrupt handler to the GPIO interrupt
	 *   enable  - Enable or disable the GPIO interrupt
	 *   clear   - Acknowledge/clear any pending GPIO interrupt as necessary.
	 *   pwrdown - Power the LEPTON up or down.
	 *   audio   - Enable audio (i.e., set the external audio amplifier
	 *             shutdown pin to the appropriate level to enable or disable
	 *             the external audio amplifier)
	 *   destroy - The driver has been unlinked. Cleanup as necessary.
	 */

	CODE int (*attach)(FAR const struct lepton_config_s *lower, xcpt_t isr,
					   FAR void *arg);
	CODE void (*enable)(FAR const struct lepton_config_s *lower, bool enable);
	CODE void (*clear)(FAR const struct lepton_config_s *lower);
	CODE void (*pwrdown)(FAR const struct lepton_config_s *lower, bool pwrdown);
	CODE void (*destroy)(FAR const struct lepton_config_s *lower);
};

typedef enum
{
	LEPTON_OK = 0,						  /* Camera ok */
	LEPTON_COMM_OK = LEPTON_OK,			  /* Camera comm ok (same as LEPTON_OK) */
	LEPTON_ERROR = -1,					  /* Camera general error */
	LEPTON_NOT_READY = -2,				  /* Camera not ready error */
	LEPTON_RANGE_ERROR = -3,			  /* Camera range error */
	LEPTON_CHECKSUM_ERROR = -4,			  /* Camera checksum error */
	LEPTON_BAD_ARG_POINTER_ERROR = -5,	  /* Camera Bad argument error */
	LEPTON_DATA_SIZE_ERROR = -6,		  /* Camera byte count error */
	LEPTON_UNDEFINED_FUNCTION_ERROR = -7, /* Camera undefined function error */
	LEPTON_FUNCTION_NOT_SUPPORTED = -8,	  /* Camera function not yet supported error */
	LEPTON_DATA_OUT_OF_RANGE_ERROR = -9,  /* Camera input DATA is out of valid range error */
	LEPTON_COMMAND_NOT_ALLOWED = -11,	  /* Camera unable to execute command due to current camera
		state */
	/* OTP access errors */
	LEPTON_OTP_WRITE_ERROR = -15,		   /*!< Camera OTP write error */
	LEPTON_OTP_READ_ERROR = -16,		   /* double bit error detected (uncorrectible) */
	LEPTON_OTP_NOT_PROGRAMMED_ERROR = -18, /* Flag read as non-zero */
	/* I2C Errors */
	LEPTON_ERROR_I2C_BUS_NOT_READY = -20,	 /* I2C Bus Error - Bus Not Avaialble */
	LEPTON_ERROR_I2C_BUFFER_OVERFLOW = -22,	 /* I2C Bus Error - Buffer Overflow */
	LEPTON_ERROR_I2C_ARBITRATION_LOST = -23, /* I2C Bus Error - Bus Arbitration Lost */
	LEPTON_ERROR_I2C_BUS_ERROR = -24,		 /* I2C Bus Error - General Bus Error */
	LEPTON_ERROR_I2C_NACK_RECEIVED = -25,	 /* I2C Bus Error - NACK Received */
	LEPTON_ERROR_I2C_FAIL = -26,			 /* I2C Bus Error - General Failure */
	/* Processing Errors */
	LEPTON_DIV_ZERO_ERROR = -80, /* Attempted div by zero */
	/* Comm Errors */
	LEPTON_COMM_PORT_NOT_OPEN = -101,	   /* Comm port not open */
	LEPTON_COMM_INVALID_PORT_ERROR = -102, /* Comm port no such port error */
	LEPTON_COMM_RANGE_ERROR = -103,		   /* Comm port range error */
	LEPTON_ERROR_CREATING_COMM = -104,	   /* Error creating comm */
	LEPTON_ERROR_STARTING_COMM = -105,	   /* Error starting comm */
	LEPTON_ERROR_CLOSING_COMM = -106,	   /* Error closing comm */
	LEPTON_COMM_CHECKSUM_ERROR = -107,	   /* Comm checksum error */
	LEPTON_COMM_NO_DEV = -108,			   /* No comm device */
	LEPTON_TIMEOUT_ERROR = -109,		   /* Comm timeout error */
	LEPTON_COMM_ERROR_WRITING_COMM = -110, /* Error writing comm */
	LEPTON_COMM_ERROR_READING_COMM = -111, /* Error reading comm */
	LEPTON_COMM_COUNT_ERROR = -112,		   /* Comm byte count error */
	/* Other Errors */
	LEPTON_OPERATION_CANCELED = -126,  /* Camera operation canceled */
	LEPTON_UNDEFINED_ERROR_CODE = -127 /* Undefined error */
} Lepton_HandleTypeDef;

int lepton_register(FAR struct i2c_master_s *i2c, FAR struct spi_dev_s *spi,
					FAR const struct lepton_config_s *lower);

#endif /* __INCLUDE_NUTTX_CAMERA_LEPTON_H */