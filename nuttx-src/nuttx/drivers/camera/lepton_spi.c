#include "lepton.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lepton_select
 *
 * Description:
 *   Select the LEPTON part
 *
 ****************************************************************************/

void lepton_select(FAR struct lepton_dev_s *priv)
{
  // caminfo("Mode: %d Bits: 8 Frequency: %" PRId32 "\n",
  //         SPIDEV_MODE3, CONFIG_LEPTON_SPI_FREQUENCY);

  DEBUGASSERT(priv != NULL);

  /* Lock the SPI bus */

  SPI_LOCK(priv->spi, true);

  /* Configure SPI for the LEPTON */

  SPI_SETMODE(priv->spi, SPIDEV_MODE3);
  SPI_SETBITS(priv->spi, 8);
  SPI_HWFEATURES(priv->spi, HWFEAT_MSBFIRST);
  SPI_SETFREQUENCY(priv->spi, CONFIG_LEPTON_SPI_FREQUENCY);

  /* Select SPI device */

  SPI_SELECT(priv->spi, SPIDEV_CAMERA(0), true);
}

/****************************************************************************
 * Name: lepton_deselect
 *
 * Description:
 *   De-select the LEPTON part
 *
 ****************************************************************************/

void lepton_deselect(FAR struct lepton_dev_s *priv)
{
  /* Des-select the LEPTON device */
  SPI_SELECT(priv->spi, SPIDEV_CAMERA(0), false);
  /* Unlock bus */
  SPI_LOCK(priv->spi, false);
}


int lepton_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
  FAR struct inode *inode;
  FAR struct lepton_dev_s *priv;

  DEBUGASSERT(filep != NULL);
  inode = filep->f_inode;
  DEBUGASSERT(inode != NULL && inode->i_private != NULL);
  priv  = inode->i_private;
  DEBUGASSERT(priv != NULL && buffer != NULL && buflen > 0);

  lepton_select(priv);
  SPI_RECVBLOCK(priv->spi, buffer, buflen);
  lepton_deselect(priv);

  return OK;
}