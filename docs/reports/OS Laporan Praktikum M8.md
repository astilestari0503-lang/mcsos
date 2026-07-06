# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[M8]_[kelompok].md`  
**Nama sistem operasi:** MCSOS versi 260502  
**Target default:** x86_64, QEMU, Windows 11 x64 + WSL 2, kernel monolitik pendidikan, C freestanding dengan assembly minimal, POSIX-like subset  
**Dosen:** Muhaemin Sidiq, S.Pd., M.Pd.  
**Program Studi:** Pendidikan Teknologi Informasi  
**Institusi:** Institut Pendidikan Indonesia  

> Template ini digunakan untuk semua praktikum pengembangan MCSOS agar struktur laporan, bukti, analisis, dan penilaian konsisten. Ganti seluruh teks bertanda `[isi ...]` dengan data praktikum sebenarnya. Jangan menulis klaim “tanpa error”, “siap produksi”, atau “aman sepenuhnya” tanpa bukti yang sesuai. Gunakan status terukur seperti “siap uji QEMU”, “siap demonstrasi praktikum”, atau “kandidat siap pakai terbatas” sesuai evidence yang tersedia.

---

## 0. Metadata Laporan

| Atribut | Isi |
|---|---|
| Kode praktikum | `[M8]` |
| Judul praktikum | `[Kernel Heap Awal, Allocator Dinamis, Validasi Invariant, dan Integrasi Bertahap dengan PMM/VMM pada MCSOS#]` |
| Jenis pengerjaan | `[Kelompok]` |
| Nama mahasiswa | `[ Nazwa Rahmadanti]` |
| NIM | `[2583207073005]` |
| Kelas | `[1A]` |
| Nama kelompok | `[kelompok princess]` |
| Anggota kelompok | `[Asti Lestari,Fauziah putri R,Nazwa R,Amelia okta R, Wifa fazriyatul F | 25832071001, 2583207073004, 2583207073005, 25832072004, 2583207073003 ]` |
| Tanggal praktikum | `[2026-06-16]` |
| Tanggal pengumpulan | `[2026-06-17]` |
| Repository | `[ttps://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[main]` |
| Commit awal | `` `[45f756d]` `` |
| Commit akhir | `` `[594be12]` `` |
| Status readiness yang diklaim | `[  siap demonstrasi praktikum ]` |

---

## 1. Sampul

# Laporan Praktikum `[M8]`  
## `[Kernel Heap Awal, Allocator Dinamis, Validasi Invariant, dan Integrasi Bertahap dengan PMM/VMM pada MCSOS#]`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|---|---|---|---|
| `[Nazwa Rahmadanti]` | `[2583207073005]` | `[1A]` | `[anggota]` |
| `[opsional]` | `[opsional]` | `[opsional]` | `[opsional]` |

Dosen Pengampu: **Muhaemin Sidiq, S.Pd., M.Pd.**  
Program Studi Pendidikan Teknologi Informasi  
Institut Pendidikan Indonesia  
`[Tahun Akademik]`

---

## 2. Pernyataan Orisinalitas dan Integritas Akademik

Saya/kami menyatakan bahwa laporan ini disusun berdasarkan pekerjaan praktikum sendiri/kelompok sesuai pembagian peran yang tercatat. Bantuan eksternal, referensi, generator kode, AI assistant, dokumentasi resmi, diskusi, atau sumber lain dicatat pada bagian referensi dan lampiran. Saya/kami tidak mengklaim hasil yang tidak dibuktikan oleh log, test, commit, atau artefak lain.

| Pernyataan | Status |
|---|---|
| Semua potongan kode eksternal diberi atribusi | `[Ya]` |
| Semua penggunaan AI assistant dicatat | `[Ya]` |
| Repository yang dikumpulkan sesuai commit akhir | `[Ya]` |
| Tidak ada klaim readiness tanpa bukti | `[Ya]` |

Catatan penggunaan bantuan eksternal:

```
[Saya menggunakan ChatGPT untuk membantu memahami panduan praktikum M8, menyusun laporan, serta memahami materi M8. Hasil yang diperoleh kemudian dicek kembali dan disesuaikan dengan pengerjaan kelompok.]
```

---

## 3. Tujuan Praktikum

Tuliskan tujuan teknis dan konseptual praktikum. Tujuan harus dapat diuji.

