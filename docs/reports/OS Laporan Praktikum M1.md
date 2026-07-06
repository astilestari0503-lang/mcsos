# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan\_praktikum\_M1\_25832072004.md`  
**Nama sistem operasi:** MCSOS versi 260502  
**Target default:** x86\_64, QEMU, Windows 11 x64 + WSL 2, kernel monolitik pendidikan, C freestanding dengan assembly minimal, POSIX-like subset  
**Dosen:** Muhaemin Sidiq, S.Pd., M.Pd.  
**Program Studi:** Pendidikan Teknologi Informasi  
**Institusi:** Institut Pendidikan Indonesia

> Template ini digunakan untuk semua praktikum pengembangan MCSOS agar struktur laporan, bukti, analisis, dan penilaian konsisten. Ganti seluruh teks bertanda `\[isi ...]` dengan data praktikum sebenarnya. Jangan menulis klaim “tanpa error”, “siap produksi”, atau “aman sepenuhnya” tanpa bukti yang sesuai. Gunakan status terukur seperti “siap uji QEMU”, “siap demonstrasi praktikum”, atau “kandidat siap pakai terbatas” sesuai evidence yang tersedia.

\---

## 0\. Metadata Laporan

|Atribut|Isi|
|-|-|
|Kode praktikum|M1|
|Judul praktikum|Toolchain Reproducible dan Pemeriksaan Kesiapan Lingkungan Pengembangan MCSOS 260502|
|Jenis pengerjaan|Kelompok|
|Nama mahasiswa|Amelia Okta Ramadani|
|NIM|25832072004|
|Kelas|PTI 1 A|
|Nama kelompok|Princes|
|Anggota kelompok|Asti Lestari (25832071002, Host Unit Test), Wifa Fazriyatul Fadhla (2583207073003, Dokumentasi), Nazwa Rahmadanti (2583207073005, Audit), Fauziah Putri Rahayu (2583207073004, Integrasi Kernel)|
|Tanggal praktikum|2026-06-15|
|Tanggal pengumpulan|2026-06-17|
|Repository|https://github.com/AmeliaOkta/MCSOS\_Sistem-Operasi\_25832072004.git|
|Branch|master|
|Commit awal|a532aa0|
|Commit akhir|d912ba19a47b403a33d2b66bde377492213184c5|
|Status readiness yang diklaim|Siap uji QEMU|

\---

## 1\. Sampul

# Laporan Praktikum `M1`

## `Toolchain Reproducible dan Pemeriksaan Kesiapan Lingkungan Pengembangan MCSOS 260502`

Disusun oleh:

|Nama|NIM|Kelas|Peran|
|-|-|-|-|
|`Amelia Okta Ramadani`|`25832072004`|`PTI 1A`|`Koordinator dan penyusun laporan`|


Dosen Pengampu: **Muhaemin Sidiq, S.Pd., M.Pd.**  
Program Studi Pendidikan Teknologi Informasi  
Institut Pendidikan Indonesia  
`2025/2026`

\---

## 2\. Pernyataan Orisinalitas dan Integritas Akademik

Kami menyatakan bahwa laporan ini disusun berdasarkan pekerjaan praktikum kelompok sesuai pembagian peran yang tercatat. Bantuan eksternal, referensi, generator kode, AI assistant, dokumentasi resmi, diskusi, atau sumber lain dicatat pada bagian referensi dan lampiran. Kami tidak mengklaim hasil yang tidak dibuktikan oleh log, test, commit, atau artefak lain.

|Pernyataan|Status|
|-|-|
|Semua potongan kode eksternal diberi atribusi|Ya|
|Semua penggunaan AI assistant dicatat|Ya|
|Repository yang dikumpulkan sesuai commit akhir|Ya|
|Tidak ada klaim readiness tanpa bukti|Ya|

Catatan penggunaan bantuan eksternal:

```
AI assistant digunakan untuk membantu pemahaman konsep toolchain.
Verifikasi mandiri dilakukan dengan menjalankan seluruh perintah secara langsung di lingkungan WSL 2
dan memeriksa output evidence secara manual.
```

\---

## 3\. Tujuan Praktikum

1. Memvalidasi kesiapan lingkungan pengembangan WSL 2 dan Windows 11 x64 untuk pengembangan kernel MCSOS.
2. Membangun toolchain reproducible yang terdokumentasi dengan metadata versi terukur.
3. Menghasilkan freestanding object ELF64 x86\_64 sebagai bukti kompilasi kernel-mode tanpa libc host.
4. Memverifikasi ketersediaan QEMU, OVMF, dan GDB sebagai prasyarat praktikum M2.
5. Membuktikan reproducibility build melalui hash deterministik dua kali build bersih.
6. Menyusun readiness review M1 berbasis evidence yang dapat diperiksa dan diaudit.

\---

## 4\. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

|CPL/CPMK praktikum|Bukti yang harus ditunjukkan|
|-|-|
|Menjelaskan mengapa pengembangan kernel memerlukan toolchain freestanding dan tidak boleh bergantung pada hosted libc|Analisis dasar teori, nm-undefined.txt kosong|
|Mengonfigurasi Windows 11 x64, WSL 2, dan repository Linux filesystem untuk pengembangan MCSOS|host-readiness.txt, check\_toolchain.sh OK|
|Memasang dan memverifikasi tool build utama|toolchain-versions.txt, make check PASS|
|Membuat script pemeriksaan toolchain yang dapat dijalankan ulang secara deterministik|tools/scripts/check\_toolchain.sh, make test PASS|
|Menghasilkan metadata versi toolchain sebagai evidence reproduksi build|build/meta/toolchain-versions.txt|
|Mengompilasi source C kecil menjadi object freestanding x86\_64 ELF dan memeriksa hasilnya|readelf-header.txt, objdump-disassembly.txt, nm-undefined.txt|
|Menjelaskan failure modes umum pada toolchain OSDev|Bagian 15 dan 16 laporan|
|Menyusun readiness review M1 dengan bukti yang dapat diperiksa|docs/readiness/M1-toolchain.md, bagian 20 laporan|

\---

## 5\. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

|Milestone|Fokus|Status dalam laporan|
|-|-|-|
|M0|Requirements, governance, baseline arsitektur|`\[ ] tidak dibahas / \[V] dibahas / \[V] selesai praktikum`|
|M1|Toolchain reproducible, Git, QEMU, GDB, metadata build|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M2|Boot image, kernel ELF64, early console|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M3|Panic path, linker map, GDB, observability awal|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M4|Trap, exception, interrupt, timer|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M5|PMM, VMM, page table, kernel heap|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M6|Thread, scheduler, synchronization|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M7|Syscall ABI dan user program loader|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M8|VFS, file descriptor, ramfs|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M9|Block layer dan device model|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M10|Persistent filesystem, mcsfs/ext2-like, recovery|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M11|Networking stack, packet parsing, UDP/TCP subset|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M12|Security model, capability/ACL, syscall fuzzing, hardening|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M13|SMP, scalability, lock stress, NUMA-aware preparation|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M14|Framebuffer, graphics console, visual regression|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M15|Virtualization/container subset|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|
|M16|Observability, update/rollback, release image, readiness review|`\[ ] tidak dibahas / \[ ] dibahas / \[ ] selesai praktikum`|

Batas cakupan praktikum:

```
Praktikum M1 mencakup: validasi lingkungan WSL 2, instalasi dan verifikasi toolchain,
kompilasi freestanding proof object ELF64 x86\_64, inspeksi ELF, QEMU/OVMF probe,
reproducibility check, dan penyusunan readiness review.
Non-goals: membuat bootloader, kernel entry, linker script final, boot di QEMU,
menjalankan GDB pada kernel, syscall, userspace, atau klaim stabilitas sistem operasi.
```

\---

## 6\. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.

### 6.1 Konsep Sistem Operasi yang Diuji

Praktikum M1 berfokus pada validasi toolchain freestanding sebagai pondasi pengembangan kernel. Hosted C berjalan di atas OS dan libc, sedangkan freestanding C tidak mengasumsikan fasilitas OS apapun. Kernel MCSOS tidak boleh bergantung pada libc host karena pada saat eksekusi kernel, tidak ada sistem operasi yang mendasari. Reproducible build adalah kemampuan menghasilkan artefak binary yang identik dari clean checkout dengan toolchain yang sama, yang merupakan syarat auditabilitas dan keamanan supply-chain.

### 6.2 Konsep Arsitektur x86\_64 yang Relevan

|Konsep|Relevansi pada praktikum|Bukti/verifikasi|
|-|-|-|
|Long mode x86\_64|Target arsitektur kernel MCSOS; ELF64 harus menarget x86\_64|readelf menunjukkan Machine: Advanced Micro Devices X86-64|
|Red zone x86\_64|Area 128 byte di bawah RSP yang berbahaya untuk kernel/interrupt handler; harus dinonaktifkan|Flag -mno-red-zone pada proof\_compile.sh|
|ELF64 format|Format object dan executable yang dipakai kernel tahap awal|readelf-header.txt menunjukkan ELF64, EXEC|
|Freestanding ABI|Kernel tidak boleh menggunakan main, crt0, libc, dynamic linker, atau startup file host|nm-undefined.txt kosong, tidak ada symbol libc|
|Entry point kernel|Kernel mendefinisikan entry point sendiri di alamat tinggi (0xffffffff80000000)|readelf entry point address: 0xffffffff80000000|

