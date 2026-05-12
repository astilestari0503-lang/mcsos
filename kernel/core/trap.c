#include <stdint.h>

#include <mcsos/kernel/log.h>
#include <mcsos/kernel/panic.h>

typedef struct x86_64_trap_frame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t vector;
    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
} x86_64_trap_frame_t;

void x86_64_trap_dispatch(x86_64_trap_frame_t *frame) {
    log_write("[M4] exception/trap received\n");

    if (frame->vector == 3) {
        log_write("[M4] breakpoint handled\n");
        return;
    }

    kernel_panic_at(
        "trap.c",
        39,
        "Unhandled CPU exception",
        frame->vector
    );
}
