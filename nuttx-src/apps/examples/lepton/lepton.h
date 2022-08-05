// TODO : put license here

#ifndef __APPS_EXAMPLES_LEPTON_LEPTON_H
#define __APPS_EXAMPLES_LEPTON_LEPTON_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/ioctl.h>
#include <nuttx/config.h>
#include <nuttx/compiler.h>
#include <nuttx/camera/ioctl.h>

#include <stdio.h>
#include <debug.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <nuttx/camera/lepton.h>

#ifdef CONFIG_EXAMPLES_LEPTON

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* NOTE: These rely on internal definitions from compiler.h and debug.h.
 * Could be a porting issue.
 */

#if !defined(GRAPHICS_LEPTON_DEBUG_ERROR)
#  define lepton_err _none
#elif defined(CONFIG_CPP_HAVE_VARARGS)
#  define lepton_err(format, ...) \
     fprintf(stderr, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#  define lepton_err printf
#endif

#if !defined(GRAPHICS_LEPTON_DEBUG_WARN)
#  define lepton_warn _none
#elif defined(CONFIG_CPP_HAVE_VARARGS)
#  define lepton_warn(format, ...) \
     fprintf(stderr, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#  define lepton_warn printf
#endif

#if !defined(GRAPHICS_LEPTON_DEBUG_INFO)
#  define lepton_info _none
#elif defined(CONFIG_CPP_HAVE_VARARGS)
#  define lepton_info(format, ...) \
     printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#  define lepton_info printf
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
// struct lepton_exampleinfo_s
// {
//   FAR const char *name;
//   lepton_example_t func;
// };

typedef struct lepton_image_s
{
  uint16_t buffer[LEPTON_FRAME_SIZE];
};
/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This is the type of one display example entry point */

typedef CODE int (*lepton_example_t)(int fd,FAR struct lepton_image_s *image);



/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/


#endif /* CONFIG_EXAMPLES_LEPTON */
#endif /* __APPS_EXAMPLES_LEPTON_LEPTON_H */
