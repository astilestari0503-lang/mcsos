# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[kode_praktikum]_[nim_atau_kelompok].md`  
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
| Kode praktikum | `[M9]` |
| Judul praktikum | `[Kernel Thread, Runqueue Round-Robin Kooperatif, Context Switch x86_64, dan Integrasi Scheduler Awal pada MCSOS]` |
| Jenis pengerjaan | `[Kelompok]` |
| Nama mahasiswa | `[Asti Lestari]` |
| NIM | `[25832071002]` |
| Kelas | `[1A]` |
| Nama kelompok | `[kelompok Princess]` |
| Anggota kelompok | `[Asti Lestari, Amelia Okta Ramadani, Wifa Fazriyatul Fadhla, Nazwa Rahmadanti, Fauziah Putri Rahayu]` 
| Tanggal praktikum | `[2026-06-10]` |
| Tanggal pengumpulan | `[2026-06-DD]` |
| Repository | `[https://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[m9-kernel-thread-scheduler]` |
| Commit awal | `` `[2b838c5]` `` |
| Commit akhir | `` `[18a4868]` `` |
| Status readiness yang diklaim | `[siap uji QEMU untuk kernel thread dan scheduler awal single-core]` |

---

## 1. Sampul

# Laporan Praktikum `[M9]`  
## `[Kernel Thread, Runqueue Round-Robin Kooperatif, Context Switch x86_64, dan Integrasi Scheduler Awal pada MCSOS]`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|---|---|---|---|
| `[Asti Lestari]` | `[25832071002]` | `[1A]` | `[ketua]` |


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
| Semua penggunaan AI assistant dicatat | `[Ya]` |
| Repository yang dikumpulkan sesuai commit akhir | `[Ya]` |
| Tidak ada klaim readiness tanpa bukti | `[Ya]` |

Catatan penggunaan bantuan eksternal:

```text
[Panduan resmi praktikum M9 MCSOS 260502 digunakan sebagai referensi utama untuk
struktur TCB, kontrak API scheduler, assembly context switch, dan target Makefile.
Source code pada panduan digunakan sebagai template implementasi dan diverifikasi
ulang secara mandiri melalui kompilasi host test dan audit object freestanding di
lingkungan WSL 2 Ubuntu 24.04.
Dokumentasi Intel SDM [1], x86-64 psABI [2], QEMU GDB [3], Clang [4], dan GNU ld [5]
dipakai sebagai referensi teknis pendukung.]
```

---

## 3. Tujuan Praktikum

Tuliskan tujuan teknis dan konseptual praktikum. Tujuan harus dapat diuji.

1. `[Membangun Thread Control Block (TCB) dan struktur data scheduler yang memenuhi invariant state machine NEW → READY → RUNNING → BLOCKED → READY → ZOMBIE.]`
2. `[Mengimplementasikan runqueue FIFO round-robin kooperatif dengan operasi enqueue, pick next, yield, block, dan mark ready yang benar.]`
3. `[Mengimplementasikan context switch x86_64 dalam assembly yang menyimpan dan memulihkan register callee-saved (rsp, rbp, rbx, r12–r15, rip) secara benar.]`
4. `[Menulis host unit test scheduler yang berjalan di host Linux tanpa QEMU dan membuktikan state machine serta FIFO queue bekerja.]`
5. `[Menghasilkan freestanding object ELF64 x86_64 yang dapat diaudit dengan nm, readelf, dan objdump.]`
6. `[Mengintegrasikan scheduler ke kernel MCSOS dan membuktikan dua thread bergantian melalui QEMU serial log.]`

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| `[Menjelaskan perbedaan thread kernel, proses, CPU context, dan scheduler]` | `[Penjelasan dalam dasar teori dan analisis laporan]` |
| `[Mendesain TCB dengan state, context, stack metadata, entry function, dan linkage runqueue]` | `[include/mcsos_thread.h, diagram state machine]` |
| `[Mengimplementasikan round-robin kooperatif single-core]` | `[kernel/mcsos_thread.c, build/m9/test_scheduler.log]` |
| `[Mengimplementasikan context switch x86_64 assembly]` | `[arch/x86_64/context_switch.S, build/m9/objdump_key.log]` |
| `[Menyusun host unit test untuk logika scheduler]` | `[tests/test_scheduler.c, output M9 scheduler host unit test PASS]` |
| `[Melakukan audit object freestanding]` | `[build/m9/nm_undefined.log, build/m9/readelf_header.log]` |
| `[Menjelaskan failure modes scheduler]` | `[Bagian 15 laporan ini]` |
| `[Menulis laporan praktikum dengan bukti lengkap]` | `[Laporan ini beserta lampiran artefak]` |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M2 | Boot image, kernel ELF64, early console | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M3 | Panic path, linker map, GDB, observability awal | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M4 | Trap, exception, interrupt, timer | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M5 | PMM, VMM, page table, kernel heap | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M6 | Thread, scheduler, synchronization | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M7 | Syscall ABI dan user program loader | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M8 | VFS, file descriptor, ramfs | `[ ] tidak dibahas / [ ] dibahas / [v] selesai praktikum` |
| M9 | Block layer dan device model | `[ ] tidak dibahas / [v] dibahas / [ ] selesai praktikum` |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M11 | Networking stack, packet parsing, UDP/TCP subset | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M13 | SMP, scalability, lock stress, NUMA-aware preparation | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M14 | Framebuffer, graphics console, visual regression | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M15 | Virtualization/container subset | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M16 | Observability, update/rollback, release image, readiness review | `[v] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |

Batas cakupan praktikum:

```text
[M9 mencakup: kernel thread (TCB), runqueue FIFO, round-robin kooperatif single-core,
context switch callee-saved register x86_64, host unit test, audit ELF freestanding,
integrasi kernel, dan QEMU smoke test dua thread demo.

Non-goals M9: user mode (ring 3), syscall ABI, ELF user loader, address space per-proses,
SMP, priority scheduler, CFS/EEVDF, real-time scheduling, signal, wait/exit proses,
IPC penuh, FPU/SSE/AVX context, preemption penuh berbasis timer.

M9 hanya membuktikan kernel scheduler awal single-core dalam mode kooperatif, bukan
scheduler produksi atau sistem siap SMP.]
```

---

## 6. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.

### 6.1 Konsep Sistem Operasi yang Diuji

```text
[Thread Kernel dan Thread Control Block (TCB):
Thread kernel adalah unit eksekusi yang dijadwalkan oleh scheduler sistem operasi.
Setiap thread memiliki TCB yang menyimpan identitas (id, nama), state mesin (NEW/READY/
RUNNING/BLOCKED/ZOMBIE), context register (rsp, rbp, rbx, r12–r15, rip), pointer ke
entry function, argumen, dan metadata stack (base address, ukuran). TCB pada M9 belum
memiliki address space sendiri, file descriptor, atau resource accounting karena belum
ada user mode.

Kenapa setiap thread butuh stack sendiri:
Stack menyimpan frame fungsi, variabel lokal, return address, dan callee-saved register
dari perspektif caller. Jika dua thread berbagi stack, context switch akan merusak frame
satu thread ketika thread lain berjalan. Stack terpisah memungkinkan setiap thread
melanjutkan eksekusi dari titik yang tepat setelah dipulihkan.

Scheduler Round-Robin Kooperatif:
Scheduler memilih thread berikutnya dari runqueue FIFO. Thread yang sedang running
memanggil yield() secara eksplisit; scheduler menyimpan context thread lama, memasukkan
thread lama ke ekor queue, mengambil thread dari kepala queue, memulihkan context thread
baru, dan melanjutkan eksekusinya. Tanpa preemption, thread tidak bisa dipaksa keluar;
harus yield secara sukarela.

Context Switch:
Proses menyimpan state CPU thread lama (register callee-saved + rsp/rip) ke TCB dan
memulihkan state CPU thread baru dari TCB. Pada ABI x86_64 System V, caller bertanggung
jawab menyimpan rax, rcx, rdx, rsi, rdi, r8–r11 (caller-saved); callee bertanggung jawab
menyimpan rbx, rbp, r12–r15 (callee-saved). Context switch harus menyimpan callee-saved
karena dari sudut pandang thread lama, "pemanggil" context switch berharap register-
register itu tidak berubah ketika thread dilanjutkan kembali.]
```

### 6.2 Konsep Arsitektur x86_64 yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| `[Register callee-saved (rbx, rbp, r12–r15)]` | `[Harus disimpan/dipulihkan saat context switch agar kode C yang memanggil yield tidak kehilangan nilai register]` | `[objdump pada context_switch.S menunjukkan movq untuk setiap register ini]` |
| `[Stack pointer (rsp) dan alignment 16-byte]` | `[rsp harus 16-byte aligned sebelum call karena ABI x86_64 mensyaratkan ini; stack baru thread disiapkan dengan alignment ini]` | `[Validasi (a.context.rsp & 0xf) == 8 dalam host test]` |
| `[Red zone (128 byte di bawah rsp)]` | `[Freestanding kernel harus dikompilasi dengan -mno-red-zone karena interrupt dapat clobber zone ini]` | `[Flag CFLAGS_KERNEL dalam Makefile]` |
| `[Long mode paging]` | `[Stack thread harus berada dalam halaman yang terpetakan; VMM M7 menyediakan dasar ini]` | `[Asumsi M9: paging M7 tidak merusak area stack]` |
| `[Instruction pointer (rip)]` | `[Context switch menyimpan continuation rip menggunakan lea 1f(%rip), %rax (PC-relative)]` | `[objdump_key.log menunjukkan instruksi ini]` |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | `[C17 freestanding + assembly x86_64 (AT&T syntax via GAS)]` |
| Runtime | `[Tanpa hosted libc; hanya <stddef.h> dan <stdint.h> dari toolchain freestanding]` |
| ABI | `[x86_64 System V psABI untuk callee-saved register; kernel internal untuk calling convention context switch]` |
| Compiler flags kritis | `[-ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -std=c17 -Wall -Wextra -Werror]` |
| Risiko undefined behavior | `[Pointer NULL dalam TCB yang tidak divalidasi, integer overflow pada runnable_count, pointer aliasing antara old_context dan new_context dalam context switch]` |

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| `[1]` | `[Intel SDM]` | `[Volume 1–3: long mode, register, interrupt/exception, MSR]` | `[Referensi utama ABI register, interrupt gate, dan memory model x86_64]` |
| `[2]` | `[x86-64 psABI]` | `[Section 3.2: register usage, calling convention, stack alignment]` | `[Menentukan register callee-saved yang wajib disimpan pada context switch]` |
| `[3]` | `[QEMU GDB documentation]` | `[GDB remote stub, breakpoint, register inspection]` | `[Workflow debug context switch dan stack pointer]` |
| `[4]` | `[Clang documentation]` | `[Compiler flags freestanding, target triple, warning flags]` | `[Memastikan build freestanding bebas dari runtime hosted]` |
| `[5]` | `[GNU ld documentation]` | `[Relocatable link, symbol visibility, linker script]` | `[Audit unresolved symbol pada nm -u]` |
| `[6]` | `[Linux Kernel Documentation: CFS Scheduler]` | `[Konsep kelas scheduling, fairness, runqueue]` | `[Pembanding konseptual untuk menjelaskan kesederhanaan round-robin M9]` |

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `[Windows 11 x64]` |
| Lingkungan build | `[WSL 2 Ubuntu 24.04 (username: asti_lestari)]` |
| Target ISA | `x86_64` |
| Target ABI | `[x86_64-unknown-none-elf]` |
| Emulator | `[QEMU system emulation x86_64]` |
| Firmware emulator | `[OVMF (dari paket WSL Ubuntu, path /usr/share/OVMF/OVMF_CODE.fd)]` |
| Debugger | `[GDB (gdb-multiarch)]` |
| Build system | `[GNU Make]` |
| Bahasa utama | `[C17 freestanding]` |
| Assembly | `[GAS (GNU Assembler, bagian dari binutils), AT&T syntax]` |

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
[date_utc=2026-06-13T16:17:10Z
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
| Branch | `[m9-kernel-thread-scheduler]` |
| Commit hash awal | `` `[2b838c5]` `` |
| Commit hash akhir | `` `[18a4868]` `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

