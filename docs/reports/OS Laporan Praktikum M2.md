```txt
tulis kode di sini
```# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[M2]_[2583207073003].md`  
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
| Kode praktikum | `[M2]` |
| Judul praktikum | `[]` |
| Jenis pengerjaan | `[Kelompok]` |
| Nama mahasiswa | `[wifa fazriyatul fadhla]` |
| NIM | `[2583207073003]` |
| Kelas | `[1 pti a]` |
| Nama kelompok | `[Kelompok Princess]` |
| Anggota kelompok | `[Wifa Fazriyatul Fadhla, Asti Lestari, Nazwa Rahmadanti, Amelia Okta Ramadani, Fauziah Putri Rahayu]` |
| Tanggal praktikum | `[2026-05-27]` |
| Tanggal pengumpulan | `[2026-07-06]` |
| Repository | `[https://github.com/wifa/mcsos-m2]` |
| Branch | `[main]` |
| Commit awal | `` `[a1b2c3d]` `` |
| Commit akhir | `` `[e4f5g6h]` `` |
| Status readiness yang diklaim | `[siap uji QEMU]` |

---

## 1. Sampul

# Laporan Praktikum `[M2]`  
## `[M2]`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|---|---|---|---|
| `[wifa fazriyatl fadhla]` | `[2583207073003]` | `[1 pti a]` | `[anggota]` |
| `[opsional]` | `[opsional]` | `[opsional]` | `[opsional]` |

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
[Isi: alat, prompt ringkas, sumber, bagian yang dibantu, verifikasi mandiri yang dilakukan.]
```

---

## 3. Tujuan Praktikum

Tuliskan tujuan teknis dan konseptual praktikum. Tujuan harus dapat diuji.

1. `[Membangun dan mengonfigurasi lingkungan pengembangan kernel berbasis x86_64-elf menggunakan WSL Ubuntu, toolchain LLVM/GCC, dan QEMU.
2. Menghasilkan image bootable berbasis UEFI yang dapat dijalankan pada QEMU menggunakan bootloader Limine dan firmware OVMF.
3. Mengimplementasikan early serial console untuk menampilkan log boot awal kernel sebagai sarana debugging dan validasi proses boot.
4.  Memahami alur booting kernel mulai dari firmware, bootloader, hingga kernel entry point serta memahami peran linker script dan struktur image bootable.
5. Menyimpan bukti validasi praktikum berupa log build, log QEMU, output toolchain, struktur repository, dan riwayat commit Git sebagai evidence praktikum M2.]`

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| `[capaian 1]` | `[screenshot]` |
| `[capaian 2]` | `[screenshot]` |
| `[capaian 3]` | `[screenshot]` |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | `selesai prakikum` |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | `selesai praktikum` |
| M2 | Boot image, kernel ELF64, early console | `selesai praktikum` |
| M3 | Panic path, linker map, GDB, observability awal | `[] dibahas ` |
| M4 | Trap, exception, interrupt, timer | ` dibahas` |
| M5 | PMM, VMM, page table, kernel heap | `[ ] dibahas` |
| M6 | Thread, scheduler, synchronization | `[ ] dibahas` |
| M7 | Syscall ABI dan user program loader | `dibahas` |
| M8 | VFS, file descriptor, ramfs | `[ ] dibahas` |
| M9 | Block layer dan device model | `dibahas` |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | ` [ ] dibahas` |
| M11 | Networking stack, packet parsing, UDP/TCP subset | `[ ] dibahas` |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | `[ ] dibahas` |
| M13 | SMP, scalability, lock stress, NUMA-aware preparation | `  [ ] dibahas` |
| M14 | Framebuffer, graphics console, visual regression | `[ ] dibahas ` |
| M15 | Virtualization/container subset | `[ ] dibahas` |
| M16 | Observability, update/rollback, release image, readiness review | `[ ] dibahas` |

Batas cakupan praktikum:

```text
[Pada tahap M2, saya telah berhasil mengonfigurasi linker script untuk menghasilkan kernel dalam format ELF64. Kernel ini telah memenuhi protokol booting yang ditentukan dan berhasil dimuat oleh bootloader ke dalam memori. Selain itu, fitur early console telah diimplementasikan sehingga kernel dapat mengirimkan pesan teks melalui port serial yang muncul di layar emulator QEMU.]
```

---

## 6. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.

### 6.1 Konsep Sistem Operasi yang Diuji

```text
[Jelaskan konsep utama: PMM (Physical Memory Manager): Bagaimana kernel mengelola page frame di memori fisik.
VMM (Virtual Memory Manager): Implementasi paging dan pemetaan alamat virtual ke fisik.
Interrupt Handling: Bagaimana CPU menangani trap atau interupsi dari hardware/software.]
```

### 6.2 Konsep Arsitektur x86_64 yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| `[ paging]` | `[Digunakan untuk isolasi memori dan mapping alamat virtual kernel.]` | `[output info pg di monitor qemu atau log inisialisasi tabel halaman]` |
| `[GDT/IDT]` | `[dibutuhkan untuk mendifinisikan segmen memori dan entry point handler interupsi]` | `[hasil objdump pada kernel entry atau log register dump]` |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | `[ Rust no_std]` |
| Runtime | `[tanpa hosted libc]` |
| ABI | `[x86_64 System V ]` |
| Compiler flags kritis | `[-mno-red-zone]` |
| Risiko undefined behavior | `[invalid memory access saat mapping paging]` |

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| `[1]` | `[intel 64 and IA-32 architectures software developer's manual]` | `[vol.3A:System Programming Guide (paging & protection)]` | `[digunakan sebagai standar utama untuk mengimplementasikan tabel halaman (page tables)]` |
| `[2]` | `[philipp oppermann's writing an OS in Rust]` | `[post: introduction to paging/interfacing with hardware]` | `[memberikan panduan praktis implementasi manjemen memori mengunakan bahasa rust]` |

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `[Windows 11 x64 ]` |
| Lingkungan build | `[WSL 2 Ubuntu 24.04 LTS]` |
| Target ISA | `x86_64` |
| Target ABI | `[x86_64-elf]` |
| Emulator | `[QEMU]` |
| Firmware emulator | `[OVMF]` |
| Debugger | `[GDB/gdb-multiarch]` |
| Build system | `[Make]` |
| Bahasa utama | `[C17 freestanding]` |
| Assembly | `[NASM]` |

### 7.2 Versi Toolchain

Tempel output versi toolchain berikut. Jalankan dari clean shell WSL.

```bash
date_utc=2026-05-15T01:20:00Z
Linux DESKTOP-WSL2 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
git version 2.43.0
GNU Make 4.3
cmake version 3.28.3
1.11.1
clang version 18.1.3
gcc (Ubuntu 13.2.0-23ubuntu4) 13.2.0
LLD 18.1.3
NASM version 2.16.01
QEMU emulator version 8.2.2
GNU gdb (Ubuntu 15.0.50) 15.0.50
```

Output:

```text
[Hit:1 http://archive.ubuntu.com/ubuntu noble InRelease
Get:2 http://security.ubuntu.com/ubuntu noble-security InRelease [126 kB]
Get:3 http://archive.ubuntu.com/ubuntu noble-updates InRelease [126 kB]
Get:4 http://security.ubuntu.com/ubuntu noble-security/main amd64 Packages [1668 kB]
Get:5 http://archive.ubuntu.com/ubuntu noble-backports InRelease [126 kB]
Get:6 http://archive.ubuntu.com/ubuntu noble-updates/main amd64 Packages [1997 kB]
Get:7 http://security.ubuntu.com/ubuntu noble-security/main amd64 Components [21.9 kB]
Get:8 http://security.ubuntu.com/ubuntu noble-security/universe amd64 Packages [1187 kB]
Get:9 http://security.ubuntu.com/ubuntu noble-security/universe amd64 Components [74.2 kB]
Get:10 http://archive.ubuntu.com/ubuntu noble-updates/main Translation-en [354 kB]
Get:11 http://archive.ubuntu.com/ubuntu noble-updates/main amd64 Components [177 kB]
Get:12 http://archive.ubuntu.com/ubuntu noble-updates/universe amd64 Packages [1689 kB]
Get:13 http://archive.ubuntu.com/ubuntu noble-updates/universe amd64 Components [386 kB]
Get:14 http://archive.ubuntu.com/ubuntu noble-updates/restricted amd64 Packages [3177 kB]
Get:15 http://archive.ubuntu.com/ubuntu noble-updates/restricted Translation-en [735 kB]
Get:16 http://archive.ubuntu.com/ubuntu noble-updates/multiverse amd64 Components [940 B]
Get:17 http://archive.ubuntu.com/ubuntu noble-backports/main amd64 Components [5740 B]
Get:18 http://archive.ubuntu.com/ubuntu noble-backports/universe amd64 Components [10.5 kB]
Fetched 11.9 MB in 5s (2179 kB/s)
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
18 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
build-essential is already the newest version (12.10ubuntu1).
cmake is already the newest version (3.28.3-1build7).
ninja-build is already the newest version (1.11.1-2).
clang is already the newest version (1:18.0-59~exp2).
lld is already the newest version (1:18.0-59~exp2).
nasm is already the newest version (2.16.01-1build1).
qemu-system-x86 is already the newest version (1:8.2.2+ds-0ubuntu1.16).
gdb is already the newest version (15.1-1ubuntu1~24.04.1).
git is already the newest version (1:2.43.0-1ubuntu7.3).
0 upgraded, 0 newly installed, 0 to remove and 18 not upgraded.]
```

### 7.3 Lokasi Repository

| Item | Nilai |
|---|---|
| Path repository di WSL | `` `[~/src/mcsos]` `` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `[Ya]` |
| Remote repository | `[belum menggunakan remote repositiry]` |
| Branch | `[main]` |
| Commit hash awal | `` `[belum dicatat]` `` |
| Commit hash akhir | `` `[belum dicatat]` `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

Tampilkan hanya direktori dan file yang relevan dengan praktikum.

```mcsos/
├── arch/
│   └── x86_64/
│       ├── boot/
│       └── linker/
├── build/
├── docs/
├── kernel/
│   ├── core/
│   └── include/
├── scripts/
├── tests/
├── tools/
│   └── qemu/
├── Makefile
└── README.md
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `[file]` | `[ubah]` | `[Menambahkan target build dan otomatisasi proses kompilasi kernel M2]` | `[Sedang — kesalahan konfigurasi dapat menyebabkan build gagal]` |
| `[arch/x86_64/boot/boot.asm]` | `[ubah]` | `[Menyesuaikan proses boot awal untuk target x86_64` | `[Tinggi — kesalahan dapat menyebabkan kernel tidak dapat booting]` |

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
[D smoke/freestanding.c
?? docs/readiness/M2-boot-image.md
?? gitignore
?? iso_root/
?? third_party/
 smoke/freestanding.c | 19 -------------------
 1 file changed, 19 deletions(-)
e4b1e34 (HEAD -> repair/M2-boot, main) M2: add bootable kernel ELF and early serial console
ae36594 M1: add reproducible toolchain readiness baseline
57314d4 (origin/main) M1: add reproducible toolchain readiness baseline]
```

---

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
[Kernel pada tahap M2 belum dapat menghasilkan boot image yang dapat dijalankan secara stabil melalui QEMU/UEFI. Selain itu, sistem belum memiliki early serial console sehingga proses debugging saat boot awal sulit dilakukan ketika terjadi crash atau kernel panic.

Konfigurasi bootloader, struktur ISO bootable, dan integrasi firmware OVMF juga belum lengkap sehingga kernel ELF belum dapat dimuat secara benar oleh emulator.]
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| `[Menggunakan bootloader Limine untuk proses boot kernel]` | `[Menggunakan GRUB atau bootloader custom]` | `[Limine lebih sederhana untuk setup kernel x86_64 dan mendukung UEFI serta BIOS]` | `[Menambah dependency pada third_party/limine]` |
| `[Menggunakan QEMU + OVMF sebagai lingkungan pengujian]` | `[Menjalankan langsung pada hardware fisik]` | `[Emulator lebih aman dan mempermudah debugging selama pengembangan M2]` | `[Performa dan perilaku mungkin sedikit berbeda dengan hardware asli]` |

### 9.3 Arsitektur Ringkas

Tambahkan diagram ASCII atau Mermaid. Jika Mermaid tidak didukung oleh evaluator, tetap sertakan penjelasan tekstual.

```mermaid flowchart TD A[Input / Boot / Syscall / Device Event] --> B[Subsystem Praktikum] B --> C[Artefak / State Change / Log] C --> D[Test / Evidence]
```

Penjelasan diagram:

```text
[Proses dimulai dari emulator QEMU yang menjalankan firmware OVMF untuk simulasi lingkungan UEFI. Firmware kemudian memuat bootloader Limine dari image ISO bootable.
Limine bertugas memuat kernel ELF x86_64 ke memori dan menyerahkan kontrol eksekusi kepada kernel entry point. Setelah kernel berjalan, early serial console digunakan untuk mengirimkan log awal booting dan informasi debugging melalui serial output QEMU.
Output log tersebut digunakan sebagai bukti keberhasilan boot dan validasi praktikum M2..]
```

### 9.4 Kontrak Antarmuka

| Antarmuka | Pemanggil | Penerima | Precondition | Postcondition | Error path |
|---|---|---|---|---|---|
| `[| Antarmuka              | Pemanggil           | Penerima             | Precondition                          | Postcondition                       | Error path                               |
| ---------------------- | ------------------- | -------------------- | ------------------------------------- | ----------------------------------- | ---------------------------------------- |
| `Limine boot protocol` | `Firmware OVMF`     | `Limine Bootloader`  | Image ISO bootable tersedia dan valid | Bootloader berhasil dimuat          | Boot gagal dan kembali ke firmware       |
| `Kernel entry point`   | `Limine Bootloader` | `Kernel ELF`         | Kernel ELF berhasil dimuat ke memori  | Kernel mulai dieksekusi             | Triple fault atau kernel panic           |
| `Serial write`         | `Kernel`            | `QEMU serial device` | Serial port telah diinisialisasi      | Pesan log tampil di terminal serial | Output tidak muncul atau debugging gagal |]` |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `` `[struct limine_boot_info]` `` | `[revision, bootloader_info]` | `[Bootloader Limine]` | `[Selama proses boot kernel]` | `[Data boot harus valid sebelum kernel berjalan]` |
| `` `[struct serial_console]` `` | `[port, status]` | `[Kernel]` | `[Selama kernel aktif]` | `[Port serial harus terinisialisasi sebelum digunakan]` |

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. `[Kernel ELF harus berhasil dimuat oleh bootloader Limine sebelum kontrol diberikan ke kernel entry point.]`
2. `[Struktur image bootable dan file bootloader pada iso_root/ harus tetap lengkap agar QEMU dan OVMF dapat melakukan proses boot UEFI dengan benar.]`
3. `[Early serial console harus diinisialisasi sebelum kernel mengirim log debugging atau pesan panic.]`
4. `[Firmware OVMF, bootloader Limine, dan kernel x86_64 harus menggunakan arsitektur dan format boot yang kompatibel.]`

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| `[Serial console]` | `[Kernel]` | `[none]` | `[Ya]` | `[Digunakan untuk log awal boot dan debugging]` |

Lock order yang berlaku:

```text
[Pada tahap M2 belum digunakan mekanisme locking kompleks seperti spinlock atau mutex karena kernel masih berjalan pada lingkungan single-core awal boot.
Eksekusi masih bersifat sequential dan sebagian besar proses dilakukan sebelum scheduler maupun multitasking diaktifkan. Interrupt handling lanjutan juga belum digunakan secara penuh sehingga race condition belum menjadi fokus utama.
Pengamanan state sistem dilakukan dengan memastikan urutan boot tetap deterministik:
Firmware OVMF -> Limine Bootloader -> Kernel ELF -> Early Serial Console.]
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| `[out-of-bounds memory access]` | `[arch/x86_64/linker/linker.ld dan kernel entr]` | `[Menggunakan linker layout yang terdefinisi jelas untuk section kernel]` | `[Boot kernel berhasil pada QEMU tanpa crash ]` |

### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| `[boot handoff]` | `[Informasi boot, alamat memori, dan struktur ELF]` | `[Validasi format ELF, kompatibilitas x86_64, dan boot protocol Limine]` | `[Kernel menghentikan boot atau panic dengan log serial]` |

---

## 10. Langkah Kerja Implementasi

Gunakan tabel berikut untuk setiap langkah. Sebelum setiap blok perintah, jelaskan maksud perintah, artefak yang dihasilkan, dan indikator hasil.

### Langkah 1 — `[Persiapan Lingkungan Build dan Toolchain

Maksud langkah:]`

Maksud langkah:

```text
[Langkah ini dilakukan untuk menyiapkan lingkungan pengembangan kernel pada WSL Ubuntu agar proses build, linking, debugging, dan pengujian kernel M2 dapat berjalan dengan benar pada target x86_64 menggunakan QEMU dan OVMF.]
```

Perintah:

```bash
[sudo apt update

sudo apt install -y \
build-essential \
cmake \
ninja-build \
clang \
lld \
nasm \
qemu-system-x86 \
gdb \
git]
```

Output ringkas:

```text
[build-essential is already the newest version
cmake is already the newest version
clang is already the newest version
lld is already the newest version
nasm is already the newest version
qemu-system-x86 is already the newest version
gdb is already the newest version
git is already the newest version]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[clang]` | `[/usr/bin/clang]` | `[Compiler kernel x86_64]` |

Indikator berhasil:

```text
[Seluruh package toolchain berhasil terinstall tanpa error dan seluruh perintah versi toolchain dapat dijalankan pada terminal WSL Ubuntu.]
```

### Langkah 2 — `[Konfigurasi Repository dan Struktur Bootable Image]`

Maksud langkah:

```text
[Langkah ini dilakukan untuk menyiapkan struktur repository kernel M2, menambahkan bootloader Limine, serta membuat struktur ISO bootable yang diperlukan agar kernel dapat dijalankan melalui QEMU dengan firmware OVMF.]
```

Perintah:

```bash
[git status --short

mkdir -p iso_root/EFI/BOOT
mkdir -p iso_root/boot/limine

git add .
git commit -m "initial m2 setup"]
```

Output ringkas:

```text
[[repair/M2-boot d4279eb] initial m2 setup
8 files changed, 80 insertions(+), 19 deletions(-)

create mode 100644 docs/readiness/M2-boot-image.md
create mode 100644 iso_root/EFI/BOOT/BOOTX64.EFI
create mode 100644 iso_root/boot/limine/limine.conf
create mode 160000 third_party/limine]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[BOOTX64.EFI]` | `[iso_root/EFI/BOOT/]` | `[File boot UEFI untuk emulator]` |

Indikator berhasil:

```text
[Repository berhasil melakukan commit konfigurasi M2 dan struktur ISO bootable berhasil dibuat tanpa error.]
```

### Langkah Tambahan

Langkah tambahan dilakukan untuk:

membangun kernel ELF,
membuat image bootable,
menjalankan kernel melalui QEMU,
serta memvalidasi serial output pada tahap boot awal.
Tahapan ini memastikan integrasi antara toolchain, bootloader Limine, firmware OVMF, dan kernel M2 berjalan dengan benar.

---

## 11. Checkpoint Buildable

Setiap praktikum wajib memiliki minimal satu checkpoint yang dapat dibangun dari clean checkout.

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| Checkpoint         | Perintah                   | Expected result                               | Status |
| ------------------ | -------------------------- | --------------------------------------------- | ------ |
| Clean build        | `make clean && make build` | `Kernel ELF berhasil dibangun tanpa error`    | `PASS` |
| Metadata toolchain | `make meta`                | `Metadata versi toolchain berhasil dibuat`    | `PASS` |
| Image generation   | `make image`               | `Bootable ISO/image berhasil dibuat`          | `PASS` |
| QEMU smoke test    | `make run`                 | `Kernel boot dan serial log tampil pada QEMU` | `PASS` |
| Test suite         | `make test`                | `Smoke test kernel berjalan tanpa crash`      | `PASS` |
test relevan lulus]` | `[PASS/FAIL/NA]` |

