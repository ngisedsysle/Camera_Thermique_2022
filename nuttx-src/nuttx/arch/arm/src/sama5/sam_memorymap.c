/****************************************************************************
 * arch/arm/src/sama5/sam_memorymap.c
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

/* chip.h holds the characteristics of the configured chip */

#include <nuttx/config.h>
#include <arch/sama5/chip.h>

/* Include the correct logic for the configured chip */

#if defined(ATSAMA5D2)
#  include "sama5d2x_memorymap.c"
#elif defined(ATSAMA5D3)
#  include "sama5d3x_memorymap.c"
#elif defined(ATSAMA5D4)
#  include "sama5d4x_memorymap.c"
#else
#  error Unrecognized SAMA5 family
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
