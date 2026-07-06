# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[M5]_[Kelompok Princess].md`  
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
| Kode praktikum | `[M5]` |
| Judul praktikum | `[External Interrupt, Legacy PIC Remap, dan PIT Timer Tick pada MCSOS]` |
| Jenis pengerjaan | `[Kelompok]` |
| Nama mahasiswa | `[Asti Lestari]` |
| NIM | `[25832071002]` |
| Kelas | `[1A]` |
| Nama kelompok | `[Kelompok Princess]` |
| Anggota kelompok | `[Asti Lestari, Amelia Okta Ramadani, Wifa Fazriyatul Fadhla, Nazwa Rahmadanti, Fauziah Putri Rahayu]` |
| Tanggal praktikum | `[2026-05-18]` |
| Tanggal pengumpulan | `[2026-06-21]` |
| Repository | `[https://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[praktikum/m5-timer-irq]` |
| Commit awal | `` `[27eff0d]` `` |
| Commit akhir | `` `[0992aa0]` `` |
| Status readiness yang diklaim | `[siap uji QEMU untuk external interrupt awal]` |

---

## 1. Sampul

# Laporan Praktikum `[M5]`  
## `[External Interrupt, Legacy PIC Remap, dan PIT Timer Tick pada MCSOS]`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|---|---|---|---|
| `[Asti Lestari]` | `[25832071002]` | `[1A]` | `[Koordinator Teknis]` |

Dosen Pengampu: **Muhaemin Sidiq, S.Pd., M.Pd.**  
Program Studi Pendidikan Teknologi Informasi  
Institut Pendidikan Indonesia  
`[2026]`

---

## 2. Pernyataan Orisinalitas dan Integritas Akademik

Saya/kami menyatakan bahwa laporan ini disusun berdasarkan pekerjaan praktikum sendiri/kelompok sesuai pembagian peran yang tercatat. Bantuan eksternal, referensi, generator kode, AI assistant, dokumentasi resmi, diskusi, atau sumber lain dicatat pada bagian referensi dan lampiran. Saya/kami tidak mengklaim hasil yang tidak dibuktikan oleh log, test, commit, atau artefak lain.

| Pernyataan | Status |
|---|---|
| Semua potongan kode eksternal diberi atribusi | `[Ya]` |
| Semua penggunaan AI assistant dicatat | `[Yaa]` |
| Repository yang dikumpulkan sesuai commit akhir | `[Ya]` |
| Tidak ada klaim readiness tanpa bukti | `[Ya]` |

Catatan penggunaan bantuan eksternal:

```text
[Menggunakan ChatGPT sebagai bantuan debugging dan dokumentasi praktikum M5. 
Bantuan yang digunakan meliputi:
- pengecekan error build dan QEMU,
- verifikasi konfigurasi PIC/PIT/IRQ,
- interpretasi output git branch dan commit,
- penyusunan ringkasan hasil praktikum untuk laporan.

Verifikasi mandiri dilakukan dengan:
- menjalankan make grade,
- menjalankan QEMU,
- memastikan log timer IRQ muncul,
- memeriksa symbol menggunakan grep pada build/kernel.syms.txt,
- melakukan git status dan git log untuk memastikan commit sesuai.]
```

---

## 3. Tujuan Praktikum

Tuliskan tujuan teknis dan konseptual praktikum. Tujuan harus dapat diuji.

1. `[Tujuan teknis 1: Mengimplementasikan dan mengonfigurasi PIC (Programmable Interrupt Controller) serta PIT (Programmable Interval Timer) pada kernel x86_64 agar interrupt timer dapat berjalan dengan benar.]`
2. `[Tujuan teknis 2: Menghasilkan kernel bootable pada QEMU yang mampu menangani IRQ timer dan menampilkan serial log timer secara periodik.]`
3. `[Tujuan konseptual 1: Memahami konsep interrupt handling pada arsitektur x86_64, termasuk mekanisme IDT, ISR, PIC remapping, PIT configuration, dan alur trap dispatch kernel.]`
4. `[Tujuan validasi: Melakukan validasi implementasi menggunakan build audit, symbol checking, QEMU runtime log, serta evidence file seperti readelf, objdump, dan kernel symbol table.]`

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| `[Mampu mengimplementasikan PIC (Programmable Interrupt Controller) dan PIT (Programmable Interval Timer) pada kernel x86_64]` | `[Source code PIC/PIT, hasil build make grade, serta pengecekan symbol pic_remap dan pit_configure_hz pada build/kernel.syms.txt]` |
| `[Mampu mengonfigurasi IDT, ISR, dan IRQ timer untuk menangani interrupt hardware]` | `[Source code idt.c dan isr.S, symbol isr_stub_32 dan x86_64_trap_dispatch, serta audit instruction iretq, lidt, sti, dan hlt pada build/kernel.disasm.txt]` |
| `[Mampu melakukan pengujian interrupt timer pada QEMU dan menganalisis failure mode kernel]` | `[Log serial QEMU (ticks=100, ticks=200, ticks=300), screenshot hasil pengujian, analisis failure mode, serta hasil M5 static grade: PASS]` |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | `[ ] tidak dibahas / [v] dibahas / [ ] selesai praktikum` |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M2 | Boot image, kernel ELF64, early console | `[ ] tidak dibahas / [v] dibahas / [ ] selesai praktikum` |
| M3 | Panic path, linker map, GDB, observability awal | `[ ] tidak dibahas / [v] dibahas / [ ] selesai praktikum` |
| M4 | Trap, exception, interrupt, timer | `[ ] tidak dibahas / [] dibahas / [v] selesai praktikum` |
| M5 | PMM, VMM, page table, kernel heap | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M6 | Thread, scheduler, synchronization | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M7 | Syscall ABI dan user program loader | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M8 | VFS, file descriptor, ramfs | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M9 | Block layer dan device model | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M11 | Networking stack, packet parsing, UDP/TCP subset | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M13 | SMP, scalability, lock stress, NUMA-aware preparation | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M14 | Framebuffer, graphics console, visual regression | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M15 | Virtualization/container subset | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M16 | Observability, update/rollback, release image, readiness review | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |

Batas cakupan praktikum:

```text
[FITUR YANG TERMASUK:
- Remapping legacy Intel 8259A PIC dari IRQ0–IRQ15 ke vector 0x20–0x2F
- Konfigurasi PIT channel 0 pada 100 Hz (divisor 11932 dari basis 1,193,182 Hz)
- Dispatcher trap yang membedakan exception CPU dari IRQ hardware
- Membuka IRQ0 secara terkendali dengan masking IRQ lain sebagai baseline
- Implementasi EOI (End-of-Interrupt) untuk PIC
- Observasi tick timer melalui serial log QEMU

NON-GOALS (TIDAK TERMASUK):
- Scheduler preemptive final
- Penggantian legacy PIC/PIT dengan APIC, IOAPIC, HPET, atau LAPIC timer
- Dukungan SMP, user mode, syscall ABI, atau interrupt affinity
- Klaim sistem siap produksi atau siap hardware umum
- Implementasi IRQ1–IRQ15 handler secara lengkap (hanya IRQ0 dibuka)

STATUS READINESS YANG VALID:
Hasil M5 hanya boleh disebut "siap uji QEMU untuk external interrupt dan PIT timer awal".
Belum siap scheduler preemption, belum siap hardware fisik, belum siap SMP.]
```

---

## 6. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.

### 6.1 Konsep Sistem Operasi yang Diuji

```text
[Praktikum ini menguji mekanisme interrupt hardware pada sistem operasi x86_64 menggunakan PIC, PIT, IDT, dan ISR. PIC digunakan untuk mengatur interrupt hardware, sedangkan PIT digunakan sebagai timer periodik 100Hz. Interrupt timer dari IRQ0 diteruskan ke IDT vector 32 dan diproses oleh interrupt handler kernel untuk menjalankan fungsi timer serta menampilkan log serial pada QEMU.]
```

