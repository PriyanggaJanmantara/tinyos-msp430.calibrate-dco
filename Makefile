# Makefile for calibrate DCO oscilator

TARGET=calibrate_dco

CC=msp430-gcc
CFLAGS+=-Os -Wall -mmcu=$(MCU) -mno-stack-init
LDFLAGS+=-mmcu=$(MCU)

CFLAGS+=-DWRITE_TO_FLASH
CFLAGS+=-DERASE_SEGMENT_A

ifneq ($(ERRATA),)
CFLAGS+=$(foreach _errata,$(ERRATA),$(patsubst %,-DERRATA_%,$(_errata)))
endif

msp430x2013:
	$(MAKE) clean $(TARGET) MCU=msp430x2013 ERRATA='BCL12'

msp430x2131:
	$(MAKE) clean $(TARGET) MCU=msp430x2131 ERRATA='BCL12'

msp430x2274:
	$(MAKE) clean $(TARGET) MCU=msp430x2274 ERRATA='BCL12'

msp430x1611:
	$(MAKE) clean $(TARGET) MCU=msp430x1611

calibrate_dco: calibrate_dco.o flash.o dco.o

clean:
	-rm -f *~ *.o $(TARGET)

.PHONY: all clean