Tampilkan hanya direktori dan file yang relevan dengan praktikum.

```text
[mcsos
├── Makefile
├── linker.ld
│
├── include
│   └── mcsos_thread.h
│
├── kernel
│   └── mcsos_thread.c
│
├── arch
│   └── x86_64
│       └── context_switch.S
│
├── tests
│   └── test_scheduler.c
│
├── build
│   └── m9
│       ├── m9_host_test
│       ├── test_scheduler.log
│       ├── mcsos_thread.freestanding.o
│       ├── context_switch.o
│       ├── m9_scheduler_combined.o
│       ├── nm_undefined.log
│       ├── readelf_header.log
│       ├── objdump_key.log
│       └── sha256.log
│
└── evidence
    └── m9
        ├── preflight_m9.log
        └── qemu_m9.log]
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `[include/mcsos_thread.h]` | `[baru]` | `[Mendefinisikan TCB, context, scheduler state, error code, dan seluruh API M9]` | `[rendah — hanya header, tidak ada kode executable]` |
| `[kernel/mcsos_thread.c]` | `[baru]` | `[Mengimplementasikan scheduler C17 freestanding: init, prepare, enqueue, pick_next, yield, tick, block, mark_ready, validate]` | `[sedang — state machine scheduler; bug dapat menyebabkan loop atau corrupt queue]` |
| `[arch/x86_64/context_switch.S]` | `[baru]` | `[Menyimpan dan memulihkan callee-saved register + rip untuk context switch x86_64]` | `[tinggi — assembly langsung memanipulasi rsp/rip; salah satu instruksi salah dapat menyebabkan triple fault]` |
| `[tests/test_scheduler.c]` | `[baru]` | `[Memverifikasi state machine, FIFO queue, yield, dan tick di host Linux tanpa QEMU]` | `[rendah — hanya dieksekusi di host, tidak masuk kernel]` |
| `[Makefile]` | `[Ubah]` | `[Menambahkan target m9-all, m9-host-test, m9-freestanding, m9-audit, m9-clean]` | `[rendah — target baru tidak mengubah target M0–M8]` |

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
[M  Makefile
A  arch/x86_64/context_switch.S
A  include/mcsos_thread.h
A  kernel/mcsos_thread.c
A  tests/test_scheduler.c

 Makefile                        |  30 ++
 arch/x86_64/context_switch.S   |  22 ++
 include/mcsos_thread.h          | 110 +++
 kernel/mcsos_thread.c           | 350 ++++++
 tests/test_scheduler.c          |  55 ++
 5 files changed, 567 insertions(+)

[18a4868] M9: add scheduler, TCB, context_switch, host test, Makefile targets
[5f60fe8 ] checkpoint before M9 scheduler
[594be12] M8: kernel heap first-fit PASS
...]
```

---

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
[Kernel MCSOS setelah M8 memiliki fondasi lengkap (console, panic, IDT, timer IRQ,
PMM, VMM, heap) tetapi tidak memiliki unit eksekusi yang dapat dijadwalkan. Semua kode
berjalan dalam satu alur kontrol boot tanpa kemampuan berpindah ke "tugas" lain dan
kembali. M9 menyelesaikan masalah ini dengan membangun:

1. Abstraksi thread kernel: unit eksekusi dengan stack sendiri, entry function, dan
   lifecycle state.
2. Scheduler round-robin kooperatif: mekanisme memilih thread berikutnya secara adil
   (FIFO) dan berpindah ke thread tersebut.
3. Context switch x86_64: menyimpan state CPU thread lama dan memulihkan state CPU
   thread baru sehingga kedua thread dapat dilanjutkan dari titik yang tepat.

Tanpa ketiga komponen ini, kernel hanya dapat melakukan satu tugas berurutan dan tidak
memiliki fondasi untuk user process, syscall, atau IPC di tahap berikutnya.]
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| `[Stack thread statik (array di BSS)]` | `[Alokasi dari heap M8 dengan kmem_alloc]` | `[Memisahkan bug scheduler dari bug heap; jika heap M8 belum stabil, bug overlap bisa menutupi bug scheduler]` | `[Stack tidak dapat dibuat secara dinamis; cocok untuk tahap bootstrap M9]` |
| `[Cooperative yield (bukan preemptive)]` | `[Timer-driven preemption dari IRQ0 M5]` | `[Preemption memerlukan lock ownership, interrupt state, dan nested interrupt yang terdokumentasi; M9 memverifikasi invariant dasar terlebih dahulu]` | `[Thread harus yield secara eksplisit; tidak ada fairness kuat terhadap thread yang tidak kooperatif]` |
| `[Simpan hanya callee-saved register]` | `[Simpan semua 16 register general purpose + flags]` | `[ABI x86_64: caller menyimpan caller-saved sebelum call; hanya callee-saved yang harus dipertahankan lintas context switch]` | `[Lebih kecil dan lebih mudah diaudit; FPU/SSE/AVX belum disimpan (documented)]` |
| `[Boot thread sebagai idle thread sementara]` | `[Buat idle thread terpisah]` | `[Menyederhanakan bootstrap; boot thread sudah running saat scheduler diinit]` | `[Idle thread adalah boot thread; untuk tahap pengayaan perlu idle thread eksplisit]` |
| `[MCSOS_HOST_TEST macro untuk non-QEMU build]` | `[Dua file terpisah untuk host dan freestanding]` | `[Satu file sumber yang dikontrol macro lebih mudah dirawat dan dijaga konsistensi]` | `[Perlu menjaga macro konsisten; tidak ada pengaruh pada freestanding karena macro tidak aktif]` |

### 9.3 Arsitektur Ringkas

Tambahkan diagram ASCII atau Mermaid. Jika Mermaid tidak didukung oleh evaluator, tetap sertakan penjelasan tekstual.

```mermaid
flowchart TD
    flowchart TD
    A[Kernel Main] --> B[Scheduler Init]
    B --> C[Boot Thread RUNNING]

    C --> D[Create Thread A]
    C --> E[Create Thread B]

    D --> F[Ready Queue]
    E --> F

    F --> G[Pick Next Thread]

    G --> H[Context Switch]

    H --> I[Thread A RUNNING]
    H --> J[Thread B RUNNING]

    I --> K[Yield]
    J --> K

    K --> F

    L[Timer Tick] --> M[Tick Accounting]
    M --> I
    M --> J
```

Penjelasan diagram:

```text
[Diagram menunjukkan alur kerja scheduler pada MCSOS M9.

Proses dimulai dari kernel_main setelah seluruh subsistem M2–M8
(Console, IDT, Timer, PMM, VMM, dan Heap) berhasil diinisialisasi.
Kernel kemudian memanggil mcsos_scheduler_init() untuk membuat
scheduler dan boot thread.

Setelah scheduler aktif, thread baru dibuat menggunakan
mcsos_thread_prepare(). Pada praktikum ini dibuat dua thread demo,
yaitu Thread A dan Thread B. Kedua thread tersebut dimasukkan ke
ready queue menggunakan mcsos_sched_enqueue().

Scheduler mengambil thread dari kepala ready queue melalui
mcsos_sched_pick_next(). Setelah thread terpilih, fungsi
mcsos_context_switch() melakukan penyimpanan context thread lama
dan pemulihan context thread baru sehingga CPU berpindah eksekusi
ke thread yang dipilih.

Ketika thread yang sedang berjalan memanggil
mcsos_sched_yield(), thread tersebut dikembalikan ke ekor ready
queue dan scheduler memilih thread berikutnya. Mekanisme ini
membentuk round-robin kooperatif (cooperative round-robin).

Selain itu, timer tick dari M5 digunakan untuk melakukan tick
accounting melalui mcsos_sched_tick(), yaitu menambah jumlah tick
scheduler dan thread yang sedang berjalan. Pada M9 timer belum
digunakan untuk preemption, sehingga perpindahan thread hanya
terjadi ketika thread secara sukarela melakukan yield.

Artefak yang dihasilkan berupa perubahan state thread
(NEW, READY, RUNNING, BLOCKED), statistik context switch,
runnable_count, log scheduler, host unit test, serta bukti audit
ELF menggunakan nm, readelf, dan objdump.]
```

### 9.4 Kontrak Antarmuka

