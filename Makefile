# -*- mode: makefile-gmake; mode: flyspell; -*-
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

# Options may be specified by OPTION variable in command line.
#
# WRITE_TO_FLASH
#   if you want to erase the Segment A flash and write the
#
# USE_ROSC
#   if you want to use external Rosc.
#
# LFXT1_HZ=<frequency_in_hz>
#   if you want to use the XTAL for LFXT1 other than 32.768kHz
#
# FREQn_KHZ=<nth_calibration_frequency_in_khz>
#   if you want to alter the calibration frequency. n=2..4

# For example
#
#   make msp430x2132 OPTION='WRITE_TO_FLASH LFXT1=32768' ERRATA='BCL12 XOSC8'
# 
# will make calibrate_dco which writes calibrated values of DCO, using
# 32768kHz LFXT1 xtal, onto Segment A flash for msp430x2131, which has
# errata BCL12 and XOSC8.

default: msp430x2013

msp430%:
ifneq ($(ERRATA),)
	$(MAKE) clean $(TARGET) MCU=$@ ERRATA="$(ERRATA)"
else
	$(MAKE) clean $(TARGET) MCU=$@ ERRATA="$(ERRATA_$@)"
endif

# Default silicon errata table
ERRATA_msp430x1132=BCL5
ERRATA_msp430x1232=BCL5
ERRATA_msp430x1611=BCL5
ERRATA_msp430x1612=BCL5
ERRATA_msp430x2012=BCL12
ERRATA_msp430x2013=BCL12
ERRATA_msp430x2131=BCL12
ERRATA_msp430x2132=BCL12 XOSC8
ERRATA_msp430x2274=BCL12 DCOR

CC=msp430-gcc
CFLAGS+=-O2 -Wall -mmcu=$(MCU) -mno-stack-init
LDFLAGS+=-mmcu=$(MCU)

ifneq ($(OPTION),)
CFLAGS+=$(foreach _opt,$(OPTION),$(patsubst %,-D%,$(_opt)))
endif

ifneq ($(ERRATA),)
CFLAGS+=$(foreach _errata,$(ERRATA),$(patsubst %,-DERRATA_%,$(_errata)))
endif

calibrate_dco: calibrate_dco.o flash.o dco.o copy.o

clean:
	-rm -f *~ *.o $(TARGET)

.PHONY: all clean

# vim: set noet ts=8 sw=8:
