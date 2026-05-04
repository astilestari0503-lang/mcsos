# Assumptions and Non-Goals MCSOS 260502 — M0

## Assumptions

1. Target arsitektur awal adalah x86_64 long mode.
2. Host pengembangan adalah Windows 11 x64.
3. Build dilakukan di WSL 2 Linux environment.
4. Repository utama berada di filesystem Linux WSL.
5. Emulator utama untuk milestone awal adalah QEMU system x86_64.
6. Firmware emulator menggunakan OVMF/UEFI.
7. Bootloader awal menggunakan Limine atau bootloader setara dengan handoff terdokumentasi.
8. Bahasa kernel awal adalah freestanding C17 dengan assembly minimal.
9. Target compatibility adalah POSIX-like subset, bukan Linux ABI penuh.
10. Setiap milestone harus menghasilkan bukti (log, output, image, checksum, trace, atau laporan).

## Non-Goals M0

1. M0 tidak membuat kernel bootable.
2. M0 tidak mengimplementasikan bootloader.
3. M0 tidak membuat linker script final.
4. M0 tidak mengimplementasikan interrupt, paging, scheduler, syscall, VFS, driver, networking, graphics, atau security enforcement.
5. M0 tidak bertujuan membuat sistem produksi.
6. M0 tidak menjamin kompatibilitas semua hardware x86_64.
7. M0 hanya berjalan di emulator (QEMU).
8. M0 tidak menuntut build 100% deterministic.
