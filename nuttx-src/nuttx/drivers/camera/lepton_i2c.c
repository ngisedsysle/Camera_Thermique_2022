#include "lepton.h"

uint16_t lepton_getreg(FAR lepton_dev_t *priv, uint16_t regaddr)
{
  struct i2c_msg_s msg[2];
  uint16_t regval = 0;
  int ret;

  uint8_t regaddr_buf[2];

  regaddr_buf[0] = (regaddr >> 8);
  regaddr_buf[1] = regaddr & 0xff;

  // caminfo("lepton_getregs: regaddr=%04x, regaddr_buf: %04x\n", regaddr, *((uint16_t *)regaddr_buf));

  uint8_t regval_buf[2];

  DEBUGASSERT(priv != NULL);
  /* Set up the I2C configuration
   * Start <SlaveAddress> ACK <RegisterAddress1> ACK <RegisterAddress0> RESTART <DATA> ACK <Stop>
   */
  msg[0].frequency = CONFIG_LEPTON_I2C_FREQUENCY;
  msg[0].addr = LEPTON_DEVICE_ADDR ;
  msg[0].flags = I2C_M_NOSTOP;
  msg[0].buffer = regaddr_buf;
  msg[0].length = 2;

  msg[1].frequency = CONFIG_LEPTON_I2C_FREQUENCY;
  msg[1].addr = LEPTON_DEVICE_ADDR;
  msg[1].flags = I2C_M_READ;
  msg[1].buffer = regval_buf;
  msg[1].length = 2;

  /* Write the register address */
  ret = I2C_TRANSFER(priv->i2c, msg, 2);
  // ret = i2c_writeread(priv->i2c, &config, (FAR uint8_t *)buffer, 2, (FAR uint8_t *)&regval, 2);
  if (ret < 0)
  {
    camerr("i2c_writeread failed: %d\n", ret);
  }

  regval = (regval_buf[0] << 8) | regval_buf[1];

  return regval;
}

int lepton_putreg(lepton_dev_t *priv, uint16_t regaddr, uint16_t *regval, uint16_t regsize)
{
  struct i2c_msg_s msg;
  uint8_t txbuffer[regsize + 2];
  int ret;

  // caminfo("lepton_putreg: regaddr: %04x, txbuffer: %04x, regval: %04x\n", regaddr, *((uint16_t *)txbuffer), *regval);

  txbuffer[0] = (regaddr >> 8);
  txbuffer[1] = regaddr & 0xff;

  // LSB to MSB
  for(int i=2; i< regsize + 2; i+=2)
  {
    txbuffer[i] = (*regval >> 8);
    txbuffer[i+1] = *regval & 0xff;
    regval++;
  }
  /* Set up the I2C configuration */

  msg.frequency = CONFIG_LEPTON_I2C_FREQUENCY;
  msg.addr = LEPTON_DEVICE_ADDR;
  msg.flags = 0;         /* Write transaction, beginning with START */
  msg.buffer = txbuffer; /* Transfer from this address */
  msg.length = regsize + 2;

  /* Perform the transfer */

  ret = I2C_TRANSFER(priv->i2c, &msg, 1);

  if (ret < 0)
  {
    camerr("i2c_write failed: %d\n", ret);
  }

  return ret;
}

void lepton_getregs(FAR lepton_dev_t *priv, uint16_t regaddr, FAR uint16_t *buffer, uint16_t regsize)
{
  struct i2c_msg_s msg[2];
  int ret;

  uint8_t regaddr_buf[2];

  regaddr_buf[0] = (regaddr >> 8);
  regaddr_buf[1] = regaddr & 0xff;

  /* Set up the I2C configuration */

  msg[0].frequency = CONFIG_LEPTON_I2C_FREQUENCY;
  msg[0].addr = LEPTON_DEVICE_ADDR;
  msg[0].flags = 0;
  msg[0].buffer = (uint8_t *)regaddr_buf;
  msg[0].length = 2;

  msg[1].frequency = CONFIG_LEPTON_I2C_FREQUENCY;
  msg[1].addr = LEPTON_DEVICE_ADDR;
  msg[1].flags = I2C_M_READ;
  msg[1].buffer = (uint8_t *)buffer;
  msg[1].length = regsize;

  /* Write the register address */

  ret = I2C_TRANSFER(priv->i2c, msg, 2);
  // ret = i2c_write(priv->i2c, &config, (FAR uint8_t *)txbuffer, 2);
  if (ret < 0)
  {
    camerr("i2c_write failed: %d\n", ret);
  }
}

static bool lepton_is_busy(FAR lepton_dev_t *priv)
{
  bool busy = lepton_getreg(priv, LEPTON_REG_STATUS) & LEPTON_BUSY_MASK;
  caminfo("lepton_is_busy : %d\n", busy);
  return busy;
}

int lepton_command(FAR lepton_dev_t *priv, uint8_t module, uint8_t id, uint8_t type,
                   uint16_t datalength, FAR uint16_t *data, FAR uint16_t *buffer)
{
  caminfo("sending command...\n");
  int ret;
  /* check if the camera is ready */
  while (lepton_is_busy(priv))
  {
    camwarn("lepton 2.5 is not ready\n");
  }
  /* write the number of bytes to transfer */
  ret = lepton_putreg(priv, LEPTON_REG_DATA_LENGTH, &datalength, 2);
  if (ret < 0)
  {
    camerr("ERROR %d : impossible to write number of bytes to transfer to lepton\n", ret);
    return ret;
  }
  if (type == SET)
  {
    /* write the new value in data register */
    ret = lepton_putreg(priv, LEPTON_REG_DATA, data, 2 * datalength);
    if (ret < 0)
    {
      camerr("ERROR %d : impossible to write in lepton data register\n", ret);
      return ret;
    }
  }
  /* send the command */
  uint8_t protectionBit = (module == OEM) || (module == RAD) ? 1 : 0;
  uint16_t command = (protectionBit << 14 | module << 8) | (id | type);
  caminfo("command : %04x\n", command);
  ret = lepton_putreg(priv, LEPTON_REG_COMMAND, &command, 2);
  if (ret < 0)
  {
    camerr("ERROR %d : impossible to send the command to lepton\n", ret);
    return ret;
  }
  /* wait for the command to be executed */
  while (lepton_is_busy(priv))
  {
    camwarn("lepton 2.5 is not ready\n");
  }

  /* read the answer */
  if (type != RUN && type != SET)
  {
    lepton_getregs(priv, LEPTON_REG_DATA, buffer, 2 * datalength);
  }
  for(int i = 0; i < 2 * datalength; i++)
  {
    uint16_t byte = buffer[i];
    buffer[i] = byte << 8 | byte >> 8;
  }
  return OK;
}