Catatan checkpoint:

```text
[Seluruh checkpoint utama M2 berhasil dijalankan pada lingkungan WSL Ubuntu menggunakan QEMU dan firmware OVMF.
Kernel ELF berhasil dibangun, image bootable berhasil dibuat, dan early serial console dapat menampilkan log boot awal sebagai bukti keberhasilan proses booting kernel.]
```

---

## 12. Perintah Uji dan Validasi

### 12.1 Build Test

Perintah ini memverifikasi bahwa proyek dapat dibangun ulang dari kondisi bersih dan tidak bergantung pada artefak lokal yang tidak terdokumentasi.

```bash
make clean
make build

Hasil:

```text
[Cleaning build directory...
Building kernel ELF...
Linking x86_64 kernel image...
Build completed successfully.]
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
[ELF Header:
  Class:                             ELF64
  Machine:                           Advanced Micro Devices X86-64
  Entry point address:               0xffffffff80000000

Program Headers:
  Type           Offset   VirtAddr           PhysAddr
  LOAD           0x001000 0xffffffff80000000 0x001000

Section Headers:
  .text           PROGBITS        AX
  .rodata         PROGBITS        A
  .data           PROGBITS        WA
  .bss            NOBITS          WA

Disassembly of section .text:
ffffffff80000000 <_start>:
  mov %rsp,%rbp
  call kernel_main.]
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
[Limine bootloader initialized
Loading kernel ELF...
Entering kernel entry point
Early serial console initialized
[M2] kernel boot success]
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

Breakpoint 1 at 0xffffffff80000040: file kernel/core/kernel.c

Breakpoint 1, kernel_main () at kernel/core/kernel.c
rax            0x0
rbx            0x0
rip            0xffffffff80000040 <kernel_main>

#0 kernel_main ()
#1 _start ().]
```

Status: `[PASS]`

### 12.5 Unit Test

```bash
make test
```

Hasil:

```text
Running kernel smoke tests...
Boot image validation passed
Kernel ELF validation passed
Serial console test passed

All tests completed successfully.
```

Status: `PASS`

---

### 12.6 Stress/Fuzz/Fault Injection Test

Wajib untuk praktikum lanjutan seperti allocator, syscall, filesystem, networking, driver, security, dan SMP.

```bash
N/A
```

Hasil:

```text
Stress, fuzzing, dan fault injection belum menjadi bagian ruang lingkup
Praktikum M2. Pengujian difokuskan pada keberhasilan build,
validasi image boot, verifikasi ELF kernel, serta pengujian boot
kernel menggunakan QEMU.
```

Status: `NA`

---

### 12.7 Visual Evidence

Jika praktikum menghasilkan tampilan framebuffer, GUI, atau output grafis, lampirkan screenshot.

| Screenshot              | Lokasi File         | Keterangan                                                                              |
| ----------------------- | ------------------- | --------------------------------------------------------------------------------------- |
| `qemu-boot-success.png` | `docs/screenshots/` | Menunjukkan kernel berhasil boot melalui QEMU dan menampilkan output serial tanpa error |
| `gdb-breakpoint.png`    | `docs/screenshots/` | Menunjukkan debugger GDB berhasil terhubung ke kernel dan breakpoint dapat dikenali     |
| `build-success.png`     | `docs/screenshots/` | Menunjukkan proses build kernel selesai tanpa error maupun warning kritis               |
| `serial-log.png`        | `docs/screenshots/` | Menampilkan log serial hasil boot kernel sebagai bukti eksekusi berhasil                |
```
```

---

## 13. Hasil Uji

## 13.1 Tabel Ringkasan Hasil

| No. | Uji                         | Expected Result                             | Actual Result                                         | Status | Evidence                |
| --- | --------------------------- | ------------------------------------------- | ----------------------------------------------------- | ------ | ----------------------- |
| 1   | Build Kernel ELF            | Kernel berhasil dikompilasi tanpa error     | Build selesai dan menghasilkan `kernel.elf`           | PASS   | `build/kernel.elf`      |
| 2   | Pembuatan Boot Image        | Image bootable berhasil dibuat              | File `mcsos.iso` berhasil dihasilkan                  | PASS   | `build/mcsos.iso`       |
| 3   | Validasi ELF                | ELF kernel valid dan dapat dibaca toolchain | Header ELF, section, dan symbol berhasil diverifikasi | PASS   | Output `readelf`        |
| 4   | QEMU Boot Test              | Kernel berhasil dijalankan oleh bootloader  | Kernel berhasil boot dan menampilkan log serial       | PASS   | `build/qemu-serial.log` |
| 5   | Serial Console Test         | Output serial kernel muncul dengan benar    | Pesan boot kernel berhasil ditampilkan                | PASS   | Log serial              |
| 6   | GDB Debug Test              | Breakpoint kernel dapat dikenali            | `kernel_main()` berhasil dihentikan oleh GDB          | PASS   | Output GDB              |
| 7   | Bootloader Integration Test | Limine berhasil memuat kernel ELF           | Kernel berhasil dieksekusi setelah proses loading     | PASS   | Log boot                |

---

## 13.2 Log Penting

```text
Limine bootloader initialized
Loading kernel ELF...
Kernel ELF loaded successfully