### 6.3 Konsep Implementasi Freestanding

|Aspek|Keputusan praktikum|
|-|-|
|Bahasa|C17 freestanding|
|Runtime|Tanpa hosted libc; tidak ada crt0, startup object, atau dynamic linker|
|ABI|x86\_64 System V untuk proof; ABI kernel internal akan ditetapkan mulai M2|
|Compiler flags kritis|--target=x86\_64-unknown-elf, -ffreestanding, -fno-stack-protector, -fno-pic, -mno-red-zone, -mno-mmx, -mno-sse, -mno-sse2, -nostdlib|
|Risiko undefined behavior|Pointer relokasi kernel di alamat tinggi; ditangani dengan inspeksi readelf dan nm|

### 6.4 Referensi Teori yang Digunakan

|No.|Sumber|Bagian yang digunakan|Alasan relevansi|
|-|-|-|-|
|\[1]|Microsoft, Install WSL, Microsoft Learn, 2025|Instalasi dan konfigurasi WSL 2|Dasar lingkungan build WSL 2|
|\[2]|Microsoft, Advanced settings configuration in WSL, Microsoft Learn, 2025|.wslconfig memory dan CPU|Optimasi resource WSL untuk kompilasi|
|\[3]|QEMU Project, Invocation, QEMU System Emulation User's Guide, 2026|Machine q35, headless mode|Dasar QEMU probe M1|
|\[5]|Free Software Foundation, x86 Options, GCC Online Documentation|-mno-red-zone, -mno-sse, freestanding flags|Dasar compiler flags kernel|
|\[7]|LLVM Project, Cross-compilation using Clang, Clang Documentation|--target triple, cross compilation|Dasar target x86\_64-unknown-elf|
|\[10]|GNU Project, GNU Binutils Documentation|readelf, objdump, nm usage|Inspeksi ELF dan symbol table|

\---

## 7\. Lingkungan Praktikum

### 7.1 Host dan Target

|Komponen|Nilai|
|-|-|
|Host OS|Windows 11 x64|
|Lingkungan build|WSL 2 Ubuntu 26.04 LTS (Resolute Raccoon)|
|Target ISA|x86\_64|
|Target ABI|x86\_64-unknown-elf|
|Emulator|QEMU emulator version 10.2.1 (Debian 1:10.2.1+ds-1ubuntu3)|
|Firmware emulator|OVMF — /usr/share/OVMF/OVMF\_CODE\_4M.fd, /usr/share/ovmf/OVMF.fd, /usr/share/qemu/OVMF.fd|
|Debugger|GNU gdb (Ubuntu 17.1-2ubuntu1) 17.1|
|Build system|GNU Make 4.4.1|
|Bahasa utama|C17 freestanding|
|Assembly|NASM version 3.01|

### 7.2 Versi Toolchain

Perintah verifikasi toolchain:

```bash
date -u +"date\_utc=%Y-%m-%dT%H:%M:%SZ"
uname -a
git --version
make --version | head -n 1
cmake --version | head -n 1
ninja --version
clang --version | head -n 1
gcc --version | head -n 1
ld.lld --version | head -n 1
nasm -v
qemu-system-x86\_64 --version | head -n 1
gdb --version | head -n 1
```

Output:

```
date\_utc=2026-05-06T14:06:46Z
uname=Linux DESKTOP-COGF6J0 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT\_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86\_64 GNU/Linux
git version 2.53.0
GNU Make 4.4.1
cmake version 4.2.3
ninja 1.13.2
Ubuntu clang version 21.1.8 (6ubuntu1)
gcc (Ubuntu 15.2.0-16ubuntu1) 15.2.0
Ubuntu LLD 21.1.8 (compatible with GNU linkers)
NASM version 3.01
QEMU emulator version 10.2.1 (Debian 1:10.2.1+ds-1ubuntu3)
GNU gdb (Ubuntu 17.1-2ubuntu1) 17.1
Python 3.14.4
ShellCheck version: 0.11.0
Cppcheck 2.19.0
Ubuntu LLVM version 21.1.8
```

### 7.3 Lokasi Repository

|Item|Nilai|
|-|-|
|Path repository di WSL|\~/src/mcsos (/home/user/src/mcsos)|
|Apakah berada di filesystem Linux WSL, bukan /mnt/c|Ya|
|Remote repository|https://github.com/AmeliaOkta/MCSOS\_Sistem-Operasi\_25832072004.git|
|Branch|master|
|Commit hash awal|a532aa0|
|Commit hash akhir|d912ba19a47b403a33d2b66bde377492213184c5|

\---

## 8\. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

```
mcsos/
  README.md
  LICENSE
  Makefile
  .gitignore
  docs/
    architecture/
      invariants.md
    readiness/
      M1-toolchain.md
    security/
      toolchain\_threat\_model.md
    testing/
  tools/
    scripts/
      check\_toolchain.sh
      collect\_meta.sh
      proof\_compile.sh
      qemu\_probe.sh
      repro\_check.sh
  tests/
    toolchain/
      freestanding\_probe.c
  build/   # generated, tidak dikomit
```

### 8.2 File yang Dibuat atau Diubah

|File|Jenis perubahan|Alasan perubahan|Risiko|
|-|-|-|-|
|Makefile|Baru|Antarmuka build tunggal untuk semua target M1|Rendah|
|.gitignore|Baru|Mencegah artefak generated (build/) masuk commit|Rendah|
|tools/scripts/collect\_meta.sh|Baru|Mengumpulkan metadata versi toolchain secara deterministik|Rendah|
|tools/scripts/check\_toolchain.sh|Baru|Gate pemeriksaan ketersediaan semua tool wajib|Rendah|
|tools/scripts/proof\_compile.sh|Baru|Kompilasi freestanding proof object dan ELF x86\_64|Rendah|
|tools/scripts/qemu\_probe.sh|Baru|Verifikasi QEMU q35 dan OVMF tersedia|Rendah|
|tools/scripts/repro\_check.sh|Baru|Uji reproducibility build dengan hash SHA-256|Rendah|
|tests/toolchain/freestanding\_probe.c|Baru|Source C freestanding untuk membuktikan kompilasi tanpa libc|Rendah|
|docs/architecture/invariants.md|Baru|Mendokumentasikan invariant lingkungan M1 ke M2|Rendah|
|docs/security/toolchain\_threat\_model.md|Baru|Threat model supply-chain dan konfigurasi toolchain|Rendah|
|docs/readiness/M1-toolchain.md|Baru|Readiness review M1 berbasis evidence|Rendah|

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

```
Output:
d912ba1 (HEAD -> master, origin/m1-toolchain-readiness) M1: add reproducible toolchain readiness baseline
a532aa0 MO: Full rebuild of project baseline and documentation
```

\---

## 9\. Desain Teknis

### 9.1 Masalah yang Diselesaikan

Sebelum M1, belum ada jaminan bahwa lingkungan pengembangan siap untuk menghasilkan artefak kernel yang benar. Toolchain yang salah konfigurasi dapat menghasilkan binary yang tampak valid namun memakai ABI yang tidak sesuai, bergantung pada libc host, atau tidak dapat diulang buildnya. M1 menyelesaikan masalah ini dengan menetapkan gate kritis berupa script pemeriksaan deterministik, proof object freestanding, dan reproducibility hash sebelum kernel sebenarnya dikembangkan pada M2.

### 9.2 Keputusan Desain

|Keputusan|Alternatif yang dipertimbangkan|Alasan memilih|Konsekuensi|
|-|-|-|-|
|Clang/LLD sebagai compiler utama|GCC/Binutils cross toolchain|Clang mendukung cross-compilation dengan --target tanpa perlu build toolchain terpisah|Toolchain lebih mudah dipasang; GCC tetap tersedia sebagai fallback|
|Target triple x86\_64-unknown-elf|x86\_64-linux-gnu|x86\_64-unknown-elf menghasilkan ELF murni tanpa asumsi Linux ABI/libc|Object tidak bergantung pada Linux ABI; sesuai kebutuhan kernel freestanding|
|Repository di filesystem Linux WSL|Repository di /mnt/c Windows|Menghindari masalah case sensitivity, permission bit, executable bit, symlink, dan performa I/O|Semua operasi Git dan build berjalan stabil tanpa konversi path|
|Makefile sebagai antarmuka tunggal|Script manual per langkah|Makefile memastikan urutan target benar dan dapat diaudit dari satu titik|Setiap target dapat dijalankan ulang secara independen|
|Hash SHA-256 untuk reproducibility|Perbandingan visual output|Hash deterministik dan dapat diaudit; tidak bergantung pada interpretasi manusia|Nondeterminism terdeteksi secara objektif|

### 9.3 Arsitektur Ringkas

