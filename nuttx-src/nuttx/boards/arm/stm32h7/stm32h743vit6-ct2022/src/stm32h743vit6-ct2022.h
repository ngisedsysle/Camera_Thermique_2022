/****************************************************************************
 * boards/arm/stm32h7/stm32h743vit6-ct2022/src/stm32h743vit6-ct2022.h
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

#ifndef __BOARDS_ARM_STM32H7_STM32H743VIT6_CT2022_SRC_STM32H743VIT6_CT2022_H
#define __BOARDS_ARM_STM32H7_STM32H743VIT6_CT2022_SRC_STM32H743VIT6_CT2022_H

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

#define HAVE_PROC 1
#define HAVE_USBDEV 1
#define HAVE_USBHOST 1
#define HAVE_USBMONITOR 1

/* Can't support USB host or device features if USB OTG FS is not enabled */

#ifndef CONFIG_STM32H7_OTGFS
#undef HAVE_USBDEV
#undef HAVE_USBHOST
#endif

/* Can't support USB device if USB device is not enabled */

#ifndef CONFIG_USBDEV
#undef HAVE_USBDEV
#endif

/* Can't support USB host is USB host is not enabled */

#ifndef CONFIG_USBHOST
#undef HAVE_USBHOST
#endif

/* Check if we should enable the USB monitor before starting NSH */

#ifndef CONFIG_USBMONITOR
#undef HAVE_USBMONITOR
#endif

#ifndef HAVE_USBDEV
#undef CONFIG_USBDEV_TRACE
#endif

#ifndef HAVE_USBHOST
#undef CONFIG_USBHOST_TRACE
#endif

#if !defined(CONFIG_USBDEV_TRACE) && !defined(CONFIG_USBHOST_TRACE)
#undef HAVE_USBMONITOR
#endif

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#ifdef CONFIG_NSH_PROC_MOUNTPOINT
#define STM32_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#else
#define STM32_PROCFS_MOUNTPOINT "/proc"
#endif
#endif

/* Check if we can support the RTC driver */

#define HAVE_RTC_DRIVER 1
#if !defined(CONFIG_RTC) || !defined(CONFIG_RTC_DRIVER)
#undef HAVE_RTC_DRIVER
#endif

/* LED
 *
 * LD1 Red     PC0
 */

#define GPIO_LD1 (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                  GPIO_OUTPUT_CLEAR | GPIO_PORTC | GPIO_PIN0)

#define GPIO_LED_RED GPIO_LD1

#define LED_DRIVER_PATH "/dev/userleds"

/* USB OTG FS
 *
 * PA9  OTG_FS_VBUS VBUS sensing (also connected to the green LED)
 */

#define GPIO_OTGFS_VBUS (GPIO_INPUT | GPIO_FLOAT | GPIO_SPEED_100MHz | \
                         GPIO_OPENDRAIN | GPIO_PORTA | GPIO_PIN9)


/* LEPTON2.5 GUI Discrete I/O (See README.txt for details):
 *
 * ------ --------   --------------------
 * NAME   LEPTON2.5     STM32
 * ------ --------   --------------------
 * CS       Pin 10     PE11
 * SCK      Pin  7     PE12
 * MOSI     Pin  9     GND(NC)
 * MISO     Pin  12    PE13
 *
 *
 * SCL      Pin   8    PD12
 * SDA      Pin   5    PD13
 *
 * NRST     Pin  17    PB0
 * PWDN     Pin  20    PB1
 */

#if defined(CONFIG_CAMERA_LEPTON)
#define LEPTON_SPIBUS 4
#define LEPTON_I2CBUS 4
#define GPIO_LEPTON_CS (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN11)
#define GPIO_LEPTON_RST (GPIO_INPUT | GPIO_FLOAT | \
                         GPIO_EXTI | GPIO_PORTB | GPIO_PIN0)
#define GPIO_LEPTON_PD (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_CLEAR | GPIO_PORTB | GPIO_PIN1)
#endif

/* FT80x GUI Discrete I/O (See README.txt for details):
 *
 * ------  --------------------
 * NAME    STM32
 * ------ --------------------
 * CS    PB12
 * INT   PB15
 * PD    PD8
 */

#if defined(CONFIG_LCD_FT80X_SPI)
#define FT80X_SPIBUS 2
#define GPIO_FT80X_CS (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                       GPIO_OUTPUT_SET | GPIO_PORTB | GPIO_PIN12)
#endif
#define GPIO_FT80X_INT (GPIO_INPUT | GPIO_FLOAT | \
                        GPIO_EXTI | GPIO_PORTB | GPIO_PIN15)
#define GPIO_FT80X_PD (GPIO_OUTPUT | GPIO_OPENDRAIN | GPIO_SPEED_50MHz | \
                      GPIO_OUTPUT_CLEAR | GPIO_PORTD | GPIO_PIN8)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

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
 *   Called to configure SPI chip select GPIO pins for the
 *   Nucleo-H743ZI board.
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
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_usbinitialize very early in inialization to setup
 *   USB-related GPIO pins for the STM32H743VIT6-CT2022 board.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_OTGFS
void weak_function stm32_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host
 *   functionality. This function will start a thread that will monitor for
 *   device connection/disconnection events.
 *
 ****************************************************************************/

#if defined(CONFIG_STM32H7_OTGFS) && defined(CONFIG_USBHOST)
int stm32_usbhost_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_mcp794xx_initialize
 *
 * Description:
 *   Initialize and register the MCP794xx RTC
 *
 ****************************************************************************/

#ifdef CONFIG_RTC_MCP794XX
int stm32_mcp794xx_initialize(void);
#endif


/****************************************************************************
 * Name: stm32_pwm_setup
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

#ifdef CONFIG_PWM
int stm32_pwm_setup(void);
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
#if defined(CONFIG_LCD_FT80X_SPI) 
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

#endif /*__BOARDS_ARM_STM32H7_STM32H743VIT6_CT2022_SRC_STM32H743VIT6_CT2022_H*/