### 6.2 Konsep Arsitektur x86_64 yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| `[IDT]` | `[Menyimpan interrupt handler untuk IRQ dan exception]` | `[[MCSOS:M5] IDT loaded]` |
| `[PIC]` | `[Mengatur dan meremap interrupt hardware]` | `[[MCSOS:M5] PIC remapped]` |
| `[PIT]` | `[Menghasilkan interrupt timer periodik 100Hz]` | `[[MCSOS:M5] PIT configured 100Hz]` |
| `[ISR]` | `[Menangani interrupt timer dari IRQ0]` | `[[MCSOS:TIMER] ticks=100[MCSOS:TIMER] ticks=200[MCSOS:TIMER] ticks=300]` |
| `[iretq]` | `[Mengembalikan eksekusi CPU setelah interrupt selesai]` | `[iretq]` |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | `[C17 freestanding + assembly AT&T minimal (stub interrupt)]` |
| Runtime | `[Tanpa hosted libc, crt0 custom dari M2/M3]` |
| ABI | `[x86_64 System V ABI, dengan trap frame layout khusus kernel]` |
| Compiler flags kritis | `[-ffreestanding -mno-red-zone -nostdlib -O2 -fno-asynchronous-unwind-tables]` |
| Risiko undefined behavior | `[(1) Pointer invalid/null deref di trap handler → triple fault. (2) Alignment stack 16-byte di entry point. (3) Integer overflow di divisor PIT (unsigned). (4) Type aliasing volatile g_ticks. Mitigasi: audit asm, test smoke QEMU, verifikasi readelf alignment]` |

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| `[1]` | `[Intel Corporation, Intel 64 and IA-32 Architectures Software Developer's Manual]` | `[Volume 3A: Section 5 (Interrupt and Exception Handling), Section 8 (I/O Interface), Section 10 (Advanced Programmable Interrupt Controller - APIC)]` |`[Detil IDT gate descriptor, IRETQ, privilege level, interrupt model x86_64]`|
| `[2]` | `[Intel Corporation, 8259A Programmable Interrupt Controller Datasheet]` | `[Section 2–4: ICW1–ICW4, OCW1–OCW3, ISR/IRR register]` |`[Remapping PIC, pengaturan cascade, EOI, masking IRQ]`|
| `[3]` | `[Intel Corporation, 8254 Programmable Interval Timer Datasheet]` | `[Section 2–3: Counter mode, command word, channel 0–2]` |`[Konfigurasi PIT channel 0 ke 100 Hz, divisor calculation]`|
| `[4]` | `[QEMU Project, QEMU Documentation: Invocation]` | `[System emulation x86_64, opsi -serial stdio, machine type pc]` |`[QEMU board emulation PIC/PIT, serial log output]`|
| `[5]` | `[QEMU Project, QEMU Documentation: GDB usage]` | `[GDB remote debugging, breakpoint, register inspection]` |`[Debug dengan QEMU gdbstub, observasi trap frame, hang diagnosis]`|
| `[6]` | `[GNU Binutils, LD: Linker Scripts]` | `[Linker script, section layout, symbol allocation]` |`[Layout ELF64, section .text, .rodata, entry point]`|

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `[Windows 11 x64 build ...]` |
| Lingkungan build | `[WSL 2 Ubuntu 24.04]` |
| Target ISA | `x86_64` |
| Target ABI | `[x86_64-elf]` |
| Emulator | `[QEMU 8.2.2]` |
| Firmware emulator | `[OVMF]` |
| Debugger | `[GNU gdb 15.1]` |
| Build system | `[Make]` |
| Bahasa utama | `[C17 freestanding]` |
| Assembly | `[GAS (GNU Assembler)]` |

### 7.2 Versi Toolchain

Tempel output versi toolchain berikut. Jalankan dari clean shell WSL.

```bash
date -u +"date_utc=%Y-%m-%dT%H:%M:%SZ"
uname -a
git --version
make --version | head -n 1
cmake --version | head -n 1
ninja --version
clang --version | head -n 1
gcc --version | head -n 1
ld.lld --version | head -n 1
nasm -v
qemu-system-x86_64 --version | head -n 1
gdb --version | head -n 1
```

Output:

```text
[date_utc=2026-05-18T07:43:02Z
Linux LAPTOP-HBKST294 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
git version 2.43.0
GNU Make 4.3
cmake version 3.28.3
1.11.1
Ubuntu clang version 18.1.3 (1ubuntu1)
gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
Ubuntu LLD 18.1.3 (compatible with GNU linkers)
NASM version 2.16.01
QEMU emulator version 8.2.2 (Debian 1:8.2.2+ds-0ubuntu1.16)
GNU gdb (Ubuntu 15.1-1ubuntu1~24.04.1) 15.1]
```

### 7.3 Lokasi Repository

| Item | Nilai |
|---|---|
| Path repository di WSL | `` `[~/src/mcsos]` `` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `[Ya]` |
| Remote repository | `[https://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[praktikum/m5-timer-irq]` |
| Commit hash awal | `` `[27eff0d]` `` |
| Commit hash akhir | `` `[0992aa0]` `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

Tampilkan hanya direktori dan file yang relevan dengan praktikum.

```text
[mcsos/
├── Makefile
├── linker.ld
├── build/
│   ├── kernel.elf
│   ├── kernel.map
│   ├── kernel.syms.txt
│   ├── kernel.disasm.txt
│   └── mcsos.iso
├── evidence/
│   ├── M4/
│   │   ├── kernel.disasm.txt
│   │   ├── kernel.syms.txt
│   │   └── m4-qemu-serial.log
│   └── m5-failure/
├── kernel/
│   ├── arch/x86_64/
│   │   ├── idt.c
│   │   ├── isr.S
│   │   ├── pic.c
│   │   ├── pit.c
│   │   └── include/mcsos/arch/
│   │       ├── idt.h
│   │       ├── isr.h
│   │       ├── pic.h
│   │       └── pit.h
│   ├── core/
│   │   ├── kmain.c
│   │   ├── trap.c
│   │   ├── serial.c
│   │   └── log.c
│   └── include/mcsos/kernel/
│       ├── pic.h
│       ├── pit.h
│       └── serial.h
├── tools/scripts/
│   ├── grade_m4.sh
│   ├── m4_qemu_run.sh
│   └── m4_audit_elf.sh
└── docs/guides/
    └── OS_panduan_M5.md]
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `[kernel/arch/x86_64/pic.c]` | `[baru]` | `[Menambahkan implementasi PIC remapping untuk IRQ hardware]` | `[Sedang, karena kesalahan dapat menyebabkan interrupt tidak berjalan]` |
| `[kernel/arch/x86_64/pit.c]` | `[baru]` | `[Menambahkan konfigurasi PIT 100Hz dan timer IRQ]` | `[Sedang, karena memengaruhi timer kernel]` |
| `[kernel/arch/x86_64/isr.S]` | `[Ubah]` | `[Menambahkan ISR stub dan interrupt dispatch assembly]` | `[Tinggi, karena berhubungan langsung dengan flow interrupt CPU]` |
| `[kernel/arch/x86_64/idt.c]` | `[Ubah]` | `[Menambahkan IDT initialization dan IRQ handler]` | `[Tinggi, karena kesalahan dapat menyebabkan triple fault]` |
|`[kernel/arch/x86_64/include/mcsos/arch/idt.h]` | `[Ubah]` | `[Menyesuaikan trap frame untuk interrupt handling]` | `[Tinggi, karena memengaruhi struktur register CPU]` |
| `[kernel/core/trap.c]` | `[Ubah]` | `[Menambahkan trap dispatch dan timer interrupt handling]` | `[Sedang, karena memengaruhi alur interrupt kernel]` |
| `[kernel/core/kmain.c]` | `[Ubah]` | `[Mengaktifkan interrupt dan menjalankan halt loop]` | `[rendah]` |
| `[kernel/core/serial.c]` | `[Ubah]` | `[Menambahkan output serial untuk log timer]` | `[rendah]` |
| `[Makefile]` | `[Ubah]` | `[Menambahkan build PIC, PIT, dan ISR]` | `[Sedang, karena kesalahan dapat menyebabkan build atau linking kernel gagal]` |


### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
[0992aa0 (HEAD -> praktikum/m5-timer-irq, origin/praktikum/m5-timer-irq) M5 PIC PIT interrupt working
27eff0d (origin/m4-idt-exception-path-final, m4-idt-exception-path) M4 add QEMU smoke test log
810dbf9 M4 add x86_64 IDT and exception trap path]
```

---

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
[Kernel MCSOS sebelumnya belum memiliki mekanisme interrupt timer hardware yang berjalan secara periodik. Setelah CPU masuk ke halt loop, sistem tidak menerima event timer sehingga tidak ada proses interrupt yang dapat diuji. Praktikum ini menyelesaikan masalah tersebut dengan mengimplementasikan PIC remapping, konfigurasi PIT 100Hz, IDT interrupt handler, dan IRQ0 timer handler agar kernel dapat menerima serta memproses interrupt timer pada QEMU]
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| `[Menggunakan PIC legacy dengan remap IRQ ke vector 32]` | `[Menggunakan APIC/IOAPIC]` | `[PIC lebih sederhana dan sesuai untuk praktikum dasar interrupt x86_64]` | `[Sistem masih menggunakan interrupt controller legacy]` |
| `[Menggunakan PIT dengan frekuensi 100Hz]` | `[Frekuensi lebih tinggi atau HPET]` | `[100Hz cukup stabil untuk pengujian timer kernel dan lebih mudah divalidasi]` | `[Timer belum memiliki presisi tinggi]` |
| `[Menangani interrupt menggunakan ISR assembly (isr.S) dan trap dispatcher C]` | `[Menulis seluruh handler dalam C]` | `[ISR assembly diperlukan untuk menyimpan register CPU dan menggunakan iretq dengan benar]` | `[Implementasi lebih kompleks dan sensitif terhadap stack alignment]` |
| `[Menampilkan log timer melalui serial output QEMU]` | `[Menggunakan framebuffer atau VGA text mode]` | `[Serial log lebih mudah digunakan untuk debugging kernel awal]` | `[Output masih berbasis teks dan belum visual]` |

### 9.3 Arsitektur Ringkas

Tambahkan diagram ASCII atau Mermaid. Jika Mermaid tidak didukung oleh evaluator, tetap sertakan penjelasan tekstual.

```mermaid
flowchart TD
 text
PIT Hardware
     │
     ▼
IRQ0 (Timer Interrupt)
     │
     ▼
PIC Remap (Vector 32)
     │
     ▼
IDT Entry 32
     │
     ▼
ISR Stub (isr.S)
     │
     ▼
x86_64_trap_dispatch()
     │
     ▼
timer_on_irq0()
     │
     ▼
Serial Log QEMU
```

Penjelasan diagram:

```text
[PIT menghasilkan interrupt timer periodik pada IRQ0. Interrupt tersebut diteruskan oleh PIC setelah dilakukan remapping ke interrupt vector 32 pada IDT. Ketika interrupt terjadi, CPU menjalankan ISR stub pada file isr.S untuk menyimpan register CPU dan memanggil x86_64_trap_dispatch(). Trap dispatcher kemudian menjalankan timer_on_irq0() untuk menambah nilai tick dan menampilkan log serial pada QEMU.]
```

### 9.4 Kontrak Antarmuka

| Antarmuka | Pemanggil | Penerima | Precondition | Postcondition | Error path |
|---|---|---|---|---|---|
| `[pic_remap()]` | `[kmain.c]` | `[pic.c]` | `[PIC belum diremap dan IDT sudah tersedia]` | `[IRQ hardware dipindahkan ke vector interrupt baru]` | `[Interrupt hardware dapat bentrok dengan CPU exception]` |
| `[pit_configure_hz()]` | `[kmain.c]` | `[pit.c]` | `[PIC dan IDT sudah aktif]` | `[PIT menghasilkan interrupt timer periodik 100Hz]` | `[Timer interrupt tidak berjalan]` |
| `[isr_stub_32]` | `[CPU interrupt handler]` | `[isr.S]` | `[IRQ0 diterima CPU dan IDT valid]` | `[Register CPU disimpan lalu trap dispatcher dipanggil]` | `[Triple fault jika stack/frame salah]` |
| `[x86_64_trap_dispatch()]` | `[isr.S]` | `[trap.c]` | `[Trap frame valid dan interrupt vector tersedia]` | `[Interrupt timer diproses oleh kernel]` | `[Kernel panic jika vector tidak valid]` |
| `[timer_on_irq0()]` | `[x86_64_trap_dispatch()]` | `[pit.c]` | `[IRQ0 timer diterima]` | `[Tick bertambah dan log serial ditampilkan]` | `[Tick tidak bertambah atau interrupt berhenti]` |
| `[iretq]` | `[ISR handler]` | `[CPU]` | `[Trap frame dan stack alignment valid]` | `[CPU kembali ke eksekusi normal]` | `[Triple fault atau crash CPU]` |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `` `[struct trap_frame]` `` | `[vector, error_code, rip, rflags, register CPU]` | `[Kernel interrupt subsystem]` | `[Dibuat saat interrupt terjadi dan dihapus setelah iretq]` | `[Urutan register harus sesuai dengan ISR stack layout]` |
| `` `[struct idt_entry]` `` | `[offset_low, selector, type_attr, offset_high]` | `[IDT subsystem]` | `[Dibuat saat inisialisasi IDT dan aktif selama kernel berjalan]` | `[Setiap entry harus menunjuk ke ISR handler valid]` |
| `` `[struct idt_pointer]` `` | `[limit, base]` | `[IDT subsystem]` | `[Digunakan saat lidt dipanggil]` | `[Base dan limit harus sesuai ukuran IDT]` |
| `` `[g_idt[256]]` `` | `[Array entry IDT]` | `[Kernel architecture layer]` | `[Aktif selama kernel berjalan]` | `[Vector interrupt harus memiliki handler yang valid]` |
| `` `[g_ticks]` `` | `[Counter timer tick]` | `[PIT/timer subsystem]` | `[Bertambah selama kernel berjalan]` | `[Nilai tick hanya bertambah setiap IRQ0 diterima]` |

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. `[Invariant 1: IDT harus sudah ter-load sebelum interrupt diaktifkan menggunakan instruksi sti.]`
2. `[Invariant 2: PIC harus diremap ke interrupt vector non-reserved sebelum IRQ hardware digunakan.]`
3. `[Invariant 3: ISR handler harus menjaga urutan stack dan register CPU agar iretq dapat kembali dengan benar.]`
4. `[Invariant 4: Nilai g_ticks hanya boleh bertambah ketika IRQ0 timer berhasil diterima dan diproses kernel.]`

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| `[g_idt[256]]` | `[IDT subsystem kernel]` | `[none]` | `[Ya]` | `[IDT hanya diinisialisasi saat boot kernel]` |
| `[struct trap_frame]` | `[ISR dan trap dispatcher]` | `[none]` | `[Ya]` | `[Digunakan sementara selama interrupt berlangsung]` |
| `[g_ticks]` | `[PIT/timer subsystem]` | `[none]` | `[Ya]` | `[Diakses pada single-core interrupt context]` |
| `[PIC I/O port]` | `[PIC driver]` | `[none]` | `[Ya]` | `[Akses dilakukan langsung melalui port I/O]` |
| `[PIT I/O port]` | `[PIT driver]` | `[none]` | `[Ya]` | `[Digunakan untuk konfigurasi timer hardware]` |
| `[Serial log output]` | `[Serial subsystem]` | `[none]` | `[Ya]` | `[Digunakan untuk debugging interrupt runtime]` |


Lock order yang berlaku:

```text
[Pada praktikum ini belum digunakan mekanisme locking karena kernel masih berjalan pada mode single-core dan seluruh interrupt handling dilakukan secara sederhana tanpa concurrency antar CPU. Interrupt context dianggap cukup aman selama handler tidak melakukan operasi blocking.]
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| `[Stack alignment error]` | `[kernel/arch/x86_64/isr.S]` | `[Menambahkan subq $8, %rsp sebelum call x86_64_trap_dispatch]` | `[QEMU berhasil melewati STI: enabling interrupts dan timer berjalan normal]` |
| `[Trap frame tidak sesuai ISR stack layout]` | `[kernel/arch/x86_64/include/mcsos/arch/idt.h]` | `[Menyesuaikan urutan struct trap_frame dengan urutan pushq register di isr.S]` | `[Log QEMU berhasil menampilkan ticks=100, ticks=200, ticks=300]` |
| `[Error pada interrupt return iretq]` | `[kernel/arch/x86_64/isr.S]` | `[Menjaga urutan pushq/popq dan addq $16, %rsp sebelum iretq]` | `[Audit grep iretq build/kernel.disasm.txt berhasil menemukan instruction iretq]` |
| `[Symbol interrupt tidak ter-link]` | `[Makefile dan proses linking kernel]` | `[Menambahkan object PIC/PIT/ISR ke proses build kernel]` | `[grep pic_remap build/kernel.syms.txt dan grep pit_configure_hz build/kernel.syms.txt berhasil]` |
| `[Format output timer salah (hexadecimal)]` | `[kernel/arch/x86_64/pit.c]` | `[Mengganti output menjadi serial_write_dec64(g_ticks)]` | `[Output QEMU berubah menjadi ticks=100, ticks=200 sesuai panduan praktikum]` |


### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| `[Interrupt hardware (IRQ0 dari PIT)]` | `[Interrupt vector dan trap frame dari hardware]` | `[Validasi vector interrupt pada x86_64_trap_dispatch()]` | `[Kernel panic dan serial log jika vector tidak valid]` |
| `[IDT interrupt entry]` | `[Alamat ISR dan descriptor interrupt]` | `[IDT diinisialisasi menggunakan handler yang valid]` | `[CPU akan menolak interrupt invalid atau kernel panic]` |
| `[ISR stack frame]` | `[Register CPU dan stack interrupt]` | `[Menjaga urutan pushq/popq dan alignment stack]` | `[Triple fault dihindari dengan validasi layout ISR]` |
| `[PIC remap configuration]` | `[IRQ mapping hardware]` | `[PIC diremap ke vector non-reserved (32–47)]` | `[Menghindari konflik dengan CPU exception]` |
| `[Serial log output]` | `[Data runtime kernel]` | `[Logging dilakukan melalui fungsi serial kernel]` | `[Error/debug dapat dilihat melalui QEMU serial log]` |

---

## 10. Langkah Kerja Implementasi

Gunakan tabel berikut untuk setiap langkah. Sebelum setiap blok perintah, jelaskan maksud perintah, artefak yang dihasilkan, dan indikator hasil.

### Langkah 1 — `[Implementasi PIC dan PIT]`

Maksud langkah:

```text
[Langkah ini dilakukan untuk menambahkan PIC remapping dan PIT timer agar kernel dapat menerima interrupt timer periodik pada QEMU.]
```

Perintah:

```bash
[nano kernel/arch/x86_64/pic.c
nano kernel/arch/x86_64/pit.c
make clean
make grade]
```

Output ringkas:

```text
[grep -q 'pic_remap' build/kernel.syms.txt
grep -q 'pit_configure_hz' build/kernel.syms.txt
grep -q 'timer_on_irq0' build/kernel.syms.txt
M5 static grade: PASS]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[pic.c]` | `[kernel/arch/x86_64/pic.c]` | `[Implementasi PIC remapping untuk IRQ hardware]` |
| `[pit.c]` | `[kernel/arch/x86_64/pit.c]` | `[Implementasi timer PIT 100Hz]` |
| `[kernel.elf]` | `[build/kernel.elf]` | `[Binary kernel hasil build]` |
| `[kernel.syms.txt]` | `[build/kernel.syms.txt]` | `[Validasi symbol PIC, PIT, ISR, dan trap handler]` |
| `[kernel.disasm.txt]` | `[build/kernel.disasm.txt]` | `[Audit instruction interrupt seperti iretq, lidt, sti, dan hlt]` |
| `[kernel.map]` | `[build/kernel.map]` | `[Informasi mapping symbol dan section kernel]` |
| `[mcsos.iso]` | `[build/mcsos.iso]` | `[Image bootable kernel untuk QEMU]` |


Indikator berhasil:

```text
[Build berhasil tanpa error dan audit symbol PIC, PIT, ISR, serta interrupt instruction menghasilkan status “M5 static grade: PASS”.]
```

### Langkah 2 — `[Implementasi ISR dan IDT]`

Maksud langkah:

```text
[Langkah ini dilakukan untuk menambahkan interrupt service routine (ISR), trap dispatcher, dan konfigurasi IDT agar interrupt timer dari IRQ0 dapat diproses kernel dengan benar.]
```

Perintah:

```bash
nano kernel/arch/x86_64/isr.S
nano kernel/arch/x86_64/idt.c
make clean
make grade
```

Output ringkas:

```text
grep -q 'x86_64_idt_init' build/kernel.syms.txt
grep -q 'x86_64_trap_dispatch' build/kernel.syms.txt
grep -q 'iretq' build/kernel.disasm.txt
grep -q 'lidt' build/kernel.disasm.txt
grep -q 'sti' build/kernel.disasm.txt
grep -q 'hlt' build/kernel.disasm.txt
grep -q 'isr_stub_32' build/kernel.syms.txt
M5 static grade: PASS
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[isr.S]` | `[kernel/arch/x86_64/isr.S]` | `[Implementasi ISR stub dan interrupt return (iretq)]` |
| `[idt.c]` | `[kernel/arch/x86_64/idt.c]` | `[Implementasi IDT dan trap dispatcher kernel]` |
| `[kernel.disasm.txt]` | `[build/kernel.disasm.txt]` | `[Audit instruction interrupt (iretq, lidt, sti, hlt)]` |
| `[kernel.syms.txt]` | `[build/kernel.syms.txt]` | `[Validasi symbol ISR, IDT, dan trap handler]` |
| `[kernel.elf]` | `[build/kernel.elf]` | `[Binary kernel hasil build dan linking]` |
| `[kernel.map]` | `[build/kernel.map]` | `[Informasi mapping symbol dan section kerne]` |


Indikator berhasil:

```text
[Instruction interrupt (`iretq`, `lidt`, `sti`, `hlt`) dan symbol ISR (`isr_stub_32`) berhasil ditemukan pada audit kernel serta build menghasilkan status “M5 static grade: PASS”]
```

### Langkah 3 — `[Pengujian Timer IRQ pada QEMU]`

Maksud langkah:

```text
[Langkah ini dilakukan untuk memastikan interrupt timer dari PIT berhasil diterima dan diproses oleh kernel melalui IRQ0 pada QEMU.]
```

Perintah:

```bash
[qemu-system-x86_64 \
-M q35 \
-m 512M \
-cdrom build/mcsos.iso \
-serial stdio \
-no-reboot \
-no-shutdown]
```

Output ringkas:

```text
[[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts
[MCSOS:TIMER] ticks=100
[MCSOS:TIMER] ticks=200
[MCSOS:TIMER] ticks=300]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[mcsos.iso]` | `[build/mcsos.iso]` | `[Image bootable kernel untuk pengujian QEMU]` |
| `[Serial log QEMU]` | `[Terminal QEMU]` | `[Bukti runtime interrupt timer berjalan]` |
| `[m4-qemu-serial.log]` | `[evidence/M4/m4-qemu-serial.log]` | `[Penyimpanan evidence serial output praktikum]` |
| `[kernel.elf]` | `[build/kernel.elf]` | `[Binary kernel yang dijalankan pada QEMU]` |


Indikator berhasil:

```text
Nilai tick bertambah secara periodik tanpa kernel panic atau triple fault, menandakan IRQ0 timer berjalan normal.
```

---

## 11. Checkpoint Buildable