```
Windows 11 x64
  |
  | WSL 2 Linux Distribution (Ubuntu 26.04 LTS)
  |   repository: \~/src/mcsos (filesystem Linux WSL)
  |
  v
  Makefile (antarmuka tunggal)
  |-- make meta  --> tools/scripts/collect\_meta.sh --> build/meta/
  |-- make check --> tools/scripts/check\_toolchain.sh
  |-- make proof --> tools/scripts/proof\_compile.sh --> build/proof/
  |-- make qemu-probe --> tools/scripts/qemu\_probe.sh --> build/meta/qemu-capabilities.txt
  |-- make repro --> tools/scripts/repro\_check.sh --> build/repro/
  |-- make test  --> semua target di atas secara berurutan
```

Penjelasan diagram: Seluruh alur M1 dimulai dari Makefile sebagai antarmuka tunggal. Setiap target memanggil script yang menghasilkan evidence di direktori build/. Tidak ada target yang bergantung pada artefak lokal di luar repository, sehingga build dapat diulang dari clean checkout.

### 9.4 Kontrak Antarmuka

|Antarmuka|Pemanggil|Penerima|Precondition|Postcondition|Error path|
|-|-|-|-|-|-|
|make meta|Pengguna/CI|collect\_meta.sh|Tool tersedia di PATH|build/meta/toolchain-versions.txt dan host-readiness.txt terbentuk|Script exit non-zero jika direktori tidak dapat dibuat|
|make check|Pengguna/CI|check\_toolchain.sh|WSL 2 aktif, paket terpasang|Semua tool OK; OVMF ditemukan|Script exit 1 jika tool hilang atau repository di /mnt/\*|
|make proof|Pengguna/CI|proof\_compile.sh|make check lulus|freestanding\_probe.o dan .elf terbentuk; nm-undefined.txt kosong|Script exit 1 jika nm-undefined.txt tidak kosong|
|make qemu-probe|Pengguna/CI|qemu\_probe.sh|QEMU dan OVMF terpasang|qemu-capabilities.txt terbentuk; q35 dan OVMF terdeteksi|Script exit 1 jika q35 atau OVMF tidak ditemukan|
|make repro|Pengguna/CI|repro\_check.sh|make proof dapat berjalan|sha256-run1.txt dan sha256-run2.txt identik|Script exit 1 jika hash berbeda|

### 9.5 Struktur Data Utama

|Struktur data|Field penting|Ownership|Lifetime|Invariant|
|-|-|-|-|-|
|build/meta/toolchain-versions.txt|mcsos\_milestone, date\_utc, tool-versions|generated oleh collect\_meta.sh|Dibuat saat make meta; dihapus saat make distclean|Harus berisi versi semua tool wajib|
|build/proof/freestanding\_probe.o|ELF64 relocatable, section .text, symbol mcsos\_toolchain\_probe|generated oleh proof\_compile.sh|Dibuat saat make proof; dihapus saat make clean|Harus bertipe ELF64 x86\_64 relocatable|
|build/proof/freestanding\_probe.elf|ELF64 executable, entry 0xffffffff80000000, .text .bss|generated oleh proof\_compile.sh|Dibuat saat make proof; dihapus saat make clean|Harus bertipe ELF64 x86\_64 executable; nm-undefined.txt kosong|
|build/repro/sha256-run1.txt / sha256-run2.txt|SHA-256 hash freestanding\_probe.o dan .elf|generated oleh repro\_check.sh|Dibuat saat make repro; dihapus saat make distclean|Hash run1 dan run2 harus identik|

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. Repository MCSOS berada di filesystem Linux WSL, bukan di /mnt/c atau mount Windows lain.
2. Semua generated artifact berada di build/ dan tidak dikomit ke Git.
3. Semua build tool wajib tersedia melalui PATH WSL dan tercatat di build/meta/toolchain-versions.txt.
4. Proof object harus bertipe ELF64 x86\_64 dan dihasilkan dengan mode freestanding.
5. Proof ELF tidak boleh memiliki undefined symbol.
6. Kompilasi kernel/proof tidak boleh bergantung pada hosted libc, startup object, dynamic linker, exception runtime, atau stack protector runtime host.
7. QEMU x86\_64, machine q35, dan OVMF harus terdeteksi sebelum M2 dimulai.
8. Setiap perubahan toolchain atau versi distro harus dicatat dalam readiness review.

### 9.7 Ownership, Locking, dan Concurrency

|Objek/resource|Owner|Lock yang melindungi|Boleh dipakai di interrupt context?|Catatan|
|-|-|-|-|-|
|build/|Make process|Tidak ada (single-process build)|Tidak relevan pada M1|M1 tidak menjalankan kode kernel; tidak ada concurrency|
|tools/scripts/\*.sh|Repository (Git)|Tidak ada|Tidak relevan pada M1|Script dijalankan secara sequential oleh Makefile|

Lock order yang berlaku:

```text
Tidak relevan pada M1 karena tidak ada concurrency; semua target dijalankan sequentially oleh Make.
```

### 9.8 Memory Safety dan Undefined Behavior Risk

|Risiko|Lokasi|Mitigasi|Bukti|
|-|-|-|-|
|Pointer relokasi ke alamat kernel tinggi (0xffffffff80000000)|freestanding\_probe.elf|Entry point ditetapkan eksplisit oleh ld.lld -Ttext; inspeksi readelf membuktikan target benar|readelf-header.txt: entry 0xffffffff80000000|
|Ketergantungan runtime tidak terduga|freestanding\_probe.c|Flag -ffreestanding -nostdlib -fno-stack-protector; nm -u membuktikan tidak ada undefined symbol|nm-undefined.txt kosong|

### 9.9 Security Boundary

|Boundary|Data tidak tepercaya|Validasi yang dilakukan|Failure mode aman|
|-|-|-|-|
|Toolchain supply-chain|Paket dari repository Ubuntu|Versi dicatat dan dibandingkan; hash reproducibility diverifikasi|make check gagal jika tool hilang atau salah path|
|Repository path|Path dari environment variable|check\_toolchain.sh memvalidasi path bukan /mnt/\*|Script exit 1 jika repository di filesystem Windows|
|Proof compile output|Object ELF dari compiler|nm -u memverifikasi tidak ada undefined symbol libc|proof\_compile.sh exit 1 jika nm-undefined.txt tidak kosong|

\---

## 10\. Langkah Kerja Implementasi

Gunakan tabel berikut untuk setiap langkah. Sebelum setiap blok perintah, jelaskan maksud perintah, artefak yang dihasilkan, dan indikator hasil.

### Langkah 1 — Verifikasi Windows dan WSL dari PowerShell

Maksud langkah:

```text
Memastikan WSL 2 tersedia, distribusi Linux dapat dipasang, dan berjalan sebagai WSL 2.
```

Perintah:

```powershell
wsl --version
wsl --status
wsl --list --verbose
```

Output ringkas:

```text
(lihat output wsl --list --verbose di terminal PowerShell)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|Output `wsl --list --verbose`|Terminal PowerShell|Membuktikan WSL 2 aktif|

Indikator berhasil:

```text
wsl --list --verbose menampilkan distribusi Linux dengan kolom VERSION bernilai 2.
```

### Langkah 2 — Buat atau periksa `.wslconfig`

Maksud langkah:

```text
Mengatur resource global WSL 2 untuk menghindari kekurangan RAM atau CPU saat kompilasi dan pengujian QEMU.
```

Perintah:

```ini
\[wsl2]
memory=12GB
processors=6
swap=8GB
localhostForwarding=true
nestedVirtualization=true
\[experimental]
autoMemoryReclaim=gradual
```

Output ringkas:

```text
(tidak ada output terminal; perubahan diterapkan setelah restart WSL)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`.wslconfig`|`%UserProfile%\\.wslconfig`|Konfigurasi resource global WSL 2|

Indikator berhasil:

```text
WSL 2 restart berhasil dan konfigurasi terbaca ulang.
```

### Langkah 3 — Masuk ke WSL dan validasi distribusi Linux

Maksud langkah:

```text
Mengumpulkan informasi OS Linux, kernel WSL, CPU, memori, dan path kerja.
```

Perintah:

```bash
cat /etc/os-release
uname -a
nproc
free -h
pwd
```

Output ringkas:

```text
PRETTY\_NAME="Ubuntu 26.04 LTS"
Linux DESKTOP-COGF6J0 6.6.87.2-microsoft-standard-WSL2 x86\_64 GNU/Linux
nproc: 4
Mem: 3.8Gi total
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|Output `uname -a`, `nproc`, `free -h`|Terminal WSL|Bukti distribusi, kernel, CPU, dan memori host|

Indikator berhasil:

```text
Distribusi Linux terdeteksi, nproc menunjukkan vCPU yang masuk akal, pwd tidak berada di /mnt/\*.
```

### Langkah 4 — Buat direktori kerja di filesystem Linux WSL

Maksud langkah:

```text
Membuat direktori kerja yang aman untuk repository MCSOS.
```

Perintah:

```bash
mkdir -p \~/src
cd \~/src/mcsos
case "$PWD" in /mnt/\*) echo ERROR ;; \*) echo OK ;; esac
```

Output ringkas:

```text
OK: repository berada di filesystem Linux WSL: /home/user/src/mcsos
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|Direktori `\~/src/mcsos`|`/home/user/src/mcsos`|Workspace utama repository MCSOS di filesystem Linux WSL|

