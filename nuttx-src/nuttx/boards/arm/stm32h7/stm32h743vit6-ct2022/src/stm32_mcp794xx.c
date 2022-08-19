
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>
#include <assert.h>

#include <nuttx/i2c/i2c_master.h>
#include <nuttx/timers/rtc.h>
#include <nuttx/timers/mcp794xx.h>

#include "stm32.h"
#include "stm32_i2c.h"
#include "stm32h743vit6-ct2022.h"


#if defined(CONFIG_I2C) && defined(CONFIG_RTC_MCP794XX)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MCP7940_I2C_ADDR    0x6f /* MCP7940 I2C Address */
#define MCP794XX_I2C_BUS     2    /* MCP794XX is on I2C2 */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_MCP794XX_initialize
 *
 * Description:
 *   Initialize and configure the MCP794XX RTC
 *
 ****************************************************************************/

int stm32_mcp794xx_initialize(void)
{
  struct i2c_master_s *i2c;
  static bool initialized = false;
  int ret;

  /* Have we already initialized? */

  if (!initialized)
    {
      /* No.. Get the I2C bus driver */

      rtcinfo("Initialize I2C%d\n", MCP794XX_I2C_BUS);
      i2c = stm32_i2cbus_initialize(MCP794XX_I2C_BUS);
      if (!i2c)
        {
          rtcerr("ERROR: Failed to initialize I2C%d\n", MCP794XX_I2C_BUS);
          return -ENODEV;
        }

      /* Now bind the I2C interface to the MCP7940 RTC driver */

      rtcinfo("Bind the MCP7940 RTC driver to I2C%d\n", MCP794XX_I2C_BUS);
      ret = mcp794xx_rtc_initialize(i2c, MCP7940_I2C_ADDR);
      if (ret < 0)
        {
          rtcerr("ERROR: Failed to bind I2C%d to the MCP7940 RTC driver\n",
                 MCP794XX_I2C_BUS);
          return -ENODEV;
        }

#ifdef CONFIG_I2C_DRIVER
      /* Register the I2C to get the "nsh> i2c bus" command working */

      ret = i2c_register(i2c, MCP794XX_I2C_BUS);
      if (ret < 0)
        {
          rtcerr("ERROR: Failed to register I2C%d driver: %d\n", bus, ret);
          return -ENODEV;
        }
#endif

      /* Synchronize the system time to the RTC time */

      clock_synchronize(NULL);

      /* Now we are initialized */

      initialized = true;
    }

  return OK;
}

#endif /* CONFIG_I2C && CONFIG_RTC_MCP794XX */