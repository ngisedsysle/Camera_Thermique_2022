/****************************************************************************
 * boards/arm/stm32h7/stm32h747i-disco/src/stm32h747i-disco.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_ARM_STM32H7_STM32H747I_DISCO_SRC_STM32H747I_DISCO_H
#define __BOARDS_ARM_STM32H7_STM32H747I_DISCO_SRC_STM32H747I_DISCO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define STM32_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define STM32_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/* Check if we can support the RTC driver */

#define HAVE_RTC_DRIVER 1
#if !defined(CONFIG_RTC) || !defined(CONFIG_RTC_DRIVER)
#  undef HAVE_RTC_DRIVER
#endif

/* LED
 *
 * LD1 Green   PI12
 * LD2 Orange  PI13
 * LD3 Red     PI14
 * LD4 Blue    PI15
 */

#define GPIO_LD1        (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | GPIO_OUTPUT_SET | \
                         GPIO_PORTI | GPIO_PIN12)
#define GPIO_LD2        (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | GPIO_OUTPUT_SET | \
                         GPIO_PORTI | GPIO_PIN13)
#define GPIO_LD3        (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | GPIO_OUTPUT_SET | \
                         GPIO_PORTI | GPIO_PIN14)
#define GPIO_LD4        (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | GPIO_OUTPUT_SET | \
                         GPIO_PORTI | GPIO_PIN15)

#define GPIO_LED_GREEN  GPIO_LD1
#define GPIO_LED_ORANGE GPIO_LD2
#define GPIO_LED_RED    GPIO_LD3
#define GPIO_LED_BLUE   GPIO_LD4

#define LED_DRIVER_PATH "/dev/userleds"

/* BUTTONS
 *
 * The Blue pushbutton B1, labeled "Wakeup", is connected to GPIO PC13.  A
 * high value will be sensed when the button is depressed.
 *
 * Notes:
 *    1) That the EXTI is included in the definition to enable an interrupt
 *       on this IO.
 *    2) The following definitions assume the default Solder Bridges are
 *       installed.
 */

#define GPIO_BTN_USER  (GPIO_INPUT | GPIO_FLOAT | GPIO_EXTI | GPIO_PORTC | GPIO_PIN13)

/* SD/TF Card'detected pin */

#if defined(CONFIG_STM32H7_SDMMC1)
#  define HAVE_SDIO
#endif

#if defined(CONFIG_DISABLE_MOUNTPOINT) || !defined(CONFIG_MMCSD_SDIO)
#  undef HAVE_SDIO
#endif

#define GPIO_SDIO_NCD      (GPIO_INPUT|GPIO_FLOAT|GPIO_EXTI|GPIO_PORTI|GPIO_PIN8)

#define SDIO_SLOTNO        0
#define SDIO_MINOR         0


/* LEPTON2.5 GUI Discrete I/O (See README.txt for details):
 *
 * ------ --------  ------------ --------------------
 * NAME   LEPTON2.5 CONNECTOR 5    STM32
 * ------ --------  ------------ --------------------
 * CS       Pin 10    D10             PK1
 * SCK      Pin  7    D13             PK0
 * MOSI     Pin  9    GND(NC)         GND(NC)
 * MISO     Pin  12   D12             PJ11
 * 
 * 
 * SCL      Pin   8   D15             PD12
 * SDA      Pin   5   D14             PD13
 * 
 * NRST     Pin  17   D8              PJ5
 * PWDN     Pin  20   D7              PJ0
 */

#if defined(CONFIG_CAMERA_LEPTON)
#   define LEPTON_SPIBUS   5
#   define LEPTON_I2CBUS   4
#   define GPIO_LEPTON_CS   (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_SET | GPIO_PORTK | GPIO_PIN1)
#define GPIO_LEPTON_RST (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_SET | GPIO_PORTJ | GPIO_PIN5)
#   define GPIO_LEPTON_PD      (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_CLEAR | GPIO_PORTJ| GPIO_PIN0)
#endif

/* FT80x GUI Discrete I/O (See README.txt for details):
 *
 * ------ ----------- --------------------
 * NAME   VIEWTOOL    STM32
 * ------ ----------- --------------------
 * CS#    J8  Pin 12  PA4/NSS1  (For SPI1)
 * CS#    J8  Pin  6  PA11 (For SPI2)
 * INT#   J18 Pin  8  PC6
 * PD#    J18 Pin  6  PJ13
 */

#if defined(CONFIG_STM32H7_FT80X_SPI1)
#  define FT80X_SPIBUS    1
#  define GPIO_FT80X_CS   (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_SET | GPIO_PORTA | GPIO_PIN4)
#elif defined(CONFIG_STM32H7_FT80X_SPI2)
#  define FT80X_SPIBUS    2
#  define GPIO_FT80X_CS   (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_SET | GPIO_PORTA | GPIO_PIN11)
#endif
#define GPIO_FT80X_INT    (GPIO_INPUT | GPIO_FLOAT | \
                           GPIO_EXTI | GPIO_PORTC | GPIO_PIN6)
#define GPIO_FT80X_PD      (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                           GPIO_OUTPUT_CLEAR | GPIO_PORTJ| GPIO_PIN13)


/* USB OTG HS
 *
 * PB13  OTG_HS_VBUS VBUS sensing 
 * PC0  OTG_HS_PowerSwitchOn
 * PJ1  OTG_HS_Overcurrent
 */

#define GPIO_OTGHS_VBUS (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_OPENDRAIN|GPIO_PORTB|GPIO_PIN13)
//#define GPIO_OTGHS_PWRON (GPIO_OUTPUT|GPIO_OUTPUT_SET|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_PUSHPULL|GPIO_PORTC|GPIO_PIN4)
#define GPIO_OTGHS_OVER  (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_PUSHPULL|GPIO_PORTJ|GPIO_PIN1)
/****************************************************************************
 * Public Functions Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__
/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y && CONFIG_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_spidev_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the board.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_SPI
void stm32_spidev_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_adc_setup
 *
 * Description:
 *   Initialize ADC and register the ADC driver.
 *
 ****************************************************************************/

#ifdef CONFIG_ADC
int stm32_adc_setup(void);
#endif

/****************************************************************************
 * Name: stm32_dma_alloc_init
 *
 * Description:
 *   Called to create a FAT DMA allocator
 *
 * Returned Value:
 *   0 on success or -ENOMEM
 *
 ****************************************************************************/

#if defined (CONFIG_FAT_DMAMEMORY)
int stm32_dma_alloc_init(void);
#endif

/****************************************************************************
 * Name: stm32_sdio_initialize
 *
 * Description:
 *   Initialize SDIO-based MMC/SD card support
 *
 ****************************************************************************/

#ifdef HAVE_SDIO
int stm32_sdio_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_ft80x_setup
 *
 * Description:
 *   This function is called by board-bringup logic to configure the
 *   FT80x GUI device.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

#if defined(CONFIG_STM32H7_FT80X_SPI1) || defined(CONFIG_STM32H7_FT80X_SPI2)
int stm32_ft80x_setup(void);
#endif

/****************************************************************************
 * Name: stm32_lepton_setup
 *
 * Description:
 *   Initialize and register the Lepton driver.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

#if defined(CONFIG_CAMERA_LEPTON) 
int stm32_lepton_setup(void);
#endif

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Initialize the usb OTG HS driver
 *
 ****************************************************************************/

#if defined(CONFIG_STM32H7_OTGHS)
void stm32_usbinitialize(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_ARM_STM32H7_STM32H747I_DISCO_SRC_STM32H747I_DISCO_H */
