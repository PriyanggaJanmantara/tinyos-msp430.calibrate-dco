/* -*- mode: c; mode: flyspell-prog; -*- */
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

static inline void delay_10n(uint16_t n) {
    do {
        __asm__ __volatile__ ("nop"); /* 1 cycle */
        __asm__ __volatile__ ("jmp $+2"); /* 2 cycles */
        __asm__ __volatile__ ("jmp $+2"); /* 2 cycles */
        __asm__ __volatile__ ("jmp $+2"); /* 2 cycles */
    } while (--n != 0);         /* add, jnz: (1+2)=3 cycles */
}

static inline void delay_995(void) {
    uint16_t m = 331;           /* mov: 2 cycles */
    do {
        __asm__ __volatile__ ("");
    } while (--m != 0);         /* add, jnz: (1+2)*331=993 cycles */
}

static inline void delay_1000n(uint16_t n) {
    do {
        delay_995();
        __asm__ __volatile__ ("nop"); /* 1 cycles */
    } while (--n != 0);         /* add, cmp, jnz: (1+1+2)=4 cycles */
}

/*
 * Local Variables:
 * c-file-style: "bsd"
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 * vim: set et ts=4 sw=4:
 */
