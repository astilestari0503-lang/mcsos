#include <stdint.h>
#include <mcsos/arch/cpu.h>
#include <mcsos/arch/idt.h>
#include <mcsos/arch/pic.h>
#include <mcsos/arch/pit.h>
#include <mcsos/kernel/log.h>
#include <mcsos/kernel/panic.h>
#include <mcsos/kernel/version.h>
#include <mcsos/kernel/pmm.h>
#include "mcsos/kmem.h"
#include <mcsos/kernel/vmm.h>
#include <mcsos/kernel/serial.h>
#include "mcsos_thread.h"
#include <mcsos/syscall.h>
#include <mcsos/user/m11_elf_loader.h>
#include "mcs_vfs.h"


/* M11 integration test — defined in kernel/user/m11_integration.c */
void m11_integration_test(void);

/* M12 sync selftest — defined in kernel/m12_selftest.c */
void m12_sync_selftest(void);

extern void m14_block_demo_init(void);

extern char __kernel_start[];
extern char __kernel_end[];
static struct pmm_state kernel_pmm;
static struct vmm_space kernel_space;
static uint64_t hhdm_offset = 0;
#define M8_BOOT_HEAP_SIZE (64u * 1024u)

static unsigned char m8_boot_heap[M8_BOOT_HEAP_SIZE]
    __attribute__((aligned(4096)));

static uint8_t kernel_pmm_bitmap[PMM_BITMAP_BYTES]
    __attribute__((aligned(4096)));

static struct boot_mem_region demo_regions[] = {
    {
        .base = 0x00100000,
        .length = 0x03F00000,
        .type = BOOT_MEM_USABLE
    }
};

/* M9: scheduler global state */
static mcsos_scheduler_t g_sched;
static mcs_ramfs_t kernel_ramfs;
static mcsos_thread_t    g_boot_thread;
static mcsos_thread_t    g_thread_a;
static mcsos_thread_t    g_thread_b;
static unsigned char g_stack_a[8192] __attribute__((aligned(16)));
static unsigned char g_stack_b[8192] __attribute__((aligned(16)));

/* M10 syscall callbacks */

static uint64_t k_get_ticks(void)
{
    return g_sched.ticks;
}

static void k_yield_current(void)
{
    (void)mcsos_sched_yield(&g_sched);
}

static void k_exit_current(int code)
{
    if (g_sched.current != 0) {
        g_sched.current->exit_code = code;
        g_sched.current->state = MCSOS_THREAD_ZOMBIE;
    }
}

static int64_t k_write_serial(
    const char *buf,
    size_t len
)
{
    if (buf == 0) {
        return -1;
    }

    for (size_t i = 0; i < len; i++) {

        char tmp[2];

        tmp[0] = buf[i];
        tmp[1] = '\0';

        serial_write_string(tmp);
    }

    return (int64_t)len;
}


static void m4_selftest(void) {
    KERNEL_ASSERT(__kernel_end > __kernel_start);
    KERNEL_ASSERT(sizeof(uintptr_t) == 8u);
    KERNEL_ASSERT(sizeof(x86_64_idt_entry_t) == 16u);
    KERNEL_ASSERT(x86_64_idt_base_for_test() != 0u);
    KERNEL_ASSERT(x86_64_idt_limit_for_test() == 4095u);
    log_writeln("[M4] selftest: IDT invariants passed");
}

static uint64_t kernel_vmm_alloc(void *ctx) {
    (void)ctx;
    return pmm_alloc_frame(&kernel_pmm);
}

static void kernel_vmm_free(void *ctx, uint64_t frame_paddr) {
    (void)ctx;
    pmm_free_frame(&kernel_pmm, frame_paddr);
}

static void *kernel_phys_to_virt(void *ctx, uint64_t paddr) {
    uint64_t offset = *(uint64_t *)ctx;
    return (void *)(offset + paddr);
}

static void memzero(void *ptr, uint64_t size) {
    uint8_t *p = (uint8_t *)ptr;
    for (uint64_t i = 0; i < size; i++) {
        p[i] = 0;
    }
}

static void m8_heap_bootstrap(void) {
    int rc = kmem_init(m8_boot_heap, sizeof(m8_boot_heap));
    if (rc != 0) {
        KERNEL_PANIC("M8 kmem_init failed", rc);
    }
    void *probe = kmem_alloc(128);
    if (probe == 0) {
        KERNEL_PANIC("M8 kmem_alloc probe failed", 0);
    }
    if (kmem_free_checked(probe) != 0) {
        KERNEL_PANIC("M8 kmem_free_checked probe failed", 0);
    }
    kmem_stats_t st;
    kmem_get_stats(&st);
    log_writeln("[M8] kmem initialized");
    serial_write_string("[M8] total=");
    serial_write_dec64(st.total_bytes);
    serial_write_string(" free=");
    serial_write_dec64(st.free_bytes);
    serial_write_string(" largest=");
    serial_write_dec64(st.largest_free);
    serial_write_string(" blocks=");
    serial_write_dec64(st.block_count);
    serial_write_string("\n");
}

/* M9: thread entry functions */
static void demo_thread_a(void *arg) {
    (void)arg;
    for (;;) {
        log_writeln("[M9] thread A tick");
        mcsos_sched_yield(&g_sched);
    }
}

static void demo_thread_b(void *arg) {
    (void)arg;
    for (;;) {
        log_writeln("[M9] thread B tick");
        mcsos_sched_yield(&g_sched);
    }
}