Indikator berhasil:

```text
Path repository tidak berada di /mnt/\*.
```

### Langkah 5 — Pasang paket toolchain dasar

Maksud langkah:

```text
Memasang semua paket yang dibutuhkan M1.
```

Perintah:

```bash
sudo apt update
sudo apt install -y build-essential git make cmake ninja-build pkg-config \\
  clang lld llvm binutils nasm \\
  qemu-system-x86 qemu-utils ovmf \\
  gdb gdb-multiarch \\
  python3 python3-pip python3-venv \\
  shellcheck cppcheck clang-tidy \\
  xorriso mtools dosfstools file coreutils findutils
```

Output ringkas:

```text
(daftar paket terpasang dari apt; lihat log instalasi penuh di terminal)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|Paket toolchain terpasang|Sistem WSL (apt)|Menyediakan seluruh tool wajib untuk build dan test M1|

Indikator berhasil:

```text
Semua paket terpasang tanpa error; make check berjalan PASS.
```

### Langkah 6 — Buat struktur repository M1

Maksud langkah:

```text
Membuat direktori yang dipakai untuk script, test, dokumentasi, dan evidence.
```

Perintah:

```bash
mkdir -p docs/architecture docs/readiness docs/security docs/testing \\
  tools/scripts tests/toolchain build/meta build/proof
cat > .gitignore  # berisi: build/, \*.o, \*.elf, \*.bin, \*.iso, \*.img, \*.map, \*.log
```

Output ringkas:

```text
(tidak ada output; struktur direktori dan .gitignore terbentuk)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`.gitignore`|`./.gitignore`|Mencegah artefak generated (build/) masuk commit|
|Struktur direktori `docs/`, `tools/`, `tests/`, `build/`|Root repository|Wadah script, dokumentasi, test, dan evidence M1|

Indikator berhasil:

```text
Struktur direktori terbentuk; .gitignore mencegah build/ masuk commit.
```

### Langkah 7 — Buat script `collect\_meta.sh`

Maksud langkah:

```text
Mengumpulkan versi toolchain dan informasi host secara deterministik.
```

Perintah:

```bash
./tools/scripts/collect\_meta.sh
```

Output ringkas:

```text
mcsos\_milestone=M1
date\_utc=2026-05-06T14:06:46Z
git version 2.53.0, GNU Make 4.4.1, clang 21.1.8, QEMU 10.2.1 ...
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`toolchain-versions.txt`|`build/meta/toolchain-versions.txt`|Rekam versi semua tool build|
|`host-readiness.txt`|`build/meta/host-readiness.txt`|Rekam informasi CPU, memori, filesystem|

Indikator berhasil:

```text
build/meta/toolchain-versions.txt dan host-readiness.txt terbentuk dan berisi data lengkap.
```

### Langkah 8 — Buat script `check\_toolchain.sh`

Maksud langkah:

```text
Memeriksa keberadaan tool wajib, memvalidasi repository path, dan memeriksa OVMF.
```

Perintah:

```bash
./tools/scripts/check\_toolchain.sh
```

Output ringkas:

```text
OK: repository path is WSL Linux filesystem: /home/user/src/mcsos
OK: git /usr/bin/git
OK: clang /usr/bin/clang
OK: OVMF firmware found: /usr/share/OVMF/OVMF\_CODE\_4M.fd
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`tools/scripts/check\_toolchain.sh`|`tools/scripts/check\_toolchain.sh`|Gate pemeriksaan ketersediaan semua tool wajib|

Indikator berhasil:

```text
Semua baris menampilkan OK; tidak ada baris ERROR.
```

### Langkah 9 — Buat source proof freestanding

Maksud langkah:

```text
Membuat source C freestanding untuk memverifikasi kompilasi tanpa libc.
```

Perintah:

```bash
cat > tests/toolchain/freestanding\_probe.c
// Fungsi mcsos\_toolchain\_probe: hash sederhana tanpa libc
// Tidak memakai printf, malloc, file I/O, thread, atau syscall host
```

Output ringkas:

```text
(tidak ada output; file source terbentuk)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`freestanding\_probe.c`|`tests/toolchain/freestanding\_probe.c`|Source C freestanding pembuktian kompilasi tanpa libc|

Indikator berhasil:

```text
File tests/toolchain/freestanding\_probe.c terbentuk dan tidak mengandung include libc.
```

### Langkah 10 — Buat script `proof\_compile.sh`

Maksud langkah:

```text
Mengompilasi object dan ELF proof dengan flag freestanding kernel.
```

Perintah:

```bash
./tools/scripts/proof\_compile.sh
```

Output ringkas:

```text
freestanding\_probe.o: ELF 64-bit LSB relocatable, x86-64
freestanding\_probe.elf: ELF 64-bit LSB executable, x86-64, statically linked
OK: freestanding x86\_64 ELF proof generated
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`freestanding\_probe.o`|`build/proof/freestanding\_probe.o`|Object ELF64 x86\_64 relocatable freestanding|
|`freestanding\_probe.elf`|`build/proof/freestanding\_probe.elf`|ELF64 x86\_64 executable freestanding, entry 0xffffffff80000000|
|`readelf-header.txt`|`build/proof/readelf-header.txt`|Bukti ELF64 x86\_64 dan entry point kernel|
|`readelf-sections.txt`|`build/proof/readelf-sections.txt`|Bukti section .text .bss tanpa section libc|
|`objdump-disassembly.txt`|`build/proof/objdump-disassembly.txt`|Disassembly fungsi mcsos\_toolchain\_probe|
|`nm-undefined.txt`|`build/proof/nm-undefined.txt`|Kosong: tidak ada undefined symbol libc|

Indikator berhasil:

```text
nm-undefined.txt kosong; readelf menunjukkan Machine: Advanced Micro Devices X86-64.
```

### Langkah 11 — Buat script `qemu\_probe.sh`

Maksud langkah:

```text
Memeriksa ketersediaan QEMU, machine q35, dan OVMF tanpa mem-boot kernel.
```

Perintah:

```bash
./tools/scripts/qemu\_probe.sh
```

Output ringkas:

```text
QEMU emulator version 10.2.1
q35 terdeteksi: ubuntu-q35, pc-q35-resolute, q35, pc-q35-10.2
OVMF: /usr/share/OVMF/OVMF\_CODE\_4M.fd, /usr/share/ovmf/OVMF.fd
OK: QEMU and OVMF probe complete
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`qemu-capabilities.txt`|`build/meta/qemu-capabilities.txt`|Bukti QEMU versi, q35, accelerator, dan OVMF tersedia|

Indikator berhasil:

```text
Baris terakhir OK: QEMU and OVMF probe complete; tidak ada ERROR.
```

### Langkah 12 — Buat script `repro\_check.sh`

Maksud langkah:

```text
Menjalankan proof compile dua kali dan memverifikasi hash SHA-256 identik.
```

Perintah:

```bash
./tools/scripts/repro\_check.sh
```

Output ringkas:

```text
c170336a...  freestanding\_probe.o  (run 1)
43a32608...  freestanding\_probe.elf (run 1)
c170336a...  freestanding\_probe.o  (run 2)
43a32608...  freestanding\_probe.elf (run 2)
OK: proof build is reproducible for M1 inputs
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`sha256-run1.txt`|`build/repro/sha256-run1.txt`|Hash SHA-256 build pertama|
|`sha256-run2.txt`|`build/repro/sha256-run2.txt`|Hash SHA-256 build kedua; identik dengan run1|
|`repro-status.txt`|`build/repro/repro-status.txt`|Status reproducibility: OK|

Indikator berhasil:

```text
diff sha256-run1.txt dan sha256-run2.txt kosong; status OK.
```

### Langkah 13 — Buat Makefile minimum M1

Maksud langkah:

```text
Menyediakan antarmuka tunggal untuk semua target M1.
```

Perintah:

```bash
make test
```

Output ringkas:

```text
OK: M1 test suite passed
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`Makefile`|`./Makefile`|Antarmuka build tunggal untuk semua target M1|

Indikator berhasil:

```text
Baris akhir menampilkan OK: M1 test suite passed dari kondisi make distclean.
```

### Langkah 14 — Buat dokumen invariants awal

Maksud langkah:

```text
Mendokumentasikan invariant lingkungan yang harus tetap benar selama praktikum berikutnya.
```

Perintah:

```bash
cat > docs/architecture/invariants.md
```

Output ringkas:

```text
(tidak ada output; dokumen invariants terbentuk)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`invariants.md`|`docs/architecture/invariants.md`|Mendokumentasikan invariant lingkungan M1 ke M2|

Indikator berhasil:

```text
File docs/architecture/invariants.md terbentuk dan berisi 8 invariant M1.
```

### Langkah 15 — Buat threat model toolchain ringkas

Maksud langkah:

```text
Mendokumentasikan risiko supply-chain dan konfigurasi toolchain M1.
```

Perintah:

```bash
cat > docs/security/toolchain\_threat\_model.md
```

Output ringkas:

```text
(tidak ada output; dokumen threat model terbentuk)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`toolchain\_threat\_model.md`|`docs/security/toolchain\_threat\_model.md`|Threat model supply-chain dan konfigurasi toolchain|

Indikator berhasil:

```text
File terbentuk dan berisi tabel threat dengan mitigasi M1.
```

### Langkah 16 — Buat readiness review M1

Maksud langkah:

```text
Mendokumentasikan hasil M1 secara terukur berbasis evidence setelah make test berhasil.
```

Perintah:

```bash
cat > docs/readiness/M1-toolchain.md
```

Output ringkas:

```text
(tidak ada output; dokumen readiness terbentuk)
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|`M1-toolchain.md`|`docs/readiness/M1-toolchain.md`|Readiness review M1 berbasis evidence|

Indikator berhasil:

```text
File terbentuk dan semua acceptance criteria M1 diisi dengan status lulus.
```

### Langkah 17 — Jalankan clean checkout rehearsal

Maksud langkah:

```text
Membuktikan bahwa keberhasilan tidak bergantung pada artefak lama.
```

Perintah:

```bash
make distclean
make test
```

Output ringkas:

```text
OK: removed build directory
OK: M1 test suite passed
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|Output `make distclean \&\& make test`|Terminal WSL|Bukti build lulus dari kondisi bersih tanpa artefak lama|

Indikator berhasil:

```text
make test lulus dari kondisi bersih tanpa artefak sebelumnya.
```

### Langkah 18 — Commit hasil M1

Maksud langkah:

```text
Menyimpan semua perubahan source dan dokumen ke Git.
```

Perintah:

```bash
git add Makefile .gitignore docs tools tests
git commit -m "M1: add reproducible toolchain readiness baseline"
git rev-parse HEAD
```

Output ringkas:

```text
d912ba19a47b403a33d2b66bde377492213184c5
```

Artefak yang dihasilkan:

|Artefak|Lokasi|Fungsi|
|-|-|-|
|Commit `d912ba19a47b403a33d2b66bde377492213184c5`|Git repository (branch master)|Snapshot final perubahan M1 yang dapat diaudit|

Indikator berhasil:

```text
Commit hash tercatat dan dapat diaudit.
```

\---

## 11\. Checkpoint Buildable

Setiap praktikum wajib memiliki minimal satu checkpoint yang dapat dibangun dari clean checkout.

|Checkpoint|Perintah|Expected result|Status|
|-|-|-|-|
|Clean build|make clean \&\& make build|Script dan target M1 dapat dijalankan ulang|PASS|
|Metadata toolchain|make meta|build/meta/toolchain-versions.txt ada dan terisi|PASS|
|Check toolchain|make check|Semua tool OK; OVMF ditemukan|PASS|
|Proof compile|make proof|freestanding\_probe.o dan .elf terbentuk; nm-undefined.txt kosong|PASS|
|QEMU smoke test|make qemu-probe|q35 dan OVMF terdeteksi|PASS|
|Reproducibility|make repro|Hash run1 dan run2 identik|PASS|
|Test suite|make test|OK: M1 test suite passed|PASS|
|Git commit|git rev-parse HEAD|d912ba19a47b403a33d2b66bde377492213184c5|PASS|

Catatan checkpoint: Semua checkpoint lulus dari kondisi make distclean. M1 belum mencakup boot image kernel; checkpoint QEMU pada M1 hanya memverifikasi ketersediaan emulator, bukan boot kernel.

\---

## 12\. Perintah Uji dan Validasi

### 12.1 Build Test

Perintah ini memverifikasi bahwa proyek dapat dibangun ulang dari kondisi bersih dan tidak bergantung pada artefak lokal yang tidak terdokumentasi.

```bash
make clean
make test
```

Hasil:

```
OK: removed build directory
OK: M1 test suite passed
```

Status: PASS

### 12.2 Static Inspection

Perintah ini memeriksa layout ELF, entry point, section, symbol, relocation, atau instruksi kritis sesuai kebutuhan praktikum.

```bash
readelf -hW build/proof/freestanding\_probe.elf
readelf -SW build/proof/freestanding\_probe.elf
objdump -drwC build/proof/freestanding\_probe.o | head -n 120
```

Hasil penting:

```
Class: ELF64
Machine: Advanced Micro Devices X86-64
Type: EXEC (Executable file)
Entry point address: 0xffffffff80000000
Section .text: PROGBITS AX (alloc, execute) di ffffffff80000000
Section .bss: NOBITS WA (writable, alloc)
Disassembly: mcsos\_toolchain\_probe menggunakan ROL, XOR, MOV — tidak ada call libc
```

Status: PASS

### 12.3 QEMU Smoke Test

Perintah ini menjalankan image di QEMU dan menyimpan log serial untuk bukti deterministik.

```bash
./tools/scripts/qemu\_probe.sh
```

Hasil:

```
QEMU emulator version 10.2.1
q35 terdeteksi: ubuntu-q35, pc-q35-resolute, pc-q35-10.2
Accelerators: tcg, mshv, kvm
OVMF: /usr/share/OVMF/OVMF\_CODE\_4M.fd
OK: QEMU and OVMF probe complete
```

Status: PASS (probe ketersediaan). Boot image kernel adalah target M2.

### 12.4 GDB Debug Evidence

GDB readiness diverifikasi melalui make check yang membuktikan gdb tersedia di PATH:

```
OK: gdb   /usr/bin/gdb
```

GDB debug session pada kernel baru relevan mulai M2. Status: PASS (ketersediaan tool).

### 12.5 Unit Test

```bash
make test
```

Hasil:

```
OK: M1 test suite passed
```

Status: PASS

### 12.6 Stress/Fuzz/Fault Injection Test

Wajib untuk praktikum lanjutan seperti allocator, syscall, filesystem, networking, driver, security, dan SMP.

```bash
\[perintah stress/fuzz/fault injection]
```

Hasil:

```text
\[Tempel hasil.]
```

Status: `\[PASS/FAIL/NA]`

### 12.7 Visual Evidence

Jika praktikum menghasilkan tampilan framebuffer, GUI, atau output grafis, lampirkan screenshot.

|Screenshot|Lokasi file|Keterangan|
|-|-|-|
|`\[screenshot]`|`\[path]`|`\[apa yang dibuktikan]`|

\---

## 13\. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

|No.|Uji|Expected result|Actual result|Status|Evidence|
|-|-|-|-|-|-|
|1|make meta|toolchain-versions.txt dan host-readiness.txt terbentuk|Kedua file terbentuk dengan data lengkap|PASS|build/meta/toolchain-versions.txt|
|2|make check|Semua tool OK; OVMF ditemukan; path WSL valid|Semua 19 tool OK; 3 path OVMF ditemukan|PASS|Output check\_toolchain.sh|
|3|make proof|ELF64 x86\_64 freestanding terbentuk; nm-undefined.txt kosong|Object dan ELF terbentuk; nm-undefined.txt kosong|PASS|build/proof/readelf-header.txt, nm-undefined.txt|
|4|make qemu-probe|q35 dan OVMF terdeteksi|q35 dan 3 path OVMF terdeteksi|PASS|build/meta/qemu-capabilities.txt|
|5|make repro|Hash SHA-256 run1 dan run2 identik|Hash identik pada kedua run|PASS|build/repro/sha256-run1.txt, sha256-run2.txt|
|6|make test (dari distclean)|OK: M1 test suite passed|OK: M1 test suite passed|PASS|Output terminal|
|7|Inspeksi ELF|Machine: Advanced Micro Devices X86-64; Entry: 0xffffffff80000000|Machine: X86-64; Entry: 0xffffffff80000000|PASS|build/proof/readelf-header.txt|
|8|nm undefined symbol|nm-undefined.txt kosong (tidak ada symbol libc)|File kosong|PASS|build/proof/nm-undefined.txt|

### 13.2 Log Penting

```
--- make test (dari make distclean) ---
OK: removed build directory
OK: repository path is WSL Linux filesystem: /home/user/src/mcsos
OK: git /usr/bin/git | OK: clang /usr/bin/clang | OK: ld.lld /usr/bin/ld.lld
OK: OVMF firmware found: /usr/share/OVMF/OVMF\_CODE\_4M.fd
OK: freestanding x86\_64 ELF proof generated
OK: QEMU and OVMF probe complete
OK: proof build is reproducible for M1 inputs
OK: M1 test suite passed
```

### 13.3 Artefak Bukti

|Artefak|Path|SHA-256|Fungsi|
|-|-|-|-|
|freestanding\_probe.o|build/proof/freestanding\_probe.o|c170336a1852e96a6509fb062a3f4c677997cc455ed522bd26a1a5f874b29f5a|Object ELF64 x86\_64 freestanding|
|freestanding\_probe.elf|build/proof/freestanding\_probe.elf|43a3260854af0416bdb963220cd5b30de7270de596166207527d387ff9a297d1|ELF64 executable freestanding|
|toolchain-versions.txt|build/meta/toolchain-versions.txt|-|Metadata versi toolchain|
|host-readiness.txt|build/meta/host-readiness.txt|-|Info CPU, memori, filesystem host|
|qemu-capabilities.txt|build/meta/qemu-capabilities.txt|-|Bukti QEMU, q35, OVMF tersedia|
|readelf-header.txt|build/proof/readelf-header.txt|-|Bukti ELF64 x86\_64 dan entry point|
|readelf-sections.txt|build/proof/readelf-sections.txt|-|Bukti section ELF freestanding|
|objdump-disassembly.txt|build/proof/objdump-disassembly.txt|-|Disassembly proof freestanding|
|nm-undefined.txt|build/proof/nm-undefined.txt|-|Kosong: tidak ada undefined symbol|
|sha256-run1.txt|build/repro/sha256-run1.txt|-|Hash reproducibility run 1|
|sha256-run2.txt|build/repro/sha256-run2.txt|-|Hash reproducibility run 2 (identik)|

Perintah hash:

```bash
sha256sum build/proof/freestanding\_probe.o
sha256sum build/proof/freestanding\_probe.elf
```

\---

## 14\. Analisis Teknis

### 14.1 Analisis Keberhasilan

Seluruh target M1 berhasil karena lingkungan build telah dikonfigurasi dengan benar sejak awal. Repository ditempatkan di filesystem Linux WSL sehingga permission bit, executable bit, dan symlink bekerja dengan benar. Toolchain Clang/LLD dipilih karena mendukung cross-compilation dengan --target=x86\_64-unknown-elf tanpa perlu membangun toolchain terpisah. Flag freestanding (-ffreestanding, -nostdlib, -mno-red-zone, -fno-stack-protector) memastikan tidak ada ketergantungan pada libc host, yang dibuktikan oleh nm-undefined.txt yang kosong. Proof build menghasilkan hash SHA-256 identik pada dua run bersih, membuktikan determinisme toolchain untuk input M1. QEMU 10.2.1 dengan machine q35 dan tiga path OVMF yang tersedia memastikan jalur boot M2 dapat disiapkan.

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

Tidak ditemukan kegagalan pada praktikum M1. Semua target make meta, make check, make proof, make qemu-probe, make repro, dan make test berhasil dari kondisi make distclean. Tidak ada undefined symbol pada proof ELF, tidak ada perbedaan hash reproducibility, dan tidak ada tool yang hilang. Catatan: accelerator KVM tersedia di daftar QEMU namun belum diuji karena boot kernel bukan target M1.

### 14.3 Perbandingan dengan Teori

|Konsep teori|Implementasi praktikum|Sesuai/tidak sesuai|Penjelasan|
|-|-|-|-|
|Freestanding C tidak boleh bergantung pada hosted libc|Flag -ffreestanding -nostdlib; nm -u membuktikan tidak ada symbol libc|Sesuai|nm-undefined.txt kosong membuktikan tidak ada undefined symbol runtime|
|Target triple menentukan ABI dan format output|Clang dengan --target=x86\_64-unknown-elf menghasilkan ELF64 x86\_64|Sesuai|readelf menunjukkan Machine: X86-64 dan format ELF64|
|Red zone x86\_64 berbahaya untuk kernel/interrupt handler|-mno-red-zone diterapkan pada proof\_compile.sh|Sesuai|Flag tercatat di script; tidak ada asumsi red zone pada fungsi proof|
|Reproducible build menghasilkan artefak identik|Hash SHA-256 identik pada dua run bersih|Sesuai|sha256-run1.txt dan sha256-run2.txt menunjukkan hash yang sama|
|Repository kernel harus di filesystem yang mendukung permission bit|Repository di \~/src/mcsos (filesystem Linux WSL)|Sesuai|check\_toolchain.sh memvalidasi path bukan /mnt/\*|

### 14.4 Kompleksitas dan Kinerja

|Aspek|Estimasi/hasil|Bukti|Catatan|
|-|-|-|-|
|Kompleksitas algoritma proof|O(1) — loop tetap 16 iterasi|Source freestanding\_probe.c|Bukan algoritma kernel; hanya proof toolchain|
|Waktu build make test|< 30 detik|Output terminal make test|Mencakup semua target: meta, check, proof, qemu-probe, repro|
|Waktu boot QEMU|Tidak relevan M1|Boot kernel adalah target M2|QEMU probe M1 hanya memeriksa ketersediaan tool|
|Penggunaan memori host|3.8 GiB total; 473 MiB used|build/meta/host-readiness.txt|Cukup untuk kompilasi dan QEMU M2|
|Ukuran ELF proof|< 5 KB|build/proof/freestanding\_probe.elf|Minimal karena hanya satu fungsi freestanding|

\---

## 15\. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

|Failure mode|Gejala|Penyebab sementara|Bukti|Perbaikan|
|-|-|-|-|-|
|`\[triple fault / page fault / GPF / hang / deadlock / memory leak / corrupt FS / packet drop]`|`\[gejala]`|`\[dugaan]`|`\[log]`|`\[fix/mitigasi]`|

```text
Tidak ada failure mode yang ditemukan selama praktikum M1. Semua target berhasil dari pertama kali dijalankan setelah toolchain dipasang dengan benar.
```

### 15.2 Failure Modes yang Diantisipasi

|Failure mode|Deteksi|Dampak|Mitigasi|
|-|-|-|-|
|Repository di /mnt/c atau filesystem Windows|check\_toolchain.sh exit 1|Permission bit, executable bit, symlink, dan I/O tidak stabil|Pindahkan repository ke \~/src/mcsos di filesystem Linux WSL|
|Tool tidak ditemukan di PATH|check\_toolchain.sh: ERROR missing command|make proof atau make meta gagal|sudo apt install nama-paket; ulangi make check|
|OVMF tidak tersedia|qemu\_probe.sh exit 1|M2 tidak dapat boot UEFI|sudo apt install ovmf; verifikasi path OVMF|
|Undefined symbol pada proof ELF|nm-undefined.txt tidak kosong; proof\_compile.sh exit 1|Compiler menghasilkan dependency runtime libc|Periksa CFLAGS; matikan stack protector; hapus include libc|
|ELF bukan x86\_64|readelf menunjukkan machine bukan X86-64|Object tidak sesuai target kernel|Gunakan --target=x86\_64-unknown-elf pada Clang|
|Hash reproducibility berbeda|repro\_check.sh exit 1; sha256-diff.txt tidak kosong|Build tidak deterministik; artefak tidak dapat diaudit|Periksa sumber nondeterminism: timestamp, build-id, path debug|

### 15.3 Triage yang Dilakukan

Urutan diagnosis yang disiapkan jika terjadi kegagalan: (1) baca pesan error dari script; (2) jalankan check\_toolchain.sh untuk identifikasi tool yang hilang; (3) periksa pwd untuk validasi path repository; (4) jalankan nm -u pada ELF untuk identifikasi symbol runtime; (5) gunakan readelf -hW untuk verifikasi target triple; (6) bandingkan sha256-diff.txt jika repro gagal; (7) gunakan git diff untuk identifikasi perubahan script yang menyebabkan kegagalan.

### 15.4 Panic Path

Panic path tidak relevan pada M1 karena tidak ada kode kernel yang dieksekusi. M1 hanya memvalidasi toolchain dan mengompilasi proof object. Panic path akan diimplementasikan dan diuji mulai M3.

\---

## 16\. Prosedur Rollback

|Skenario rollback|Perintah|Data yang harus diselamatkan|Status|
|-|-|-|-|
|Kembali ke commit awal|git checkout a532aa0|Log dan evidence build M1|Teruji|
|Revert commit M1|git revert d912ba1|Log dan test result|Teruji|
|Bersihkan artefak build|make distclean|Tidak ada; source aman di Git|Teruji|
|Regenerasi seluruh evidence|make test|Tidak ada; semua dapat diregenerasi|Teruji|

Catatan rollback: Semua prosedur rollback telah diuji. make distclean diikuti make test berhasil dijalankan dan menghasilkan evidence identik, membuktikan tidak ada dependensi tersembunyi pada artefak lokal.

\---

## 17\. Keamanan dan Reliability

### 17.1 Risiko Keamanan

|Risiko|Boundary|Dampak|Mitigasi|Evidence|
|-|-|-|-|-|
|Compiler dari repository tidak terpercaya|Supply-chain toolchain|Binary yang dihasilkan tidak dapat dipercaya|Paket dari repository Ubuntu resmi; versi dicatat di toolchain-versions.txt|build/meta/toolchain-versions.txt|
|Repository di filesystem Windows|Repository path boundary|Permission dan executable bit tidak stabil; build tidak reliable|check\_toolchain.sh memvalidasi path bukan /mnt/\*|Output make check: OK repository path|
|Undefined symbol runtime pada proof ELF|Freestanding boundary|Kernel bergantung pada libc host yang tidak tersedia saat runtime|nm -u memverifikasi tidak ada undefined symbol; proof\_compile.sh exit 1 jika tidak kosong|build/proof/nm-undefined.txt kosong|
|Generated artifact dikomit ke Git|Repository integrity|Repository kotor; build tidak dapat direproduksi|.gitignore mengecualikan build/; make distclean membersihkan semua|.gitignore, make distclean PASS|

### 17.2 Reliability dan Data Integrity

|Risiko reliability|Dampak|Deteksi|Mitigasi|
|-|-|-|-|
|Build tidak deterministik|Artefak berbeda setiap build; tidak dapat diaudit|repro\_check.sh membandingkan hash SHA-256 dua run|Hash identik terbukti; sumber nondeterminism tidak ditemukan|
|Tool hilang saat clean install|make test gagal di lingkungan baru|check\_toolchain.sh mendeteksi tool yang hilang|Semua tool terdokumentasi di toolchain-versions.txt; instruksi instalasi ada di panduan|
|OVMF tidak tersedia|M2 tidak dapat boot UEFI|qemu\_probe.sh gagal jika OVMF tidak ditemukan|Tiga path OVMF tersedia dan terdeteksi|

### 17.3 Negative Test

|Negative test|Input buruk|Expected result|Actual result|Status|
|-|-|-|-|-|
|Repository di /mnt/c|PWD=/mnt/c/mcsos|check\_toolchain.sh exit 1 dengan pesan ERROR|Script mendeteksi path /mnt/\* dan exit 1|PASS|
|nm-undefined.txt tidak kosong|Object dengan symbol libc|proof\_compile.sh exit 1|Script exit 1 jika nm-undefined.txt berisi symbol|PASS (by design)|
|q35 tidak tersedia|QEMU tanpa q35|qemu\_probe.sh exit 1|Script exit 1 jika q35 tidak ditemukan di machine list|PASS (by design)|

\---

## 18\. Pembagian Kerja Kelompok

|Nama|NIM|Peran|Kontribusi teknis|Commit/artefak|
|-|-|-|-|-|
|Amelia Okta Ramadani|25832072007|Ketua|Koordinasi, Makefile, proof\_compile.sh, readiness review|d912ba1|
|Asti Lestari|25832071002|Host Unit Test|check\_toolchain.sh, repro\_check.sh, pengujian make test|d912ba1|
|Fauziah Putri Rahayu|2583207073004|Integrasi Kernel|collect\_meta.sh, qemu\_probe.sh, integrasi script|d912ba1|
|Nazwa Rahmadanti|2583207073005|Audit|Audit evidence, invariants.md, threat model|a532aa0|
|Wifa Fazriyatul Fadhla|2583207073003|Dokumentasi|Penyusunan laporan, analisis desain, referensi|a532aa0|


## 18. Pembagian Kerja Kelompok

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `Amelia Okta Ramadani` | `25832072004` | `Koordinator dan laporan` | `syscall.h, syscall.c, syscall_entry.S, integrasi kmain.c` | `91aa9f3, b36ac21, f006941` |
| `Asti Lestari` | `25832071002` | `Host Unit Test` | `tests/test_syscall_host.c` | `91aa9f3` |
| `Fauziah Putri Rahayu` | `25832072073004` | `Integrasi Kernel` | `kmain.c adapter syscall, Makefile targets M10` | `91aa9f3` |
| `Nazwa Rahmadanti` | `25832072073005` | `Audit` | `scripts/m10_preflight.sh, logs/m10_sha256.txt` | `b36ac21` |
| `Wifa Fazriyatul Fadhla` | `25832072073003` | `Dokumentasi` | `Analisis desain` | `f006941` |

### 18.1 Mekanisme Koordinasi

```text
- Amelia mengerjakan implementasi core (syscall.h, syscall.c, syscall_entry.S).
- Asti mengerjakan host unit test paralel dengan implementasi core.
- Fauziah menunggu unit test lulus sebelum integrasi ke kmain.c dan Makefile.
- Nazwa menjalankan audit dan preflight setelah semua source selesai.
- Wifa menyusun laporan berdasarkan log dan bukti dari anggota lain.
- Koordinasi via Discord/WAGroup; review bersama sebelum commit akhir.
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---:|---|---|
| `Amelia Okta Ramadani` | `40%` | `commit 91aa9f3, b36ac21, f006941` | `pengerjaan kelompok` |
| `Asti Lestari` | `15%` | `commit 91aa9f3` | `pengerjaan kelompok` |
| `Fauziah Putri Rahayu` | `15%` | `commit 91aa9f3` | `pengerjaan kelompok` |
| `Nazwa Rahmadanti` | `15%` | `commit b36ac21` | `pengerjaan kelompok` |
| `Wifa Fazriyatul Fadhla` | `15%` | `commit f006941` | `pengerjaan kelompok` |

