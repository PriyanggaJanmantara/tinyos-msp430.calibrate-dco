#include <io.h>

#if !defined(CALBC1_1MHZ_)
#define CALDCO_16MHZ_         0x10F8    /* DCOCTL  Calibration Data for 16MHz */
#define CALBC1_16MHZ_         0x10F9    /* BCSCTL1 Calibration Data for 16MHz */
#define CALDCO_12MHZ_         0x10FA    /* DCOCTL  Calibration Data for 12MHz */
#define CALBC1_12MHZ_         0x10FB    /* BCSCTL1 Calibration Data for 12MHz */
#define CALDCO_8MHZ_          0x10FC    /* DCOCTL  Calibration Data for 8MHz */
#define CALBC1_8MHZ_          0x10FD    /* BCSCTL1 Calibration Data for 8MHz */
#define CALDCO_1MHZ_          0x10FE    /* DCOCTL  Calibration Data for 1MHz */
#define CALBC1_1MHZ_          0x10FF    /* BCSCTL1 Calibration Data for 1MHz */
#endif

#if defined(__MSP430_HAS_BC2__)
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

extern uint16_t dco_1mhz_calibrated;
void dco_set(uint16_t calib);
uint16_t dco_calibrate(uint16_t dco_khz);
void dco_setup(void);
