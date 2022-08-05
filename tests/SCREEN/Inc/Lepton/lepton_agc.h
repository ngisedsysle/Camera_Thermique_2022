
#ifndef __LEPTON_AGC_H_
#define __LEPTON_AGC_H_

/* AGC Module Command IDs
*/
#define AGC 0x01

#define LEP_AGC_MODULE_BASE                     0x0100

#define LEP_CID_AGC_ENABLE_STATE                0x0000
#define LEP_CID_AGC_POLICY                      0x0004
#define LEP_CID_AGC_ROI                         0x0008
#define LEP_CID_AGC_STATISTICS                  0x000C
#define LEP_CID_AGC_HISTOGRAM_CLIP_PERCENT      0x0010
#define LEP_CID_AGC_HISTOGRAM_TAIL_SIZE         0x0014
#define LEP_CID_AGC_LINEAR_MAX_GAIN             0x0018
#define LEP_CID_AGC_LINEAR_MIDPOINT             0x001C
#define LEP_CID_AGC_LINEAR_DAMPENING_FACTOR     0x0020
#define LEP_CID_AGC_HEQ_DAMPENING_FACTOR        0x0024
#define LEP_CID_AGC_HEQ_MAX_GAIN                0x0028
#define LEP_CID_AGC_HEQ_CLIP_LIMIT_HIGH         0x002C
#define LEP_CID_AGC_HEQ_CLIP_LIMIT_LOW          0x0030
#define LEP_CID_AGC_HEQ_BIN_EXTENSION           0x0034
#define LEP_CID_AGC_HEQ_MIDPOINT                0x0038
#define LEP_CID_AGC_HEQ_EMPTY_COUNTS            0x003C
#define LEP_CID_AGC_HEQ_NORMALIZATION_FACTOR    0x0040
#define LEP_CID_AGC_HEQ_SCALE_FACTOR            0x0044
#define LEP_CID_AGC_CALC_ENABLE_STATE           0x0048
#define LEP_CID_AGC_HEQ_LINEAR_PERCENT          0x004C

/* AGC Module Attribute Scaling and Module Attribute Limits
*/
/* Linear
*/
#define LEP_AGC_MAX_HISTOGRAM_CLIP_PERCENT      100         /* Scale is 10x  100 == 10.0%  */
#define LEP_AGC_MAX_HISTOGRAM_TAIL_SIZE         80*60     /* Scale is 1x  units: pixels  */
#define LEP_AGC_MIN_LINEAR_MAX_GAIN              1          /* Scale is 1x    */
#define LEP_AGC_MAX_LINEAR_MAX_GAIN              4          /* Scale is 1x    */
#define LEP_AGC_MIN_LINEAR_MIDPOINT              1          /* Scale is 1x    */
#define LEP_AGC_MAX_LINEAR_MIDPOINT              254        /* Scale is 1x    */
#define LEP_AGC_MAX_LINEAR_DAMPENING_FACTOR      100        /* Scale is 1x  units: percent  */

/* Histogram Equalization
*/
#define LEP_AGC_MAX_HEQ_DAMPENING_FACTOR         100        /* Scale is 1x  units: percent  */
#define LEP_AGC_MIN_HEQ_MAX_GAIN                 1          /* Scale is 1x    */
#define LEP_AGC_MAX_HEQ_MAX_GAIN                 4          /* Scale is 1x    */

#define LEP_AGC_HEQ_CLIP_LIMIT_HIGH             80*60     /* Scale is 1x  units: pixels  */
#define LEP_AGC_HEQ_CLIP_LIMIT_LOW              80*60     /* Scale is 1x  units: pixels  */
#define LEP_AGC_HEQ_MAX_BIN_EXTENSION            16         /* Scale is 1x  units: bins  */

#define LEP_AGC_MIN_HEQ_MIDPOINT                127         /* Scale is 1x    */
#define LEP_AGC_MAX_HEQ_MIDPOINT                65534       /* Scale is 1x    */

/* ROI
*/
#define LEP_AGC_MAX_COL                         79
#define LEP_AGC_MAX_ROW                         59
#define LEP_AGC_MIN_COL                         0
#define LEP_AGC_MIN_ROW                         0

/* AGC Enable Enum
*/
typedef enum LEP_AGC_ENABLE_TAG
{
   LEP_AGC_DISABLE = 0,
   LEP_AGC_ENABLE,
   LEP_END_AGC_ENABLE

}LEP_AGC_ENABLE_E, *LEP_AGC_ENABLE_E_PTR;

/* AGC Policy Enum
*/
typedef enum LEP_AGC_POLICY_TAG
{
   LEP_AGC_LINEAR = 0,
   LEP_AGC_HEQ,
   LEP_END_AGC_POLICY
}LEP_AGC_POLICY_E, *LEP_AGC_POLICY_E_PTR;

/* AGC Output Scale Factor Structure
*/
typedef enum LEP_AGC_SCALE_FACTOR_E_TAG
{
   LEP_AGC_SCALE_TO_8_BITS = 0,
   LEP_AGC_SCALE_TO_14_BITS,
   LEP_AGC_END_SCALE_TO
}LEP_AGC_HEQ_SCALE_FACTOR_E, *LEP_AGC_HEQ_SCALE_FACTOR_E_PTR;

/* AGC Calc Enable Enum
*/
typedef enum LEP_AGC_CALC_ENABLE_TAG
{
   LEP_AGC_CALC_DISABLE = 0,
   LEP_AGC_CALC_ENABLE,
   LEP_END_AGC_CALC_ENABLE

}LEP_AGC_CALC_ENABLE_E, *LEP_AGC_CALC_ENABLE_E_PTR;
#endif
