#ifndef __LEPTON_VID_H_
#define __LEPTON_VID_H_

#define VID 0x03
/* VID Module Command IDs
*/
#define LEPTON_VID_MODULE_BASE                  0x0300

#define LEPTON_CID_VID_POLARITY_SELECT         0x0000
#define LEPTON_CID_VID_LUT_SELECT              0x0004
#define LEPTON_CID_VID_LUT_TRANSFER            0x0008
#define LEPTON_CID_VID_FOCUS_CALC_ENABLE       0x000C
#define LEPTON_CID_VID_FOCUS_ROI               0x0010
#define LEPTON_CID_VID_FOCUS_THRESHOLD         0x0014
#define LEPTON_CID_VID_FOCUS_METRIC            0x0018
#define LEPTON_CID_VID_SBNUC_ENABLE            0x001C
#define LEPTON_CID_VID_GAMMA_SELECT            0x0020
#define LEPTON_CID_VID_FREEZE_ENABLE           0x0024
#define LEPTON_CID_VID_BORESIGHT_CALC_ENABLE   0x0028
#define LEPTON_CID_VID_BORESIGHT_COORDINATES   0x002C
#define LEPTON_CID_VID_VIDEO_OUTPUT_FORMAT     0x0030
#define LEPTON_CID_VID_LOW_GAIN_COLOR_LUT      0x0034

/* VID Module Object Sizes
*/
#define LEPTON_COLOR_LUT_SIZE               256     /* 8-bit LUT as 256 x 8-bits */

/******************************************************************************/
/** EXPORTED TYPE DEFINITIONS                                                **/
/******************************************************************************/

typedef enum LEPTON_POLARITY_E_TAG
{
  LEPTON_VID_WHITE_HOT=0,
  LEPTON_VID_BLACK_HOT,
  LEPTON_VID_END_POLARITY

}LEPTON_POLARITY_E, *LEPTON_POLARITY_E_PTR;


/* Video Pseudo color LUT Enum
*/
typedef enum LEPTON_PCOLOR_LUT_E_TAG
{
  LEPTON_VID_WHEEL6_LUT=0,
  LEPTON_VID_FUSION_LUT,
  LEPTON_VID_RAINBOW_LUT,
  LEPTON_VID_GLOBOW_LUT,
  LEPTON_VID_SEPIA_LUT,
  LEPTON_VID_COLOR_LUT,
  LEPTON_VID_ICE_FIRE_LUT,
  LEPTON_VID_RAIN_LUT,
  LEPTON_VID_USER_LUT,
  LEPTON_VID_END_PCOLOR_LUT

}LEPTON_PCOLOR_LUT_E, *LEPTON_PCOLOR_LUT_E_PTR;
/* Video Focus Metric Calculation Enable Enum
*/
typedef enum LEPTON_VID_ENABLE_TAG
{
  LEPTON_VID_FOCUS_CALC_DISABLE=0,
  LEPTON_VID_FOCUS_CALC_ENABLE,
  LEPTON_VID_END_FOCUS_CALC_ENABLE

}LEPTON_VID_FOCUS_CALC_ENABLE_E, *LEPTON_VID_FOCUS_CALC_ENABLE_E_PTR;
/* Video Scene-Based NUC Enable Enum
*/
typedef enum LEPTON_VID_SBNUC_ENABLE_TAG
{
  LEPTON_VID_SBNUC_DISABLE = 0,
  LEPTON_VID_SBNUC_ENABLE,
  LEPTON_VID_END_SBNUC_ENABLE

}LEPTON_VID_SBNUC_ENABLE_E, *LEPTON_VID_SBNUC_ENABLE_E_PTR ;


/* Video Freeze Output Enable Enum
*/
typedef enum LEPTON_VID_FREEZE_ENABLE_TAG
{
  LEPTON_VID_FREEZE_DISABLE = 0,
  LEPTON_VID_FREEZE_ENABLE,
  LEPTON_VID_END_FREEZE_ENABLE

}LEPTON_VID_FREEZE_ENABLE_E, *LEPTON_VID_FREEZE_ENABLE_E_PTR ;

typedef enum LEPTON_VID_BORESIGHT_CALC_ENABLE_STATE_E_TAG
{
  LEPTON_VID_BORESIGHT_CALC_DISABLED = 0,
  LEPTON_VID_BORESIGHT_CALC_ENABLED,

  LEPTON_VID_END_BORESIGHT_CALC_ENABLE_STATE,
} LEPTON_VID_BORESIGHT_CALC_ENABLE_STATE_E, *LEPTON_VID_BORESIGHT_CALC_ENABLE_STATE_E_PTR;

typedef enum LEPTON_VID_VIDEO_OUTPUT_FORMAT_TAG
{
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW8 = 0,          // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW10,             // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW12,             // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RGB888,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RGB666,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RGB565,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_YUV422_8BIT,       // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW14,             // SUPPORTED in this release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_YUV422_10BIT,      // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_USER_DEFINED,      // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW8_2,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW8_3,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW8_4,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW8_5,            // To be supported in later release
  LEPTON_VID_VIDEO_OUTPUT_FORMAT_RAW8_6,            // To be supported in later release
  LEPTON_END_VID_VIDEO_OUTPUT_FORMAT

}LEPTON_VID_VIDEO_OUTPUT_FORMAT_E, *LEPTON_VID_VIDEO_OUTPUT_FORMAT_E_PTR;

#endif
