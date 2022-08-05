#ifndef __INCLUDE_NUTTX_LEPTON_OEM_H_
#define __INCLUDE_NUTTX_LEPTON_OEM_H_

#define OEM 0x08

#define LEPTON_OEM_MODULE_BASE                     0x0800

#define LEPTON_CID_OEM_POWER_DOWN                  0x0000
#define LEPTON_CID_OEM_STANDBY                     0x0004
#define LEPTON_CID_OEM_LOW_POWER_MODE_1            0x0008
#define LEPTON_CID_OEM_LOW_POWER_MODE_2            0x000C
#define LEPTON_CID_OEM_BIT_TEST                    0x0010
#define LEPTON_CID_OEM_MASK_REVISION               0x0014
//#define LEPTON_CID_OEM_MASTER_ID                   0x0018
#define LEPTON_CID_OEM_FLIR_PART_NUMBER            0x001C
#define LEPTON_CID_OEM_SOFTWARE_VERSION            0x0020
#define LEPTON_CID_OEM_VIDEO_OUTPUT_ENABLE         0x0024
#define LEPTON_CID_OEM_VIDEO_OUTPUT_FORMAT         0x0028
#define LEPTON_CID_OEM_VIDEO_OUTPUT_SOURCE         0x002C
#define LEPTON_CID_OEM_VIDEO_OUTPUT_CHANNEL        0x0030
#define LEPTON_CID_OEM_VIDEO_GAMMA_ENABLE          0x0034
#define LEPTON_CID_OEM_CUST_PART_NUMBER            0x0038
#define LEPTON_CID_OEM_VIDEO_OUTPUT_CONSTANT       0x003C
#define LEPTON_CID_OEM_REBOOT                      0x0040
#define LEPTON_CID_OEM_FFC_NORMALIZATION_TARGET    0x0044
#define LEPTON_CID_OEM_STATUS                      0x0048
#define LEPTON_CID_OEM_SCENE_MEAN_VALUE            0x004C
#define LEPTON_CID_OEM_POWER_MODE                  0x0050

#define LEPTON_CID_OEM_GPIO_MODE_SELECT            0x0054
#define LEPTON_CID_OEM_GPIO_VSYNC_PHASE_DELAY      0x0058

#define LEPTON_CID_OEM_USER_DEFAULTS               0x005C
#define LEPTON_CID_OEM_USER_DEFAULTS_RESTORE       0x0060
#define LEPTON_CID_OEM_SHUTTER_PROFILE_OBJ         0x0064
#define LEPTON_CID_OEM_THERMAL_SHUTDOWN_ENABLE_STATE 0x0068
#define LEPTON_CID_OEM_BAD_PIXEL_REPLACE_CONTROL   0x006C
#define LEPTON_CID_OEM_TEMPORAL_FILTER_CONTROL     0x0070
#define LEPTON_CID_OEM_COLUMN_NOISE_ESTIMATE_CONTROL 0x0074
#define LEPTON_CID_OEM_PIXEL_NOISE_ESTIMATE_CONTROL 0x0078



#define LEPTON_OEM_MAX_PART_NUMBER_CHAR_SIZE       32

typedef enum LEP_OEM_VIDEO_OUTPUT_ENABLE_TAG
{
   LEP_VIDEO_OUTPUT_DISABLE = 0,
   LEP_VIDEO_OUTPUT_ENABLE,
   LEP_END_VIDEO_OUTPUT_ENABLE

}LEP_OEM_VIDEO_OUTPUT_ENABLE_E, *LEP_OEM_VIDEO_OUTPUT_ENABLE_E_PTR;