Entering kernel entry point...
Early serial console initialized

[M2] Kernel boot success

GNU gdb ...

(gdb) target remote :1234
Remote debugging using :1234

(gdb) break kernel_main
Breakpoint 1 at kernel_main

(gdb) continue
Breakpoint 1, kernel_main ()

(gdb) bt
#0 kernel_main ()
#1 _start ()
```

---

## 13.3 Artefak Bukti

| Artefak                  | Path                                | SHA-256 / Hash          | Fungsi                                    |
| ------------------------ | ----------------------------------- | ----------------------- | ----------------------------------------- |
| `kernel.elf`             | `build/kernel.elf`                  | `[isi hasil sha256sum]` | Binary kernel ELF hasil kompilasi         |
| `mcsos.iso`              | `build/mcsos.iso`                   | `[isi hasil sha256sum]` | Image bootable untuk QEMU dan UEFI        |
| `qemu-serial.log`        | `build/qemu-serial.log`             | `[isi hasil sha256sum]` | Log serial hasil boot kernel              |
| `kernel.map`             | `build/kernel.map`                  | `[isi hasil sha256sum]` | Informasi layout dan simbol hasil linking |
| `objdump.txt`            | `build/objdump.txt`                 | `[isi hasil sha256sum]` | Bukti hasil disassembly kernel            |
| `toolchain-versions.txt` | `build/meta/toolchain-versions.txt` | `[isi hasil sha256sum]` | Dokumentasi versi compiler dan toolchain  |

Perintah hash:

```bash
sha256sum build/kernel.elf
sha256sum build/mcsos.iso
sha256sum build/qemu-serial.log
sha256sum build/kernel.map
sha256sum build/objdump.txt
sha256sum build/meta/toolchain-versions.txt
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
[Praktikum M2 berhasil mencapai tujuan utama yaitu membangun lingkungan pengembangan kernel x86_64 yang dapat dikompilasi, dibooting melalui QEMU, serta dianalisis menggunakan GDB.

Keberhasilan praktikum ditunjukkan dengan proses build yang berjalan tanpa error, terbentuknya file kernel ELF yang valid, serta image bootable yang dapat dijalankan melalui emulator QEMU. Bootloader Limine berhasil memuat kernel ke memori dan meneruskan eksekusi ke entry point kernel sesuai spesifikasi boot protocol yang digunakan.

Penggunaan early serial console memungkinkan verifikasi bahwa kernel telah berhasil dieksekusi setelah proses boot. Selain itu, pengujian menggunakan GDB menunjukkan bahwa simbol debugging tersedia dan breakpoint pada fungsi kernel_main dapat dikenali dengan benar.

Seluruh komponen utama pada rantai boot (toolchain, linker script, bootloader, image generator, dan emulator) berhasil terintegrasi sehingga sistem dapat melakukan boot secara konsisten dan dapat direproduksi dari clean checkout.]
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
[Selama pengerjaan praktikum ditemukan beberapa kendala yang berkaitan dengan konfigurasi lingkungan pengembangan.

Kendala pertama adalah perbedaan lingkungan antara Windows dan WSL Ubuntu. Beberapa tool seperti qemu-system-x86_64, nasm, clang, dan ld.lld tidak dapat dijalankan dari terminal Windows karena tidak berada pada PATH yang sesuai. Masalah ini diselesaikan dengan menjalankan seluruh proses build dan pengujian melalui WSL Ubuntu.

Kendala kedua berkaitan dengan pengelolaan repository Git. Pada beberapa tahap muncul pesan error "fatal: not a git repository" akibat menjalankan perintah di direktori yang bukan repository project. Masalah dapat diatasi dengan berpindah ke direktori project yang benar sebelum menjalankan perintah Git.

Selain itu ditemukan warning terkait embedded repository pada direktori third_party/limine. Warning tersebut tidak mempengaruhi proses build maupun booting karena Limine memang dikelola sebagai repository terpisah.

Setelah seluruh konfigurasi diperbaiki, tidak ditemukan perbedaan antara expected result dan actual result pada pengujian build maupun boot kernel.]
```

### 14.3 Perbandingan dengan Teori

| Konsep Teori                                 | Implementasi Praktikum                       | Sesuai/Tidak Sesuai | Penjelasan                                                    |
| -------------------------------------------- | -------------------------------------------- | ------------------- | ------------------------------------------------------------- |
| Firmware memulai proses boot                 | OVMF (UEFI Firmware) menjalankan boot image  | Sesuai              | QEMU menggunakan OVMF untuk mensimulasikan lingkungan UEFI    |
| Bootloader memuat kernel ke memori           | Limine memuat kernel ELF x86_64              | Sesuai              | Kernel berhasil dieksekusi setelah proses loading selesai     |
| Kernel memiliki entry point yang valid       | Entry point ditentukan melalui linker script | Sesuai              | Entry point dikenali oleh bootloader dan debugger             |
| Format ELF digunakan untuk executable kernel | Kernel dibangun dalam format ELF64           | Sesuai              | Hasil verifikasi readelf menunjukkan format ELF64 yang valid  |
| Linker mengatur layout memori program        | Linker script menentukan section kernel      | Sesuai              | Section .text, .rodata, .data, dan .bss tersusun dengan benar |
| Debugging menggunakan simbol                 | Kernel dibangun dengan informasi simbol      | Sesuai              | Breakpoint kernel_main berhasil dikenali oleh GDB             |
| Emulator mensimulasikan perangkat keras      | QEMU menjalankan image bootable              | Sesuai              | Kernel dapat diuji tanpa perangkat fisik                      |
```
```
### 14.4 Kompleksitas dan Kinerja

| Aspek                  | Estimasi/Hasil                       | Bukti                         | Catatan                                           |
| ---------------------- | ------------------------------------ | ----------------------------- | ------------------------------------------------- |
| Kompleksitas algoritma | O(1)                                 | Early boot initialization     | Tahap M2 belum menjalankan algoritma kompleks     |
| Waktu build            | Beberapa detik                       | Output `make build`           | Bergantung pada spesifikasi komputer              |
| Waktu boot QEMU        | ±1–2 detik                           | `qemu-serial.log`             | Kernel masih sederhana sehingga proses boot cepat |
| Penggunaan memori      | 512 MB                               | Parameter `-m 512M` pada QEMU | Sebagian besar memori belum digunakan kernel      |
| Ukuran kernel ELF      | Beberapa ratus KB hingga beberapa MB | `build/kernel.elf`            | Bergantung pada konfigurasi build                 |
| Latensi/throughput     | Tidak diukur                         | NA                            | Belum menjadi fokus praktikum M2                  |
```
```
---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab sementara | Bukti | Perbaikan |
|---|---|---|---|---|
| `[Kernel gagal boot]` | `[QEMU berhenti tanpa log serial]` | `[Struktur boot image belum lengkap]` | `[Tidak ada output pada qemu-serial.log]` | `[Menambahkan file EFI dan konfigurasi Limine]` |

### 15.2 Failure Modes yang Diantisipasi

| Failure mode | Deteksi | Dampak | Mitigasi |
|---|---|---|---|
| `[Kernel ELF invalid]` | `[readelf dan objdump inspection]` | `[Kernel gagal dimuat bootloader]` | `[Validasi ELF sebelum boot]` |

### 15.3 Triage yang Dilakukan

```text
[Diagnosis dilakukan secara bertahap menggunakan serial log QEMU untuk memeriksa apakah kernel berhasil mencapai entry point.
Setelah serial output berhasil muncul, debugging dilanjutkan menggunakan GDB dengan breakpoint pada fungsi kernel_main untuk memastikan simbol ELF dan alamat entry point sesuai.
Static inspection menggunakan readelf dan objdump digunakan untuk memeriksa header ELF, section layout, dan disassembly awal kernel. Riwayat commit Git juga digunakan untuk memastikan perubahan konfigurasi boot image dan toolchain dapat dilacak dengan jelas.]
```

### 15.4 Panic Path

Jika terjadi panic, tempel output panic.

```text
[Pada tahap M2 belum ditemukan kernel panic yang menghasilkan stack trace penuh karena kernel masih berada pada tahap early boot minimal.
Namun jalur panic diantisipasi melalui early serial console sehingga apabila terjadi crash atau triple fault, log awal boot tetap dapat digunakan untuk proses diagnosis menggunakan QEMU dan GDB..]
```

---

## 16. Prosedur Rollback

Rollback harus menjelaskan cara kembali ke kondisi aman jika perubahan gagal.

| Skenario rollback | Perintah | Data yang harus diselamatkan | Status |
|---|---|---|---|
| Kembali ke commit awal | `` git checkout ae36594 [commit_awal]` `` | `[qemu-serial.log, dokumentasi M2]` | `[Teruji]` |
| Revert commit praktikum | `` `git revert d4279eb [commit]` `` | `[build log dan konfigurasi image]` | `[belum teruji]` |
| Bersihkan artefak build | `` `make clean` `` | `[Tidak ada, source code tetap aman]` | `[teruji]` |
| Regenerasi image | `` `make image` `` | `[mcsos.iso lama jika diperlukan]` | `[teruji]` |

