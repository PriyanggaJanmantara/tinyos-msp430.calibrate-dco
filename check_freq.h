/* -*- mode: c; mode: flyspell-prog; -*- */
/*
 * Copyright (C) 2010 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <io.h>

#if defined(__MSP430_HAS_BC2__)

#if !defined(FREQ2_KHZ)
#define FREQ2_KHZ 8000          /* 8MHz */
#elif FREQ2_KHZ < 100
#error "FREQ2_KHZ is too low"
#elif FREQ2_KHZ > 16000
#error "FREQ2_KHZ is too high"
#endif

#if !defined(FREQ3_KHZ)
#define FREQ3_KHZ 12000         /* 12MHz */
#elif FREQ3_KHZ < 100
#error "FREQ3_KHZ is too low"
#elif FREQ3_KHZ > 16000
#error "FREQ3_KHZ is too high"
#endif

#if !defined(FREQ4_KHZ)
#define FREQ4_KHZ 16000         /* 16MHz */
#elif FREQ4_KHZ < 100
#error "FREQ4_KHZ is too low"
#elif FREQ4_KHZ > 16000
#error "FREQ4_KHZ is too high"
#endif

#else

#if !defined(FREQ2_KHZ)
#define FREQ2_KHZ 8000          /* 8MHz */
#elif FREQ2_KHZ < 100
#error "FREQ2_KHZ is too low"
#elif FREQ2_KHZ > 8000
#error "FREQ2_KHZ is too high"
#endif

#if !defined(FREQ3_KHZ)
#define FREQ3_KHZ 2000          /* 2MHz */
#elif FREQ3_KHZ < 100
#error "FREQ3_KHZ is too low"
#elif FREQ3_KHZ > 8000
#error "FREQ3_KHZ is too high"
#endif

#if !defined(FREQ4_KHZ)
#define FREQ4_KHZ 4000          /* 4MHz */
#elif FREQ4_KHZ < 100
#error "FREQ4_KHZ is too low"
#elif FREQ4_KHZ > 8000
#error "FREQ4_KHZ is too high"
#endif

#endif

/*
 * Local Variables:
 * c-file-style: "bsd"
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vim: set et ts=4 sw=4:
 */