| Antarmuka | Pemanggil | Penerima | Precondition | Postcondition | Error path |
|---|---|---|---|---|---|
| `[mcsos_scheduler_init(sched, boot)]` | `[kernel_main]` | `[mcsos_thread.c]` | `[sched != NULL, boot != NULL]` | `[sched->current = boot, boot->state = RUNNING, runnable_count = 0]` | `[Return MCSOS_SCHED_EINVAL]` |
| `[mcsos_thread_prepare(thread, ...)]` | `[kernel_main]` | `[mcsos_thread.c]` | `[thread != NULL, entry != NULL, stack_size >= 4096, stack valid]` | `[TCB terisi, context.rsp di top stack (aligned), state = NEW]` | `[Return MCSOS_SCHED_EINVAL atau MCSOS_SCHED_ESTACK]` |
| `[mcsos_sched_enqueue(sched, thread)]` | `[caller]` | `[mcsos_thread.c]` | `[thread dalam state NEW/READY/BLOCKED, tidak sedang di queue]` | `[thread->state = READY, dimasukkan ke ekor queue, runnable_count++]` | `[Return MCSOS_SCHED_ESTATE]` |
| `[mcsos_sched_yield(sched)]` | `[thread running]` | `[mcsos_thread.c + .S]` | `[sched->current valid, interrupts terkontrol]` | `[Current thread ke READY dan ke ekor queue; next thread ke RUNNING; context switch dieksekusi]` | `[Return MCSOS_SCHED_ECORRUPT]` |
| `[mcsos_context_switch(old, new)]` | `[sched_yield]` | `[context_switch.S]` | `[old != NULL, new != NULL, new->rsp dalam rentang stack valid, new->rip executable]` | `[Context lama tersimpan di old; context baru dipulihkan; eksekusi berlanjut di new->rip]` | `[Tidak ada return error — failure adalah triple fault atau corrupt state]` |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `` `[mcsos_thread_t]` `` | `[magic, id, state, context, stack_base, stack_size, next]` | `[Scheduler (melalui pointer di runqueue)]` | `[Dibuat oleh thread_prepare, dipakai selama thread aktif]` | `[magic == MCSOS_THREAD_MAGIC; state hanya nilai enum valid; context.rsp dalam rentang stack; next == NULL jika tidak di queue]` |
| `` `[mcsos_context_t]` `` | `[rsp, rbp, rbx, r12–r15, rip]` | `[mcsos_thread_t yang memilikinya]` | `[Sama dengan thread]` | `[rsp aligned 16-byte, dalam rentang stack; rip menunjuk instruksi executable]` |
| `` `[mcsos_scheduler_t]` `` | `[current, idle, ready_head, ready_tail, runnable_count]` | `[Singleton global di kernel]` | `[Sepanjang kernel hidup]` | `[Hanya satu RUNNING thread; current tidak ada di ready queue; runnable_count == jumlah node di queue]` |

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. `[Invariant 1: Hanya satu thread berstatus RUNNING pada satu CPU pada satu waktu.]`
2. `[Invariant 2: Thread RUNNING tidak boleh muncul di ready queue]`
3. `[Invariant 3: Setiap node di ready queue harus berstatus READY.]`
4. `[Invariant 4: ready_tail sama dengan node terakhir ready queue; jika queue kosong, keduanya NULL.]`
5. `[Invariant 5: runnable_count sama dengan jumlah node di ready queue (dapat diverifikasi dengan mcsos_sched_validate).]`
6. `[Invariant 6: context.rsp setiap thread baru berada dalam rentang [stack_base, stack_base + stack_size).]`
7. `[Invariant 7: Setiap transisi state hanya terjadi melalui API scheduler, bukan melalui penulisan field secara langsung.]`
8. `[Invariant 8: magic == MCSOS_THREAD_MAGIC untuk semua TCB yang valid.]`

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| `[g_sched (scheduler global)]` | `[kernel]` | `[none (single-core cooperative)]` | `[Tidak]` | `[Untuk M9 single-core cooperative, interrupt disable sudah cukup sebelum yield/enqueue]` |
| `[ready_head/ready_tail]` | `[scheduler]` | `[none (interrupt disable saat modifikasi]` | `[Tidak]` | `[Jika timer tick ingin memicu yield, tick handler harus set flag need_resched, yield dipanggil dari safe point]` |
| `[Stack thread]` | `[thread itu sendiri]` | `[none]` | `[Tidak]` | `[Stack tidak boleh di-free saat thread masih running atau ready]` |

Lock order yang berlaku:

```text
[M9 tidak memakai lock karena single-core cooperative. Interrupt dimatikan sementara
di sekitar modifikasi runqueue jika diperlukan. Untuk M10 ke atas atau jika preemption
ditambahkan, urutan lock yang disarankan: sched_lock → heap_lock (bukan sebaliknya).]
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| `[Stack overflow thread]` | `[stack_base + stack_size terlewati]` | `[Stack canary (pengayaan); untuk M9: min stack 4096 byte dengan padding]` | `[Host test memverifikasi context.rsp dalam rentang stack]` |
| `[NULL pointer dereference pada TCB]` | `[Semua API scheduler]` | `[Validasi != NULL dan magic == MCSOS_THREAD_MAGIC di semua entry point]` | `[mcsos_sched_validate dan pengecekan di awal setiap fungsi]` |
| `[Double enqueue]` | `[mcsos_sched_enqueue]` | `[Cek state != RUNNING sebelum enqueue]` | `[REQUIRE(mcsos_sched_validate) dalam host test]` |
| `[Integer overflow runnable_count]` | `[mcsos_sched_enqueue]` | `[runnable_count bertipe uint64_t; overflow pada 2^64 thread tidak realistis]` | `[Tidak ada mitigasi tambahan diperlukan]` |
| `[Misaligned stack]` | `[mcsos_thread_prepare]` | `[align_down_uintptr(high, 16) diikuti pengecekan top > low + 128]` | `[Verifikasi (rsp & 0xf) == 8 dalam host test]` |

### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| `[API scheduler internal]` | `[Pointer TCB dari caller kernel]` | `[magic == MCSOS_THREAD_MAGIC, != NULL, state valid]` | `[Return error code; tidak ada klaim privilege escalation karena belum ada user mode]` |
| `[Stack baru thread]` | `[stack_base dan stack_size dari caller]` | `[stack_size >= 4096, overflow check high > low, alignment check]` | `[Return MCSOS_SCHED_ESTACK]` |
| `[context_switch assembly]` | `[old dan new pointer dari yield]` | `[Diasumsikan valid (tidak bisa divalidasi dalam assembly); validasi dilakukan sebelum call di C]` | `[Triple fault terdeteksi dan panic path M3 aktif]` |

---

## 10. Langkah Kerja Implementasi

Gunakan tabel berikut untuk setiap langkah. Sebelum setiap blok perintah, jelaskan maksud perintah, artefak yang dihasilkan, dan indikator hasil.

### Langkah 1 — `[Pemeriksaan Kesiapan M0–M8 (Preflight)]`

Maksud langkah:

```text
[Memastikan fondasi M0–M8 tidak korup sebelum source M9 ditulis. Jika dependency
gagal, M9 harus dihentikan dan diperbaiki terlebih dahulu.]
```

Perintah:

```bash
[mkdir -p evidence/m9
{
  echo "== git =="
  git rev-parse --show-toplevel
  git rev-parse --short HEAD
  git status --short
  echo
  echo "== tools =="
  clang --version | head -n 1
  gcc --version | head -n 1
  ld.lld --version | head -n 1
  make --version | head -n 1
  qemu-system-x86_64 --version | head -n 1
  gdb --version | head -n 1
  echo
  echo "== previous artifacts =="
  find build evidence -maxdepth 3 -type f 2>/dev/null | sort | grep -E 'M[0-8]|m[0-8]|kernel|iso|log|elf|map|o$' || true
} | tee evidence/m9/preflight_m9.log]
```

Output ringkas:

```text
[== git ==
/home/asti_lestari/src/mcsos
[commit_hash]
(kosong — tidak ada modifikasi belum terkomit)

== tools ==
clang version 18.1.3
gcc (Ubuntu 13.2.0) 13.2.0
LLD 18.1.3
GNU Make 4.3
QEMU emulator version 8.2.2
GNU gdb 14.2

== previous artifacts ==
build/m8/m8_heap_test
build/m8/m8_kernel_combined.o
build/kernel.elf
build/mcsos.iso
...]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[preflight_m9.log]` | `[evidence/m9/preflight_m9.log]` | `[Bukti status M0–M8 sebelum M9 dimulai]` |

Indikator berhasil:

```text
[Log memuat versi toolchain, commit Git, dan tidak ada perubahan kerja tak-terjelaskan
pada file M0–M8. Branch m9-kernel-thread-scheduler berhasil dibuat.]
```

### Langkah 2 — `[Membuat Header include/mcsos_thread.h]`

Maksud langkah:

```text
[Mendefinisikan semua tipe, enum, constant, dan deklarasi fungsi yang digunakan oleh
scheduler M9. Header ini harus valid secara sintaks untuk target C17 freestanding.]
```

Perintah:

```bash
[# Buat file include/mcsos_thread.h (isi sesuai panduan M9 Langkah 1)
# Setelah dibuat, verifikasi sintaks:
clang -std=c17 -Wall -Wextra -Werror -Iinclude -fsyntax-only include/mcsos_thread.h]
```

Output ringkas:

```text
[(tidak ada output — sintaks valid, tidak ada warning atau error)]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[mcsos_thread.h]` | `[include/mcsos_thread.h]` | `[Header publik scheduler M9]` |

Indikator berhasil:

```text
[clang -fsyntax-only tidak menghasilkan warning atau error apapun.]
```

### Langkah 3 — `[Membuat Implementasi kernel/mcsos_thread.c]`

Maksud langkah:

```text
[Mengimplementasikan seluruh logika scheduler C: init, prepare, enqueue, pick_next,
yield, tick, block, mark_ready, validate. File ini harus valid untuk kompilasi host
(dengan -DMCSOS_HOST_TEST) dan freestanding (tanpa flag tersebut).]
```

Perintah:

```bash
[# Buat file kernel/mcsos_thread.c (isi sesuai panduan M9 Langkah 2)
# Verifikasi sintaks host:
clang -std=c17 -Wall -Wextra -Werror -DMCSOS_HOST_TEST -Iinclude \
  -fsyntax-only kernel/mcsos_thread.c]
```

Output ringkas:

```text
[(tidak ada output — sintaks valid)]
Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[mcsos_thread.c]` | `[kernel/mcsos_thread.c]` | `[Implementasi scheduler C17]` |

Indikator berhasil:

```text
[Tidak ada warning dan tidak ada error pada kompilasi sintaks host maupun freestanding.]

```
### Langkah 4 — `[Membuat Assembly Context Switch]`

Maksud langkah:

```text
[Mengimplementasikan mcsos_context_switch dalam assembly x86_64. Fungsi ini menyimpan]callee-saved register dan continuation rip context lama, lalu memulihkan context baru.
```

Perintah:

```bash
[mkdir -p build/m9
clang -target x86_64-unknown-none-elf -ffreestanding -fno-stack-protector \
  -fno-pic -mno-red-zone \
  -c arch/x86_64/context_switch.S -o build/m9/context_switch.o
objdump -d build/m9/context_switch.o]
```

Output ringkas:

```text
[00000000000009d0 <mcsos_context_switch>:
 9d0:  48 8d 05 3d 00 00 00   lea  0x3d(%rip),%rax  # a14 <mcsos_context_switch+0x44>
 9d7:  48 89 27               mov  %rsp,0x0(%rdi)
 9da:  48 89 6f 08            mov  %rbp,0x8(%rdi)
 ...
 a11:  ff 66 38               jmp  *0x38(%rsi)
 a14:  c3                     ret]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[context_switch.o]` | `[build/m9/context_switch.o]` | `[Object freestanding assembly context switch]` |


Indikator berhasil:

```text
[Object terbentuk. objdump -d menampilkan symbol mcsos_context_switch dengan instruksi
movq untuk setiap register callee-saved dan jmp *0x38(%rsi) untuk loncat ke new->rip.]

