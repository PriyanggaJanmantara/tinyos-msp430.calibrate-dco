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

#include "flash.h"
#include "dco.h"
#include "copy.h"

uint16_t code_in_ram[18];
#define CODE_IN_FLASH 0           /* must be 0, debug purpose */

static void flash_lock_segment(void) __attribute__ ((noinline));
static void flash_lock_segment(void) {
#if defined(__MSP430_HAS_FLASH2__)
    uint16_t lock_bits = (FCTL3 & LOCKA) ^ LOCKA;
    FCTL3 = FWKEY | LOCK | lock_bits;
#else
    FCTL3 = FWKEY | LOCK;
#endif
}

static void flash_unlock_segment(void) __attribute__ ((noinline));
static void flash_unlock_segment(void) {
#if defined(__MSP430_HAS_FLASH2__)
    uint16_t lock_bits = FCTL3 & LOCKA;
    FCTL3 = FWKEY | lock_bits;
#else
    FCTL3 = FWKEY;
#endif
}

void flash_setup(uint16_t calibrated_1mhz_dco) {
    dco_set(calibrated_1mhz_dco);
    FCTL2 = FWKEY | FSSEL_1 | (3 - 1); /* fFTG=MCLK/3=333kHz */
}

void flash_erase_segment_a() {
    flash_unlock_segment();
    FCTL1 = FWKEY | ERASE;
    *(uint16_t *)0x10c0 = 0;    /* trigger erase segment A */
    FCTL1 = FWKEY;
    flash_lock_segment();
}

#if CODE_IN_FLASH
static void do_mass_erase(uint16_t *etext) __attribute__ ((noreturn,noinline));
static void do_mass_erase(uint16_t *etext) {
    FCTL3 = FWKEY;              /* unlock main flash */
    FCTL1 = FWKEY | MERAS;
    *etext = 0;                 /* trigger mass erase main flash */
    while (FCTL3 & BUSY)        /* wait for erasing */
        ;
    FCTL3 = FWKEY | LOCK;       /* lock main flash */
    P1OUT |= 1;                 /* turn LED on */
    for (;;)
        ;
}
#else
typedef void (*do_mass_erase_t)(uint16_t *etext) __attribute__ ((noreturn));
#define do_mass_erase(etext)                                            \
    do {                                                                \
        copy_word(code_in_ram, do_mass_erase_body, ARRAY_SIZE(do_mass_erase_body)); \
        ((do_mass_erase_t)code_in_ram)(etext);                          \
    } while (0)
static const uint16_t do_mass_erase_body[] = {
    0x40b2, 0xa500, 0x012c,     /* mov #FWKEY,&FCTL3 */
    0x40b2, 0xa504, 0x0128,     /* mov #FWKEY|MERAS,&FCTL1 */
    0x438f, 0x0000,             /* mov #0,0(r15) */
    0xb392, 0x012c,             /* 1: bit #BUSY,&FCTL3 */
    0x23fd,                     /* jnz 1b */
    0x40b2, 0xa510, 0x012c,     /* mov #FWKEY|LOCK,&FCTL3 */
    0xd3d2, 0x0021,             /* bis.b #1,&P1OUT */
    0x3fff,                     /* 2: jmp 2b */
};
#endif

void flash_erase_mass() {
    extern uint16_t _etext[];   /* end of .text section */
    do_mass_erase(_etext);
}

#if CODE_IN_FLASH
static void do_block_write(uint16_t *to, const uint16_t *from, uint16_t *end)
    __attribute__ ((noinline));
static void do_block_write(uint16_t *to, const uint16_t *from, uint16_t *end) {
    FCTL1 = FWKEY | BLKWRT | WRT; /* enable block write */
    while (to < end) {
        *to++ = *from++;
        while ((FCTL3 & WAIT) == 0) /* wait for buffer ready */
            ;
    }
    FCTL1 = FWKEY;
    while (FCTL3 & BUSY)        /* wait for writing completion */
        ;
}
#else
typedef void (*do_block_write_t)(uint16_t *to, const uint16_t *from, uint16_t *end);
#define do_block_write(to, from, end)                                   \
    do {                                                                \
        copy_word(code_in_ram, do_block_write_body, ARRAY_SIZE(do_block_write_body)); \
        ((do_block_write_t)code_in_ram)((to), (from), (end));           \
    } while (0)
static const uint16_t do_block_write_body[] = {
    0x40b2, 0xa5c0, 0x0128,     /* mov #FWKEY|BLKWRT|WRT,&FCTL1 */
    0x4ebf, 0x0000,             /* 1: mov @r14+,0(r15) */
    0xb2b2, 0x012c,             /* 2: bit #WAIT,&FCTL3 */
    0x27fd,                     /* jz 2b */
    0x532f,                     /* incd r15 */
    0x9d0f,                     /* cmp r13,r15 */
    0x2bf8,                     /* jnc 1b */
    0x40b2, 0xa500, 0x0128,     /* mov #FWKEY,&FCTL1 */
    0xb392, 0x012c,             /* 3: bit #BUSY,&FCTL3 */
    0x23fd,                     /* jnz 3b */
    0x4130,                     /* ret */
};
#endif

void flash_write_block(const uint16_t *from, uint16_t *to, uint16_t words) {
    flash_unlock_segment();
    while (words > 0) {
        uint16_t size = (words < 32) ? words : 32;
        do_block_write(to, from, to + size);
        to += size;
        from += size;
        words -= size;
    }
    flash_unlock_segment();
}

#if 0
void flash_write_word(uint16_t data, uintptr_t addr) {
    flash_unlock_segment();
    FCTL1 = FWKEY | WRT;
    *(uint16_t *)addr = data;
    FCTL1 = FWKEY;
    flash_lock_segment();
}
#endif