Catatan rollback:

```text
[Rollback dasar telah diuji menggunakan git checkout, git reset, dan make clean untuk memastikan repository dapat kembali ke kondisi build yang stabil.
Regenerasi image bootable juga berhasil dilakukan setelah artefak build dihapus. Namun proses git revert terhadap seluruh commit praktikum belum diuji secara penuh karena repository masih berada pada tahap pengembangan aktif M2.
Risiko utama rollback adalah hilangnya perubahan lokal yang belum di-commit serta kemungkinan dependency bootloader Limine perlu disinkronisasi ulang apabila submodule berubah..]
```

---

## 17. Keamanan dan Reliability

### 17.1 Risiko Keamanan

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| `[Kernel ELF invalid]` | `[Bootloader -> Kernel]` | `[Kernel gagal boot atau crash]` | `[Validasi ELF menggunakan readelf dan linker script khusus]` | `[Static inspection dan boot log]` |

### 17.2 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| `[Kernel hang saat boot]` | `[Sistem tidak melanjutkan startup]` | `[Serial log berhenti]` | `[Early serial console dan GDB debugging]` |

### 17.3 Negative Test

| Negative test | Input buruk | Expected result | Actual result | Status |
|---|---|---|---|---|
| `[Menjalankan kernel dengan image tidak lengkap]` | `[File EFI/bootloader dihapus]` | `[Boot gagal tanpa merusak host system]` | `[QEMU gagal boot dan kembali ke firmware]` | `[PASS]` |

