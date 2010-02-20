#include <io.h>

#if defined(WRITE_TO_FLASH)
void flash_erase_segment_a();
void flash_erase_mass() __attribute__ ((noreturn));
#else
static inline void flash_erase_segment_a() {}
static inline void flash_erase_mass() {}
#endif
void flash_write_word(uint16_t data, uintptr_t addr);

