# MCSOS 260502

MCSOS 260502 adalah proyek sistem operasi pendidikan yang dikembangkan bertahap (milestone M0–M16) untuk target x86_64, dengan host pengembangan Windows 11 x64 melalui WSL2.

- **Arsitektur target:** x86_64
- **Emulator:** QEMU system x86_64
- **Firmware emulator:** OVMF / UEFI
- **Bahasa kernel:** freestanding C17 dan assembly x86_64 minimal
- **Model kernel:** monolithic educational kernel dengan boundary modular internal
- **Toolchain build:** `clang` + `ld.lld`

Status saat ini: **M16 selesai** — write-ahead journal MCSFS1J di atas filesystem persisten minimal MCSFS1, lengkap dengan scheduler, syscall layer, ELF loader, VFS, dan block device layer.

## Status Milestone

| Milestone | Fokus | Status |
|---|---|---|
| M0 | Baseline environment: setup WSL2, compile C17 freestanding pertama | ✅ Selesai |
| M1 | Reproducibility toolchain (script build + Makefile awal) | ✅ Selesai |
| M2 | Kernel image bootable dengan output serial (bootloader Limine) | ✅ Selesai |
| M3 | Mekanisme panic handler & debug audit dasar | ✅ Selesai |
| M4 | Jalur IDT & exception handling — trigger breakpoint & panic terkontrol (`MCSOS_M4_TRIGGER_BREAKPOINT` / `_PANIC`) | ✅ Selesai |
| M5 | Remap PIC 8259A, konfigurasi PIT 8254, penanganan interrupt eksternal | ✅ Selesai |
| M6 | Physical Memory Manager (PMM) dasar — `pmm.c`/`pmm.h`, host test alokasi frame | ✅ Selesai |
| M7 | Virtual Memory Manager (VMM) — `kernel/core/vmm.c` | ✅ Selesai |
| M8 | Kernel memory allocator (`kmem`) freestanding + host test | ✅ Selesai |
| M9 | Scheduler round-robin & context switch x86_64 | ✅ Selesai |
| M10 | Syscall ABI layer + dispatcher `int 0x80` | ✅ Selesai |
| M11 | ELF64 user program loader | ✅ Selesai |
| M12 | Sinkronisasi kernel: spinlock, mutex, dan lock dependency tracker (lockdep) — `kernel/sync/` | ✅ Selesai |
| M13 | Virtual File System, RAMFS, file descriptor table | ✅ Selesai |
| M14 | Block device layer, RAM block driver, buffer cache | ✅ Selesai |
| M15 | Filesystem persisten minimal — MCSFS1 | ✅ Selesai |
| M16 | Write-ahead journal — MCSFS1J | ✅ Selesai |


## Struktur Proyek

```
mcsos/
├── arch/           # kode spesifik arsitektur (x86_64: context switch, dll)
├── artifacts/      # hasil build & test per milestone (mis. artifacts/m15)
├── configs/        # konfigurasi build/bootloader
├── docs/           # dokumentasi praktikum
├── evidence/       # bukti audit tiap milestone (nm/readelf/objdump/log)
├── fs/             # implementasi filesystem (mcsfs1, dst)
├── include/        # header umum
├── iso_root/       # root image ISO untuk boot
├── kernel/         # source kernel utama (core, mm, syscall, dll)
├── limine/         # bootloader Limine
├── scripts/        # script bantu (audit, smoke test, dll)
├── smoke/          # smoke test
├── tests/          # unit test host-side per milestone
├── tools/          # tooling tambahan
├── linker.ld       # linker script kernel
├── Makefile        # build utama + target M7–M10, M15
├── Makefile.m11    # target khusus M11
├── Makefile.m12    # target khusus M12
├── Makefile.m13    # target khusus M13
└── Makefile.m14    # target khusus M14
```

## Requirements

- WSL2 (Ubuntu) atau Linux native
- `clang` dan `ld.lld`
- `make`, `objdump`, `readelf`, `nm`, `sha256sum`

## Build & Test

Target utama yang tersedia di `Makefile`:

```bash
# Build kernel freestanding (image utama)
make build

# Build varian breakpoint / panic (untuk audit M4)
make breakpoint
make panic

# Audit ELF hasil build (readelf, nm, disasm, cek simbol wajib)
make inspect
make audit

# Grading statis M5 (cek simbol IDT/PIC/PIT/timer)
make grade

# Host test M7 (VMM)
make check

# Host test + freestanding + audit M9 (scheduler)
make m9-all

# Host test + freestanding + audit M10 (syscall) — alias: make test
make test

# Host test + freestanding + audit M15 (MCSFS1)
make m15-all

# Bersihkan build artifacts
make clean
make distclean
```

Untuk milestone M11–M14, gunakan file Makefile terpisah:

```bash
make -f Makefile.m11 CC=clang
make -f Makefile.m12 CC=clang
make -f Makefile.m13
make -f Makefile.m14
```


## Evidence

Setiap milestone punya folder bukti audit sendiri di `evidence/<milestone>/`, berisi log `nm -u` (cek simbol undefined), `readelf -h`/`-l` (header & program header ELF), dan `objdump -dr` (disassembly + relokasi). Ini dipakai untuk memverifikasi kernel/objek freestanding bebas dependency libc dan sesuai kontrak ABI x86_64.

## Author

- Asti Lestari
- Amelia Okta Ramadani
- Wifa Fazriyatul Fadhla
- Nazwa Rahmadanti
- Fauziah Putri Rahayu

Institut Pendidikan Indonesia, Program Studi Pendidikan Teknologi Informasi (NIM 25832071002)

