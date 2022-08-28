#include "stm32h7xx_hal.h"
#include "Lcd/lcd.h"

/* Exported types ------------------------------------------------------------*/
/** @defgroup Mode_Exported_Types Mode Exported Types
  * @brief Mode Exported types
  * @{
  */

/**
  * @brief  camera display modes
  */
typedef enum{
	MODE1,		/*!< both the infrared and visible maps are displayed     */
	MODE2		/*!< only the infrared map is displayed     */
}Mode;

void MODE_init(Mode mode, UART_HandleTypeDef *ptrhuart1, SPI_HandleTypeDef *ptrhspi2);
void MODE_touchInterrupt(void);
