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

#if defined(__MSP430_HAS_BC2__) && !defined(USE_ROSC)
static const uint8_t RSEL_MASK = RSEL3|RSEL2|RSEL1|RSEL0;
static const uint8_t RSEL_MAXBIT = RSEL3;
#else
static const uint8_t RSEL_MASK = RSEL2|RSEL1|RSEL0;
static const uint8_t RSEL_MAXBIT = RSEL2;
#endif

static inline uint8_t calib2dco(uint16_t calib) {
    return calib & 0xff;
}

static inline uint8_t calib2rsel(uint16_t calib) {
    return (calib >> 8) & RSEL_MASK;
}

static inline uint16_t rsel2calib(uint8_t rsel) {
    return rsel << 8;
}

void dco_set(uint16_t calib);
void dco_setup_calibrate(void);
uint16_t dco_calibrate(uint16_t dco_khz);

/*
 * Local Variables:
 * c-file-style: "bsd"
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vim: set et ts=4 sw=4:
 */
