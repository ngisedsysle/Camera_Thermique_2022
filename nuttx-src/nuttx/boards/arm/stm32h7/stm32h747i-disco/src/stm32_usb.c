/****************************************************************************
 * boards/arm/stm32h7/stm32h747i-disco/src/stm32_usb.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/kthread.h>
#include <nuttx/usb/usbdev.h>
#include <nuttx/usb/usbhost.h>
#include <nuttx/usb/usbdev_trace.h>

#include "arm_internal.h"
#include "chip.h"
#include "stm32_gpio.h"
#include "stm32_otg.h"
#include "stm32h747i-disco.h"

#ifdef CONFIG_STM32H7_OTGHS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_USBDEV) || defined(CONFIG_USBHOST)
#  define HAVE_USB 1
#else
#  warning "CONFIG_STM32H7_OTGHS is enabled but neither CONFIG_USBDEV nor CONFIG_USBHOST"
#  undef HAVE_USB
#endif

#ifndef CONFIG_STM32H747XI_DISCO_USBHOST_PRIO
#  define CONFIG_STM32H747XI_DISCO_USBHOST_PRIO 100
#endif

#ifndef CONFIG_STM32H747XI_DISCO_USBHOST_STACKSIZE
#  define CONFIG_STM32H747XI_DISCO_USBHOST_STACKSIZE 1024
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_USBHOST
static struct usbhost_connection_s *g_usbconn;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_usbinitialize very early in inialization to setup
 *   USB-related GPIO pins for the STM32H747I DISCO board.
 *
 ****************************************************************************/

void stm32_usbinitialize(void)
{
  /* Configure the Overcurrent GPIO */

#ifdef CONFIG_STM32H7_OTGHS
  // stm32_configgpio(GPIO_OTGHS_OVER);
  // stm32_configgpio(GPIO_OTGHS_VBUS);
  stm32_configgpio(GPIO_OTGHS_DP);
#endif
}

/****************************************************************************
 * Name:  stm32_usbpullup
 *
 * Description:
 *   If USB is supported and the board supports a pullup via GPIO (for USB
 *   software connect and disconnect), then the board software must provide
 *   stm32_pullup. See include/nuttx/usb/usbdev.h for additional description
 *   of this method. Alternatively, if no pull-up GPIO the following EXTERN
 *   can be redefined to be NULL.
 *
 ****************************************************************************/

int stm32_usbpullup(struct usbdev_s *dev, bool enable)
{
  usbtrace(TRACE_DEVPULLUP, (uint16_t)enable);
  stm32_gpiowrite(GPIO_OTGHS_DP, enable);
  return OK;
}

/****************************************************************************
 * Name:  stm32_usbsuspend
 *
 * Description:
 *   Board logic must provide the stm32_usbsuspend logic if the USBDEV driver
 *   is used.  This function is called whenever the USB enters or leaves
 *   suspend mode. This is an opportunity for the board logic to shutdown
 *   clocks, power, etc. while the USB is suspended.
 *
 ****************************************************************************/

void stm32_usbsuspend(struct usbdev_s *dev, bool resume)
{
  uinfo("resume: %d\n", resume);
}

#endif /* CONFIG_STM32_OTGHS */
