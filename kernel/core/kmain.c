#include <stdint.h>
#include <mcsos/arch/cpu.h>
#include <mcsos/arch/idt.h>
#include <mcsos/arch/pic.h>
#include <mcsos/arch/pit.h>
#include <mcsos/kernel/log.h>
#include <mcsos/kernel/panic.h>
#include <mcsos/kernel/version.h>

extern char __kernel_start[];
extern char __kernel_end[];

static void m4_selftest(void) {
    KERNEL_ASSERT(__kernel_end > __kernel_start);
    KERNEL_ASSERT(sizeof(uintptr_t) == 8u);
    KERNEL_ASSERT(sizeof(x86_64_idt_entry_t) == 16u);
    KERNEL_ASSERT(x86_64_idt_base_for_test() != 0u);
    KERNEL_ASSERT(x86_64_idt_limit_for_test() == 4095u);
    log_writeln("[M4] selftest: IDT invariants passed");
}

void kmain(void) {
    /* 1. Matikan interrupt selama init */
    cpu_cli();
    
    /* 2. Init serial/logging */
    log_init();
    log_write(MCSOS_NAME);
    log_write(" ");
    log_write(MCSOS_VERSION);
    log_write(" ");
    log_write(MCSOS_MILESTONE);
    log_writeln(" kernel entered");
    log_key_value_hex64("kernel_start", (uint64_t)(uintptr_t)__kernel_start);
    log_key_value_hex64("kernel_end", (uint64_t)(uintptr_t)__kernel_end);
    
    /* 3. Init IDT (M4 + M5) */
    log_key_value_hex64("rflags_before_idt", cpu_read_rflags());
    x86_64_idt_init();
    m4_selftest();
    
    /* 4. Init PIC (M5) */
    pic_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pic_mask_all();
    pic_unmask_irq(0);  /* Buka IRQ0 (timer) */
    log_write("[MCSOS:M5] PIC remapped; mask master=0x");
    log_key_value_hex64("", pic_read_master_mask());
    log_write(" slave=0x");
    log_key_value_hex64("", pic_read_slave_mask());
    log_writeln("");
    
    /* 5. Init PIT 100Hz (M5) */
    pit_configure_hz(100u);
    log_writeln("[MCSOS:M5] PIT configured 100Hz");
    
    /* 6. Aktifkan interrupt */
    log_writeln("[MCSOS:M5] STI: enabling interrupts");
    cpu_sti();
    
    /* 7. Halt loop - tunggu interrupt */
    log_writeln("[MCSOS:M5] Entering halt loop, waiting for timer IRQ...");
    
#ifdef MCSOS_M4_TRIGGER_BREAKPOINT
    log_writeln("[M4] triggering intentional breakpoint exception");
    x86_64_trigger_breakpoint_for_test();
    log_writeln("[M4] returned from breakpoint handler");
#endif

#ifdef MCSOS_M4_TRIGGER_PANIC
    KERNEL_PANIC("intentional M4 panic test", 0x4D43534F533034u);
#endif

    for (;;) {
        cpu_hlt();
    }
}
