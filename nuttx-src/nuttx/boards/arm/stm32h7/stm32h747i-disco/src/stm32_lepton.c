/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <nuttx/irq.h>
#include <nuttx/board.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/spi/spi.h>
#include <nuttx/camera/lepton.h>
// #include <arch/board/board.h>

// #include "stm32.h"

#include "arm_internal.h"
#include "stm32_gpio.h"
#include "stm32_i2c.h"
#include "stm32_spi.h"

#include "stm32h747i-disco.h"

#if defined(CONFIG_CAMERA_LEPTON)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct stm32h7_leptonlower_s
{
  /* Standard lepton interface */

  struct lepton_config_s config;

  /* Extensions for the stm32h7 board */

  xcpt_t handler;
  void *arg;
};
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* IRQ/GPIO access callbacks.  These operations all hidden behind callbacks
 * to isolate the lepton driver from differences in GPIO interrupt handling
 * by varying boards and MCUs.
 *
 * Interrupts should be configured on the falling edge of nINT.
 *
 *   attach  - Attach the ADS7843E interrupt handler to the GPIO interrupt
 *   enable  - Enable or disable the GPIO interrupt
 *   clear   - Acknowledge/clear any pending GPIO interrupt as necessary.
 *   pwrdown - Power the FT80X up or down.
 *   audio   - Enable audio (i.e., set the external audio amplifier shutdown
 *             pin to the appropriate level to enable or disable the
 *             external audio amplifier)
 *   destroy - The driver has been unlinked. Cleanup as necessary.
 */
static int  lepton_attach(const struct lepton_config_s *lower, xcpt_t isr,
                         void *arg);
static void lepton_enable(const struct lepton_config_s *lower,
                         bool enable);
static void lepton_clear(const struct lepton_config_s *lower);

static void lepton_pwrdown(const struct lepton_config_s *lower,
                          bool pwrdown);
static void lepton_destroy(const struct lepton_config_s *lower);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* A reference to a structure of this type must be passed to the LEPTON
 * driver.  This structure provides information about the configuration
 * of the LEPTON and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active. The
 * memory must be writable because, under certain circumstances, the driver
 * may modify certain values.
 */

static struct stm32h7_leptonlower_s g_leptonlower =
    {
        .config =
            {
                .spi_frequency = CONFIG_LEPTON_SPI_FREQUENCY,
                .i2c_frequency = CONFIG_LEPTON_I2C_FREQUENCY,
                .address = LEPTON_DEVICE_ADDR,

                .attach         = lepton_attach,
                .enable         = lepton_enable,
                .clear          = lepton_clear,
                .pwrdown        = lepton_pwrdown,
                .destroy        = lepton_destroy,
            },
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int lepton_attach(const struct lepton_config_s *lower, xcpt_t isr,
                        void *arg)
{
  struct stm32h7_leptonlower_s *priv =
    (struct stm32h7_leptonlower_s *)lower;

  if (isr)
    {
      /* Just save the address of the handler for now.  The new handler will
       * be attached when the interrupt is next enabled.
       */

      iinfo("Attaching %p\n", isr);
      priv->handler = isr;
      priv->arg     = arg;
    }
  else
    {
      iinfo("Detaching %p\n", priv->handler);
      lepton_enable(lower, false);
      priv->handler = NULL;
      priv->arg     = NULL;
    }

  return OK;
}

static void lepton_enable(const struct lepton_config_s *lower,
                         bool enable)
{
  struct stm32h7_leptonlower_s *priv =
    (struct stm32h7_leptonlower_s *)lower;
  irqstate_t flags;

  /* Attach and enable, or detach and disable.  Enabling and disabling GPIO
   * interrupts is a multi-step process so the safest thing is to keep
   * interrupts disabled during the reconfiguration.
   */

  flags = enter_critical_section();
  if (enable && priv->handler)
    {
      /* Configure the EXTI interrupt using the saved handler */

      stm32_gpiosetevent(GPIO_FT80X_INT, true, true, true,
                         priv->handler, priv->arg);
    }
  else
    {
      /* Configure the EXTI interrupt with a NULL handler to disable it.
       *
       * REVISIT:  There is a problem here... interrupts received while
       * the EXTI is de-configured will not pend but will be lost.
       */

     stm32_gpiosetevent(GPIO_FT80X_INT, false, false, false,
                        NULL, NULL);
    }

  leave_critical_section(flags);
}

static void lepton_clear(const struct lepton_config_s *lower)
{
  /* Does nothing */
  // TODO
}

static void lepton_pwrdown(const struct lepton_config_s *lower,
                          bool pwrdown)
{
  /* Powerdown pin is active low.  Hence, it is really a power up pin. */

  stm32_gpiowrite(GPIO_LEPTON_PD, !pwrdown);
}

static void lepton_destroy(const struct lepton_config_s *lower)
{
  /* Does nothing */
  // TODO 
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_lepton_setup
 *
 * Description:
 *   This function is called by board-bringup logic to configure the
 *   LEPTON GUI device.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

int stm32_lepton_setup(void)
{
  struct spi_dev_s *spi;
  struct i2c_master_s *i2c;
  int ret;

  /* Configure the LEPTON interrupt pin as an input and powerdown pin as an
   * output.  Device is initially powered down.
   */
  stm32_configgpio(GPIO_LEPTON_RST);
  stm32_configgpio(GPIO_LEPTON_PD);

  /* Get an instance of the SPI interface */

  spi = stm32_spibus_initialize(LEPTON_SPIBUS);
  if (!spi)
  {
    camerr("ERROR: Failed to initialize SPI%d\n", LEPTON_SPIBUS);
    return -ENODEV;
  }
  i2c = stm32_i2cbus_initialize(LEPTON_I2CBUS);
  if (!i2c)
  {
    _err("ERROR: Failed to initialize I2C%d\n", LEPTON_I2CBUS);
  }

  /* Initialize and register the SPI device */

  ret = lepton_register(i2c, spi, &g_leptonlower.config);
  if (ret < 0)
  {
    camerr("ERROR %d: Failed to register lepton device\n", ret);

    /* up_spiuninitialize(spi); */

    return -ENODEV;
  }
  return OK;
}

// static void lepton_destroy(const struct lepton_config_s *lower)
// {
//   /* Does nothing */
// }

#endif /* CONFIG_STM32H7_LEPTON */
