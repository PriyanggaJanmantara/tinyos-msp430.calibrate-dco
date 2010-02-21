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
#include "flash.h"
#include "delay.h"
#include "copy.h"

#if defined(__MSP430_HAS_FLASH2__)
#define SEGMENT_A 0x10c0
uint16_t backup_segment_a[32];  /* 64 bytes */
#define INDEX_16MHZ 28
#define INDEX_12MHZ 29
#define INDEX_8MHZ 30
#define INDEX_1MHZ 31
#else
#define SEGMENT_A 0x1080
uint16_t backup_segment_a[64];  /* 128 byte */
#define INDEX_4MHZ 60
#define INDEX_2MHZ 61
#define INDEX_8MHZ 62
#define INDEX_1MHZ 63
#endif

static void blink_led(void) {
    dco_set(backup_segment_a[INDEX_1MHZ]);
    uint16_t notify_loop = 10;
    while (--notify_loop != 0) {
        /* turn on LED */
        P1OUT = 0x01;
        delay_1000n(100);

        /* turn off LED */
        P1OUT = 0x00;
        delay_1000n(900);
    }
}

int main() __attribute__ ((noreturn));
int main() {
    /* Configure P1.0 as output */
    P1DIR = 0x01;

    copy_word(backup_segment_a, (const uint16_t *)SEGMENT_A, ARRAY_SIZE(backup_segment_a));

    dco_setup_calibrate();
#if defined(__MSP430_HAS_BC2__)
    backup_segment_a[INDEX_1MHZ] = dco_calibrate(1000); /* 1MHz */
    backup_segment_a[INDEX_8MHZ] = dco_calibrate(8000); /* 8MHz */
    backup_segment_a[INDEX_12MHZ] = dco_calibrate(12000); /* 12MHz */
    backup_segment_a[INDEX_16MHZ] = dco_calibrate(16000); /* 16MHz */
#else
    backup_segment_a[INDEX_1MHZ] = dco_calibrate(1000); /* 1MHz */
    backup_segment_a[INDEX_8MHZ] = dco_calibrate(8000); /* 8MHz */
    backup_segment_a[INDEX_2MHZ] = dco_calibrate(2000); /* 2MHz */
    backup_segment_a[INDEX_4MHZ] = dco_calibrate(4000); /* 4MHz */
#endif

#if defined(WRITE_TO_FLASH)
    flash_setup(backup_segment_a[INDEX_1MHZ]);
    flash_erase_segment_a();
    flash_write_block(backup_segment_a, (uint16_t *)SEGMENT_A, ARRAY_SIZE(backup_segment_a));
    blink_led();
    flash_erase_mass();         /* erase main flash, then turn on LED */
#else
    for (;;)
        blink_led();
#endif
}
