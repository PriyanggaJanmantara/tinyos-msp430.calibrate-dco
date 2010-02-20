#include "dco.h"
#include "flash.h"
#include "delay.h"

int main() __attribute__ ((noreturn));

int main() {
    /* Configure P1.0 as output */
    P1DIR = 0x01;

    dco_setup();

    uint16_t calib = dco_calibrate(1000); /* 1MHz */
    dco_1mhz_calibrated = calib;
#if defined(ERASE_SEGMENT_A)
    flash_erase_segment_a();
#endif
    flash_write_word(calib, CALDCO_1MHZ_);

    calib = dco_calibrate(8000); /* 8MHz */
    flash_write_word(calib, CALDCO_8MHZ_);

    calib = dco_calibrate(12000); /* 12MHz */
    flash_write_word(calib, CALDCO_12MHZ_);

    calib = dco_calibrate(16000); /* 16MHz */
    flash_write_word(calib, CALDCO_16MHZ_);

    dco_set(dco_1mhz_calibrated);
    
    uint16_t notify_loop = 10;
    while (--notify_loop != 0) {
        /* turn on LED */
        P1OUT = 0x01;
        delay_1000n(100);

        /* turn off LED */
        P1OUT = 0x00;
        delay_1000n(900);
    }

    flash_erase_mass();
}
