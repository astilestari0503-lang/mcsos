#include <mcsos/user/m11_elf_loader.h>
#include <mcsos/kernel/log.h>
#include <mcsos/kernel/serial.h>

/*
 * Kontrak integrasi M11 — hanya planning, belum ring 3.
 * Dipanggil dari kmain() setelah M10 syscall init selesai.
 */

/* ELF sintetis minimal: 2 PT_LOAD segment, entry = 0x401000 */
static unsigned char m11_synthetic_elf[12288];
static int m11_synthetic_built = 0;

static void m11_build_synthetic_elf(void) {
    /* zero seluruh buffer */
    for (int i = 0; i < 12288; i++) m11_synthetic_elf[i] = 0;

    struct m11_elf64_ehdr *eh = (struct m11_elf64_ehdr *)(void *)m11_synthetic_elf;
    eh->e_ident[0] = M11_ELFMAG0;
    eh->e_ident[1] = M11_ELFMAG1;
    eh->e_ident[2] = M11_ELFMAG2;
    eh->e_ident[3] = M11_ELFMAG3;
    eh->e_ident[4] = M11_ELFCLASS64;
    eh->e_ident[5] = M11_ELFDATA2LSB;
    eh->e_ident[6] = M11_EV_CURRENT;
    eh->e_type        = M11_ET_EXEC;
    eh->e_machine     = M11_EM_X86_64;
    eh->e_version     = M11_EV_CURRENT;
    eh->e_entry       = 0x0000000000401000ULL;
    eh->e_phoff       = sizeof(struct m11_elf64_ehdr);
    eh->e_ehsize      = sizeof(struct m11_elf64_ehdr);
    eh->e_phentsize   = sizeof(struct m11_elf64_phdr);
    eh->e_phnum       = 2u;

    struct m11_elf64_phdr *ph = (struct m11_elf64_phdr *)(void *)
        (m11_synthetic_elf + eh->e_phoff);

    ph[0].p_type   = M11_PT_LOAD;
    ph[0].p_flags  = M11_PF_R | M11_PF_X;
    ph[0].p_offset = 0x1000u;
    ph[0].p_vaddr  = 0x0000000000400000ULL;
    ph[0].p_filesz = 16u;
    ph[0].p_memsz  = 4096u;
    ph[0].p_align  = M11_PAGE_SIZE;

    ph[1].p_type   = M11_PT_LOAD;
    ph[1].p_flags  = M11_PF_R | M11_PF_W;
    ph[1].p_offset = 0x2000u;
    ph[1].p_vaddr  = 0x0000000000401000ULL;
    ph[1].p_filesz = 8u;
    ph[1].p_memsz  = 4096u;
    ph[1].p_align  = M11_PAGE_SIZE;

    m11_synthetic_built = 1;
}

void m11_integration_test(void) {
    log_writeln("[M11] elf: membangun ELF sintetis...");

    m11_build_synthetic_elf();

    struct m11_user_region uregion;
    uregion.base  = 0x0000000000400000ULL;
    uregion.limit = 0x0000800000000000ULL;

    struct m11_process_image_plan plan;

    log_writeln("[M11] elf: ident ok");

    int rc = m11_elf64_plan_load(
        m11_synthetic_elf,
        12288u,
        uregion,
        &plan
    );

    if (rc != M11_OK) {
        log_write("[M11] elf: REJECTED ");
        log_writeln(m11_error_name(rc));
        return;
    }

    serial_write_string("[M11] elf: phnum=");
    serial_write_dec64((uint64_t)plan.segment_count);
    serial_write_string("\n");

    for (uint32_t i = 0u; i < plan.segment_count; i++) {
        serial_write_string("[M11] elf: load segment vaddr=0x");
        serial_write_hex64(plan.segments[i].vaddr);
        serial_write_string(" filesz=");
        serial_write_dec64(plan.segments[i].filesz);
        serial_write_string(" memsz=");
        serial_write_dec64(plan.segments[i].memsz);
        serial_write_string(" flags=0x");
        serial_write_hex64((uint64_t)plan.segments[i].flags);
        serial_write_string("\n");
    }

    serial_write_string("[M11] elf: plan ok entry=0x");
    serial_write_hex64(plan.entry);
    serial_write_string("\n");

    log_writeln("[M11] user image plan ready");
}