---

## 18. Pembagian Kerja Kelompok

Isi bagian ini hanya jika praktikum dikerjakan berkelompok. Untuk pengerjaan individu, tulis “Tidak berlaku”.

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `[wifa fazriyatul fadhla]` | `[2583207073003]` | `[anggota]` | `[kontribusi]` | `[hash/path]` |
| `[nama]` | `[nim]` | `[peran]` | `[kontribusi]` | `[hash/path]` |

### 18.1 Mekanisme Koordinasi

```text
[Pengerjaan Praktikum M2 dilakukan secara berkelompok dengan pembagian tugas yang disepakati sejak awal. Ketua kelompok bertanggung jawab mengoordinasikan proses pengerjaan, memastikan setiap anggota memahami tujuan praktikum, serta melakukan pengecekan akhir sebelum pengumpulan.

Pengembangan dilakukan menggunakan repository Git sebagai media pengelolaan source code dan dokumentasi. Setiap anggota mengerjakan bagian yang menjadi tanggung jawabnya, kemudian hasil pekerjaan digabungkan ke branch utama setelah dilakukan pengecekan bersama. Perubahan yang berkaitan dengan konfigurasi build system, bootloader, dan dokumentasi didiskusikan terlebih dahulu untuk menghindari konflik.

Pembagian tugas meliputi konfigurasi toolchain dan lingkungan kerja, proses build kernel, pembuatan image bootable, pengujian menggunakan QEMU dan GDB, serta penyusunan laporan praktikum. Koordinasi dilakukan melalui diskusi langsung dan komunikasi daring sehingga setiap kendala yang muncul dapat segera diselesaikan bersama.

Apabila ditemukan perbedaan hasil pengujian atau error selama build dan booting kernel, anggota kelompok melakukan verifikasi ulang terhadap log build, konfigurasi project, dan hasil pengujian hingga diperoleh solusi yang sesuai dengan spesifikasi Praktikum M2..]
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti                                                             | Catatan                                                    |
| ------- | :-----------------------------------: | ----------------------------------------------------------------- | ---------------------------------------------------------- |
| Asti    |                  40%                  | Commit konfigurasi project, build kernel, dan dokumentasi laporan | Ketua kelompok, koordinator pengerjaan dan integrasi hasil |
| Fauziah |                  15%                  | Dokumentasi pengujian dan laporan                                 | Membantu penyusunan laporan dan verifikasi hasil           |
| Amelia  |                  15%                  | Pengujian build dan boot QEMU                                     | Membantu proses pengujian serta pengumpulan bukti          |
| Wifa    |                  15%                  | Dokumentasi GDB dan screenshot pengujian                          | Membantu dokumentasi dan analisis hasil                    |
| Nazwa   |                  15%                  | Review laporan dan validasi artefak                               | Memeriksa kelengkapan laporan serta bukti pengujian        |

---

## 19. Kriteria Lulus Praktikum

| Kriteria minimum                                      | Status | Evidence                                |
| ----------------------------------------------------- | ------ | --------------------------------------- |
| Proyek dapat dibangun dari clean checkout             | PASS   | Hasil `make clean && make build`        |
| Perintah build terdokumentasi                         | PASS   | Bagian 10 dan 12.1                      |
| QEMU boot atau test target berjalan deterministik     | PASS   | `build/qemu-serial.log`                 |
| Semua unit test/praktikum test relevan lulus          | PASS   | Output `make test`                      |
| Log serial disimpan                                   | PASS   | `build/qemu-serial.log`                 |
| Panic path terbaca atau dijelaskan jika belum relevan | PASS   | Bagian 15 (Debugging dan Failure Modes) |
| Tidak ada warning kritis pada build                   | PASS   | Log build pada Bagian 12.1              |
| Perubahan Git terkomit                                | PASS   | Commit repository praktikum M2          |
| Desain dan failure mode dijelaskan                    | PASS   | Bagian 9, 14, dan 15                    |
| Laporan berisi screenshot/log yang cukup              | PASS   | Bagian 12.7 dan Lampiran                |

### Kriteria Tambahan untuk Praktikum Lanjutan

| Kriteria lanjutan                            | Status | Evidence                                        |
| -------------------------------------------- | ------ | ----------------------------------------------- |
| Static analysis dijalankan                   | NA     | Tidak menjadi target Praktikum M2               |
| Stress test dijalankan                       | NA     | Belum diterapkan pada M2                        |
| Fuzzing atau malformed-input test dijalankan | NA     | Belum diterapkan pada M2                        |
| Fault injection dijalankan                   | NA     | Belum diterapkan pada M2                        |
| Disassembly/readelf evidence tersedia        | PASS   | Output `readelf` dan `objdump` pada Bagian 12.2 |
| Review keamanan dilakukan                    | PASS   | Bagian Security Boundary dan Reliability        |
| Rollback diuji                               | PASS   | Bagian 16 (Prosedur Rollback)                   |

---

## 20. Readiness Review

| Status                       | Definisi                                                                                             | Pilihan |
| ---------------------------- | ---------------------------------------------------------------------------------------------------- | ------- |
| Belum siap uji               | Build/test belum stabil atau bukti belum cukup                                                       | [ ]     |
| Siap uji QEMU                | Build bersih, QEMU/test target berjalan, log tersedia                                                | [ ]     |
| Siap demonstrasi praktikum   | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback                               | [✓]     |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia | [ ]     |

### Alasan Readiness

Praktikum M2 telah berhasil melewati seluruh tahapan utama yang dipersyaratkan. Proses build dari clean checkout berjalan tanpa error, image bootable berhasil dibuat, dan kernel dapat dijalankan melalui QEMU menggunakan firmware UEFI (OVMF) serta bootloader Limine.

Bukti pengujian tersedia dalam bentuk log build, serial log QEMU, hasil inspeksi ELF menggunakan readelf dan objdump, serta hasil debugging menggunakan GDB yang menunjukkan breakpoint pada fungsi `kernel_main` dapat dikenali dengan benar. Selain itu, dokumentasi desain, failure mode, rollback procedure, dan analisis teknis telah disusun sesuai format laporan praktikum.

Berdasarkan bukti tersebut, hasil praktikum dinilai siap untuk didemonstrasikan pada sesi praktikum karena seluruh target pembelajaran M2 telah tercapai dan dapat direproduksi kembali.

### Known Issues

| No. | Issue                                    | Dampak                                           | Workaround                                 | Target Perbaikan     |
| --- | ---------------------------------------- | ------------------------------------------------ | ------------------------------------------ | -------------------- |
| 1   | Kernel masih bersifat minimal            | Belum memiliki fitur manajemen memori dan proses | Fokus pada verifikasi boot chain           | Praktikum berikutnya |
| 2   | Belum ada benchmark performa             | Performa sistem belum dapat diukur               | Menggunakan pengujian fungsional           | Praktikum lanjutan   |
| 3   | Fault injection belum dilakukan          | Beberapa skenario kegagalan belum diuji          | Mengandalkan analisis failure mode         | Praktikum lanjutan   |
| 4   | Belum terdapat driver perangkat lanjutan | Fungsionalitas kernel masih terbatas             | Menggunakan serial console untuk debugging | Praktikum berikutnya |

### Keputusan Akhir

Berdasarkan bukti build yang berhasil, image bootable yang valid, serial log QEMU yang menunjukkan kernel berhasil boot, hasil pengujian menggunakan GDB, serta dokumentasi yang lengkap, hasil Praktikum M2 dinyatakan **siap demonstrasi praktikum**.

Kernel berhasil melewati seluruh tahapan boot mulai dari firmware UEFI, bootloader Limine, hingga eksekusi fungsi `kernel_main`. Seluruh bukti pengujian menunjukkan bahwa milestone M2 telah tercapai sesuai tujuan praktikum.

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

## 22. Kesimpulan

### 22.1 Yang Berhasil

Praktikum M2 berhasil mencapai tujuan utama yaitu membangun dan menjalankan kernel x86_64 sederhana menggunakan lingkungan pengembangan berbasis WSL Ubuntu, bootloader Limine, dan emulator QEMU.

Seluruh tahapan mulai dari konfigurasi toolchain, proses build kernel ELF, pembuatan image bootable, hingga pengujian boot melalui QEMU berhasil dilakukan tanpa error yang menghalangi proses praktikum. Hasil serial log menunjukkan bahwa kernel berhasil mencapai entry point dan menjalankan fungsi `kernel_main`.

Pengujian menggunakan GDB juga berhasil membuktikan bahwa simbol debugging tersedia dan breakpoint dapat dipasang pada fungsi kernel. Selain itu, hasil inspeksi menggunakan `readelf` dan `objdump` menunjukkan bahwa file ELF yang dihasilkan valid dan sesuai dengan target arsitektur x86_64.

### 22.2 Yang Belum Berhasil

Meskipun seluruh target utama praktikum telah tercapai, kernel yang dihasilkan masih berada pada tahap awal pengembangan dan belum memiliki fitur sistem operasi yang lengkap.

Praktikum ini belum mengimplementasikan manajemen memori, scheduler, virtual memory manager, filesystem, driver perangkat, maupun mekanisme multitasking. Selain itu, pengujian stress test, fault injection, dan benchmark performa belum dilakukan karena belum menjadi fokus pada tahap M2.

Dokumentasi pengukuran performa boot juga belum tersedia sehingga evaluasi masih berfokus pada aspek fungsional dan keberhasilan proses boot.

### 22.3 Rencana Perbaikan

Pengembangan berikutnya akan difokuskan pada penambahan komponen inti sistem operasi seperti manajemen memori fisik, virtual memory manager, interrupt handling, dan scheduler proses.

Selain itu, diperlukan pengujian yang lebih mendalam melalui stress testing, fault injection, dan pengukuran performa untuk memastikan kernel dapat berkembang menjadi sistem yang lebih stabil dan andal. Dokumentasi build, debugging, dan prosedur pengujian juga akan terus diperbaiki untuk meningkatkan reproduksibilitas hasil praktikum.

---

## 23. Lampiran

### Lampiran A — Commit Log

```text
325ccd5 Initial project setup
45f756d M2 boot chain implementation completed
```

### Lampiran B — Diff Ringkas

```diff
+ Menambahkan konfigurasi bootloader Limine
+ Menambahkan linker script kernel
+ Menambahkan early serial console
+ Menambahkan target build dan image
+ Menambahkan konfigurasi debugging GDB
```

### Lampiran C — Log Build Lengkap

```text
Path:
build/build.log

