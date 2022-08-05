// TODO : put license here

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "lepton.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_CAMERA_LEPTON

static int lepton_init(void)
{
  // FAR struct lepton_dev_s *priv = g_lepton_private;
  // int ret = 0;

  // priv->i2c      = board_lepton_initialize();         //TODO: declare the function
  // priv->i2c_addr = LEPTON_I2C_SLV_ADDR;
  // priv->i2c_freq = I2CFREQ_STANDARD;

  // ret = board_lepton_power_on();
  // if (ret < 0)
  //   {
  //     verr("Failed to power on %d\n", ret);
  //     return ret;
  //   }

  // ret = init_lepton(priv);
  // if (ret < 0)
  //   {
  //     verr("Failed to init_lepton %d\n", ret);
  //     board_lepton_set_reset();
  //     board_lepton_power_off();
  //     return ret;
  //   }

  // return ret;
  // TODO : modify the code
  printf("lepton_init()\n");
  return OK;
}

static int lepton_uninit(void)
{
  // FAR struct lepton_dev_s *priv = g_lepton_private;

  // int ret = 0;

  // if (priv->state == STATE_LEPTON_ACTIVE)
  //   {
  //     board_lepton_set_sleep(1);
  //   }

  // board_lepton_set_reset();

  // ret = board_lepton_power_off();
  // if (ret < 0)
  //   {
  //     verr("Failed to power off %d\n", ret);
  //     return ret;
  //   }

  // board_lepton_uninitialize(priv->i2c);

  // priv->i2c_freq = I2CFREQ_STANDARD;
  // priv->state    = STATE_LEPTON_POWEROFF;

  // return ret;
  // TODO : modify the code
  printf("lepton_uninit()\n");
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int lepton_initialize(FAR struct lepton_dev_s *priv)
{
  uint16_t status;
  caminfo("Waiting for camera Lepton to be ready...\n");
  do
  {
    up_mdelay(20);
    status = lepton_getreg(priv, LEPTON_REG_STATUS);
  } while (!(status & LEPTON_BOOT_MASK) || (status & LEPTON_BUSY_MASK));
  caminfo("Camera Lepton is ready\n");
  // TODO : add preprocessing
  return OK;
}

int lepton_uninitialize(void)
{
  /* No procedure */

  return OK;
}

/****************************************************************************
 * Name: lepton_register
 *
 * Description:
 *   It's there the driver file is registered and the gpios configured
 *
 * Input Parameters:
 *   spi   - An SPI driver instance
 *   i2c   - An I2C master driver instance
 *   lower - Persistent board configuration data / lower half interface
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

int lepton_register(FAR struct i2c_master_s *i2c, FAR struct spi_dev_s *spi,
                    FAR const struct lepton_config_s *lower)
{
  FAR struct lepton_dev_s *priv;
  int ret;

  DEBUGASSERT(lower != NULL);
  DEBUGASSERT(spi != NULL);
  DEBUGASSERT(i2c != NULL);

  /* Allocate the driver state structure */

  priv = (FAR struct lepton_dev_s *)kmm_zalloc(sizeof(struct lepton_dev_s));
  if (priv == NULL)
  {
    camerr("ERROR: Failed to allocate state structure\n");
    return -ENOMEM;
  }

  /* Save the lower level interface and configuration information */

  priv->lower = lower;
  priv->spi = spi;
  priv->i2c = i2c;

  /* Initialize the LEPTON */

  ret = lepton_initialize(priv);
  if (ret < 0)
  {
    camerr("ERROR: Failed to initialize LEPTON\n");
    return -ENOMEM;
  }

  /* Register the LEPTON character driver */

  ret = register_driver(CONFIG_EXAMPLES_LEPTON_DEVPATH, &g_lepton_fops, 0666, priv);
  if (ret < 0)
  {
    camerr("ERROR: Failed to register the LEPTON character driver\n");
    return -ENOMEM;
  }
  
  return OK;
}

/****************************************************************************
 * Name: lepton_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the PWM work is done.
 *
 ****************************************************************************/

int lepton_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  FAR struct inode *inode;
  FAR struct lepton_dev_s *priv;
  int ret = 0;

  DEBUGASSERT(filep != NULL);
  inode = filep->f_inode;
  DEBUGASSERT(inode != NULL && inode->i_private != NULL);
  priv = inode->i_private;

  caminfo("cmd: %04x arg: %lu\n", cmd, arg);

  /* Handle built-in ioctl commands */

  switch (cmd)
  {
    /* SNIOC_IMAGE:
     *   Description:  get the infrared image
     *   Argument:     A reference to a display list structure instance.
     *                 See struct lepton_displaylist_s.
     *   Returns:      None
     */

  case SNIOC_IMAGE:
    caminfo("SNIOC_IMAGE\n");

    /* TODO: get the image */
    break;
  case SNIOC_TEMP:
  {
    caminfo("SNIOC_TEMP\n");
    unsigned int temp = ((uintptr_t)arg);

    ret = lepton_command(priv, SYS, LEPTON_SYS_FPA_TEMPERATURE_KELVIN, GET, 1, NULL, (uint16_t *)&temp);
    if (ret < 0)
    {
      camerr("ERROR: Failed to get FPA temperature\n");
      return ret;
    }

    caminfo("FPA Temp = %04x\n", (uint16_t)temp);
  }
  break;

  case SNIOC_MAXTEMP:
    caminfo("SNIOC_MAXTEMP\n");

    /* TODO: get the max temp */
    break;

  case SNIOC_MINTEMP:
    caminfo("SNIOC_MINTEMP\n");

    /* TODO: get the min temp */
    break;
  /* Unrecognized IOCTL command */
  default:
    caminfo("default\n");
    camerr("ERROR: Unrecognized cmd: %d arg: %ld\n", cmd, arg);
    ret = -ENOTTY;
    break;
  }
  return ret;
}

int lepton_open(FAR struct file *filep)
{
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
  FAR struct inode *inode;
  FAR struct lepton_dev_s *priv;
  uint8_t tmp;
  int ret;

  DEBUGASSERT(filep != NULL);
  inode = filep->f_inode;
  DEBUGASSERT(inode != NULL && inode->i_private != NULL);
  priv = inode->i_private;

  /* Increment the count of references to the device */

  tmp = priv->crefs + 1;
  if (tmp == 0)
  {
    /* More than 255 opens; uint8_t overflows to zero */

    ret = -EMFILE;
  }
  else
  {
    /* Save the new open count */

    priv->crefs = tmp;
    ret = OK;
  }

  return ret;
#else
  return OK;
#endif
}

int lepton_close(FAR struct file *filep)
{
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
  FAR struct inode *inode;
  FAR struct lepton_dev_s *priv;
  int ret;

  DEBUGASSERT(filep != NULL);
  inode = filep->f_inode;
  DEBUGASSERT(inode != NULL && inode->i_private != NULL);
  priv = inode->i_private;

  /* Will the count decrement to zero? */

  if (priv->crefs <= 1)
  {
    /* Yes.. if the driver has been unlinked, then we need to destroy the
     * driver instance.
     */

    priv->crefs = 0;
    // if (priv->unlinked)
    //   {
    //     lepton_destroy(priv);
    //     return OK;
    //   }
  }
  else
  {
    /* NO.. decrement the number of references to the driver. */

    priv->crefs--;
  }

  ret = OK;

  return ret;
#else
  return OK;
#endif
}

#endif /* CONFIG_CAMERA_LEPTON */