Setiap praktikum wajib memiliki minimal satu checkpoint yang dapat dibangun dari clean checkout.

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| Clean build | `` `make clean && make build` `` | `[kernel.elf, kernel.syms.txt, dan kernel.disasm.txt berhasil dibuat]` | `[PASS]` |
| Metadata toolchain | `` `date, uname -a, clang --version, gcc --version, qemu-system-x86_64 --version` `` | `[Informasi toolchain berhasil ditampilkan]` | `[PASS]` |
| Image generation | `` `make grade` `` | `[build/mcsos.iso berhasil dibuat]` | `[PASS]` |
| QEMU smoke test | `` `qemu-system-x86_64 -M q35 -m 512M -cdrom build/mcsos.iso -serial stdio` `` | `[Muncul log PIT configured 100Hz dan ticks=100]` |`[PASS]` |
| Test suite | `` `grep -q 'iretq' build/kernel.disasm.txt dan symbol audit lainnya` `` | `[Semua audit PIC, PIT, ISR, dan interrupt instruction lulus]` | `[PASS]` |

Catatan checkpoint:

```text
[Jelaskan checkpoint yang belum lulus dan alasan teknisnya.]
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
[clang --target=x86_64-unknown-none-elf ...
clang --target=x86_64-unknown-none-elf -c kernel/arch/x86_64/idt.c
clang --target=x86_64-unknown-none-elf -c kernel/arch/x86_64/pic.c
clang --target=x86_64-unknown-none-elf -c kernel/arch/x86_64/pit.c
clang --target=x86_64-unknown-none-elf -c kernel/core/trap.c
clang --target=x86_64-unknown-none-elf -c kernel/arch/x86_64/isr.S
ld.lld -nostdlib -static -T linker.ld -o build/kernel.elf]
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
[ELF64
Machine: Advanced Micro Devices X86-64

x86_64_idt_init
x86_64_trap_dispatch
pic_remap
pit_configure_hz
timer_on_irq0
isr_stub_32

iretq
lidt
sti
hlt]
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
[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts
[MCSOS:TIMER] ticks=100
[MCSOS:TIMER] ticks=200
[MCSOS:TIMER] ticks=300

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
[Remote debugging using :1234
Breakpoint 1 at 0xffffffff80000000: file kernel/core/kmain.c

Breakpoint 1, kmain () at kernel/core/kmain.c
info registers
rip            0xffffffff80000000
rsp            0x0000000000100000

bt
#0  kmain ()]
```

Status: `[PASS]`

### 12.5 Unit Test

```bash
make test
```

Hasil:

```text
Tidak terdapat unit test khusus pada praktikum M5.
Validasi dilakukan menggunakan static inspection, symbol audit, dan QEMU runtime test.
```

Status: `[NA]`

### 12.6 Stress/Fuzz/Fault Injection Test

Wajib untuk praktikum lanjutan seperti allocator, syscall, filesystem, networking, driver, security, dan SMP.

```bash
[perintah stress/fuzz/fault injection]
```

Hasil:

```text
[Praktikum M5 tidak mencakup stress test, fuzzing, atau fault injection karena fokus praktikum berada pada implementasi dasar PIC, PIT, IDT, dan interrupt timer pada kernel x86_64.]
```

Status: `[NA]`

### 12.7 Visual Evidence

Jika praktikum menghasilkan tampilan framebuffer, GUI, atau output grafis, lampirkan screenshot.

| Screenshot | Lokasi file | Keterangan |
|---|---|---|
| `[Screenshot serial log QEMU]` | `[evidence/M4/m4-qemu-serial.log]` | `[Membuktikan PIT berhasil dikonfigurasi dan interrupt timer berjalan (ticks=100, ticks=200, ticks=300)]` |
| `[Screenshot hasil make grade]` | `[build/kernel.syms.txt dan build/kernel.disasm.txt]` | `[Membuktikan audit symbol PIC, PIT, ISR, dan instruction interrupt berhasil]` |
| `[Screenshot GitHub push branch]` | `[Repository GitHub praktikum/m5-timer-irq]` | `[Membuktikan commit praktikum M5 berhasil diupload ke repository]` |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | `[Clean build kernel]` | `[Kernel berhasil dibangun tanpa error]` | `[kernel.elf, kernel.syms.txt, dan kernel.disasm.txt berhasil dibuat]` | `[PASS]` | `[Output make build]` |
| 2 | `[Symbol audit PIC dan PIT]` | `[Symbol pic_remap dan pit_configure_hz ditemukan]` | `[Symbol berhasil ditemukan pada build/kernel.syms.txt]` | `[PASS]` | `[build/kernel.syms.txt]` |
| 3 | `[Audit instruction interrupt]` | `[Instruction iretq, lidt, sti, dan hlt ditemukan]` | `[Semua instruction berhasil ditemukan]` | `[PASS]` | `[build/kernel.disasm.txt]` |
| 4 | `[ISR dan trap dispatcher]` | `[Symbol isr_stub_32 dan x86_64_trap_dispatch tersedia]` | `[Symbol berhasil ditemukan]` | `[PASS]` | `[build/kernel.syms.txt]` |
| 5 | `[QEMU smoke test]` | `[Kernel menerima interrupt timer periodik]` | `[Muncul log ticks=100, ticks=200, ticks=300]` | `[PASS]` | `[Serial log QEMU]` |
| 6 | `[PIT timer configuratio]` | `[PIT berhasil dikonfigurasi 100Hz]` | `[Muncul log PIT configured 100Hz]` | `[PASS]` | `[Serial log QEMU]` |
| 7 | `[Interrupt enable test]` | `[CPU berhasil mengaktifkan interrupt]` | `[Muncul log STI: enabling interrupts]` | `[PASS]` | `[Serial log QEMU]` |
| 8 | `[GitHub repository update]` | `[Branch praktikum berhasil dipush]` | `[Branch praktikum/m5-timer-irq tersedia di GitHub]` | `[PASS]` | `[Repository GitHub]` |

### 13.2 Log Penting

```text
[MCSOS:M5] IDT loaded
[MCSOS:M5] PIC remapped
[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts

[MCSOS:TIMER] ticks=100
[MCSOS:TIMER] ticks=200
[MCSOS:TIMER] ticks=300

M5 static grade: PASS
```

### 13.3 Artefak Bukti

| Artefak | Path | SHA-256 / hash | Fungsi |
|---|---|---|---|
| `kernel.elf` | `[build/kernel.elf]` | `[8c8016c9168a78ac72c1ecc55e58926e0dbcecced640fa7b401ab6e0d03a2bdd]` | `[Binary kernel hasil build]` |
| `mcsos.iso` | `[build/mcsos.iso]` | `[20d8d423a0a97ebe1cb8a9726bfca58d682a419001a011cd3155c5730c13cdf4]` | `[Bootable image kernel untuk QEMU]` |
| `kernel.map` | `[build/kernel.map]` | `[4f577d1473ff7d73b032686ae0334ddbc64ef1f05c80a308151007e218894578]` | `[Linker map kernel]` |
| `kernel.disasm.txt` | `[build/kernel.disasm.txt]` | `[071f220596d9e42effbe72187edf884395070ed67f1d0aa8921e91d9b4e547da]` | `[Evidence instruction interrupt (iretq, sti, hlt)]` |
| `kernel.syms.txt` | `[build/kernel.syms.txt]` | `[affa03f45ccb5e9b2e7154d09b5524a61286aeab40757ad06127f131a6f54492]` | `[Evidence symbol PIC, PIT, ISR, dan IDT]` |


Perintah hash:

```bash
sha256sum build/kernel.elf
sha256sum build/mcsos.iso
sha256sum build/kernel.map
sha256sum build/kernel.disasm.txt
sha256sum build/kernel.syms.txt
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
[Praktikum berhasil karena PIC, PIT, IDT, dan ISR berhasil diintegrasikan ke dalam kernel MCSOS. PIC berhasil diremap ke interrupt vector 32–47 sehingga tidak bertabrakan dengan CPU exception. PIT berhasil dikonfigurasi pada frekuensi 100Hz dan menghasilkan interrupt periodik melalui IRQ0.

ISR assembly pada isr.S berhasil menyimpan register CPU dan mengembalikan eksekusi menggunakan iretq tanpa menyebabkan triple fault. Trap dispatcher x86_64_trap_dispatch() berhasil memproses interrupt timer dan memanggil timer_on_irq0() untuk menambah nilai tick.

Keberhasilan dibuktikan melalui log QEMU:
[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts
[MCSOS:TIMER] ticks=100
[MCSOS:TIMER] ticks=200

Selain itu, audit symbol dan instruction juga berhasil menemukan symbol PIC/PIT/ISR serta instruction interrupt seperti lidt, sti, hlt, dan iretq.]
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
[Selama praktikum terdapat beberapa masalah pada implementasi interrupt timer. Pada awal pengujian, timer tidak berjalan karena stack alignment ISR belum sesuai sehingga CPU mengalami masalah saat kembali dari interrupt menggunakan iretq.

Masalah lain terjadi ketika output timer masih ditampilkan dalam format hexadecimal sehingga tidak sesuai dengan panduan praktikum. Hal ini diperbaiki dengan mengganti fungsi logging menjadi serial_write_dec64() agar nilai tick tampil dalam format desimal.

Selain itu, beberapa artefak build seperti kernel.syms.txt dan kernel.disasm.txt sempat hilang setelah menjalankan make clean. Masalah diselesaikan dengan menjalankan kembali make grade untuk meregenerasi seluruh evidence build dan audit.]
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| `[PIC remapping]` | `[pic_remap() memindahkan IRQ ke vector 32–47]` | `[sesuai]` | `[Menghindari konflik antara IRQ hardware dan CPU exception]` |
| `[PIT timer periodik]` | `[pit_configure_hz(100) menghasilkan interrupt 100Hz]` | `[sesuai]` | `[Timer interrupt berjalan periodik pada QEMU]` |
| `[Interrupt Service Routine (ISR)]` | `[ISR ditulis pada isr.S menggunakan pushq/popq dan iretq]` | `[sesuai]` | `[CPU berhasil kembali dari interrupt tanpa triple fault]` |
| `[IDT interrupt handling]` | `[IDT diinisialisasi melalui x86_64_idt_init()]` | `[sesuai]` | `[Interrupt vector berhasil terhubung ke ISR handler]` |
| `[Interrupt enable]` | `[CPU menggunakan instruksi sti]` | `[sesuai]` | `[Interrupt mulai aktif setelah kernel mengaktifkan interrupt flag]` |


### 14.4 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Bukti | Catatan |
|---|---|---|---|
| Kompleksitas algoritma | `[O(1) pada interrupt dispatch timer]` | `[ISR langsung memanggil x86_64_trap_dispatch() dan timer_on_irq0() tanpa traversal data]` | `[Handler interrupt sederhana dan berjalan langsung pada IRQ0]` |
| Waktu build | `[Build berhasil tanpa error]` | `[Output make build menunjukkan seluruh object berhasil di-compile dan di-link menjadi build/kernel.elf]` | `[Build dijalankan pada WSL2 Ubuntu]` |
| Waktu boot QEMU | `[Boot berhasil hingga timer aktif]` | `[Log QEMU: [MCSOS:M5] PIT configured 100Hz dan ticks=100]` | `[Kernel berhasil melewati fase interrupt initialization]` |
| Penggunaan memori | `[512M RAM virtual]` | `[Parameter QEMU -m 512M]` | `[Digunakan sebagai environment pengujian praktikum]` |
| Latensi/throughput | `[Timer periodik 100Hz berjalan normal]` | `[Log serial: ticks=100, ticks=200, ticks=300]` | `[PIT menghasilkan interrupt periodik tanpa kernel panic]` |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab sementara | Bukti | Perbaikan |
|---|---|---|---|---|
| `[Timer IRQ tidak berjalan]` | `[Tidak muncul log ticks=100 pada QEMU]` | `[PIC/PIT belum terhubung dengan IRQ handler]` | `[QEMU hanya berhenti setelah STI: enabling interrupts]` | `[Menambahkan timer_on_irq0() pada x86_64_trap_dispatch()]` |
| `[interrupt return gagal (iretq)]` | `[Kernel hang setelah interrupt aktif]` | `[Stack alignment ISR tidak sesuai]` | `[QEMU berhenti setelah interrupt pertama]` | `[Menambahkan alignment stack sebelum call x86_64_trap_dispatch]` |
| `[Output tick tampil hexadecimal]` | `[Log timer tidak sesuai panduan praktikum]` | `[Menggunakan output hex pada serial log]` | `[Nilai tick tampil dalam format hex]` | `[Mengganti logging menjadi serial_write_dec64()]` |
| `[Symbol audit gagal]` | `[grep tidak menemukan symbol interrupt]` | `[kernel.syms.txt belum dibuat ulang]` | `[Error saat menjalankan audit symbol]` | `[Menjalankan kembali make grade]` |
| `[Artefak build hilang setelah clean build]` | `[kernel.disasm.txt dan kernel.syms.txt tidak ditemukan]` | `[make clean menghapus seluruh folder build]` | `[Error No such file or directory saat sha256sum]` | `[Menjalankan kembali make grade untuk regenerasi artefak]` |


### 15.2 Failure Modes yang Diantisipasi

| Failure mode | Deteksi | Dampak | Mitigasi |
|---|---|---|---|
| `[Triple fault saat interrupt aktif]` | `[QEMU hang atau restart]` | `[Kernel gagal boot]` | `[Menjaga stack alignment ISR dan penggunaan iretq yang benar]` |
| `[PIT timer tidak berjalan]` | `[Tidak muncul log ticks=100]` | `[Interrupt timer gagal]` | `[Memastikan pit_configure_hz(100) dan IRQ0 handler aktif]` |
| `[Konflik IRQ dengan exception CPU]` | `[Interrupt masuk ke handler salah]` | `[Kernel crash]` | `[Melakukan PIC remap ke vector 32–47]` |

### 15.3 Triage yang Dilakukan

```text
[Diagnosis dilakukan menggunakan serial log QEMU untuk memeriksa apakah interrupt timer berjalan. Setelah itu dilakukan audit symbol dan disassembly menggunakan kernel.syms.txt dan kernel.disasm.txt untuk memastikan symbol ISR, PIC, PIT, serta instruction interrupt seperti lidt, sti, hlt, dan iretq berhasil ter-link.

Saat timer tidak berjalan, dilakukan pengecekan pada ISR stack alignment dan trap dispatcher di isr.S serta idt.c. Setelah perbaikan dilakukan, pengujian ulang dijalankan menggunakan QEMU hingga log ticks=100, ticks=200, dan ticks=300 berhasil muncul.]
```

### 15.4 Panic Path

Jika terjadi panic, tempel output panic.

```text
[Selama pengujian akhir praktikum M5 tidak terjadi kernel panic. Panic path sebelumnya diuji melalui trap handler pada x86_64_trap_dispatch() yang akan memanggil kernel_panic_at() jika menerima interrupt vector yang tidak valid atau exception yang tidak ditangani.]
```

---

## 16. Prosedur Rollback

Rollback harus menjelaskan cara kembali ke kondisi aman jika perubahan gagal.

| Skenario rollback | Perintah | Data yang harus diselamatkan | Status |
|---|---|---|---|
| Kembali ke commit awal | `` `git checkout [321e03e]` `` | `[Log build dan evidence praktikum M5]` | `[teruji]` |
| Revert commit praktikum | `` `git revert [0992aa0]` `` | `[kernel.elf, serial log, dan source code praktikum]` | `[belum]` |
| Bersihkan artefak build | `` `make clean` `` | `[Source code kernel]` | `[teruji]` |
| Regenerasi image | `` `make grade` `` | `[mcsos.iso lama jika diperlukan]` | `[teruji]` |

Catatan rollback:

```text
[Rollback parsial telah diuji menggunakan make clean dan rebuild kernel menggunakan make grade. Pengembalian ke commit sebelumnya dapat dilakukan menggunakan git checkout 321e03e sebagai baseline sebelum implementasi M5. Revert commit penuh belum diuji secara langsung karena implementasi M5 sudah berjalan stabil pada QEMU.]
```

---

## 17. Keamanan dan Reliability

### 17.1 Risiko Keamanan

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| `[Interrupt vector tidak valid]` | `[IDT dan ISR handler]` | `[Kernel crash atau triple fault]` | `[Validasi interrupt vector pada x86_64_trap_dispatch()]` | `[Serial log dan audit ISR berhasil]` |
| `[Stack ISR tidak aligned]` | `[Interrupt handling CPU]` | `[iretq gagal dan kernel hang]` | `[Menyesuaikan stack alignment pada isr.S]` | `[QEMU berhasil menjalankan timer interrupt]` |
| `[Konflik IRQ dengan CPU exception]` | `[PIC interrupt mapping]` | `[Interrupt masuk ke handler salah]` | `[PIC diremap ke vector 32–47]` | `[Log PIC remapped dan timer IRQ berjalan]` |

### 17.2 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| `[Kernel hang setelah interrupt aktif]` | `[Sistem berhenti merespons]` | `[QEMU berhenti setelah sti]` | `[Memperbaiki stack alignment ISR dan penggunaan iretq]` |
| `[Interrupt timer tidak berjalan]` | `[Tick tidak bertambah]` | `[Tidak muncul log ticks=100]` | `[Memastikan PIC, PIT, dan IRQ0 handler aktif]` |
| `[Artefak build hilang setelah clean]` | `[Evidence praktikum tidak tersedia]` | `[Error No such file or directory saat audit/hash]` | `[Regenerasi build menggunakan make grade]` |

### 17.3 Negative Test

| Negative test | Input buruk | Expected result | Actual result | Status |
|---|---|---|---|---|
| `[Interrupt sebelum IDT aktif]` | `[sti dijalankan tanpa IDT valid]` | `[Kernel panic atau interrupt ditolak]` | `[Kernel hang pada pengujian awal]` | `[PASS]` |
| `[PIT belum dikonfigurasi]` | `[IRQ0 aktif tanpa pit_configure_hz()]` | `[Tick timer tidak muncul]` | `[Tidak muncul log ticks=100]` | `[PASS]` |
| `[ISR stack tidak aligned]` | `[Interrupt return menggunakan stack salah]` | `[iretq gagal atau triple fault]` | `[QEMU hang sebelum perbaikan ISR]` | `[PASS]` |

---

## 18. Pembagian Kerja Kelompok

Isi bagian ini hanya jika praktikum dikerjakan berkelompok. Untuk pengerjaan individu, tulis “Tidak berlaku”.

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `[Asti Lestari]` | `[25832071002]` | `[Koordinator teknis]` | `[Menjaga konsistensi repository, branch, dan readiness checklist serta menyusun laporan]` | `[0992aa0]` |
| `[Amelia Okta Ramadani]` | `[25832072004]` | `[Toolchain engineer]` | `[Menyiapkan WSL, paket, QEMU, OVMF, compiler, assembler, debugger, dan script validasi.]` | `[0992aa0]` |
| `[Wifa Fazriyatul Fadhla]` | `[2583207073003]` | `[Documentation engineer]` | `[Menyusun baseline requirements, ADR, risk register]` | `[0992aa0]` |
| `[Nazwa Rahmadanti]` | `[2583207073005]` | `[Verification engineer]` | `[Menyusun verification matrix, menjalankan check script, dan mengumpulkan bukti.]` | `[0992aa0]` |
| `[Fauziah Putri Rahayu]` | `[2583207073004]` | `[Security reviewer]` | `[Menyusun threat model awal dan memeriksa fail-closed policy.]` | `[0992aa0]` |

### 18.1 Mekanisme Koordinasi

```text
[Koordinasi dilakukan menggunakan repository GitHub dengan branch praktikum terpisah untuk pengembangan M5. Setiap anggota memiliki tugas masing-masing seperti toolchain, testing, dokumentasi, dan security review. Integrasi perubahan dilakukan melalui commit dan push ke branch praktikum/m5-timer-irq. Validasi dilakukan bersama menggunakan make grade, audit symbol/disassembly, dan pengujian QEMU untuk memastikan seluruh fitur interrupt timer berjalan normal.]
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---:|---|---|
| `[Asti Lestari]` | `[28%]` | `[0992aa0]` | `[Mengoordinasikan pengembangan, memastikan setiap milestone terintegrasi dan lolos verifikasi, serta membuat laporan praktikum]` |
| `[Amelia Okta Ramadani]` | `[18%]` | `[0992aa0]` | `[Bertanggung jawab pada kesiapan lingkungan pengembangan dan toolchain]` |
| `[Wifa Fazriyatul Fadhla]` | `[18%]` | `[0992aa0]` | `[Menyusun dan memelihara dokumentasi teknis proyek]` |
| `[Nazwa Rahmadanti]` | `[18%]` | `[]` | `[Melaksanakan pengujian dan mengumpulkan bukti verifikasi setiap milestone]` |
| `[Fauziah Putri Rahayu]` | `[18%]` | `[0992aa0]` | `[Melakukan tinjauan keamanan dan identifikasi risiko teknis.]` |

---

## 19. Kriteria Lulus Praktikum

Bagian ini wajib diisi. Praktikum dinyatakan memenuhi kriteria minimum hanya jika bukti tersedia.

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Proyek dapat dibangun dari clean checkout | `[PASS]` | `[Output make clean && make build]` |
| Perintah build terdokumentasi | `[PASS]` | `[Bagian 10 dan 12 laporan]` |
| QEMU boot atau test target berjalan deterministik | `[PASS]` | `[Log ticks=100, ticks=200, ticks=300]` |
| Semua unit test/praktikum test relevan lulus | `[PASS]` | `[M5 static grade: PASS]` |
| Log serial disimpan | `[PASS]` | `[evidence/M4/m4-qemu-serial.log]` |
| Panic path terbaca atau dijelaskan jika belum relevan | `[PASS]` | `[Bagian 15.4 laporan]` |
| Tidak ada warning kritis pada build | `[PASS]` | `[Build kernel berhasil tanpa warning/error]` |
| Perubahan Git terkomit | `[PASS]` | `[Commit 0992aa0]` |
| Desain dan failure mode dijelaskan | `[PASS]` | `[Bagian 9 dan 15 laporan]` |
| Laporan berisi screenshot/log yang cukup | `[PASS]` | `[Screenshot QEMU, build log, dan audit evidence]` |

Kriteria tambahan untuk praktikum lanjutan:

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| Static analysis dijalankan | `[PASS]` | `[Audit kernel.syms.txt dan kernel.disasm.txt]` |
| Stress test dijalankan | `[NA]` | `[Tidak termasuk cakupan praktikum M5]` |
| Fuzzing atau malformed-input test dijalankan | `[NA]` | `[Tidak termasuk cakupan praktikum M5]` |
| Fault injection dijalankan | `[NA]` | `[Tidak termasuk cakupan praktikum M5]` |
| Disassembly/readelf evidence tersedia | `[PASS]` | `[build/kernel.disasm.txt dan readelf audit]` |
| Review keamanan dilakukan | `[PASS]` | `[Bagian 17 laporan]` |
| Rollback diuji | `[PASS]` | `[make clean dan rebuild menggunakan make grade]` |

---

## 20. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | `[ ]` |
| Siap uji QEMU | Build bersih, QEMU/test target berjalan, log tersedia | `[ ]` |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | `[v]` |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia | `[ ]` |

Alasan readiness:

```text
[Praktikum M5 dinyatakan siap demonstrasi praktikum karena kernel berhasil dibangun dari clean build, audit symbol dan disassembly berhasil lulus, serta interrupt timer berjalan normal pada QEMU. Bukti keberhasilan ditunjukkan melalui log serial seperti “PIT configured 100Hz”, “STI: enabling interrupts”, dan “ticks=100”. Selain itu, failure mode, rollback, security review, dan evidence build juga telah didokumentasikan pada laporan.]
```

Known issues:

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | `[Belum menggunakan APIC/IOAPIC]` | `[Sistem masih memakai PIC legacy]` | `[Menggunakan PIC remap untuk IRQ]` | `[Milestone interrupt lanjutan]` |
| 2 | `[Belum terdapat stress/fuzz test]` | `[Reliability jangka panjang belum diuji]` | `[Validasi menggunakan QEMU smoke test]` | `[Praktikum lanjutan]` |

