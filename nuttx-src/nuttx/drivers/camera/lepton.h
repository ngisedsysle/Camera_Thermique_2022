#ifndef __DRIVERS_CAMERA_LEPTON_H
#define __DRIVERS_CAMERA_LEPTON_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/kmalloc.h>
#include <nuttx/spi/spi.h>
#include <nuttx/config.h>
#include <nuttx/semaphore.h>
#include <nuttx/arch.h>

#include <nuttx/fs/fs.h>
#include <nuttx/i2c/i2c_master.h>
#include <arch/board/board.h>
#include <arch/irq.h>

#include <nuttx/video/imgsensor.h>
#include <nuttx/camera/lepton.h>

#include <nuttx/camera/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define DEVNAME "/dev/lepton"
#define LEPTON_I2C_SLV_ADDR (0x2A)

#define OUT_HSIZE_ORIGINAL (80)
#define OUT_VSIZE_ORIGINAL (60)
#define OUT_HSIZE_QVGA (320)
#define OUT_VSIZE_QVGA (240)

#define swap_bytes(x) (((x) >> 8) | ((x) << 8))
/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Public Types
 ****************************************************************************/

struct lepton_spiread_s
{
  uint8_t addrh; /* Bits 6-7: 00, Bits 0-5: slave Address[21:16] */
  uint8_t addrm; /* Address[15:8] */
  uint8_t addrl; /* Address[7:0] */
  uint8_t dummy; /* Dummy byte */
};

struct lepton_i2cread_s
{
  uint8_t addrh; /* Bits 6-7: 00, Bits 0-5: slave Address[21:16] */
  uint8_t addrm; /* Address[15:8] */
  uint8_t addrl; /* Address[7:0] */
};
struct lepton_i2cwrite_s
{
  uint8_t addrh; /* Bits 6-7: 10, Bits 0-5: Address[21:16] */
  uint8_t addrm; /* Address[15:8] */
  uint8_t addrl; /* Address[7:0] */
                 /* Write data follows */
};

/* This structure describes the overall state of the LEPTON driver */

struct spi_dev_s;    /* Forward reference */
struct i2c_master_s; /* Forward reference */

typedef struct lepton_dev_s
{
  FAR struct spi_dev_s *spi;               /* Cached SPI device reference */
  FAR struct i2c_master_s *i2c;            /* Cached I2C device reference */
  FAR const struct lepton_config_s *lower; /* Cached lower half instance */
  int spi_freq;                            /* SPI Frequency */
  uint8_t crefs;                           /* Number of open references */
  bool unlinked;                           /* True if the driver has been unlinked */
} lepton_dev_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* register operations */
uint16_t lepton_getreg(FAR lepton_dev_t *priv, uint16_t regaddr);
int lepton_putreg(lepton_dev_t *priv, uint16_t regaddr, uint16_t *regval, uint16_t regsize);
void lepton_getregs(FAR lepton_dev_t *priv, uint16_t regaddr, FAR uint16_t *buffer, uint16_t regsize);
int lepton_command(FAR lepton_dev_t *priv, uint8_t module, uint8_t id, uint8_t type,
                   uint16_t datalength, FAR uint16_t *data, FAR uint16_t *buffer);
/* image sensor device operations interface */

 static bool lepton_is_busy(FAR lepton_dev_t *);
// static int lepton_init(void);
// static int lepton_uninit(void);
// static void lepton_destroy(FAR struct lepton_dev_s *priv);

/* Character driver methods */

int lepton_open(FAR struct file *filep);
int lepton_close(FAR struct file *filep);
int lepton_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
int lepton_read(FAR struct file *filep, FAR char *buffer, size_t buflen);

int lepton_initialize(FAR struct lepton_dev_s *priv);
int lepton_register(FAR struct i2c_master_s *i2c, FAR struct spi_dev_s *spi,FAR const struct lepton_config_s *lower);
void lepton_select(FAR struct lepton_dev_s *priv);
void lepton_deselect(FAR struct lepton_dev_s *priv);
/****************************************************************************
 * Private Data
 ****************************************************************************/
static FAR lepton_dev_t *g_lepton_private;

// static struct imgsensor_ops_s g_lepton_ops =
// {
//   lepton_is_available,                  /* is HW available */
//   lepton_init,                          /* init */
//   lepton_uninit,                        /* uninit */
//   lepton_getreg,
//   lepton_putreg,
//   lepton_getregs,
//   lepton_command
// };

static const struct file_operations g_lepton_fops =
    {
        lepton_open,  /* open */
        lepton_close, /* close */
        lepton_read,  /* read */
        NULL,         /* write */
        NULL,         /* seek */
        lepton_ioctl, /* ioctl */
        NULL,         /* poll */
        NULL          /* unlink */

};
#endif /* __DRIVERS_CAMERA_LEPTON_H */