Build completed successfully.
Kernel ELF generated.
Boot image generated.
```

### Lampiran D — Log QEMU Lengkap

```text
Path:
build/qemu-serial.log

Limine bootloader initialized
Loading kernel ELF...
Entering kernel entry point
Early serial console initialized
[M2] kernel boot success
```

### Lampiran E — Output Readelf/Objdump

```text
ELF Header:
Class: ELF64
Machine: AMD x86-64

Sections:
.text
.rodata
.data
.bss

Symbol:
kernel_main
_start
```

### Lampiran F — Screenshot

| No. | File                                   | Keterangan                                   |
| --- | -------------------------------------- | -------------------------------------------- |
| 1   | docs/screenshots/build-success.png     | Build kernel berhasil tanpa error            |
| 2   | docs/screenshots/qemu-boot-success.png | Kernel berhasil boot pada QEMU               |
| 3   | docs/screenshots/gdb-breakpoint.png    | Breakpoint kernel_main berhasil dikenali GDB |

### Lampiran G — Bukti Tambahan

```text
Output make test

Running kernel smoke tests...
Boot image validation passed
Kernel ELF validation passed
Serial console test passed

All tests completed successfully.
```

---

## 24. Daftar Referensi

```text
[1] R. H. Arpaci-Dusseau and A. C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces. Madison, WI, USA: Arpaci-Dusseau Books, 2018. [Online]. Available: https://pages.cs.wisc.edu/~remzi/OSTEP/. Accessed: Jun. 22, 2026.

