#
#  Copyright (C) 2010 Tadashi G. Takaoka
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

# Makefile for calibrate DCO oscillator

TARGET=calibrate_dco

CC=msp430-gcc
CFLAGS+=-Os -Wall -mmcu=$(MCU) -mno-stack-init
LDFLAGS+=-mmcu=$(MCU)

CFLAGS+=-DWRITE_TO_FLASH

ifneq ($(ERRATA),)
CFLAGS+=$(foreach _errata,$(ERRATA),$(patsubst %,-DERRATA_%,$(_errata)))
endif

default: msp430x2013

msp430x1132:
	$(MAKE) clean $(TARGET) MCU=msp430x1132 ERRATA='BCL5'

msp430x1232:
	$(MAKE) clean $(TARGET) MCU=msp430x1232 ERRATA='BCL5'

msp430x1611:
	$(MAKE) clean $(TARGET) MCU=msp430x1611 ERRATA='BCL5'

msp430x1612:
	$(MAKE) clean $(TARGET) MCU=msp430x1612 ERRATA='BCL5'

msp430x2012:
	$(MAKE) clean $(TARGET) MCU=msp430x2012 ERRATA='BCL12'

msp430x2013:
	$(MAKE) clean $(TARGET) MCU=msp430x2013 ERRATA='BCL12'

msp430x2131:
	$(MAKE) clean $(TARGET) MCU=msp430x2131 ERRATA='BCL12'

msp430x2132:
	$(MAKE) clean $(TARGET) MCU=msp430x2132 ERRATA='BCL12'

msp430x2274:
	$(MAKE) clean $(TARGET) MCU=msp430x2274 ERRATA='BCL12'

calibrate_dco: calibrate_dco.o flash.o dco.o copy.o

clean:
	-rm -f *~ *.o $(TARGET)

.PHONY: all clean
