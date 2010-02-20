#include "flash.h"
#include "dco.h"

#if defined(WRITE_TO_FLASH)

static void flash_lock_segment(void) __attribute__ ((noinline));
static void flash_unlock_segment(void) __attribute__ ((noinline));
static void flash_setup(void) __attribute__ ((noinline));

extern uint16_t _etext[];       /* end of .text section */
#define ETEXT_INDEX 7
uint16_t do_mass_erase_from_ram[] = {
    0x40b2, 0xa500, 0x012c,     /* mov #FWKEY,&FCTL3 */
    0x40b2, 0xa504, 0x0128,     /* mov #FWKEY|MERAS,&FCTL1 */
    0x4382, 0x0000,             /* mov #0,&_etext */
    0xb392, 0x012c,             /* 1: bit #BUSY,&FCTL3 */
    0x23fd,                     /* jnz 1b */
    0x40b2, 0xa510, 0x012c,     /* mov #FWKEY|LOCK,&FCTL3 */
    0xd3d2, 0x0021,             /* bis.b #1,&P1OUT */
    0x3fff,                     /* 2: jmp 2b */
};
typedef void (*func_in_ram_t)(void) __attribute__ ((noreturn));

static void flash_lock_segment(void) {
#if defined(__MSP430_HAS_FLASH2__)
    uint16_t lock_bits = (FCTL3 & LOCKA) ^ LOCKA;
    FCTL3 = FWKEY | LOCK | lock_bits;
#else
    FCTL3 = FWKEY | LOCK;
#endif
}

static void flash_unlock_segment(void) {
#if defined(__MSP430_HAS_FLASH2__)
    uint16_t lock_bits = FCTL3 & LOCKA;
    FCTL3 = FWKEY | lock_bits;
#else
    FCTL3 = FWKEY;
#endif
}

static void flash_setup() {
    dco_set(dco_1mhz_calibrated);
    FCTL2 = FWKEY | FSSEL_1 | (3 - 1); /* fFTG=MCLK/3=333kHz */
}

void flash_erase_segment_a() {
    flash_setup();
    flash_unlock_segment();
    FCTL1 = FWKEY | ERASE;
    *(uint16_t *)0x10c0 = 0;    /* trigger erase segment A */
    FCTL1 = FWKEY;
    flash_lock_segment();
}

void flash_erase_mass() {
    flash_setup();
    do_mass_erase_from_ram[ETEXT_INDEX] = (uintptr_t)_etext;
    ((func_in_ram_t)do_mass_erase_from_ram)();
#if 0
    FCTL3 = FWKEY;
    FCTL1 = FWKEY | MERAS;
    *_etext = 0;                /* trigger mass erase main memory */
    while (FCTL3 & BUSY)
        ;
    FCTL3 = FWKEY | LOCK;
    P1OUT |= 1;
    for (;;)
        ;
#endif
}

#else  /* !defined(WRITE_TO_FLASH) */

uint16_t calib_table[4];

#endif  /* defined(WRITE_TO_FLASH) */

void flash_write_word(uint16_t data, uintptr_t addr) {
#if defined(WRITE_TO_FLASH)
    flash_setup();
    flash_unlock_segment();
    FCTL1 = FWKEY | WRT;
    *(uint16_t *)addr = data;
    FCTL1 = FWKEY;
    flash_lock_segment();
#else
    calib_table[(addr - CALDCO_16MHZ_) / 2] = data;
#endif
}