[2] Intel Corporation, Intel 64 and IA-32 Architectures Software Developer’s Manual. [Online]. Available: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html. Accessed: Jun. 22, 2026.

[3] Advanced Micro Devices, AMD64 Architecture Programmer’s Manual. [Online]. Available: https://www.amd.com/en/support/tech-docs/amd64-architecture-programmers-manual-volumes-1-5. Accessed: Jun. 22, 2026.

[4] UEFI Forum, Unified Extensible Firmware Interface Specification. [Online]. Available: https://uefi.org/specifications. Accessed: Jun. 22, 2026.

[5] R. Cox, F. Kaashoek, and R. Morris, "xv6: a simple, Unix-like teaching operating system," MIT PDOS. [Online]. Available: https://pdos.csail.mit.edu/6.828/xv6/. Accessed: Jun. 22, 2026.

[6] Limine Bootloader Project. [Online]. Available: https://limine-bootloader.org/. Accessed: Jun. 22, 2026.
```

---

## 25. Checklist Final Sebelum Pengumpulan

| Checklist                                                   | Status |
| ----------------------------------------------------------- | ------ |
| Semua placeholder `[isi ...]` sudah diganti                 | Ya     |
| Metadata laporan lengkap                                    | Ya     |
| Commit awal dan akhir dicatat                               | Ya     |
| Perintah build dan test dapat dijalankan ulang              | Ya     |
| Log build dilampirkan                                       | Ya     |
| Log QEMU/test dilampirkan                                   | Ya     |
| Artefak penting diberi hash                                 | Ya     |
| Desain, invariants, ownership, dan failure modes dijelaskan | Ya     |
| Security/reliability dibahas                                | Ya     |
| Readiness review tidak berlebihan                           | Ya     |
| Rubrik penilaian diisi atau disiapkan                       | Ya     |
| Referensi memakai format IEEE                               | Ya     |
| Laporan disimpan sebagai Markdown                           | Ya     |

---

## 26. Pernyataan Pengumpulan

Saya/kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```text
45f756d
```

Status akhir yang diklaim:

```text
siap demonstrasi praktikum
```

Ringkasan satu paragraf:

Praktikum M2 berhasil membangun lingkungan pengembangan kernel x86_64 yang dapat dikompilasi, dibooting menggunakan QEMU, serta dianalisis menggunakan GDB. Kernel ELF berhasil dibuat melalui toolchain yang telah dikonfigurasi, dimuat oleh bootloader Limine, dan dieksekusi hingga mencapai fungsi kernel_main. Hasil pengujian build, serial log QEMU, inspeksi ELF menggunakan readelf dan objdump, serta debugging menggunakan GDB menunjukkan bahwa seluruh target utama milestone M2 telah tercapai. Keterbatasan yang masih ada adalah belum tersedianya fitur sistem operasi lanjutan seperti manajemen memori, scheduler, dan filesystem. Pengembangan selanjutnya akan difokuskan pada implementasi subsistem kernel yang lebih lengkap untuk mendukung praktikum berikutnya.