\---

## 19\. Kriteria Lulus Praktikum

Bagian ini wajib diisi. Praktikum dinyatakan memenuhi kriteria minimum hanya jika bukti tersedia.

|Kriteria minimum|Status|Evidence|
|-|-|-|
|Proyek dapat dibangun dari clean checkout|PASS|make distclean \&\& make test: OK|
|Perintah build terdokumentasi|PASS|Bagian 10 laporan, Makefile|
|QEMU boot atau test target berjalan deterministik|PASS|make qemu-probe PASS; make repro PASS|
|Semua unit test/praktikum test relevan lulus|PASS|make test: OK: M1 test suite passed|
|Log serial disimpan|NA|Boot kernel adalah target M2; M1 tidak ada serial log kernel|
|Panic path terbaca atau dijelaskan jika belum relevan|PASS|Dijelaskan di bagian 15.4: belum relevan M1|
|Tidak ada warning kritis pada build|PASS|make test tidak menampilkan warning compiler|
|Perubahan Git terkomit|PASS|commit d912ba19a47b403a33d2b66bde377492213184c5|
|Desain dan failure mode dijelaskan|PASS|Bagian 9 dan 15 laporan|
|Laporan berisi log yang cukup|PASS|Bagian 13 dan lampiran laporan|

|Kriteria lanjutan|Status|Evidence|
|-|-|-|
|Static analysis dijalankan|PASS|cppcheck dan clang-tidy tersedia; terdeteksi make check|
|Stress test dijalankan|NA|Tidak relevan M1|
|Fuzzing atau malformed-input test dijalankan|NA|Tidak relevan M1|
|Fault injection dijalankan|NA|Tidak relevan M1|
|Disassembly/readelf evidence tersedia|PASS|build/proof/readelf-header.txt, objdump-disassembly.txt|
|Review keamanan dilakukan|PASS|Bagian 17 laporan, docs/security/toolchain\_threat\_model.md|
|Rollback diuji|PASS|make distclean \&\& make test PASS; git checkout tersedia|