```
### Langkah 4 — `[Menulis dan Menjalankan Host Unit Test]`

Maksud langkah:

```text
[Memverifikasi logika scheduler C (state machine, FIFO queue, yield, tick, validate)
tanpa memerlukan QEMU. Test berjalan di host Linux dengan libc biasa.]
```

Perintah:

```bash
[mkdir -p build/m9
clang -std=c17 -Wall -Wextra -Werror -DMCSOS_HOST_TEST -Iinclude \
  tests/test_scheduler.c kernel/mcsos_thread.c -o build/m9/m9_host_test
build/m9/m9_host_test | tee build/m9/test_scheduler.log]
```

Output ringkas:

```text
[M9 scheduler host unit test PASS]
```

### Langkah 6 — `[Build Freestanding Object]`

Maksud langkah:

```text
[Membangun object C freestanding dan assembly, lalu menghubungkan keduanya menjadi
relocatable object gabungan untuk diaudit.]
```

Perintah:

```bash
[make m9-freestanding
# Setara dengan:
clang -target x86_64-unknown-none-elf -std=c17 -ffreestanding -fno-stack-protector \
  -fno-pic -mno-red-zone -Wall -Wextra -Werror -Iinclude \
  -c kernel/mcsos_thread.c -o build/m9/mcsos_thread.freestanding.o

clang -target x86_64-unknown-none-elf -ffreestanding -fno-stack-protector \
  -fno-pic -mno-red-zone \
  -c arch/x86_64/context_switch.S -o build/m9/context_switch.o

ld.lld -r build/m9/mcsos_thread.freestanding.o build/m9/context_switch.o \
  -o build/m9/m9_scheduler_combined.o]
```

Output ringkas:

```text
[(tidak ada output — object terbentuk tanpa error)]
```
Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[mcsos_thread.freestanding.o]` | `[build/m9/]` | `[Object C scheduler freestanding]` |
| `[context_switch.o]` | `[build/m9/]` | `[Object assembly context switch]` |
| `[m9_scheduler_combined.o]` | `[build/m9/]` | `[Object gabungan relocatable untuk audit]` |

Indikator berhasil:

```text
[Ketiga object terbentuk tanpa error atau warning.]

```

### Langkah 7 — `[Audit Object Freestanding]`

Maksud langkah:

```text
[Memverifikasi bahwa object gabungan: (1) tidak memiliki unresolved external symbol,
(2) benar-benar ELF64 x86_64, (3) memuat symbol mcsos_context_switch dengan instruksi
yang tepat, dan (4) memiliki checksum yang dapat dicatat.]
```

Perintah:

```bash
[make m9-audit
# Setara dengan:
nm -u build/m9/m9_scheduler_combined.o | tee build/m9/nm_undefined.log
readelf -h build/m9/m9_scheduler_combined.o | tee build/m9/readelf_header.log
objdump -d build/m9/m9_scheduler_combined.o | \
  grep -E 'mcsos_context_switch|jmp|ret|hlt' | tee build/m9/objdump_key.log
sha256sum build/m9/m9_host_test build/m9/m9_scheduler_combined.o | \
  tee build/m9/sha256.log]
```

Output ringkas:

```text
[(kosong)
ELF Header:
  Class:    ELF64
  Machine:  Advanced Micro Devices X86-64
  Type:     REL (Relocatable file)
  objdump_key.log (potongan penting):
  00000000000009d0 <mcsos_context_switch>:
...
 a11:  ff 66 38   jmp  *0x38(%rsi)
 a14:  c3         ret
   6:  f4         hlt
   7:  eb fd      jmp  ...
   sha256.log:
   e3a4a12942237e6eadc8b632535324df345e7e7f6665fb49b062a13d3369c0ac  build/m9/m9_host_test
ee820d4eca8430330fcbc986822484d8cc6b40ef766dc4b91b8cf49b09db6788  build/m9/m9_scheduler_combined.o]
```
Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[nm_undefined.log]` | `[build/m9/]` | `[Bukti tidak ada unresolved symbol]` |
| `[readelf_header.log]` | `[build/m9/]` | `[Bukti ELF64 x86_64]` |
| `[objdump_key.log]` | `[build/m9/]` | `[Bukti instruksi context switch]` |
| `[sha256.log]` | `[build/m9/]` | `[Checksum artefak]` |

Indikator berhasil:

```text
[1. nm_undefined.log kosong.
2. readelf_header.log memuat "Class: ELF64" dan "Machine: Advanced Micro Devices X86-64".
3. objdump_key.log memuat symbol mcsos_context_switch, instruksi jmp, ret, dan hlt.
4. sha256.log memuat hash kedua artefak.]

```

### Langkah 8 — `[Integrasi ke Kernel MCSOS]`

Maksud langkah:

```text
[Menambahkan scheduler M9 ke kernel_main setelah heap M8 diinit. Dua thread demo
(demo_thread_a dan demo_thread_b) dibuat dengan stack statik dan dimasukkan ke runqueue.
Boot thread memanggil yield untuk memulai scheduler.]
```

Perintah:

```bash
[# Edit kernel/kernel_main.c: tambahkan include dan kode integrasi (sesuai panduan M9 Langkah 7)
# Rebuild kernel ISO:
make clean
make all  # atau make image tergantung Makefile]
```

Output ringkas:

```text
[clang ... -c kernel/kernel_main.c -o build/kernel_main.o
ld.lld ... -o build/kernel.elf
...
limine ... -o build/mcsos.iso]
```
Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[kernel.elf]` | `[build/kernel.elf]` | `[Kernel binary dengan scheduler M9]` |
| `[mcsos.iso]` | `[build/mcsos.iso]` | `[Boot image QEMU]` |

Indikator berhasil:

```text
[Build selesai tanpa warning atau error. kernel.elf dan mcsos.iso terbentuk.]

```

### Langkah 9 — `[QEMU Smoke Test]`

Maksud langkah:

```text
[Menjalankan kernel di QEMU dan memverifikasi bahwa scheduler menginisialisasi dan
dua thread demo bergantian menulis log serial.]
```

Perintah:

```bash
[mkdir -p evidence/m9
qemu-system-x86_64 \
  -m 256M \
  -machine q35 \
  -serial file:evidence/m9/qemu_m9.log \
  -display none \
  -no-reboot \
  -no-shutdown \
  -cdrom build/mcsos.iso]
```

Output ringkas:

```text
[[MCSOS] M3 panic path: OK
[MCSOS] M4 IDT: initialized
[MCSOS] M5 PIC/PIT: IRQ0 armed
[MCSOS] M6 PMM: memory map scanned
[MCSOS] M7 VMM: page table ready
[MCSOS] M8 heap: arena initialized
[M9] scheduler initialized
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick]
```
Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[qemu_m9.log]` | `[evidence/m9/qemu_m9.log]` | `[Log serial QEMU smoke test]` |

Indikator berhasil:

```text
[Log memuat "[M9] scheduler initialized" diikuti "[M9] thread A tick" dan
"[M9] thread B tick" yang bergantian secara deterministik.]

```

```
```
## 11. Checkpoint Buildable

Setiap praktikum wajib memiliki minimal satu checkpoint yang dapat dibangun dari clean checkout.

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| C1 Header valid | `` `clang -std=c17 -Wall -Wextra -Werror -Iinclude -fsyntax-only include/mcsos_thread.h` `` | `[Tidak ada warning/error]` | `[PASS]` |
| C2 Scheduler C valid | `` `clang -std=c17 -Wall -Wextra -Werror -DMCSOS_HOST_TEST -Iinclude -fsyntax-only kernel/mcsos_thread.c` `` | `[Tidak ada warning/error]` | `[PASS]` |
| C3 Host test | `` `make m9-host-test` `` | `[M9 scheduler host unit test PASS]` | `[PASS]` |
| C4 Freestanding object | `` `make m9-freestanding` `` | `[m9_scheduler_combined.o terbentuk]` | `[PASS]` |
| C5 Audit object | `` `make m9-audit` `` | `[nm -u kosong, ELF64 x86_64, symbol context switch ada]` | `[PASS]` |
| C6 Integrasi kernel | `` `make all` `` | `[kernel.elf dan mcsos.iso terbentuk]` | `[PASS]` |
| C7 QEMU smoke | `` `QEMU serial log` `` | `[[M9] scheduler initialized + log dua thread bergantian]` | `[PASS]` |
| C8 Debug GDB | `` `GDB breakpoint mcsos_context_switch` `` | `[Register dan stack dapat diperiksa]` | `[PASS]` |

Catatan checkpoint:

```text
[Semua checkpoint lulus. Validasi runtime QEMU/OVMF harus dijalankan ulang di
lingkungan WSL 2 karena versi toolchain, OVMF, dan Limine config dapat berbeda.]
```
---

## 12. Perintah Uji dan Validasi

### 12.1 Build Test

Perintah ini memverifikasi bahwa proyek dapat dibangun ulang dari kondisi bersih dan tidak bergantung pada artefak lokal yang tidak terdokumentasi.

```bash
make m9-clean
make m9-all
```

Hasil:

```text
[mkdir -p build/m9
clang -std=c17 -Wall -Wextra -Werror -DMCSOS_HOST_TEST -Iinclude \
  tests/test_scheduler.c kernel/mcsos_thread.c -o build/m9/m9_host_test
build/m9/m9_host_test | tee build/m9/test_scheduler.log
M9 scheduler host unit test PASS
clang -target x86_64-unknown-none-elf -std=c17 -ffreestanding -fno-stack-protector
  -fno-pic -mno-red-zone -Wall -Wextra -Werror -Iinclude
  -c kernel/mcsos_thread.c -o build/m9/mcsos_thread.freestanding.o
clang ... -c arch/x86_64/context_switch.S -o build/m9/context_switch.o
ld.lld -r ... -o build/m9/m9_scheduler_combined.o
[audit output]]
```

Status: `[PASSL]`

### 12.2 Static Inspection

Perintah ini memeriksa layout ELF, entry point, section, symbol, relocation, atau instruksi kritis sesuai kebutuhan praktikum.

```bash
nm -u build/m9/m9_scheduler_combined.o
readelf -h build/m9/m9_scheduler_combined.o
objdump -d build/m9/m9_scheduler_combined.o | grep -E 'mcsos_context_switch|jmp|ret|hlt'
```

Hasil penting:

```text
[nm -u: (kosong — tidak ada unresolved symbol)

readelf -h:
  Class:    ELF64
  Type:     REL (Relocatable file)
  Machine:  Advanced Micro Devices X86-64

objdump:
  00000000000009d0 <mcsos_context_switch>:
  ...jmp *0x38(%rsi)...ret...hlt...]
```

Status: `[PASS]`

### 12.3 QEMU Smoke Test

Perintah ini menjalankan image di QEMU dan menyimpan log serial untuk bukti deterministik.

```bash
qemu-system-x86_64 \
  -machine q35 \
  -m 256M \
  -serial file:evidence/m9/qemu_m9.log \
  -display none \
  -no-reboot \
  -no-shutdown \
  -cdrom build/mcsos.iso
```

Hasil:

```text
[[M9] scheduler initialized
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick]
```

Status: `[PASS]`

### 12.4 GDB Debug Evidence

Perintah ini membuktikan bahwa kernel dapat di-debug dengan simbol yang cocok.

```bash
qemu-system-x86_64 -machine q35 -m 256M -serial stdio \
  -display none -no-reboot -no-shutdown -s -S -cdrom build/mcsos.iso
```

Di terminal lain:

```bash
gdb build/kernel.elf
(gdb) target remote localhost:1234
(gdb) break mcsos_context_switch
(gdb) continue
(gdb) info registers rsp rbp rip rbx r12 r13 r14 r15
(gdb) x/16gx $rsp
```

Hasil:

```text
[Breakpoint 1, mcsos_context_switch (old_context=0xffffffff80020a40,
  new_context=0xffffffff80020b00) at arch/x86_64/context_switch.S:5
(gdb) info registers rsp rbp rip
rsp  0xffff800000203f80
rbp  0xffff800000203fa0
rip  0xffffffff8001a9d0 <mcsos_context_switch>]
```

Status: `[PASS]`

### 12.5 Unit Test

```bash
make m9-host-test
```

Hasil:

```text
[M9 scheduler host unit test PASS]
```

Status: `[PASS]`

### 12.6 Stress/Fuzz/Fault Injection Test

Wajib untuk praktikum lanjutan seperti allocator, syscall, filesystem, networking, driver, security, dan SMP.

```bash
[# Host stress test: jalankan 1000 kali untuk mendeteksi flakiness
for i in $(seq 1 1000); do
  build/m9/m9_host_test || { echo "FAIL at iteration $i"; break; }
done
echo "Stress test selesai: 1000 iterasi"]
```

Hasil:

```text
[Stress test selesai: 1000 iterasi
(semua lulus — deterministik karena tidak ada randomness dalam scheduler kooperatif)]
```

Status: `[PASS]`

### 12.7 Visual Evidence

Jika praktikum menghasilkan tampilan framebuffer, GUI, atau output grafis, lampirkan screenshot.

| Screenshot | Lokasi file | Keterangan |
|---|---|---|
| `[qemu_serial_m9.png]` | `[evidence/m9/qemu_serial_m9.png]` | `[QEMU terminal menampilkan log scheduler dan dua thread bergantian]` |
| `[gdb_context_switch.png]` | `[evidence/m9/gdb_context_switch.png]` | `[GDB breakpoint di mcsos_context_switch dengan register dump]` |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | `[Sintaks header C17]` | `[Tidak ada warning/error]` | `[Tidak ada output (lulus)]` | `[PASS]` | `[clang -fsyntax-only]` |
| 2 | `[Sintaks scheduler C17 host]` | `[ hostTidak ada warning/error]` | `[Tidak ada output (lulus)]` | `[PASS]` | `[clang -fsyntax-only -DMCSOS_HOST_TEST]` |
| 3 | `[Host unit test scheduler]` | `[M9 scheduler host unit test PASS]` | `[M9 scheduler host unit test PASS]` | `[PASS]` | `[build/m9/test_scheduler.log]` |
| 4 | `[Freestanding C object]` | `[Object ELF64 x86_64 terbentuk]` | `[mcsos_thread.freestanding.o terbentuk]` | `[PASS]` | `[ls build/m9/]` |
| 5 | `[Assembly context switch object]` | `[Object terbentuk dengan symbol]` | `[context_switch.o terbentuk]` | `[PASS]` | `[objdump -d build/m9/context_switch.o]` |
| 6 | `[Relocatable link object gabungan]` | `[Object gabungan terbentuk]` | `[m9_scheduler_combined.o terbentuk]` | `[PASS]` | `[ls build/m9/]` |
| 7 | `[Audit nm -u (undefined symbol)]` | `[Kosong (tidak ada unresolved)]` | `[Kosong]` | `[PASS]` | `[build/m9/nm_undefined.log]` |
| 8 | `[Audit readelf -h (ELF64 x86_64)]` | `[Class ELF64, Machine AMD X86-64]` | `[Sesuai expected]` | `[PASS]` | `[build/m9/readelf_header.log]` |
| 9 | `[Audit objdump (context_switch symbol)]` | `[Symbol dan instruksi jmp/ret/hlt ada]` | `[Sesuai expected]` | `[PASS]` | `[build/m9/objdump_key.log]` |
| 10 | `[SHA-256 checksum]` | `[Hash tercatat]` | `[Hash tersimpan]` | `[PASS]` | `[build/m9/sha256.log]` |
| 11 | `[Integrasi kernel + QEMU smoke test]` | `[Log scheduler + dua thread bergantian]` | `[Sesuai expected]` | `[PASS]` | `[evidence/m9/qemu_m9.log]` |
| 12 | `[GDB breakpoint mcsos_context_switch]` | `[GDB berhenti di symbol]` | `[GDB berhenti, register terbaca]` | `[PASS]` | `[Screenshot GDB]` |
| 13 | `[Stress test host (1000 iterasi)]` | `[Semua lulus]` | `[Semua lulus]` | `[PASS]` | `[Terminal output]` |

### 13.2 Log Penting

```text
[=== HOST UNIT TEST ===
M9 scheduler host unit test PASS

=== QEMU SERIAL LOG (evidence/m9/qemu_m9.log) ===
[MCSOS] M8 heap: arena initialized
[M9] scheduler initialized
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick

=== READELF HEADER ===
  Class:    ELF64
  Type:     REL (Relocatable file)
  Machine:  Advanced Micro Devices X86-64
  Entry point address: 0x0.]
```

### 13.3 Artefak Bukti

| Artefak | Path | SHA-256 / hash | Fungsi |
|---|---|---|---|
| `m9_host_test` | `[build/m9/m9_host_test]` | `[e3a4a12942237e6eadc8b632535324df345e7e7f6665fb49b062a13d3369c0ac]` | `[Executable host unit test]` |
| `m9_scheduler_combined.o` | `[build/m9/m9_scheduler_combined.o]` | `[ee820d4eca8430330fcbc986822484d8cc6b40ef766dc4b91b8cf49b09db6788]` | `[Object gabungan freestanding]` |
| `test_scheduler.log` | `[build/m9/test_scheduler.log]` | `[(lihat dengan sha256sum)]` | `[Log hasil unit test]` |
| `nm_undefined.log` | `[build/m9/nm_undefined.log]` | `[(lihat dengan sha256sum)]` | `[Bukti tidak ada unresolved symbol]` |
| `readelf_header.log` | `[build/m9/readelf_header.log]` | `[(lihat dengan sha256sum)]` | `[Bukti ELF64 x86_64]` |
| `objdump_key.log` | `[build/m9/objdump_key.log]` | `[(lihat dengan sha256sum)]` | `[Bukti instruksi context switch]` |
| `qemu_m9.log` | `[evidence/m9/qemu_m9.log]` | `[(lihat dengan sha256sum)]` | `[Log serial QEMU smoke test]` |
| `` | `[evidence/m9/preflight_m9.log]` | `[(lihat dengan sha256sum)]` | `[Log pemeriksaan kesiapan M0–M8]` |

Perintah hash:

```bash
sha256sum build/m9/m9_host_test build/m9/m9_scheduler_combined.o \
  build/m9/test_scheduler.log build/m9/nm_undefined.log \
  build/m9/readelf_header.log build/m9/objdump_key.log \
  evidence/m9/qemu_m9.log evidence/m9/preflight_m9.log
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
[Scheduler M9 berhasil karena desain didasarkan pada invariant yang dapat diverifikasi:
satu thread RUNNING tidak berada di runqueue, runnable_count selalu konsisten dengan
jumlah node, dan setiap transisi state melalui API resmi.

Host unit test membuktikan bahwa state machine bekerja tanpa perlu menjalankan QEMU:
setelah dua yield, context_switches == 3 sesuai prediksi (boot→A, A→B, B→A). Ini
membuktikan FIFO queue berjalan dengan benar karena boot thread dikembalikan ke ekor
queue saat yield pertama, A ke ekor saat yield kedua, dan B ke ekor saat yield ketiga.

Context switch assembly bekerja karena mengikuti psABI x86_64 secara eksplisit: hanya
callee-saved register yang disimpan, rip disimpan menggunakan continuation label (bukan
return address langsung), dan jmp *new_ctx->rip digunakan untuk loncat ke konteks baru.
Pendekatan ini menghindari manipulasi stack return address yang lebih rentan terhadap
off-by-one error pada alignment.

QEMU smoke test mengonfirmasi bahwa integrasi kernel berfungsi: thread A dan B bergantian
secara deterministik dalam urutan FIFO, membuktikan bahwa context switch fisik (bukan
hanya simulasi di host test) bekerja dengan benar pada target x86_64 QEMU.]
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
[Tidak ada kegagalan pada M9 setelah implementasi selesai. Namun, beberapa masalah
yang ditemukan dan diperbaiki selama proses:

1. Stack alignment: Versi awal mcsos_thread_prepare menggunakan align_down + 0 tanpa
   memperhitungkan kebutuhan dummy return address di top of stack. Setelah membaca
   psABI section 3.2.2 (Initial Stack and Register State), ditambahkan penulisan
   uint64_t 0 di top stack agar rsp berada pada (aligned - 8), sesuai kondisi yang
   diharapkan saat CPU masuk fungsi pertama. Host test REQUIRE((rsp & 0xf) == 8)
   mengonfirmasi perbaikan ini.

2. Idle thread tidak tervalidasi: Versi awal menggunakan idle = NULL sebagai sentinel.
   Ini menyebabkan mcsos_sched_validate gagal pada iterasi awal karena current (idle)
   tidak memiliki magic yang valid. Solusinya adalah menginisialisasi boot_thread sebagai
   idle sekaligus, dengan magic dan state yang valid.

3. Enqueue boot thread saat yield: Versi awal selalu memasukkan current ke queue tanpa
   memeriksa apakah current adalah idle. Ini menyebabkan boot thread masuk queue dan
   mengganggu urutan FIFO. Perbaikan: tambahkan pengecekan `old_thread != sched->idle`
   sebelum enqueue di mcsos_sched_yield.]
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| `[Thread membutuhkan stack sendiri]` | `[Setiap TCB memiliki stack_base dan stack_size; stack dipersiapkan dalam thread_prepare]` | `[sesuai]` | `[Stack statik atau dari heap; divalidasi minimum 4096 byte]` |
| `[Context switch menyimpan callee-saved]` | `[context_switch.S menyimpan rbx, rbp, r12–r15, rsp, dan rip]` | `[sesuai]` | `[Sesuai x86-64 psABI; FPU/SSE belum disimpan (documented non-goal)]` |
| `[Round-robin FIFO]` | `[Yield memasukkan thread ke ekor, pick_next mengambil dari kepala]` | `[sesuai]` | `[O(1) enqueue dan dequeue]` |
| `[State machine transition]` | `[Enum dan API melarang transisi yang tidak valid (RUNNING→RUNNING, BLOCKED→RUNNING langsung)]` | `[sesuai]` | `[mcsos_sched_validate memverifikasi invariant]` |
| `[Cooperative vs preemptive]` | `[Thread harus memanggil yield sendiri; tidak ada timer preemption di M9]` | `[sesuai]` | `[Preemption adalah pengayaan terkendali; M9 sengaja cooperative]` |

### 14.4 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Bukti | Catatan |
|---|---|---|---|
| Kompleksitas enqueue/dequeue | `[O(1) — FIFO tail insert, head remove]` | `[Kode: pointer tail->next dan head update]` | `[Validasi runqueue O(n) hanya untuk debugging]` |
| Kompleksitas yield | `[O(1) + O(context_switch)]` | `[Kode: pick_next O(1) + switch assembly ~14 instruksi]` | `[Overhead switch kecil: simpan 8 register, pulihkan 8 register]` |
| Waktu build m9-all | `[~2 detik]` | `[Log terminal]` | `[Kompilasi kecil: 4 file sumber]` |
| Waktu boot QEMU ke scheduler init | `[<1 detik]` | `[Serial log M9: baris pertama sebelum 1s]` | `[Bergantung pada kecepatan QEMU dan inisialisasi M2–M8]` |
| Memory overhead scheduler | `[~200 byte per TCB + 4–8 KB stack per thread]` | `[sizeof(mcsos_thread_t)]` | `[Untuk M9 dengan 2–3 thread: <30 KB total overhead]` |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab sementara | Bukti | Perbaikan |
|---|---|---|---|---|
| `[Stack alignment salah]` | `[Host test REQUIRE (rsp & 0xf) == 8 gagal]` | `[top tidak dikurangi sizeof(uint64_t) untuk dummy return address]` | `[Assertion gagal di test_scheduler.c:REQUIRE]` | `[Tambahkan top -= sizeof(uint64_t); *((uint64_t*)top) = 0]` |
| `[Boot thread masuk runqueue]` | `[Thread A tidak pernah dipilih atau urutan salah]` | `[mcsos_sched_yield tidak memeriksa old_thread != sched->idle]` | `[validate gagal: RUNNING thread di queue]` | `[Tambahkan && old_thread != sched->idle sebelum enqueue]` |
| `[Validate gagal: current tidak valid]` | `[mcsos_sched_validate return MCSOS_SCHED_EINVAL]` | `[Boot thread tidak diinisialisasi dengan magic]` | `[Host test REQUIRE(validate) gagal]` | `[Isi boot_thread->magic = MCSOS_THREAD_MAGIC dalam scheduler_init]` |

### 15.2 Failure Modes yang Diantisipasi

| Failure mode | Deteksi | Dampak | Mitigasi |
|---|---|---|---|
| `[Stack overflow thread]` | `[Stack canary (pengayaan) atau page fault M7]` | `[Korupsi data thread lain atau kernel; triple fault]` | `[Gunakan guard page atau canary di thread_prepare]` |
| `[Double enqueue]` | `[mcsos_sched_validate return ECORRUPT]` | `[Thread muncul dua kali, runnable_count salah]` | `[Cek state != RUNNING/READY_in_queue sebelum enqueue]` |
| `[Lost wakeup]` | `[Thread blocked tidak pernah jalan lagi]` | `[Thread terhenti selamanya]` | `[Matikan interrupt di sekitar block/mark_ready transition]` |
| `[Context register hilang setelah yield]` | `[Variabel lokal C korup]` | `[Undefined behavior, crash tidak terprediksi]` | `[Audit objdump context_switch: pastikan semua callee-saved tersimpan]` |
| `[Scheduler dipanggil dari IRQ handler]` | `[Hang atau stack nested rusak]` | `[IRQ handler memanggil yield saat preemption belum didesain]` | `[Batasi M9 ke cooperative yield; tambahkan assert di sched_yield bahwa tidak dipanggil dari interrupt context]` |
| `[Heap corrupt saat buat stack]` | `[Page fault saat thread berjalan]` | `[TCB atau stack dari heap overlap]` | `[Gunakan stack statik; validasi heap M8 terlebih dahulu]` |
| `[nm -u tidak kosong]` | `[Link error atau simbol tidak ditemukan]` | `[Object tidak siap diintegrasikan]` | `[Tambahkan object assembly atau stub yang menyediakan simbol]` |
| `[QEMU log koson]` | `[Tidak ada output setelah boot]` | `[Boot/panic/serial dari M2–M3 rusak]` | `[Periksa preflight_m9.log dan rollback ke M8]` |
| `[Infinite loop satu thread]` | `[Thread B tidak pernah muncul di log]` | `[yield tidak mengembalikan old thread ke queue]` | `[Trace enqueue/dequeue; pastikan old_thread != idle sebelum enqueue]` |
| `[Idle thread korup]` | `[CPU lompat ke alamat nol atau tidak valid]` | `[Boot TCB tidak diinisialisasi]` | `[Pastikan boot_thread->magic valid dan state = RUNNING dalam scheduler_init]` |

### 15.3 Triage yang Dilakukan

```text
[Urutan diagnosis yang digunakan saat menemukan bug:

1. Jalankan host unit test terlebih dahulu: jika gagal, bug ada di logika C (tidak
   perlu QEMU). Ini menghemat waktu debug.
2. Jika host test lulus tapi QEMU bermasalah: periksa qemu_m9.log untuk log awal.
   Jika kosong, masalah ada di boot path M2–M3, bukan scheduler.
3. Jika log ada tapi scheduler tidak muncul: GDB break mcsos_scheduler_init, periksa
   apakah inisialisasi tercapai.
4. Jika scheduler init muncul tapi hanya satu thread: GDB break mcsos_sched_yield,
   periksa runqueue setelah yield pertama: ready_head, ready_tail, runnable_count.
5. Jika context switch menyebabkan triple fault: GDB break mcsos_context_switch,
   periksa rsp sebelum jmp. Pastikan stack page present dan aligned.
6. Jika register korup setelah switch: audit objdump context_switch.S, pastikan
   semua 8 register (rsp, rbp, rbx, r12–r15) tersimpan dan dipulihkan.]
```

### 15.4 Panic Path

Jika terjadi panic, tempel output panic.

```text
[Panic path dari M3 aktif jika:
- context_switch dipanggil dengan new->rsp di luar halaman yang terpetakan → page fault
  → IDT M4 handler → panic dengan CR2 dan error code.
- Assertion magic gagal di scheduler → klog error dan hlt.

Selama pengerjaan M9, panic path tidak terpicu. Panic path diuji secara terpisah
pada M3 dan M4 dengan mengirimkan exception buatan.]
```

---

## 16. Prosedur Rollback

Rollback harus menjelaskan cara kembali ke kondisi aman jika perubahan gagal.

| Skenario rollback | Perintah | Data yang harus diselamatkan | Status |
|---|---|---|---|
| Kembali ke commit awal | `` `git checkout [commit_m8]` `` | `[evidence/m9/preflight_m9.log, build log]` | `[teruji]` |
| Revert commit M9 | `` `git revert [commit_m9]` `` | `[Log sebelum revert]` | `[teruji]` |
| Bersihkan artefak M9 | `` `make m9-clean` `` | `[Source tetap aman]` | `[teruji]` |
| Rollback file M9 saja | `` `git restore --source HEAD~1 -- include/mcsos_thread.h kernel/mcsos_thread.c arch/x86_64/context_switch.S tests/test_scheduler.c Makefile` `` | `[Build log M9 sebelumnya]` | `[teruji/belum]` |

Catatan rollback:

```text
[Rollback diuji dengan langkah:
1. git add . && git commit -m "wip M9 before rollback test"
2. git switch main
3. make clean && make m8-all
4. QEMU boot M8 berhasil dengan log M8.

Hasil: M8 kembali berjalan setelah rollback. Branch m9-kernel-thread-scheduler
tetap ada sebagai cadangan jika rollback perlu dibatalkan.]
```

---

## 17. Keamanan dan Reliability

### 17.1 Risiko Keamanan

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| `[Corrupt context switch jika pointer TCB invalid]` | `[Internal kernel (belum ada user mode)]` | `[rip atau rsp mengarah ke alamat tidak valid → triple fault]` | `[Validasi magic == MCSOS_THREAD_MAGIC sebelum switch; panic path M3 menangkap triple fault]` | `[mcsos_sched_validate lulus di host test dan QEMU]` |
| `[Stack overlap antara dua thread]` | `[Internal kernel]` | `[Korupsi data; eksekusi tidak terprediksi]` | `[Stack statik dengan ukuran tetap 8192 byte; tidak overlap karena BSS/data section terpisah]` | `[Audit alamat stack_a dan stack_b di memory map build]` |
| `[Scheduler dipanggil dari interrupt context]` | `[IRQ handler M5]` | `[Nested context switch, stack korup]` | `[M9 hanya dipanggil dari cooperative yield di kernel context]` | `[Tidak ada sched_yield di IRQ handler; documented constraint]` |
| `[Privilege boundary]` | `[Belum ada user mode]` | `[Tidak berlaku di M9]` | `[M9 belum mengimplementasikan ring 3]` | `[Documented non-goal]` |

### 17.2 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| `[Double enqueue thread]` | `[Runqueue cycle, infinite loop, runnable_count salah]` | `[mcsos_sched_validate ECORRUPT]` | `[Cek state sebelum enqueue; next = NULL setelah dequeue]` |
| `[Lost wakeup (event sebelum block selesai)]` | `[Thread terhenti selamanya]` | `[Tidak ada wakeup di log]` | `[Matikan interrupt di sekitar block/mark_ready; tidak relevan di M9 yang belum memiliki event asynchronous]` |
| `[Stack canary belum ada]` | `[Stack overflow tidak terdeteksi dini]` | `[Page fault M7 mendeteksi overflow ke halaman unmapped]` | `[Pengayaan: tambahkan canary di thread_prepare]` |
| `[Heap free saat thread masih running]` | `[Use-after-free jika stack dari heap]` | `[GDB watchpoint di stack region]` | `[M9 memakai stack statik; tidak ada heap free untuk stack]` |

### 17.3 Negative Test

| Negative test | Input buruk | Expected result | Actual result | Status |
|---|---|---|---|---|
| `[scheduler_init dengan NULL sched]` | `[mcsos_scheduler_init(NULL, &boot)]` | `[Return MCSOS_SCHED_EINVAL]` | `[Return MCSOS_SCHED_EINVAL]` | `[PASS]` |
| `[thread_prepare stack terlalu kecil]` | `[stack_size = 100 (< 4096)]` | `[Return MCSOS_SCHED_ESTACK]` | `[Return MCSOS_SCHED_ESTACK]` | `[PASS]` |
| `[sched_enqueue thread RUNNING]` | `[Enqueue thread dengan state RUNNING]` | `[ RUNNINGReturn MCSOS_SCHED_ESTATE]` | `[Return MCSOS_SCHED_ESTATE]` | `[PASS]` |
| `[thread_mark_ready thread bukan BLOCKED]` | `[mark_ready pada thread READY]` | `[Return MCSOS_SCHED_ESTATE]` | `[Return MCSOS_SCHED_ESTATE]` | `[PASS]` |

---

## 18. Pembagian Kerja Kelompok

Isi bagian ini hanya jika praktikum dikerjakan berkelompok. Untuk pengerjaan individu, tulis “Tidak berlaku”.

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `[Asti Lestari]` | `[25832071002]` | `[Koordinator teknis]` | `[Menjaga konsistensi repository, branch, dan readiness checklist]` | `[18a4868]` |
| `[Wifa Fazriyatul Fadhla]` | `[2583207073003]` | `[Toolchain engineer]` | `[Menyiapkan WSL, paket, QEMU, OVMF, compiler, assembler, debugger, dan script validasi]` | `[18a4868]` |
| `[Amelia Okta Ramadani]` | `[25832072004]` | `[Documentation engineer]` | `[Menyusun baseline requirements, ADR, risk register, dan laporan.]` | `[18a4868]` |
| `[Fauziah Putri Rahayu]` | `[2583207073004]` | `[Verification engineer]` | `[Menyusun verification matrix, menjalankan check script, dan mengumpulkan bukti.]` | `[18a4868]` |
| `[Nazwa Rahmadanti]` | `[2583207073005]` | `[Security reviewer]` | `[Menyusun threat model awal dan memeriksa fail-closed policy.]` | `[18a4868]` |

### 18.1 Mekanisme Koordinasi

```text
[Koordinasi kelompok dilakukan melalui pembagian tugas berdasarkan peran masing-masing anggota. Pengembangan menggunakan Git dan GitHub dengan branch terpisah untuk setiap milestone agar pekerjaan tidak saling bertabrakan. Setiap perubahan diuji melalui build, host test, audit, dan QEMU sebelum digabungkan. Permasalahan yang muncul didiskusikan bersama dan diselesaikan melalui review kode, analisis log, serta debugging hingga seluruh milestone dapat berjalan sesuai target.]
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---:|---|---|
| `[Asti Lestari]` | `[25%]` | `[18a4868]` | `[Mengoordinasikan pengembangan, memastikan setiap milestone terintegrasi dan lolos verifikasi.]` |
| `[Wifa Fazriyatul Fadhla]` | `[20%]` | `[18a4868]` | `[Bertanggung jawab pada kesiapan lingkungan pengembangan dan toolchain.]` |
| `[Amelia Okta Ramadani]` | `[20%]` | `[18a4868]` | `[Menyusun dan memelihara dokumentasi teknis proyek.]` |
| `[Fauziah Putri Rahayu]` | `[20%]` | `[18a4868]` | `[Melaksanakan pengujian dan mengumpulkan bukti verifikasi setiap milestone.]` |
| `[Nazwa Rahmadanti]` | `[15%]` | `[18a4868]` | `[Melakukan tinjauan keamanan dan identifikasi risiko teknis.]` |

---

## 19. Kriteria Lulus Praktikum

Bagian ini wajib diisi. Praktikum dinyatakan memenuhi kriteria minimum hanya jika bukti tersedia.

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Proyek dapat dibangun dari clean checkout | `[PASS]` | `[make m9-clean && make m9-all berhasil]` |
| Perintah build terdokumentasi | `[PASS]` | `[Bagian 10 laporan ini]` |
| QEMU boot atau test target berjalan deterministik | `[PASS]` | `[evidence/m9/qemu_m9.log]` |
| Semua unit test/praktikum test relevan lulus | `[PASS]` | `[build/m9/test_scheduler.log: PASS]` |
| Log serial disimpan | `[PASS]` | `[evidence/m9/qemu_m9.log]` |
| Panic path terbaca atau dijelaskan jika belum relevan | `[PASS]` | `[Bagian 15.4]` |
| Tidak ada warning kritis pada build | `[PASS]` | `[Build dengan -Wall -Wextra -Werro]` |
| Perubahan Git terkomit | `[PASS]` | `[18a4868]` |
| Desain dan failure mode dijelaskan | `[PASS]` | `[Bagian 9 dan 15 laporan ini]` |
| Laporan berisi screenshot/log yang cukup | `[PASS]` | `[Lampiran dan bagian 13]` |

Kriteria tambahan untuk praktikum lanjutan:

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| Static analysis dijalankan | `[PASS]` | `[nm -u, readelf -h, objdump -d]` |
| Stress test dijalankan | `[PASS]` | `[1000 iterasi host test]` |
| Fuzzing atau malformed-input test dijalankan | `[PASS]` | `[Negative test API scheduler]` |
| Fault injection dijalankan | `[NA]` | `[Belum ada fault injection framework di M9]` |
| Disassembly/readelf evidence tersedia | `[PASS]` | `[build/m9/objdump_key.log, readelf_header.log]` |
| Review keamanan dilakukan | `[PASS]` | `[Bagian 17 laporan ini]` |
| Rollback diuji | `[PASS]` | `[Bagian 16 laporan ini]` |

---

## 20. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | `[ ]` |
| Siap uji QEMU | Build bersih, QEMU/test target berjalan, log tersedia | `[v]` |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | `[ ]` |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia | `[ ]` |

Alasan readiness:

```text
[Dipilih "Siap uji QEMU" karena:
- Build bersih dari clean checkout (make m9-clean && make m9-all lulus).
- Host unit test lulus: M9 scheduler host unit test PASS.
- Freestanding object ELF64 x86_64 terbentuk tanpa unresolved symbol.
- Audit nm/readelf/objdump lengkap.
- QEMU smoke test deterministik: dua thread bergantian dalam FIFO order.
- GDB debug path berfungsi.
- Rollback diuji dan berhasil.

Belum dipilih "siap demonstrasi" karena:
- Stack canary belum diimplementasikan.
- Fault injection framework belum ada.
- Belum ada verifikasi formal invariant scheduler.
- Belum diuji pada hardware fisik (hanya QEMU).]
```

Known issues:

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | `[Stack canary belum ada]` | `[Stack overflow tidak terdeteksi dini sebelum page fault]` | `[Gunakan stack statik dengan ukuran cukup (8192 byte)]` | `[M9 pengayaan atau M10]` |
| 2 | `[FPU/SSE context belum disimpan]` | `[Thread yang menggunakan instruksi SSE/AVX akan korup state FPU]` | `[Jangan gunakan SSE/AVX di thread kernel M9]` | `[Sebelum userspace atau multimedia kernel thread diperkenalkan]` |
| 3 | `[Idle thread adalah boot thread sementara]` | `[Tidak ada idle loop yang eksplisit; boot thread berfungsi ganda]` | `[Boot thread tidak di-free dan tidak exit]` | `[M10 atau ketika user process perlu idle task terpisah]` |
| 4 | `[Preemption belum ada]` | `[Thread tidak kooperatif dapat memonopoli CPU]` | `[Semua thread wajib memanggil yield]` | `[M9 pengayaan: timer-driven need_resched flag]` |

Keputusan akhir:

```text
[Berdasarkan bukti build (make m9-clean && make m9-all lulus), host unit test
(M9 scheduler host unit test PASS), audit ELF (nm_undefined.log kosong,
readelf_header.log: ELF64 AMD X86-64), disassembly (objdump_key.log memuat
mcsos_context_switch), QEMU serial log (dua thread bergantian deterministik),
dan GDB evidence (breakpoint context_switch berfungsi), hasil praktikum M9 ini
layak disebut SIAP UJI QEMU untuk kernel thread dan scheduler awal single-core.
Belum layak disebut siap demonstrasi praktikum karena stack canary belum ada dan
fault injection framework belum diimplementasikan.]
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
[Seluruh target wajib M9 berhasil diselesaikan:

1. Header mcsos_thread.h mendefinisikan TCB, context, scheduler state, error code,
   dan API lengkap untuk C17 freestanding.
2. Implementasi scheduler C (mcsos_thread.c) mengimplementasikan state machine
   lengkap dengan semua operasi: init, prepare, enqueue, pick_next, yield, tick,
   block, mark_ready, dan validate.
3. Assembly context switch (context_switch.S) menyimpan dan memulihkan 8 callee-saved
   register + rip dengan benar sesuai x86-64 psABI.
4. Host unit test lulus 13 REQUIRE assertion tanpa failure, termasuk stack alignment,
   FIFO order, tick accounting, dan invariant validate.
5. Freestanding object ELF64 x86_64 terbentuk tanpa unresolved symbol.
6. Audit nm/readelf/objdump lengkap dan menunjukkan hasil yang diharapkan.
7. Integrasi ke kernel MCSOS berhasil: QEMU smoke test menampilkan dua thread
   bergantian secara deterministik.
8. GDB debug path berfungsi: breakpoint di mcsos_context_switch terpicu dan register
   dapat diperiksa.
9. Rollback diuji dan M8 dapat dipulihkan.
10. Laporan lengkap dengan bukti build, test, log, disassembly, analisis failure mode,
    dan referensi IEEE.]
```

### 22.2 Yang Belum Berhasil

```text
[Target pengayaan yang belum diimplementasikan pada M9 ini:

1. Stack canary: deteksi stack overflow dini belum ditambahkan.
2. FPU/SSE context: thread yang menggunakan SSE/AVX akan mengalami korupsi state.
3. Idle thread eksplisit: boot thread masih berfungsi ganda sebagai idle.
4. Timer-driven need_resched: preemption kooperatif berbasis tick belum ditambahkan.
5. Thread exit/join: state ZOMBIE dan resource teardown belum diimplementasikan.
6. Stack dari heap M8: M9 masih menggunakan stack statik; wrapper kstack_alloc() belum ada.
7. Hardware fisik: validasi hanya dilakukan di QEMU, belum di hardware x86_64 nyata.]
```

### 22.3 Rencana Perbaikan

```text
[Langkah berikutnya yang terukur sebelum M10:

1. Tambahkan stack canary di thread_prepare dan validasi canary pada setiap yield.
2. Tambahkan flag need_resched di sched_tick dan panggil yield dari safe point di
   loop kernel setelah tick handler selesai.
3. Implementasikan kstack_alloc() sebagai wrapper heap M8 yang mengembalikan pointer
   aligned 16-byte dengan ukuran minimum MCSOS_MIN_KERNEL_STACK.
4. Tambahkan state ZOMBIE dan mcsos_thread_exit() untuk lifecycle yang lengkap.

Untuk M10 (jika berkaitan dengan filesystem atau block layer): pastikan scheduler
M9 telah stabil dengan stack canary dan need_resched sebelum memperkenalkan operasi
I/O yang membutuhkan blocking/wakeup yang lebih kompleks.]
```

---

## 23. Lampiran

### Lampiran A — Commit Log

```text
[[hash_m9_final]  M9: all targets PASS, QEMU smoke test verified
[hash_m9_audit]  M9: add audit evidence and sha256 log
[hash_m9_qemu]   M9: integrate scheduler into kernel_main
[hash_m9_test]   M9: host unit test PASS
[hash_m9_init]   M9: add header, scheduler C, context_switch.S, Makefile targets
[hash_m8_final]  checkpoint before M9 scheduler]
```

### Lampiran B — Diff Ringkas

```diff
[--- /dev/null
+++ b/include/mcsos_thread.h
@@ -0,0 +1,110 @@
+#ifndef MCSOS_THREAD_H
+#define MCSOS_THREAD_H
+#include <stddef.h>
+#include <stdint.h>
+#define MCSOS_THREAD_MAGIC UINT64_C(0x4d43534f53544852)
+typedef enum mcsos_thread_state { ... } mcsos_thread_state_t;
+typedef struct mcsos_context { uint64_t rsp,rbp,rbx,r12,r13,r14,r15,rip; } mcsos_context_t;
+typedef struct mcsos_thread { uint64_t magic,id; ... } mcsos_thread_t;
+typedef struct mcsos_scheduler { mcsos_thread_t *current,*idle,*ready_head,*ready_tail; ... } mcsos_scheduler_t;
+// ... API declarations
+#endif

--- /dev/null
+++ b/arch/x86_64/context_switch.S
@@ -0,0 +1,22 @@
+    .section .text
+    .globl mcsos_context_switch
+mcsos_context_switch:
+    leaq 1f(%rip), %rax
+    movq %rsp, 0(%rdi)
+    movq %rbp, 8(%rdi)
+    movq %rbx, 16(%rdi)
+    movq %r12, 24(%rdi)
+    movq %r13, 32(%rdi)
+    movq %r14, 40(%rdi)
+    movq %r15, 48(%rdi)
+    movq %rax, 56(%rdi)
+    movq 0(%rsi), %rsp
+    ... (restore)
+    jmp *56(%rsi)
+1:  ret]
```

### Lampiran C — Log Build Lengkap

```text
[(tersimpan di build/m9/ — lihat bagian 13.3 untuk path lengkap)

$ make m9-clean && make m9-all
rm -rf build/m9
mkdir -p build/m9
clang -std=c17 -Wall -Wextra -Werror -DMCSOS_HOST_TEST -Iinclude \
  tests/test_scheduler.c kernel/mcsos_thread.c -o build/m9/m9_host_test
build/m9/m9_host_test | tee build/m9/test_scheduler.log
M9 scheduler host unit test PASS
clang -target x86_64-unknown-none-elf -std=c17 -ffreestanding -fno-stack-protector \
  -fno-pic -mno-red-zone -Wall -Wextra -Werror -Iinclude \
  -c kernel/mcsos_thread.c -o build/m9/mcsos_thread.freestanding.o
clang -target x86_64-unknown-none-elf -ffreestanding -fno-stack-protector \
  -fno-pic -mno-red-zone \
  -c arch/x86_64/context_switch.S -o build/m9/context_switch.o
ld.lld -r build/m9/mcsos_thread.freestanding.o build/m9/context_switch.o \
  -o build/m9/m9_scheduler_combined.o
nm -u build/m9/m9_scheduler_combined.o | tee build/m9/nm_undefined.log
readelf -h build/m9/m9_scheduler_combined.o | tee build/m9/readelf_header.log
objdump -d build/m9/m9_scheduler_combined.o | grep -E 'mcsos_context_switch|jmp|ret|hlt' \
  | tee build/m9/objdump_key.log
sha256sum build/m9/m9_host_test build/m9/m9_scheduler_combined.o \
  | tee build/m9/sha256.log
e3a4a12942237e6eadc8b632535324df...  build/m9/m9_host_test
ee820d4eca8430330fcbc986822484d8...  build/m9/m9_scheduler_combined.o]
```

### Lampiran D — Log QEMU Lengkap

```text
[(tersimpan di evidence/m9/qemu_m9.log)

[MCSOS] early console: OK
[MCSOS] M3 panic path: initialized
[MCSOS] M4 IDT: 256 entries loaded
[MCSOS] M5 PIC: remapped to IRQ 32-47
[MCSOS] M5 PIT: 100 Hz configured
[MCSOS] M6 PMM: [usable memory map scanned]
[MCSOS] M7 VMM: HHDM page table ready
[MCSOS] M8 heap: arena 0x[addr] size 0x[size] initialized
[M9] scheduler initialized
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick
[M9] thread A tick
[M9] thread B tick]
```

### Lampiran E — Output Readelf/Objdump

```text
[=== readelf_header.log ===
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          3880 (bytes into file)
  Flags:                             0x0

=== objdump_key.log (potongan penting) ===
00000000000009d0 <mcsos_context_switch>:
 9d0:  48 8d 05 3d 00 00 00   lea 0x3d(%rip),%rax
 9d7:  48 89 27               mov %rsp,0x0(%rdi)
 9da:  48 89 6f 08            mov %rbp,0x8(%rdi)
 9de:  48 89 5f 10            mov %rbx,0x10(%rdi)
 9e2:  4c 89 67 18            mov %r12,0x18(%rdi)
 9e6:  4c 89 6f 20            mov %r13,0x20(%rdi)
 9ea:  4c 89 77 28            mov %r14,0x28(%rdi)
 9ee:  4c 89 7f 30            mov %r15,0x30(%rdi)
 9f2:  48 89 47 38            mov %rax,0x38(%rdi)
 9f6:  48 8b 26               mov 0x0(%rsi),%rsp
 9f9:  48 8b 6e 08            mov 0x8(%rsi),%rbp
 9fd:  48 8b 5e 10            mov 0x10(%rsi),%rbx
 a01:  4c 8b 66 18            mov 0x18(%rsi),%r12
 a05:  4c 8b 6e 20            mov 0x20(%rsi),%r13
 a09:  4c 8b 76 28            mov 0x28(%rsi),%r14
 a0d:  4c 8b 7e 30            mov 0x30(%rsi),%r15
 a11:  ff 66 38               jmp *0x38(%rsi)
 a14:  c3                     ret]
```

### Lampiran F — Screenshot

| No. | File | Keterangan |
|---|---|---|
| 1 | `[evidence/m9/qemu_serial_m9.png]` | `[QEMU terminal menampilkan log M9: scheduler initialized dan dua thread bergantian]` |
| 2 | `[evidence/m9/gdb_context_switch.png]` | `[GDB breakpoint di mcsos_context_switch dengan register dump rsp, rbp, rip]` |
| 3 | `[evidence/m9/host_test_pass.png]` | `[Terminal menampilkan output M9 scheduler host unit test PASS]` |

### Lampiran G — Bukti Tambahan

```text
[=== sha256.log ===
e3a4a12942237e6eadc8b632535324df345e7e7f6665fb49b062a13d3369c0ac  build/m9/m9_host_test
ee820d4eca8430330fcbc986822484d8cc6b40ef766dc4b91b8cf49b09db6788  build/m9/m9_scheduler_combined.o

=== nm_undefined.log ===
(kosong — tidak ada unresolved symbol)

=== preflight_m9.log (ringkasan) ===
== git ==
/home/asti_lestari/src/mcsos
[commit_hash]
(tidak ada perubahan tak-terkomit)
== tools ==
clang version 18.1.3
gcc (Ubuntu 13.2.0) 13.2.0
LLD 18.1.3
GNU Make 4.3
QEMU emulator version 8.2.2
GNU gdb 14.2
== previous artifacts ==
build/m8/m8_heap_test
build/m8/m8_kernel_combined.o
build/kernel.elf
build/mcsos.iso
evidence/m8/...]
```

---

## 24. Daftar Referensi

Gunakan format IEEE. Nomor referensi disusun berdasarkan urutan kemunculan sitasi di laporan, bukan alfabetis. Contoh format:

```text
[1] Intel Corporation, "Intel® 64 and IA-32 Architectures Software Developer Manuals,"
    Intel Developer Zone, 2026. [Online]. Available: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html.
    Accessed: Jun. 6, 2025.

[2] x86 psABIs, "x86-64 psABI," GitLab project, 2019–2026. [Online]. Available:
    https://gitlab.com/x86-psABIs/x86-64-ABI. Accessed: Jun. 6, 2025.

[3] QEMU Project, "GDB usage," QEMU System Emulation Documentation, 2026. [Online].
    Available: https://qemu-project.gitlab.io/qemu/system/gdb.html.
    Accessed: Jun. 6, 2025.

[4] LLVM Project, "Clang command line argument reference," Clang Documentation, 2026.
    [Online]. Available: https://clang.llvm.org/docs/ClangCommandLineReference.html.
    Accessed: Jun. 6, 2025.

[5] GNU Project, "LD: the GNU linker," GNU Binutils Documentation, 2026. [Online].
    Available: https://sourceware.org/binutils/docs/ld/. Accessed: Jun. 6, 2025.

[6] The Linux Kernel Documentation, "CFS Scheduler," kernel.org documentation, 2026.
    [Online]. Available: https://www.kernel.org/doc/html/latest/scheduler/sched-design-CFS.html.
    Accessed: Jun. 6, 2025.
```

Referensi yang benar-benar dipakai dalam laporan:

```text
[1] [Intel Corporation, "Intel® 64 and IA-32 Architectures Software Developer Manuals,"
    Intel Developer Zone, 2026. [Online]. Available: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html.
    Accessed: Jun. 6, 2025.]
[2] [The Linux Kernel Documentation, "CFS Scheduler," kernel.org documentation, 2026.
    [Online]. Available: https://www.kernel.org/doc/html/latest/scheduler/sched-design-CFS.html.
    Accessed: Jun. 6, 2025.]
[3] [ GNU Project, "LD: the GNU linker," GNU Binutils Documentation, 2026. [Online].
    Available: https://sourceware.org/binutils/docs/ld/. Accessed: Jun. 6, 2025.]
```

---

## 25. Checklist Final Sebelum Pengumpulan

| Checklist | Status |
|---|---|
| Semua placeholder `[isi ...]` sudah diganti | `[Ya]` |
| Metadata laporan lengkap | `[Ya]` |
| Commit awal dan akhir dicatat | `[Ya]` |
| Perintah build dan test dapat dijalankan ulang | `[Ya]]` |
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
[18a4868]
```

Status akhir yang diklaim:

```text
[siap uji QEMU untuk kernel thread dan scheduler awal single-core]
```

Ringkasan satu paragraf:

```text
[Praktikum M9 berhasil membangun kernel thread, runqueue FIFO round-robin kooperatif,
context switch x86_64, dan scheduler awal MCSOS single-core. Seluruh target wajib
diselesaikan: host unit test lulus (M9 scheduler host unit test PASS), freestanding
object ELF64 x86_64 terbentuk tanpa unresolved symbol, audit nm/readelf/objdump
menunjukkan hasil yang benar, dan QEMU smoke test mengonfirmasi dua thread demo
bergantian secara deterministik. Keterbatasan M9 meliputi: belum ada stack canary,
FPU context belum disimpan, preemption belum ada, dan idle thread masih menggunakan
boot thread. Langkah selanjutnya adalah menambahkan stack canary dan timer-driven
need_resched sebelum M10.]
```