static void m10_syscall_smoke_direct(void)
{
    int64_t r;

    r = mcsos_syscall_dispatch(
        MCSOS_SYS_PING,
        0,0,0,0,0,0
    );

    if (r != 0x2605020A) {
        KERNEL_PANIC(
            "M10 syscall ping failed",
            0
        );
    }

    log_writeln("[M10] syscall ping ok");

    r = mcsos_syscall_dispatch(
        MCSOS_SYS_GET_TICKS,
        0,0,0,0,0,0
    );

    if (r < 0) {
        KERNEL_PANIC(
            "M10 syscall get_ticks failed",
            (uint64_t)(-r)
        );
    }

    log_writeln("[M10] syscall get_ticks ok");

    log_writeln("[M10] syscall smoke done");
}
static void m13_vfs_smoke_test(void)
{
    mcs_process_t proc;
    char buf[32];
    int fd;
    mcs_ssize_t n;

    mcs_ramfs_init(&kernel_ramfs);

    if (mcs_ramfs_seed_file(
            &kernel_ramfs,
            "/hello.txt",
            (const uint8_t *)"hello-mcsos",
            11) != MCS_OK)
    {
        KERNEL_PANIC("M13 seed file failed", 0);
    }

    proc.pid = 13;
    mcs_fd_table_init(&proc.fd_table);

    fd = mcs_sys_open(
        &proc,
        &kernel_ramfs,
        "/hello.txt",
        MCS_O_RDONLY);

    if (fd < 0) {
        KERNEL_PANIC("M13 open failed", 0);
    }

    n = mcs_sys_read(
        &proc,
        fd,
        buf,
        11);

    if (n != 11) {
        KERNEL_PANIC("M13 read failed", 0);
    }

    buf[11] = '\0';

    log_writeln("[M13] RAMFS smoke PASS");
    serial_write_string("[M13] data=");
    serial_write_string(buf);
    serial_write_string("\n");

    mcs_sys_close(&proc, fd);
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

    /* 3. Init IDT */
    log_key_value_hex64("rflags_before_idt", cpu_read_rflags());
    x86_64_idt_init();
    m4_selftest();

    bool ok = pmm_init_from_map(
        &kernel_pmm,
        demo_regions,
        1,
        kernel_pmm_bitmap,
        sizeof(kernel_pmm_bitmap),
        PMM_MAX_PHYS_BYTES
    );
    if (!ok) {
        KERNEL_PANIC("PMM init failed", 0x504D4Du);
    }
    log_writeln("[M6] PMM initialized");

    uint64_t frame = pmm_alloc_frame(&kernel_pmm);
    log_write("[M6] sample frame = ");
    log_key_value_hex64("", frame);
    pmm_free_frame(&kernel_pmm, frame);

    hhdm_offset = 0xFFFF800000000000ULL;
    uint64_t root = pmm_alloc_frame(&kernel_pmm);
    if (root == PMM_INVALID_FRAME) {
        KERNEL_PANIC("M7: cannot allocate root page table", 0x4D37u);
    }
    void *root_virt = kernel_phys_to_virt(&hhdm_offset, root);
    memzero(root_virt, 4096);

    int rc = vmm_space_init(
        &kernel_space,
        root,
        &hhdm_offset,
        kernel_vmm_alloc,
        kernel_vmm_free,
        kernel_phys_to_virt
    );
    if (rc != VMM_MAP_OK) {
        KERNEL_PANIC("M7: vmm_space_init failed", 0x4D37u);
    }
    log_writeln("[M7] VMM core initialized");

    m8_heap_bootstrap();
    log_writeln("[M8] checkpoint reached");

    /* 4. Init PIC */
    pic_remap(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pic_mask_all();
    pic_unmask_irq(0);
    log_write("[MCSOS:M5] PIC remapped; mask master=0x");
    log_key_value_hex64("", pic_read_master_mask());
    log_write(" slave=0x");
    log_key_value_hex64("", pic_read_slave_mask());
    log_writeln("");

    /* 5. Init PIT 100Hz */
    pit_configure_hz(100u);
    log_writeln("[MCSOS:M5] PIT configured 100Hz");

    /* 6. Aktifkan interrupt */
    log_writeln("[MCSOS:M5] STI: enabling interrupts");
    cpu_sti();

    /* 7. M9: inisialisasi scheduler */
    mcsos_scheduler_init(&g_sched, &g_boot_thread);
    mcsos_thread_prepare(&g_thread_a, "demo-a",
                         demo_thread_a, (void *)0,
                         g_stack_a, sizeof(g_stack_a),
                         g_sched.next_id++);
    mcsos_thread_prepare(&g_thread_b, "demo-b",
                         demo_thread_b, (void *)0,
                         g_stack_b, sizeof(g_stack_b),
                         g_sched.next_id++);
    mcsos_sched_enqueue(&g_sched, &g_thread_a);
    mcsos_sched_enqueue(&g_sched, &g_thread_b);
    log_writeln("[M9] scheduler initialized");

mcsos_syscall_ops_t ops = {
    .get_ticks = k_get_ticks,
    .yield_current = k_yield_current,
    .exit_current = k_exit_current,
    .write_serial = k_write_serial
};

mcsos_syscall_init(&ops);

mcsos_syscall_set_user_region(
    (mcsos_user_region_t){
        .base  = 0x0000000000400000ULL,
        .limit = 0x0000800000000000ULL
    }
);

log_writeln("[M10] syscall init");

m10_syscall_smoke_direct();

m11_integration_test();

/* M12: synchronization primitives selftest */
m12_sync_selftest();

/* M13: VFS/RAMFS smoke test */
m13_vfs_smoke_test();

m14_block_demo_init();
log_writeln("M14: block layer initialized");

mcsos_sched_yield(&g_sched);

    for (;;) {
        cpu_hlt();
    }
}