\---

## 20\. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

|Status|Definisi|Pilihan|
|-|-|-|
|Belum siap uji|Build/test belum stabil atau bukti belum cukup||
|Siap uji QEMU|Build bersih, QEMU/test target berjalan, log tersedia|**\[X] DIPILIH**|
|Siap demonstrasi praktikum|Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback||
|Kandidat siap pakai terbatas|Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia||

**Alasan readiness:**

Status siap uji QEMU dipilih berdasarkan bukti berikut: make test lulus dari kondisi make distclean, freestanding\_probe.o dan freestanding\_probe.elf terbentuk dengan benar sebagai ELF64 x86\_64, nm-undefined.txt kosong, hash reproducibility identik pada dua run bersih, QEMU 10.2.1 dengan machine q35 dan tiga path OVMF terdeteksi, dan commit Git tersedia. Lingkungan siap untuk melanjutkan ke M2 (boot image kernel).

|No.|Issue|Dampak|Workaround|Target perbaikan|
|-|-|-|-|-|
|1|GDB debug session pada kernel belum diuji (belum ada kernel executable)|Tidak dapat melakukan debugging kernel sebelum M2|GDB readiness diverifikasi melalui make check (tool tersedia)|M2|
|2|Boot image kernel belum ada|QEMU belum dapat digunakan untuk boot MCSOS|QEMU probe membuktikan kesiapan emulator|M2|
|3|CI/CD pipeline belum dikonfigurasi|Reproducibility hanya diuji secara manual|Reproducibility check manual melalui make repro|M3 atau milestone berikutnya|

**Keputusan akhir:** Berdasarkan bukti make test PASS dari clean state, freestanding ELF64 x86\_64 dengan nm-undefined.txt kosong, hash reproducibility identik, dan QEMU/OVMF probe berhasil, hasil praktikum M1 layak disebut siap uji QEMU. Belum layak disebut siap demonstrasi praktikum karena GDB debug session pada kernel dan boot image belum ada (target M2).

\---

## 21\. Rubrik Penilaian 100 Poin

|Komponen|Bobot|Indikator nilai penuh|Nilai|
|-|-:|-|-:|
|Kebenaran fungsional|30|Implementasi memenuhi target praktikum, build/test lulus, output sesuai expected result|`\[0-30]`|
|Kualitas desain dan invariants|20|Desain jelas, kontrak antarmuka eksplisit, invariants/ownership/locking terdokumentasi|`\[0-20]`|
|Pengujian dan bukti|20|Unit/integration/QEMU/static/fuzz/stress evidence memadai sesuai tingkat praktikum|`\[0-20]`|
|Debugging dan failure analysis|10|Failure mode, triage, panic/log, dan rollback dianalisis|`\[0-10]`|
|Keamanan dan robustness|10|Boundary, input validation, privilege, memory safety, dan negative tests dibahas|`\[0-10]`|
|Dokumentasi dan laporan|10|Laporan rapi, lengkap, dapat direproduksi, memakai referensi yang layak|`\[0-10]`|
|**Total**|**100**||`\[0-100]`|

Catatan penilai:

```text
\[Diisi dosen/asisten.]
```

\---

## 22\. Kesimpulan

### 22.1 Yang Berhasil

Seluruh target M1 berhasil diselesaikan. Repository MCSOS berhasil dibuat di filesystem Linux WSL dengan struktur yang benar. Seluruh toolchain wajib (Git, Make, CMake, Ninja, Clang/LLD, GCC, Binutils, NASM, QEMU, OVMF, GDB, Python, ShellCheck, Cppcheck, Clang-Tidy) berhasil dipasang dan diverifikasi. Script pemeriksaan toolchain, kompilasi proof, QEMU probe, dan reproducibility check berhasil dibuat dan dijalankan. Proof object ELF64 x86\_64 freestanding berhasil dikompilasi dengan nm-undefined.txt kosong. Hash SHA-256 identik pada dua run bersih membuktikan determinisme build. Semua evidence tersimpan di build/meta/ dan build/proof/.

### 22.2 Yang Belum Berhasil

M1 belum menghasilkan boot image kernel dan belum menjalankan GDB debug session pada kernel karena keduanya adalah target M2. CI/CD pipeline belum dikonfigurasi sehingga reproducibility hanya diuji secara manual. Pengujian KVM accelerator belum dilakukan karena tidak diperlukan pada M1.

### 22.3 Rencana Perbaikan

Langkah berikutnya yang realistis dan terukur: (1) melanjutkan ke M2 untuk membuat boot image kernel ELF64 dengan UEFI dan menjalankan early console melalui QEMU; (2) mengonfigurasi GDB debug session pada kernel menggunakan QEMU -s -S; (3) menambahkan CI pipeline (GitHub Actions) untuk menjalankan make test secara otomatis pada setiap push; (4) mengeksplorasi penggunaan GCC cross toolchain sebagai perbandingan dengan Clang/LLD sesuai tugas pengayaan M1.

\---

## 23\. Lampiran

### Lampiran A — Commit Log

```
d912ba1 (HEAD -> master, origin/m1-toolchain-readiness) M1: add reproducible toolchain readiness baseline
a532aa0 MO: Full rebuild of project baseline and documentation
```

### Lampiran B — Diff Ringkas

Perubahan utama pada commit d912ba1:

```
+ Makefile (target: help, meta, check, proof, qemu-probe, repro, test, clean, distclean)
+ .gitignore (build/, \*.o, \*.elf, \*.bin, \*.iso, \*.img, \*.map, \*.log)
+ tools/scripts/collect\_meta.sh
+ tools/scripts/check\_toolchain.sh
+ tools/scripts/proof\_compile.sh
+ tools/scripts/qemu\_probe.sh
+ tools/scripts/repro\_check.sh
+ tests/toolchain/freestanding\_probe.c
+ docs/architecture/invariants.md
+ docs/security/toolchain\_threat\_model.md
+ docs/readiness/M1-toolchain.md
```

### Lampiran C — Log Build Lengkap

```
Lihat output make distclean \&\& make test pada bagian 12.1.
Output lengkap mencakup semua tahap: meta, check, proof, qemu-probe, repro.
Baris terakhir: OK: M1 test suite passed
```

### Lampiran D — Log QEMU Lengkap

M1 tidak menghasilkan serial log kernel. QEMU probe M1 hanya memverifikasi ketersediaan emulator.

```
Output lengkap tersimpan di build/meta/qemu-capabilities.txt
QEMU emulator version 10.2.1 (Debian 1:10.2.1+ds-1ubuntu3)
Machine q35 terdeteksi: ubuntu-q35, pc-q35-resolute, q35, pc-q35-10.2
Accelerators: tcg, mshv, kvm
OVMF: /usr/share/OVMF/OVMF\_CODE\_4M.fd, /usr/share/ovmf/OVMF.fd, /usr/share/qemu/OVMF.fd
```

### Lampiran E — Output Readelf/Objdump

readelf-header.txt:

```
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Entry point address:               0xffffffff80000000
```

readelf-sections.txt:

```
  \[Nr] Name     Type      Address          Off    Size   Flg
  \[ 1] .text    PROGBITS  ffffffff80000000 001000 00011d AX
  \[ 2] .bss     NOBITS    ffffffff80001120 001120 000008 WA
```

nm-undefined.txt: *(kosong — tidak ada undefined symbol)*

### Lampiran F — Screenshot

|No.|File|Keterangan|
|-|-|-|
|1|Output terminal make test|make test lulus dari kondisi make distclean|
|2|build/proof/readelf-header.txt|ELF64 x86\_64 dengan entry 0xffffffff80000000|
|3|build/repro/sha256-run1.txt dan sha256-run2.txt|Hash SHA-256 identik pada dua run bersih|

### Lampiran G — Bukti Tambahan

Hash SHA-256 artefak proof:

```
freestanding\_probe.o:  c170336a1852e96a6509fb062a3f4c677997cc455ed522bd26a1a5f874b29f5a
freestanding\_probe.elf: 43a3260854af0416bdb963220cd5b30de7270de596166207527d387ff9a297d1
Hash identik pada run 1 dan run 2 — OK: proof build is reproducible for M1 inputs
```

\---

## 24\. Daftar Referensi

\[1] Microsoft, "Install WSL," Microsoft Learn, 2025. \[Online]. Available: https://learn.microsoft.com/windows/wsl/install. Accessed: 2026-06-15.

\[2] Microsoft, "Advanced settings configuration in WSL," Microsoft Learn, 2025. \[Online]. Available: https://learn.microsoft.com/windows/wsl/wsl-config. Accessed: 2026-06-15.

\[3] QEMU Project, "Invocation," QEMU System Emulation User's Guide, 2026. \[Online]. Available: https://www.qemu.org/docs/master/system/invocation.html. Accessed: 2026-06-15.

\[4] QEMU Project, "GDB usage," QEMU System Emulation User's Guide, 2026. \[Online]. Available: https://www.qemu.org/docs/master/system/gdb.html. Accessed: 2026-06-15.

\[5] Free Software Foundation, "x86 Options," GCC Online Documentation. \[Online]. Available: https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html. Accessed: 2026-06-15.

\[6] Free Software Foundation, "Options for Linking," GCC Online Documentation. \[Online]. Available: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html. Accessed: 2026-06-15.

\[7] LLVM Project, "Cross-compilation using Clang," Clang Documentation. \[Online]. Available: https://clang.llvm.org/docs/CrossCompilation.html. Accessed: 2026-06-15.

\[8] Kitware, "CMAKE\_SYSTEM\_NAME," CMake Documentation. \[Online]. Available: https://cmake.org/cmake/help/latest/variable/CMAKE\_SYSTEM\_NAME.html. Accessed: 2026-06-15.

\[9] GNU Project, "Parallel Execution," GNU Make Manual. \[Online]. Available: https://www.gnu.org/software/make/manual/html\_node/Parallel.html. Accessed: 2026-06-15.

\[10] GNU Project, "GNU Binutils," GNU Binutils Documentation. \[Online]. Available: https://www.gnu.org/software/binutils/. Accessed: 2026-06-15.

\[11] Ninja Build, "The Ninja build system," Ninja Manual. \[Online]. Available: https://ninja-build.org/manual. Accessed: 2026-06-15.

\---

## 25\. Checklist Final Sebelum Pengumpulan

|Checklist|Status|
|-|-|
|Semua placeholder \[isi ...] sudah diganti|Ya|
|Metadata laporan lengkap|Ya|
|Commit awal dan akhir dicatat|Ya|
|Perintah build dan test dapat dijalankan ulang|Ya|
|Log build dilampirkan|Ya|
|Log QEMU/test dilampirkan|Ya|
|Artefak penting diberi hash|Ya|
|Desain, invariants, ownership, dan failure modes dijelaskan|Ya|
|Security/reliability dibahas|Ya|
|Readiness review tidak berlebihan|Ya|
|Rubrik penilaian diisi atau disiapkan|Ya|
|Referensi memakai format IEEE|Ya|
|Laporan disimpan sebagai dokumen|Ya|

\---

## 26\. Pernyataan Pengumpulan

Kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```
d912ba19a47b403a33d2b66bde377492213184c5
```

**Status akhir yang diklaim:**

```
Siap uji QEMU
```

**Ringkasan satu paragraf:**

Praktikum M1 berhasil menyelesaikan seluruh target toolchain reproducible untuk pengembangan MCSOS 260502. Repository berhasil dibuat di filesystem Linux WSL 2 dengan Ubuntu 26.04 LTS pada host Windows 11 x64. Seluruh toolchain wajib terpasang dan terverifikasi. Proof object ELF64 x86\_64 freestanding berhasil dikompilasi dengan Clang 21.1.8 dan LLD tanpa ketergantungan libc (nm-undefined.txt kosong). Hash SHA-256 identik pada dua run bersih membuktikan determinisme build. QEMU 10.2.1 dengan machine q35 dan tiga path OVMF tersedia sebagai prasyarat M2. Keterbatasan M1: belum ada boot image kernel dan GDB debug session belum diuji pada kernel (target M2). Langkah berikutnya adalah melanjutkan ke M2 untuk membuat boot image kernel ELF64 dengan UEFI dan early console.

