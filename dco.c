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

#include "dco.h"
#include "delay.h"

void dco_set(uint16_t calib) {
#if defined(ERRATA_BCL12)
    /* Errata BCL12 - Switching RSEL can cause DCO dead time
     *  RSEL=14..15 -> RSEL=0..12 : switch to 13 first, then to target RSEL
     *  RSEL=0..12  -> RSEL=13..15: set DCO to 0 first, then set target RSEL, and set DCO. */
    uint8_t old_bc1 = BCSCTL1;
    uint8_t cur_rsel = old_bc1 & RSEL_MASK;
    uint8_t new_rsel = calib2rsel(calib);
    old_bc1 &= ~RSEL_MASK;
    if (cur_rsel >= 14 && new_rsel <= 12) {
        BCSCTL1 = old_bc1 | 13;
        BCSCTL1 = old_bc1 | new_rsel;
        DCOCTL = calib2dco(calib);
    } else {
        DCOCTL &= ~(DCO2|DCO1|DCO0); /* DCO=0 */
        BCSCTL1 = old_bc1 | new_rsel;
        DCOCTL = calib2dco(calib);
    }
#elif defined(ERRATA_BCL5)
    /* Errata BCL5 - RSEL bit modification can generate high frequency spikes on MCLK
     *  DIVM=0 or 1 cause this issue.
     * Setting DIVM=2 or 3 and modify RSEL and restore DIVM will work. */
    uint8_t old_bc2 = BCSCTL2;
    BCSCTL2 |= DIVM1;
    BCSCTL1 = (BCSCTL1 & ~RSEL_MASK) | calib2rsel(calib);
    DCOCTL = calib2dco(calib);
    BCSCTL2 = old_bc2;
#else
    BCSCTL1 = (BCSCTL1 & ~RSEL_MASK) | calib2rsel(calib);
    DCOCTL = calib2dco(calib);
#endif
}

static uint16_t dco_fine(uint16_t calib, uint16_t dco_khz) {
    dco_set(calib);
    P1OUT ^= 1;
    TACTL |= TACLR;             /* clear TAR */
    delay_1000n(dco_khz);
    return TAR;
}

static uint16_t dco_coarse(uint16_t calib, uint16_t dco_khz) {
    dco_set(calib);
    P1OUT ^= 1;
    TACTL |= TACLR;             /* clear TAR */
    delay_10n(dco_khz);
    return TAR;
}

uint16_t dco_calibrate(uint16_t dco_khz) {
    uint16_t calib = 0;
    uint16_t step;
    for (step = rsel2calib(RSEL_MAXBIT); step >= DCO0; step >>= 1) {
        if (dco_coarse(calib | step, dco_khz) > 32768/100)
            calib |= step;
    }

    for (step = rsel2calib(RSEL_MAXBIT); step != 0; step >>= 1) {
        if (dco_fine(calib | step, dco_khz) > 32768) {
            calib |= step;
        } else {
            calib &= ~step;
        }
    }

    if ((calib2dco(calib) & (DCO2 | DCO1 | DCO0)) == (DCO2 | DCO1 | DCO0))
        calib &= ~(MOD4 | MOD3 | MOD2 | MOD1 | MOD0); /* clear MODx */

    return calib | (XT2OFF << 8);
}

void dco_setup_calibrate() {
    /* set default DCO */
    dco_set(rsel2calib(RSEL_MAXBIT));  /* RSEL=MAX/2, DCO,MOD=0 */

    /* set LFXT1/32.768kHz XTAL oscillator */
    BCSCTL1 &= ~(XTS | DIVA1 | DIVA0); /* LF mode, ACLK=LFXT1CLK/1 */
#if defined(__MSP430_HAS_BC2__)
    BCSCTL3 = LFXT1S_0 | XCAP_2; /* LFXT1=32.768kHz, CAP=10pF */
    P2SEL |= 0xc0;
#endif

    /* turn on DCO and LFXT1 */
    _BIC_SR(SCG0 | OSCOFF);

    /* wait for oscillating */
    do {
        IFG1 &= ~OFIFG;
        delay_995();
    } while ((IFG1 & OFIFG) != 0);

    /* MCLK=DCOCLK/1, SMCLK=DCOCLK/1, Rosc external */
#if defined(__MSP430_HAS_BC2__)
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;
#else
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0 | DCOR;
#endif

    /* TimerA=ACLK/1, continuous mode, reset */
    TACTL = TASSEL_1 | ID_0 | MC_2 | TACLR;
}
