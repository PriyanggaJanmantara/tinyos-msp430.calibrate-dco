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

void flash_setup(uint16_t calibrated_1mhz_dco);
void flash_erase_segment_a();
void flash_erase_mass() __attribute__ ((noreturn));
void flash_write_block(const uint16_t *from, uint16_t *to, uint16_t words);
#if 0
void flash_write_word(uint16_t data, uintptr_t addr);
#endif
