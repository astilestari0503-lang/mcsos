# Threat Model Ringkas M1 - Toolchain dan Lingkungan

## Assets (Aset yang dilindungi)
1. Source code MCSOS
2. Script build dan test
3. Toolchain (compiler, linker, assembler, emulator, debugger)
4. Generated artifacts (object files, ELF, logs, metadata)
5. Bukti praktikum dan laporan

## Trust Assumptions (Asumsi Kepercayaan)
1. Paket Ubuntu/Debian berasal dari repository resmi
2. Mahasiswa tidak mengubah binary compiler/linker secara manual
3. Repository berada pada filesystem Linux WSL
4. Build M1 belum mengeksekusi kode guest MCSOS

## Known Threats dan Mitigasi
| Threat | Dampak | Mitigasi M1 |
|--------|--------|------------|
| Compiler host salah target | Object tidak sesuai untuk kernel | Inspeksi `readelf` dan target triple |
| Linker memakai libc/startup host | Kernel bergantung runtime yang tidak tersedia | Gunakan `-nostdlib`, cek `nm -u` kosong |
| Repository di `/mnt/c` | Permission, symlink tidak stabil | Validasi path di `check_toolchain.sh` |
| Generated artifact dikomit | Repository kotor | `.gitignore` dan `make distclean` |
| OVMF tidak tersedia | M2 gagal boot UEFI | `qemu_probe.sh` validasi |
| Versi tool tidak dicatat | Build tidak dapat diaudit | `collect_meta.sh` |