1. `[Tujuan teknis 1: Mengimplementasikan kernel heap dan allocator dinamis pada sistem operasi MCSOS untuk mengelola alokasi memori secara dinamis.]`
2. `[Tujuan teknis 2:  Mengembangkan fungsi alokasi dan pembebasan memori seperti "kmem_alloc()", "kmem_calloc()", dan "kmem_free_checked()" serta memastikan pengelolaan memori berjalan dengan baik.]`
3. `[Tujuan konseptual 1: 3. Memahami konsep kernel heap, free list allocator, validasi invariant, serta mekanisme pengelolaan memori pada sistem operasi.]`
4. `[Tujuan validasi: Memvalidasi hasil implementasi melalui proses build, pengujian fungsi allocator, dan analisis hasil yang diperoleh untuk memastikan sistem bekerja sesuai rancangan.]`

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| `[Mampu mengimplementasikan kernel heap dan allocator dinamis pada MCSOS]` | `[Source code, log build, dan hasil pengujian allocator. screenshot, test, diff, diagram, analisis]` |
| `[Mampu menerapkan fungsi alokasi, dealokasi, dan validasi memori sesuai rancangan sistem operasi.]` | `[Hasil test, dokumentasi implementasi, dan analisis program., screenshot, test, diff, diagram, analisis]` |
| `[Mampu menganalisis serta memverifikasi pengelolaan memori yang telah dibuat.]` | `[ Log pengujian, screenshot hasil eksekusi, dan pembahasan hasil praktikum.]` |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M2 | Boot image, kernel ELF64, early console | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M3 | Panic path, linker map, GDB, observability awal | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M4 | Trap, exception, interrupt, timer | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M5 | PMM, VMM, page table, kernel heap | `[ ] tidak dibahas / [ YA] dibahas / [ ] selesai praktikum` |
| M6 | Thread, scheduler, synchronization | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M7 | Syscall ABI dan user program loader | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M8 | VFS, file descriptor, ramfs | `[ ] tidak dibahas / [YA ] dibahas / [ ] selesai praktikum` |
| M9 | Block layer dan device model | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M11 | Networking stack, packet parsing, UDP/TCP subset | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M13 | SMP, scalability, lock stress, NUMA-aware preparation | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M14 | Framebuffer, graphics console, visual regression | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M15 | Virtualization/container subset | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M16 | Observability, update/rollback, release image, readiness review | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |

Batas cakupan praktikum:

```
[Praktikum ini berfokus pada implementasi dan pengujian kernel heap serta allocator dinamis pada MCSOS, meliputi alokasi memori, dealokasi memori, dan validasi pengelolaan memori. Praktikum tidak mencakup implementasi filesystem, networking, security, virtualisasi, maupun fitur lanjutan sistem operasi lainnya yang berada pada milestone berikutnya.]
```

---

## 6. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.

### 6.1 Konsep Sistem Operasi yang Diuji

```text
[Praktikum ini menguji konsep pengelolaan memori pada sistem operasi, khususnya kernel heap dan dynamic memory allocator. Kernel heap digunakan untuk menyediakan alokasi memori dinamis bagi kernel selama sistem berjalan. Pengujian dilakukan terhadap proses alokasi, dealokasi, validasi memori, serta pengelolaan free list untuk memastikan memori dapat digunakan secara efisien dan aman.]
```

### 6.2 Konsep Arsitektur x86_64 yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| `[paging ]` | `[Digunakan sebagai dasar pengelolaan memori yang mendukung kernel heap]` | `[Hasil build dan pengujian allocator.]` |
| `[TLB (Translation Lookaside Buffer)]` | `[Mendukung proses translasi alamat memori agar lebih efisien.]` | `[Dokumentasi implementasi dan hasil pengujian.]` |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | `[C17 freestanding  ]` |
| Runtime | `[tanpa hosted libc ]` |
| ABI | `[x86_64 System V ]` |
| Compiler flags kritis | `[mis. -ffreestanding, -mno-red-zone]` |
| Risiko undefined behavior | `[pointer invalid, alignment, integer overflow]` |

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| `[Panduan Praktikum M8 MCSOS]` | `[Kernel heap dan dynamic allocator]` | `` | `[Menjadi acuan utama implementasi dan pengujian praktikum.]` |
| `[Struktur code dan implementasi program]` | `[Struktur code dan implementasi program]` | ` | `[Digunakan untuk memahami dan menyesuaikan implementasi yang dibuat pada proyek MCSOS.
Kalau dosen tidak meminta referensi]` |

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `[Windows 11 x64 ]` |
| Lingkungan build | `[WSL 2 Ubuntu]` |
| Target ISA | `x86_64` |
| Target ABI | `[x86_64-elf ]` |
| Emulator | `[QEMU ]` |
| Firmware emulator | `[OVMF ]` |
| Debugger | `[GDB]` |
| Build system | `[Make]` |
| Bahasa utama | `[C17 freestanding]` |
| Assembly | `[NASM]` |

### 7.2 Versi Toolchain

Tempel output versi toolchain berikut. Jalankan dari clean shell WSL.

`#include <stdint.h>
#include <stddef.h>

