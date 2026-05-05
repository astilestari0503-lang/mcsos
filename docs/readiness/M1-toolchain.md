# Readiness Review M1 - Toolchain Reproducible

## Identitas Mahasiswa/Kelompok

- **Nama Mahasiswa/Kelompok**: ________________
- **NIM Anggota**: ________________
- **Kelas**: ________________
- **Dosen**: Muhaemin Sidiq, S.Pd., M.Pd.
- **Program Studi**: Pendidikan Teknologi Informasi, Institut Pendidikan Indonesia
- **Tanggal**: ________________
- **Commit Hash**: ________________

## Ringkasan Hasil

[ISI SETELAH SEMUA SELESAI]

## Evidence Checklist

| Evidence | Path | Status |
|----------|------|--------|
| Toolchain versions | `build/meta/toolchain-versions.txt` | ☐ PASS |
| Host readiness | `build/meta/host-readiness.txt` | ☐ PASS |
| QEMU capabilities | `build/meta/qemu-capabilities.txt` | ☐ PASS |
| Freestanding object | `build/proof/freestanding_probe.o` | ☐ PASS |
| Freestanding ELF | `build/proof/freestanding_probe.elf` | ☐ PASS |
| ELF header | `build/proof/readelf-header.txt` | ☐ PASS |
| ELF sections | `build/proof/readelf-sections.txt` | ☐ PASS |
| Disassembly | `build/proof/objdump-disassembly.txt` | ☐ PASS |
| Undefined symbols | `build/proof/nm-undefined.txt` | ☐ PASS |
| Reproducibility run 1 | `build/repro/sha256-run1.txt` | ☐ PASS |
| Reproducibility run 2 | `build/repro/sha256-run2.txt` | ☐ PASS |

## Acceptance Criteria M1

| Kriteria | Status |
|----------|--------|
| Repository berada di filesystem Linux WSL | ☐ PASS |
| Semua tool wajib tersedia dan terdeteksi | ☐ PASS |
| `make meta` berhasil | ☐ PASS |
| `make check` berhasil tanpa ERROR | ☐ PASS |
| `make proof` berhasil | ☐ PASS |
| `make qemu-probe` berhasil | ☐ PASS |
| `make repro` berhasil | ☐ PASS |
| `make test` berhasil dari clean checkout | ☐ PASS |
| `nm-undefined.txt` kosong | ☐ PASS |
| `readelf` menunjukkan ELF64 x86_64 | ☐ PASS |

## Known Limitations

[Isi dengan keterbatasan yang ada]

## Risiko dan Mitigasi

[Isi dengan minimal 3 risiko teknis]

## Readiness Decision

- [ ] Belum siap lanjut M2
- [ ] Siap lanjut M2 dengan catatan
- [ ] Siap lanjut M2

**Alasan**: [Isi dengan alasan]
