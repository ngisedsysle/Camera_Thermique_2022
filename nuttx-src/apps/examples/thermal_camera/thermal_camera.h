// TODO : put license here

#ifndef __APPS_EXAMPLES_INFRAREDCAMERA_INFRAREDCAMERA_H
#define __APPS_EXAMPLES_INFRAREDCAMERA_INFRAREDCAMERA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <debug.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <nuttx/config.h>
#include <nuttx/compiler.h>
#include <nuttx/camera/ioctl.h>
#include <sys/boardctl.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

#include <nuttx/camera/lepton.h>
#include <nuttx/lcd/ft80x.h>
#include "graphics/ft80x.h"

#include <nuttx/timers/rtc.h>
//#include <arch/arm/src/stm32h7/stm32_rtc.h>
#include <nuttx/arch.h>
#include <nuttx/clock.h>
#include <nuttx/time.h>
#include <nuttx/usb/cdcacm.h>
#include <nuttx/usb/usbdev.h>
#include <nuttx/usb/usbdev_trace.h>



#ifdef CONFIG_EXAMPLES_THERMAL_CAMERA

#if !defined(GRAPHICS_LEPTON_DEBUG_ERROR)
#define lepton_err _none
#elif defined(CONFIG_CPP_HAVE_VARARGS)
#define lepton_err(format, ...) \
     fprintf(stderr, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define lepton_err printf
#endif

#if !defined(GRAPHICS_LEPTON_DEBUG_WARN)
#define lepton_warn _none
#elif defined(CONFIG_CPP_HAVE_VARARGS)
#define lepton_warn(format, ...) \
     fprintf(stderr, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define lepton_warn printf
#endif

#if !defined(GRAPHICS_LEPTON_DEBUG_INFO)
#define lepton_info _none
#elif defined(CONFIG_CPP_HAVE_VARARGS)
#define lepton_info(format, ...) \
     printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define lepton_info printf
#endif

#define SCALE_COLOR_WIDTH 20
#define SCALE_COLOR_HEIGHT 160
#define SCALE_COLOR_BORDER 1
#define SCALE_COLOR_DISPLAY_OFFSET 15
#define SCALE_COLOR_STRIDE (SCALE_COLOR_WIDTH * sizeof(uint16_t))
#define SCALE_COLOR_SIZE (SCALE_COLOR_HEIGHT * SCALE_COLOR_STRIDE)

#define CENTER_CROSS_SIZE 7

struct image_s
{
     uint8_t format;
     int16_t width;
     int16_t height;
     int16_t stride;
     int32_t offset;
     uint8_t *buffer;
};

struct temp_info_s
{
     uint8_t temp_buffer[LEPTON_FRAME_SIZE];
     int32_t max_idx;
     uint16_t max;
     int32_t min_idx;
     uint16_t min;
};

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This is the type of one display example entry point */

typedef CODE int (*Lepton_example_t)(int fd, FAR struct lepton_image_s *image);

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

// read functions x LEPTON 2.5
int read_image(int argc, char *argv[]);
int open_camera(void);

// display functions x FT80X
int display(int argc, char *argv[]);
int open_screen(void);

// transform functions
int transform_image(int argc, char *argv[]);
uint16_t heatmap_pixel(float heat);
uint16_t get_temp(int32_t idx);

// usb functions
int open_usb(void);
int usb_transfer(int argc, char *argv[]);

// void clock_utc2calendar(time_t days, FAR int *year, FAR int *month,FAR int *day);
// int clock_daysbeforemonth(int month, bool leapyear);
// int clock_isleapyear(int year);
// FAR struct tm *gmtime_r(FAR const time_t *timep, FAR struct tm *result);

#endif /* CONFIG_EXAMPLES_THERMAL_CAMERA */
#endif /* __APPS_EXAMPLES_INFRAREDCAMERA_INFRAREDCAMERA_H */