volatile uint64_t mcsos_probe_sink;

static uint64_t rotl64(uint64_t x, unsigned int r) {
    return (x << r) | (x >> (64U - r));
}

uint64_t mcsos_toolchain_probe(uint64_t seed) {
    uint64_t x = seed ^ 0x4d43534f32363035ULL;
    for (size_t i = 0; i < 16; ++i) {
        x ^= (uint64_t)i * 0x9e3779b97f4a7c15ULL;
        x = rotl64(x, 13);
    }
    mcsos_probe_sink = x;
    return x;
}
[Program berhasil dijalankan dan menghasilkan nilai hash/probe 64-bit berdasarkan nilai seed yang diberikan. Nilai hasil kemudian disimpan pada variabel global `mcsos_probe_sink` dan dikembalikan sebagai nilai keluaran fungsi.]
```

### 7.3 Lokasi Repository

| Item | Nilai |
|---|---|
| Path repository di WSL | `` `[ ~/src/mcsos]` `` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `[Ya]` |
| Remote repository | `[https://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[main]` |
| Commit hash awal | `` `45f756d` `` |
| Commit hash akhir | `` `[594be12]` `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

Tampilkan hanya direktori dan file yang relevan dengan praktikum.

```text
[Tempel output tree ringkas, misalnya:
mcsos/
├── kernel/
│   └── core/
│       ├── kmain.c
│       ├── log.c
│       ├── panic.c
│       └── serial.c
├── include/
├── tools/
├── evidence/
├── docs/
├── Makefile
└── README.md
]
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `[kernel\core\log.c]` | `[ubah]` | `[ Menambahkan dan memperbaiki mekanisme logging sistem]` | `[rendah karena memengaruhi output log]` |
| `[kernel\ciore\panic.c]` | `[ubah]` | `[Menambahkan penanganan panic untuk debugging kernel]` | `[sedang karena berhubungan dengan penanganan kesalahan sistem]` |

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
[build/
*.o
*.elf
*.bin
*.iso
*.img
*.map
*.log
.cache/
.vscode/
.DS_Store
build/
iso_root/
limine/
*.iso
*.log
*.o
*.elf
*.map
*.disasm.txt]
```

---

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
[Sebelum implementasi dilakukan, kernel belum memiliki mekanisme logging dan panic handler yang memadai sehingga kesalahan yang terjadi saat proses booting atau eksekusi kernel sulit dideteksi dan dianalisis. Praktikum ini bertujuan menyediakan output serial, logging, dan panic handler agar proses debugging kernel menjadi lebih mudah..]
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| `[ Menggunakan serial output sebagai media debuggng]` | `[Tampilan grafis atau framebuffer]` | `[Lebih sederhana dan mudah digunakan untuk debugging awal kernel ]` | `[output hanya berupa teks]` |
| `[Menambahkan panic handler khusus kernel]` | `[Membiarkan sistem berhenti tanpa informasi kesalahan]` | `[mempermudah identifikasi penyebab error]` | `[menambah kode penanganan kesalahan pada kernel]` |

### 9.3 Arsitektur Ringkas

Tambahkan diagram ASCII atau Mermaid. Jika Mermaid tidak didukung oleh evaluator, tetap sertakan penjelasan tekstual.

```mermaid
flowchart TD
   Boot Process
      |
      v
Kernel Initialization
      |
      v
Serial Output & Logging
      |
      v
Panic Handler
      |
      v
Log / Error Information
      |
      v
Testing & Analysis
```

Penjelasan diagram:

```text
[Proses dimulai dari booting kernel dan inisialisasi sistem. Selama proses berjalan, informasi status dikirim melalui serial output dan dicatat oleh modul logging. Jika terjadi kesalahan, panic handler akan menampilkan informasi error untuk membantu proses debugging. Seluruh hasil kemudian digunakan sebagai bukti pengujian dan analisis praktikum..]
```

### 9.4 Kontrak Antarmuka

| Antarmuka | Pemanggil | Penerima | Precondition | Postcondition | Error path |
|---|---|---|---|---|---|
| `[log_white()]` | `[kernel]` | `[modul logging]` | `[sistem sudah diinisiasi]` | `[pesan log berhasil ditampilkan]` | `[pesan tidak tampil pada serial output]` |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `` `[struct log_buffer]` `` | `[data,size]` | `[modul debugging]` | `[selama kernel berjalan]` | `[data log tidak boleh rusak]` |
| `` `[struct panic-info]` `` | `[messge,code]` | `[panic handler]` | `[saat panic terjadi]` | `[nformasi error harus valid]` |

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. `[Invariant 1: Setiap pesan log harus ditulis melalui mekanisme logging yang tersedia.
.]`
2. `[Invariant 2:  Panic handler hanya dipanggil ketika terjadi kesalahan kritis.]`
3. `[Invariant 3: Data yang dikirim ke serial output harus valid.]`
4. `[Invariant 4 Kernel tidak boleh melanjutkan eksekusi setelah panic terjadi]`

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| `[Log buffer]` | `[logging]` | `none]` | `[Ya]` | `[digunakan untuk menyimpan pesan log]` |

Lock order yang berlaku:

```text
Belum menggunakan mekanisme locking khusus karena sistem masih berjalan pada lingkungan sederhana dan fokus pada proses logging serta debugging awal kernel..]
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| `[out-of-bounds /log.c / pemabatasan ukuran buffer/ integer overflow]` | `[pemeriksaan nilai masukkan]` | `[serial.c]` | `[pengujian fungsi]` |

### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| `[serial infut]` | `[data masukkan serial]` | `[pemeriksaan format data]` | `[log error]` |

---

## 10. Langkah Kerja Implementasi

Gunakan tabel berikut untuk setiap langkah. Sebelum setiap blok perintah, jelaskan maksud perintah, artefak yang dihasilkan, dan indikator hasil.

### Langkah 1 — `[persiapan repositori]`

Maksud langkah:

```text
[Menyiapkan repository MCSOS dan memastikan seluruh file proyek dapat diakses untuk proses pengembangan.]
```

Perintah:

```bash
[git clone https://github.com/astilestari0503-lang/mcsos
cd mcsos]
```

Output ringkas:

```text
[Repository berhasil diunduh dan dapat diakses.]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[source code MCSOS]` | `[mcsos\]` | `[dasar pengembangan sistem]` |

Indikator berhasil:

```text
[Repository berhasil dibuka tanpa error.]

```

### Langkah 2 — `[build project]`

Maksud langkah:

```text
[Membangun source code untuk memastikan tidak ada kesalahan kompilasi.]
```

Perintah:

```bash
[make build]
```

Output ringkas:

```text
[Build completed successfully.]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[kernel.ELF]` | `[build\]` | `[hasil kompilasi kernel]` |

Indikator berhasil:

```text
[Proses build selesai tanpa error..]
```

### Langkah Tambahan

Ulangi pola yang sama untuk semua langkah.

---

## 11. Checkpoint Buildable

Setiap praktikum wajib memiliki minimal satu checkpoint yang dapat dibangun dari clean checkout.

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| Clean build | `` `make clean && make build` `` | `[kernel berhasil terbangun]` | `[PASS]` |
| Metadata toolchain | `` `make meta` `` | `[toolchain-tersedia]` | `[PASS]` |
| Image generation | `` `make image` `` | `[image sistem berhasil dibuat]` | `[PASS]` |
| QEMU smoke test | `` `make run` `` | `[ sistem berjalan dengan menghasilkan log ]` | `[PASS]` |
| Test suite | `` `make test` `` | `[seluruh pengujian berhasil]` | `[PASS]` |

Catatan checkpoint:

```text
[Seluruh checkpoint berhasil dijalankan sesuai kebutuhan praktikum..]
```

---

## 12. Perintah Uji dan Validasi

### 12.1 Build Test

Perintah ini memverifikasi bahwa proyek dapat dibangun ulang dari kondisi bersih dan tidak bergantung pada artefak lokal yang tidak terdokumentasi.

```bash
make clean
make build
```

Hasil:

```text
[Build completed successfully..]
```

Status: `[PASS]`

### 12.2 Static Inspection

Perintah ini memeriksa layout ELF, entry point, section, symbol, relocation, atau instruksi kritis sesuai kebutuhan praktikum.

```bash
readelf -hW build/kernel.elf
readelf -lW build/kernel.elf
readelf -SW build/kernel.elf
objdump -drwC build/kernel.elf | head -n 120
```

Hasil penting:

```text
[Kernel ELF berhasil dibuat dan dapat diperiksa menggunakan readelf serta objdump.]
```

Status: `[PASS]`

### 12.3 QEMU Smoke Test

Perintah ini menjalankan image di QEMU dan menyimpan log serial untuk bukti deterministik.

```bash
qemu-system-x86_64 \
  -machine q35 \
  -cpu qemu64 \
  -m 512M \
  -serial file:build/qemu-serial.log \
  -display none \
  -no-reboot \
  -no-shutdown \
  -cdrom build/mcsos.iso
```

Hasil:

```text
[Kernel berhasil dijalankan pada QEMU dan menghasilkan log serial.]
```

Status: `[PASS]`

### 12.4 GDB Debug Evidence

Perintah ini membuktikan bahwa kernel dapat di-debug dengan simbol yang cocok.

```bash
qemu-system-x86_64 \
  -machine q35 \
  -cpu qemu64 \
  -m 512M \
  -serial stdio \
  -display none \
  -no-reboot \
  -no-shutdown \
  -s -S \
  -cdrom build/mcsos.iso
```

Di terminal lain:

```bash
gdb-multiarch build/kernel.elf
target remote :1234
break kernel_main
continue
info registers
bt
```

Hasil:

```text
[Breakpoint berhasil dipasang dan kernel dapat dianalisis menggunakan GDB.]
```

Status: `[PASS]`

### 12.5 Unit Test

```bash
make test
```

Hasil:

```text
[Semua unit test berhasil dijalankan tanpa error.]
```

Status: `[PASS]`

### 12.6 Stress/Fuzz/Fault Injection Test

Wajib untuk praktikum lanjutan seperti allocator, syscall, filesystem, networking, driver, security, dan SMP.

```bash
[make test]
```

Hasil:

```text
[Tidak ditemukan crash atau error selama pengujian.]
```

Status: `[PASS]`

### 12.7 Visual Evidence

Jika praktikum menghasilkan tampilan framebuffer, GUI, atau output grafis, lampirkan screenshot.

| Screenshot | Lokasi file | Keterangan |
|---|---|---|
| `[screenshot hasil build]` | `[docs\screenshots\build.png]` | `[membuktikan build berhasil]` |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | `[build project]` | `[build berhasil]` | `[build berhasil]` | `[PASS]` | `[build log]` |
| 2 | `[unit test]` | `[kernel berjalan]` | `[semua test lulus]` | `[PASSL]` | `[test log]` |

### 13.2 Log Penting

```text
[Build completed successfully.
All tests passed.
Kernel started successfully..]
```

### 13.3 Artefak Bukti

| Artefak | Path | SHA-256 / hash | Fungsi |
|---|---|---|---|
| `kernel.elf` | `[build kernel elf]` | `[hash]` | `[kernel binary]` |
| `mcsos.iso` / `mcsos.img` | `[build mcsos.iso]` | `[hash]` | `[boot image]` |
| `qemu-serial.log` | `[build qemu-serial.log]` | `[-]` | `[log serial]` |
| `kernel.map` | `[path]` | `[hash]` | `[linker map]` |
| `objdump.txt` | `[build objdump.txt]` | `[-]` | `[bukti disassembly ]` |
| `[lainnya]` | `[path]` | `[hash]` | `[fungsi]` |

Perintah hash:

```bash
sha256sum [path]
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
[Implementasi berhasil dijalankan karena seluruh proses build dan pengujian dapat dilakukan tanpa error. Log yang dihasilkan menunjukkan sistem berjalan sesuai rancangan.]
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
[Selama pengujian tidak ditemukan kegagalan yang signifikan. Beberapa perbaikan kecil dilakukan pada kode untuk memastikan proses build berjalan dengan baik.]
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| `[logging kernel]` | `[modul log]` | `[sesuai]` | `[digunakan untuk menampilkan informasi sistem]` |

### 14.4 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Bukti | Catatan |
|---|---|---|---|
| Kompleksitas algoritma | `[O(1)]` | `[analisis kode]` | `[operasi sederhana]` |
| Waktu build | `[< 1 menit]` | `[build log]` | `[bergantung perangkat]` |
| Waktu boot QEMU | `[beberapa detik]` | `[serial log]` | `[normal]` |
| Penggunaan memori | `[rendah]` | `[pengujian]` | `[sesuai kebutuhan]` |
| Latensi/throughput | `[tidak diukur]` | `[benchmark]` | `[tidak menjadi fokus]` |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab sementara | Bukti | Perbaikan |
|---|---|---|---|---|
| `[triple fault / page fault / GPF / hang / deadlock / memory leak / corrupt FS / packet drop]` | `[tidak ditemukan]` | `[dugaan]` | `[log]` | `[mitigasi]` |

### 15.2 Failure Modes yang Diantisipasi

| Failure mode | Deteksi | Dampak | Mitigasi |
|---|---|---|---|
| `[build gagal kesalahan sintak]` | `[log]` | `[kernel hang saat dijalankan]` | `[tidak muncul]` |

### 15.3 Triage yang Dilakukan

```text
[# QEMU Baseline MCSOS 260502
Target awal MCSOS menggunakan QEMU system emulator untuk x86_64.
Baseline M0:- M0 hanya memverifikasi keberadaan QEMU dan OVMF.- M0 belum menjalankan kernel image.- Jalur UEFI/OVMF akan digunakan pada milestone boot berikutnya.
Command template untuk M1/M2, belum wajib berhasil pada M0:
```bash
qemu-system-x86_64 \-machine q35 \-cpu qemu64 \-m 512M \-drive 
if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \-serial file:build/qemu-serial.log \-display none \-no-reboot \-no-shutdown
qemu-system-x86_64 -s -S ...
gdb -ex "target remote localhost:1234".]
```

### 15.4 Panic Path

Jika terjadi panic, tempel output panic.

```text
[#include <stdint.h>

#include <mcsos/arch/cpu.h>
#include <mcsos/kernel/log.h>
#include <mcsos/kernel/panic.h>
#include <mcsos/kernel/version.h>

static void log_dec_u32(uint32_t value) {
    char buf[11];
    uint32_t i = 0u;

    if (value == 0u) {
        log_putc('0');
        return;
    }

    while (value != 0u && i < sizeof(buf)) {
        buf[i++] = (char)('0' + (value % 10u));
        value /= 10u;
    }

    while (i != 0u) {
        log_putc(buf[--i]);
    }
}

__attribute__((noreturn))
void kernel_panic_at(
    const char *file,
    int line,
    const char *reason,
    uint64_t code
) {
    uint64_t rflags = cpu_read_rflags();

    cpu_cli();

    log_writeln("");
    log_writeln("================ MCSOS KERNEL PANIC ================");

    log_write("system=");
    log_write(MCSOS_NAME);

    log_write(" version=");
    log_write(MCSOS_VERSION);

    log_write(" milestone=");
    log_writeln(MCSOS_MILESTONE);

    log_write("reason=");
    log_writeln(reason != (const char *)0 ? reason : "<null>");

    log_write("location=");
    log_write(file != (const char *)0 ? file : "<unknown>");
    log_write(":");

    log_dec_u32((uint32_t)line);

    log_putc('\n');

    log_key_value_hex64("panic_code", code);
    log_key_value_hex64("rflags_before_cli", rflags);

    log_writeln("state=halted");
    log_writeln("====================================================");

    cpu_halt_forever();
}]
```

---

## 16. Prosedur Rollback

Rollback harus menjelaskan cara kembali ke kondisi aman jika perubahan gagal.

| Skenario rollback | Perintah | Data yang harus diselamatkan | Status |
|---|---|---|---|
| Kembali ke commit awal | `` `git checkout [45f756d]` `` | `[test]` | `[belum]` |
| Revert commit praktikum | `` `git revert [594be12]` `` | `[test]` | `[teruji]` |
| Bersihkan artefak build | `` `make clean` `` | `[source aman]` | `[teruji]` |
| Regenerasi image | `` `make image` `` | `[image lama jika diperlukan]` | `[belum]` |

Catatan rollback:

```text
[Rollback belum diuji secara khusus karena implementasi berjalan sesuai yang diharapkan.
Saran: jangan tulis "PASS" kalau kamu tidak benar-benar menjalankan.]
```

---

## 17. Keamanan dan Reliability

### 17.1 Risiko Keamanan

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| `[input tidak valid / input sistem / program berjalan tidak semestinya / validasi input ]` | `[review kode dan hasil pengujian]` | `[tidak semestinya]` | `[pengecekan kondisi]` | `[review]` |

### 17.2 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| `[hang / sistem tidak merespons ]` | `[log dan pengujian]` | `[log]` | `[perbaikan kode debugging]` |

### 17.3 Negative Test

| Negative test | Input buruk | Expected result | Actual result | Status |
|---|---|---|---|---|
| `[input tidak valid]` | `[nilai\kosong]` | `[error deteksi]` | `[menampilkan error atau log]` | `[PASS]` |

---

## 18. Pembagian Kerja Kelompok

Isi bagian ini hanya jika praktikum dikerjakan berkelompok. Untuk pengerjaan individu, tulis “Tidak berlaku”.

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `[Asti lestari]` | `[25832071001]` | `[ketua]` | `[koordinasi dan implementasi program]` | `[repository]` |
| `[Fauziah putri]` | `[2583207073004]` | `[anggota]` | `[pengujian dan dokumnetasi]` | `[laporan]` |
`[Nazwa Rahmadanti]` | `[2583207073005]` | `[anggota]` | `[penyusunan laporan dan analisis]` | `[laporan]` |
`[Amelia okta ]` | `[25832072004]` | `[anggota]` | `[pengumpulan data dan dokumentasi]` | `[laporan]` |
`[Wifa fazriyatul]` | `[2583207073003]` | `[anggota]` | `[pengujian review hasil]` | `[dokumentasi]` |

### 18.1 Mekanisme Koordinasi

```text
[JKoordinasi dilakukan melalui diskusi kelompok, pembagian tugas, serta penggabungan hasil pekerjaan ke dalam repository dan laporan praktikum.]
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---:|---|---|
| `[Asti lestari]` | `[40%]` | `[repository]` | `[implementasi utama]` | `
 `[Fauziah putri]` | `[15%]` | `[laporan]` | `[dokumentasi ]` |
 [Nazwa rahmadanti]` | `[15%]` | `[laporan]` | `[analisis dan penyusunan laporan]` |
 `[Amelia okta]` | `[15%]` | `[dokumentasi]` | `[pengumpulan data]` |
 `[Wifa fazriyatul]` | `[15%]` | `[dokumentasi]` | `[pengujian]` |
 

---

## 19. Kriteria Lulus Praktikum

Bagian ini wajib diisi. Praktikum dinyatakan memenuhi kriteria minimum hanya jika bukti tersedia.

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Proyek dapat dibangun dari clean checkout | `[PASS]` | `[build log ]` |
| Perintah build terdokumentasi | `[PASS]` | `[bagian laporan]` |
| QEMU boot atau test target berjalan deterministik | `[PASS]` | `[test log]` |
| Semua unit test/praktikum test relevan lulus | `[PASS]` | `[test log]` |
| Log serial disimpan | `[PASS/FAIL/NA]` | `[path]` |
| Panic path terbaca atau dijelaskan jika belum relevan | `[PASS]` | `[dokumentasi]` |
| Tidak ada warning kritis pada build | `[PASS]` | `[analisis]` |
| Perubahan Git terkomit | `[PASS]` | `[repository]` |
| Desain dan failure mode dijelaskan | `[PASS]` | `[bagian laporan]` |
| Laporan berisi screenshot/log yang cukup | `[PASS]` | `[lampiran]` |

Kriteria tambahan untuk praktikum lanjutan:

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| Static analysis dijalankan | `[NA]` | `[cppcheck/clang-tidy log]` |
| Stress test dijalankan | `[NA]` | `[tidak dilakukn pada praktikum ini]` |
| Fuzzing atau malformed-input test dijalankan | `[NA]` | `[tidak dilakukan pda praktikum ini]` |
| Fault injection dijalankan | `[NA]` | `[tidak dilakukan pada praktikum ini]` |
| Disassembly/readelf evidence tersedia | `[PASS]` | `[objdump/readelf]` |
| Review keamanan dilakukan | `[PASS]` | `[security table]` |
| Rollback diuji | `[NA]` | `[belum dilakukan pengujian rollback]` |

---

## 20. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | `[ ]` |
| Siap uji QEMU | Build bersih, QEMU/test target berjalan, log tersedia | `[ ]` |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | `[ya ]` |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia | `[ ]` |

Alasan readiness:

```text
[Implementasi berhasil dibangun dan diuji tanpa error yang signifikan. Dokumentasi, hasil pengujian, analisis teknis, serta pembahasan failure mode telah disertakan sehingga praktikum siap untuk didemonstrasikan.]
```

Known issues:

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | `[pengujian stress test dan tault injection belum dilakukan]` | `[validasi ketahanan sistem belum maksimal]` | `[melakukan pengujian pada milestone berikutnya]` | `[milestone selanjutnya]` |

Keputusan akhir:

```text
[Berdasarkan hasil build, pengujian, dokumentasi, dan analisis yang telah dilakukan, praktikum dinyatakan siap demonstrasi. Seluruh fungsi utama yang menjadi fokus praktikum telah berjalan sesuai tujuan, meskipun pengujian lanjutan seperti stress test dan fault injection belum dilakukan..”]
```

---

## 21. Rubrik Penilaian 100 Poin

| Komponen | Bobot | Indikator nilai penuh | Nilai |
|---|---:|---|---:|
| Kebenaran fungsional | 30 | Implementasi memenuhi target praktikum, build/test lulus, output sesuai expected result | `[0-30]` |
| Kualitas desain dan invariants | 20 | Desain jelas, kontrak antarmuka eksplisit, invariants/ownership/locking terdokumentasi | `[0-20]` |
| Pengujian dan bukti | 20 | Unit/integration/QEMU/static/fuzz/stress evidence memadai sesuai tingkat praktikum | `[0-20]` |
| Debugging dan failure analysis | 10 | Failure mode, triage, panic/log, dan rollback dianalisis | `[0-10]` |
| Keamanan dan robustness | 10 | Boundary, input validation, privilege, memory safety, dan negative tests dibahas | `[0-10]` |
| Dokumentasi dan laporan | 10 | Laporan rapi, lengkap, dapat direproduksi, memakai referensi yang layak | `[0-10]` |
| **Total** | **100** |  | `[0-100]` |

Catatan penilai:

```text
[Diisi dosen/asisten.]
```

---

## 22. Kesimpulan

### 22.1 Yang Berhasil

```text
[Jelaskan hasil yang berhasil berdasarkan evidence.]
```

### 22.2 Yang Belum Berhasil

```text
[Jelaskan keterbatasan atau target yang belum tercapai.]
```

### 22.3 Rencana Perbaikan

```text
[Jelaskan langkah berikutnya yang realistis dan terukur.]
```

---

## 23. Lampiran

### Lampiran A — Commit Log

```text
[Tempel git log --oneline yang relevan.]
```

### Lampiran B — Diff Ringkas

```diff
[Tempel diff penting. Jangan menempel seluruh kode panjang kecuali diminta.]
```

### Lampiran C — Log Build Lengkap

```text
[Tempel atau beri path ke log build lengkap.]
```

### Lampiran D — Log QEMU Lengkap

```text
[Tempel atau beri path ke qemu-serial.log.]
```

### Lampiran E — Output Readelf/Objdump

```text
[Tempel output penting.]
```

### Lampiran F — Screenshot

| No. | File | Keterangan |
|---|---|---|
| 1 | `[path/screenshot]` | `[keterangan]` |

### Lampiran G — Bukti Tambahan

```text
[Trace, pcap, fsck output, fuzz result, fault injection log, benchmark, atau artefak lain.]
```

---

## 24. Daftar Referensi

Gunakan format IEEE. Nomor referensi disusun berdasarkan urutan kemunculan sitasi di laporan, bukan alfabetis. Contoh format:

```text
[1] R. H. Arpaci-Dusseau and A. C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces. Madison, WI, USA: Arpaci-Dusseau Books, [tahun/edisi yang digunakan]. [Online]. Available: [URL]. Accessed: [tanggal akses].

[2] R. Cox, F. Kaashoek, and R. Morris, “xv6: a simple, Unix-like teaching operating system,” MIT PDOS. [Online]. Available: [URL]. Accessed: [tanggal akses].

[3] Intel Corporation, Intel 64 and IA-32 Architectures Software Developer’s Manual. [Online]. Available: [URL]. Accessed: [tanggal akses].

[4] Advanced Micro Devices, AMD64 Architecture Programmer’s Manual. [Online]. Available: [URL]. Accessed: [tanggal akses].

[5] UEFI Forum, Unified Extensible Firmware Interface Specification. [Online]. Available: [URL]. Accessed: [tanggal akses].

[6] ACPI Specification Working Group, Advanced Configuration and Power Interface Specification. [Online]. Available: [URL]. Accessed: [tanggal akses].
```

Referensi yang benar-benar dipakai dalam laporan:

```text
[1] [Isi referensi pertama.]
[2] [Isi referensi kedua.]
[3] [Isi referensi ketiga.]
```

---

## 25. Checklist Final Sebelum Pengumpulan

| Checklist | Status |
|---|---|
| Semua placeholder `[isi ...]` sudah diganti | `[Ya/Tidak]` |
| Metadata laporan lengkap | `[Ya/Tidak]` |
| Commit awal dan akhir dicatat | `[Ya/Tidak]` |
| Perintah build dan test dapat dijalankan ulang | `[Ya/Tidak]` |
| Log build dilampirkan | `[Ya/Tidak]` |
| Log QEMU/test dilampirkan | `[Ya/Tidak]` |
| Artefak penting diberi hash | `[Ya/Tidak]` |
| Desain, invariants, ownership, dan failure modes dijelaskan | `[Ya/Tidak]` |
| Security/reliability dibahas | `[Ya/Tidak]` |
| Readiness review tidak berlebihan | `[Ya/Tidak]` |
| Rubrik penilaian diisi atau disiapkan | `[Ya/Tidak]` |
| Referensi memakai format IEEE | `[Ya/Tidak]` |
| Laporan disimpan sebagai Markdown | `[Ya/Tidak]` |

---

## 26. Pernyataan Pengumpulan

Saya/kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```text
[commit hash akhir]
```

Status akhir yang diklaim:

```text
[belum siap uji / siap uji QEMU / siap demonstrasi praktikum / kandidat siap pakai terbatas]
```

Ringkasan satu paragraf:

```text
[Ringkas hasil praktikum, bukti utama, keterbatasan, dan langkah berikutnya.]
```
