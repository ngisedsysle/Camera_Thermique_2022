/****************************************************************************
 * apps/examples/lepton/lepton.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "lepton.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lepton_image
 *
 * Description:
 *   get infrared image from lepton 2.5
 *
 ****************************************************************************/

// static int lepton_image(int fd, FAR struct lepton_image_s *buffer,
//                          FAR const struct lepton_exampleinfo_s *example)
// {
//   int ret;

//   if (ret < 0)
//     {
//       lepton_err("ERROR %d: couldn't get the infrared image\n", example->name, ret);
//       return ret;
//     }
//   sleep(2);

//   return OK;
// }

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * lepton_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  FAR struct lepton_image_s *image = malloc(sizeof( FAR struct lepton_image_s ));
  int fd;
  int temp;
  int ret;

  /* Open the configured LEPTON device */

  caminfo("Opening /dev/lepton ...\n");

  fd = open(CONFIG_EXAMPLES_LEPTON_DEVPATH, O_RDONLY);
  if (fd < 0)
    {
      int errcode = errno;
      fprintf(stderr,"ERROR: Failed to open %s: %d\n",
                CONFIG_EXAMPLES_LEPTON_DEVPATH, errcode);
      UNUSED(errcode);
      return EXIT_FAILURE;
    }

  /* Allocate the image structure */
  //image = (FAR struct lepton_image_s *)malloc(sizeof(struct lepton_image_s));

  // if (image == NULL)
  //   {
  //     lepton_err("ERROR: Failed to allocate display list buffer\n");
  //     close(fd);
  //     return EXIT_FAILURE;
  //   }

  ret = ioctl(fd, SNIOC_TEMP,(unsigned long)(&temp));
  if (ret < 0)
  {
    fprintf(stderr,"Failed to SNIOC_TEMP: errno = %d\n", errno);
    return ret;
  }
  ret = read(fd, image->buffer, LEPTON_FRAME_SIZE);
  if (ret < 0)
  {
    fprintf(stderr,"Failed to SNIOC_TEMP: errno = %d\n", errno);
    return ret;
  }
  return EXIT_SUCCESS;
}
