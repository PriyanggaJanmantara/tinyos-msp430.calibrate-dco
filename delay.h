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
