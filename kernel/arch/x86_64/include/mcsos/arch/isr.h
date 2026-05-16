#ifndef MCSOS_ARCH_ISR_H
#define MCSOS_ARCH_ISR_H

#include <stdint.h>

typedef void (*x86_64_isr_handler_t)(void);

/* Exception stubs M4 (0-31) */
extern x86_64_isr_handler_t x86_64_exception_stubs[32];

/* IRQ stubs M5 (32-47) */
extern x86_64_isr_handler_t x86_64_isr_stubs[16];

#endif
