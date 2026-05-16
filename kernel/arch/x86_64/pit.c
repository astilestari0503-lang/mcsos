#include <stdint.h>
#include <mcsos/arch/io.h>
#include <mcsos/arch/pit.h>
#include <mcsos/kernel/log.h>
#include <mcsos/kernel/serial.h>

#define PIT_CHANNEL0 0x40u
#define PIT_COMMAND  0x43u

static volatile uint64_t g_ticks = 0;

void pit_configure_hz(uint32_t hz) {
    if (hz == 0u) {
        hz = 100u;
    }
    
    uint32_t divisor = PIT_BASE_FREQUENCY_HZ / hz;
    if (divisor == 0u) {
        divisor = 1u;
    }
    if (divisor > 0xFFFFu) {
        divisor = 0xFFFFu;
    }

    /* Command: channel 0, lobyte/hibyte, mode 3 (square wave), binary */
    outb(PIT_COMMAND, 0x36u);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFFu));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8u) & 0xFFu));
}

uint64_t timer_ticks(void) {
    return g_ticks;
}

void timer_on_irq0(void) {
    ++g_ticks;
    
    /* Log setiap 100 tick (1 detik) */
    if ((g_ticks % 100u) == 0u) {
        serial_write_string("[MCSOS:TIMER] ticks=");
        serial_write_dec64(g_ticks);
        serial_write_string("\n");
    }
}