/* Video Output Format Selection
*/
typedef enum LEP_OEM_VIDEO_OUTPUT_FORMAT_TAG
{
   LEP_VIDEO_OUTPUT_FORMAT_RAW8 = 0,          // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW10,             // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW12,             // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RGB888,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RGB666,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RGB565,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_YUV422_8BIT,       // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW14,             // SUPPORTED in this release
   LEP_VIDEO_OUTPUT_FORMAT_YUV422_10BIT,      // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_USER_DEFINED,      // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW8_2,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW8_3,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW8_4,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW8_5,            // To be supported in later release
   LEP_VIDEO_OUTPUT_FORMAT_RAW8_6,            // To be supported in later release
   LEP_END_VIDEO_OUTPUT_FORMAT

}LEP_OEM_VIDEO_OUTPUT_FORMAT_E, *LEP_OEM_VIDEO_OUTPUT_FORMAT_E_PTR;

/* Video Output Source Selection
*/
typedef enum LEP_OEM_VIDEO_OUTPUT_SOURCE_TAG
{
   LEP_VIDEO_OUTPUT_SOURCE_RAW = 0,         /* Before video processing */
   LEP_VIDEO_OUTPUT_SOURCE_COOKED,        /* Post video processing - NORMAL MODE */
   LEP_VIDEO_OUTPUT_SOURCE_RAMP,          /* Software Ramp pattern - increase in X, Y */
   LEP_VIDEO_OUTPUT_SOURCE_CONSTANT,      /* Software Constant value pattern */
   LEP_VIDEO_OUTPUT_SOURCE_RAMP_H,        /* Software Ramp pattern - increase in X only */
   LEP_VIDEO_OUTPUT_SOURCE_RAMP_V,        /* Software Ramp pattern - increase in Y only */
   LEP_VIDEO_OUTPUT_SOURCE_RAMP_CUSTOM,   /* Software Ramp pattern - uses custom settings */

   /* Additions to support frame averaging, freeze frame, and data buffers
   */
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_CAPTURE,  // Average, Capture frame
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_FREEZE,   // Freeze-Frame Buffer

   /* RESERVED BUFFERS
   */
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_0,        // Reserved DATA Buffer
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_1,        // Reserved DATA Buffer
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_2,        // Reserved DATA Buffer
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_3,        // Reserved DATA Buffer
   LEP_VIDEO_OUTPUT_SOURCE_FRAME_4,        // Reserved DATA Buffer

   LEP_END_VIDEO_OUTPUT_SOURCE

}LEP_OEM_VIDEO_OUTPUT_SOURCE_E, *LEP_OEM_VIDEO_OUTPUT_SOURCE_E_PTR;

/* Video Output Channel Selection
*/
typedef enum LEP_OEM_VIDEO_OUTPUT_CHANNEL_TAG
{
   LEP_VIDEO_OUTPUT_CHANNEL_MIPI = 0,
   LEP_VIDEO_OUTPUT_CHANNEL_LEPTON,
   LEP_END_VIDEO_OUTPUT_CHANNEL

}LEP_OEM_VIDEO_OUTPUT_CHANNEL_E, *LEP_OEM_VIDEO_OUTPUT_CHANNEL_E_PTR;

/* Video Gamma Enable Enum
*/
typedef enum LEP_OEM_VIDEO_GAMMA_ENABLE_TAG
{
   LEP_VIDEO_GAMMA_DISABLE = 0,
   LEP_VIDEO_GAMMA_ENABLE,
   LEP_END_VIDEO_GAMMA_ENABLE

}LEP_OEM_VIDEO_GAMMA_ENABLE_E, *LEP_OEM_VIDEO_GAMMA_ENABLE_E_PTR;

typedef enum LEP_OEM_MEM_BUFFER_E_TAG
{
   LEP_OEM_MEM_OTP_ODAC = 0,
   LEP_OEM_MEM_OTP_GAIN,
   LEP_OEM_MEM_OTP_OFFSET_0,
   LEP_OEM_MEM_OTP_OFFSET_1,
   LEP_OEM_MEM_OTP_FFC,
   LEP_OEM_MEM_OTP_LG0,
   LEP_OEM_MEM_OTP_LG1,
   LEP_OEM_MEM_OTP_LG2,
   LEP_OEM_MEM_OTP_TFPA_LUT,
   LEP_OEM_MEM_OTP_TAUX_LUT,
   LEP_OEM_MEM_OTP_BAD_PIXEL_LIST,
   LEP_OEM_MEM_SRAM_ODAC,
   LEP_OEM_MEM_SRAM_BAD_PIXEL_LIST,
   LEP_OEM_MEM_SHARED_BUFFER_0,
   LEP_OEM_MEM_SHARED_BUFFER_1,
   LEP_OEM_MEM_SHARED_BUFFER_2,
   LEP_OEM_MEM_SHARED_BUFFER_3,
   LEP_OEM_MEM_SHARED_BUFFER_4,
   LEP_OEM_END_MEM_BUFFERS

}LEP_OEM_MEM_BUFFER_E,*LEP_OEM_MEM_BUFFER_E_PTR;