Keputusan akhir:

```text
[Berdasarkan hasil clean build, audit ELF/disassembly, serial log QEMU, dan status “M5 static grade: PASS”, praktikum ini layak disebut siap demonstrasi praktikum untuk milestone M5 interrupt timer pada kernel MCSOS.]
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
[Praktikum M5 berhasil mengimplementasikan interrupt timer pada kernel MCSOS menggunakan PIC, PIT, IDT, dan ISR pada arsitektur x86_64. Kernel berhasil dibangun dari clean build tanpa error, audit symbol dan disassembly berhasil lulus, serta interrupt timer berjalan normal pada QEMU.

Keberhasilan dibuktikan melalui log:
[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts
[MCSOS:TIMER] ticks=100
[MCSOS:TIMER] ticks=200
[MCSOS:TIMER] ticks=300

Selain itu, instruction interrupt seperti lidt, sti, hlt, dan iretq berhasil ditemukan pada kernel.disasm.txt.]
```

### 22.2 Yang Belum Berhasil

```text
[Praktikum masih menggunakan PIC legacy dan belum mengimplementasikan APIC/IOAPIC. Selain itu, belum terdapat stress test, fuzzing, maupun fault injection untuk menguji reliability interrupt subsystem secara lebih lanjut.]
```

### 22.3 Rencana Perbaikan

```text
[Pengembangan berikutnya adalah menambahkan APIC/IOAPIC, scheduler berbasis timer interrupt, dan mekanisme multitasking kernel. Selain itu, pengujian reliability akan diperluas menggunakan stress test dan fault injection agar interrupt subsystem lebih stabil pada kondisi runtime yang kompleks.]
```

---

## 23. Lampiran

### Lampiran A — Commit Log

```text
[0992aa0 (HEAD -> praktikum/m5-timer-irq, origin/praktikum/m5-timer-irq) M5 PIC PIT interrupt working
27eff0d (origin/m4-idt-exception-path-final, m4-idt-exception-path) M4 add QEMU smoke test log
810dbf9 M4 add x86_64 IDT and exception trap path
321e03e (origin/main, origin/HEAD, main) M3 complete: grading and evidence pipeline
4fd6b04 M3 complete: GDB audit and QEMU debug flow]
```

### Lampiran B — Diff Ringkas

```diff
[+ kernel/arch/x86_64/pic.c
+ kernel/arch/x86_64/pit.c
+ kernel/arch/x86_64/isr.S

* kernel/arch/x86_64/idt.c
* kernel/core/trap.c
* kernel/core/kmain.c
* Makefile]
```

### Lampiran C — Log Build Lengkap

```text
[Log build lengkap tersedia dari output:
make clean
make build

Artefak build:
build/kernel.elf
build/kernel.map
build/kernel.syms.txt
build/kernel.disasm.txt]
```

### Lampiran D — Log QEMU Lengkap

```text
[[MCSOS:M5] IDT loaded
[MCSOS:M5] PIC remapped
[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts

[MCSOS:TIMER] ticks=100
[MCSOS:TIMER] ticks=200
[MCSOS:TIMER] ticks=300]
```

### Lampiran E — Output Readelf/Objdump

```text
[ELF64
Machine: Advanced Micro Devices X86-64

x86_64_idt_init
x86_64_trap_dispatch
pic_remap
pit_configure_hz
timer_on_irq0
isr_stub_32

iretq
lidt
sti
hlt]
```

### Lampiran F — Screenshot

| No. | File | Keterangan |
|---|---|---|
| 1 | `[Screenshot QEMU serial log]` | `[Membuktikan interrupt timer berjalan]` |
| 2 | `[Screenshot make grade]` | `[Membuktikan audit M5 berhasil]` |
| 2 | `[Screenshot GitHub branch]` | `[Membuktikan commit praktikum berhasil dipush]` |
### Lampiran G — Bukti Tambahan

```text
[SHA-256 kernel.elf:
8c8016c9168a78ac72c1ecc55e58926e0dbcecced640fa7b401ab6e0d03a2bdd

SHA-256 mcsos.iso:
20d8d423a0a97ebe1cb8a9726bfca58d682a419001a011cd3155c5730c13cdf4

SHA-256 kernel.disasm.txt:
071f220596d9e42effbe72187edf884395070ed67f1d0aa8921e91d9b4e547da

SHA-256 kernel.syms.txt:
affa03f45ccb5e9b2e7154d09b5524a61286aeab40757ad06127f131a6f54492]
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
[1] [R. H. Arpaci-Dusseau and A. C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces. Madison, WI, USA: Arpaci-Dusseau Books, 2018. [Online]. Available: https://pages.cs.wisc.edu/~remzi/OSTEP/. Accessed: 18-May-2026.]

[2] [Intel Corporation, Intel 64 and IA-32 Architectures Software Developer’s Manual. [Online]. Available: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html. Accessed: 18-May-2026.]

[3] [Advanced Micro Devices, AMD64 Architecture Programmer’s Manual Volume 2: System Programming. [Online]. Available: https://www.amd.com/system/files/TechDocs/24593.pdf. Accessed: 18-May-2026.]

[4] [R. Cox, F. Kaashoek, and R. Morris, “xv6: a simple, Unix-like teaching operating system,” MIT PDOS. [Online]. Available: https://pdos.csail.mit.edu/6.828/2021/xv6.html. Accessed: 18-May-2026.]

[5] [Limine Bootloader Project, “Limine Bare Bones and Boot Protocol Documentation.” [Online]. Available: https://github.com/limine-bootloader/limine. Accessed: 18-May-2026.]

[6] [OSDev Wiki, “Interrupt Descriptor Table (IDT).” [Online]. Available: https://wiki.osdev.org/Interrupt_Descriptor_Table. Accessed: 18-May-2026.]

[7] [OSDev Wiki, “Programmable Interval Timer.” [Online]. Available: https://wiki.osdev.org/Programmable_Interval_Timer. Accessed: 18-May-2026.]

[8] [ OSDev Wiki, “8259 PIC.” [Online]. Available: https://wiki.osdev.org/8259_PIC. Accessed: 18-May-2026.]
```

---

## 25. Checklist Final Sebelum Pengumpulan

| Checklist | Status |
|---|---|
| Semua placeholder `[isi ...]` sudah diganti | `[Ya]` |
| Metadata laporan lengkap | `[Ya]` |
| Commit awal dan akhir dicatat | `[Ya]` |
| Perintah build dan test dapat dijalankan ulang | `[Ya]` |
| Log build dilampirkan | `[Ya]` |
| Log QEMU/test dilampirkan | `[Ya]` |
| Artefak penting diberi hash | `[Ya]` |
| Desain, invariants, ownership, dan failure modes dijelaskan | `[Ya]` |
| Security/reliability dibahas | `[Ya]` |
| Readiness review tidak berlebihan | `[Ya]` |
| Rubrik penilaian diisi atau disiapkan | `[Ya]` |
| Referensi memakai format IEEE | `[Ya]` |
| Laporan disimpan sebagai Markdown | `[Ya]` |

---

## 26. Pernyataan Pengumpulan

Saya/kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```text
[0992aa0]
```

Status akhir yang diklaim:

```text
[siap demonstrasi praktikum]
```

Ringkasan satu paragraf:

```text
[Praktikum M5 berhasil mengimplementasikan PIC remapping, PIT timer 100Hz, IDT, ISR, dan interrupt handling pada kernel MCSOS berbasis x86_64. Kernel berhasil dibangun dari clean build, audit symbol/disassembly berhasil lulus, serta interrupt timer berjalan normal pada QEMU dengan log ticks periodik sebagai bukti utama. Failure mode seperti interrupt hang dan stack alignment ISR berhasil dianalisis dan diperbaiki. Keterbatasan saat ini adalah sistem masih menggunakan PIC legacy dan belum memiliki stress test atau APIC/IOAPIC. Pengembangan berikutnya diarahkan pada scheduler berbasis timer interrupt dan peningkatan reliability testing.]
```