typedef enum
{
   LEP_OEM_STATUS_OTP_WRITE_ERROR = -2,
   LEP_OEM_STATUS_ERROR = -1,
   LEP_OEM_STATUS_READY = 0,
   LEP_OEM_STATUS_BUSY,
   LEP_OEM_FRAME_AVERAGE_COLLECTING_FRAMES,
   LEP_OEM_STATUS_END

} LEP_OEM_STATUS_E, *LEP_OEM_STATUS_E_PTR;

typedef enum LEP_OEM_STATE_E_TAG
{
   LEP_OEM_DISABLE = 0,
   LEP_OEM_ENABLE,
   LEP_OEM_END_STATE

}LEP_OEM_STATE_E,*LEP_OEM_STATE_E_PTR;

typedef enum LEP_OEM_POWER_STATE_E_TAG
{
   LEP_OEM_POWER_MODE_NORMAL = 0,
   LEP_OEM_POWER_MODE_LOW_POWER_1,
   LEP_OEM_POWER_MODE_LOW_POWER_2,
   LEP_OEM_END_POWER_MODE,

}LEP_OEM_POWER_STATE_E, *LEP_OEM_POWER_STATE_E_PTR;

typedef enum LEP_OEM_VSYNC_DELAY_E_TAG
{
   LEP_OEM_VSYNC_DELAY_MINUS_3 = -3,
   LEP_OEM_VSYNC_DELAY_MINUS_2 = -2,
   LEP_OEM_VSYNC_DELAY_MINUS_1 = -1,
   LEP_OEM_VSYNC_DELAY_NONE = 0,
   LEP_OEM_VSYNC_DELAY_PLUS_1 = 1,
   LEP_OEM_VSYNC_DELAY_PLUS_2 = 2,
   LEP_OEM_VSYNC_DELAY_PLUS_3 = 3,

   LEP_END_OEM_VSYNC_DELAY
} LEP_OEM_VSYNC_DELAY_E, *LEP_OEM_VSYNC_DELAY_E_PTR;

typedef enum LEP_OEM_GPIO_MODE_E_TAG
{
   LEP_OEM_GPIO_MODE_GPIO = 0,
   LEP_OEM_GPIO_MODE_I2C_MASTER = 1,
   LEP_OEM_GPIO_MODE_SPI_MASTER_VLB_DATA = 2,
   LEP_OEM_GPIO_MODE_SPIO_MASTER_REG_DATA = 3,
   LEP_OEM_GPIO_MODE_SPI_SLAVE_VLB_DATA = 4,
   LEP_OEM_GPIO_MODE_VSYNC = 5,

   LEP_OEM_END_GPIO_MODE,
}LEP_OEM_GPIO_MODE_E, *LEP_OEM_GPIO_MODE_E_PTR;

typedef enum LEP_OEM_USER_PARAMS_STATE_E_TAG
{
   LEP_OEM_USER_PARAMS_STATE_NOT_WRITTEN = 0,
   LEP_OEM_USER_PARAMS_STATE_WRITTEN,

   LEP_OEM_END_USER_PARAMS_STATE,

}LEP_OEM_USER_PARAMS_STATE_E, *LEP_OEM_USER_PARAMS_STATE_E_PTR;

#endif /*__INCLUDE_NUTTX_LEPTON_OEM_H_*/
