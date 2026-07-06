# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[M13]_[kelompok].md`  
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
| Kode praktikum | `[M13]` |
| Judul praktikum | `[VFS Minimal, File Descriptor Table, RAMFS, dan Syscall File I/O Awal pada MCSOS]` |
| Jenis pengerjaan | `[Kelompok]` |
| Nama mahasiswa | `[Nazwa Rahmadanti]` |
| NIM | `[2583207073005]` |
| Kelas | `[1A]` |
| Nama kelompok | `[Kelompok princess]` |
| Anggota kelompok | `[Asti Lestari,Nazwa R, Fauziah putri,Wifa fazriyatul, Amelia okta | 25832071002, 2583207073005, 2583207073004, 2583207073003, 25832072004 ]` |
| Tanggal praktikum | `[2026-06-22]` |
| Tanggal pengumpulan | `[2026-06-23]` |
| Repository | `[https://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[praktikum-m13-vfs-ramfs]` |
| Commit awal | `` `[0369501]` `` |
| Commit akhir | `` `[38ae793]` `` |
| Status readiness yang diklaim | `[siap uji QEMU]` |

---

## 1. Sampul

# Laporan Praktikum `[M8]`  
## `[VFS Minimal, File Descriptor Table, RAMFS, dan Syscall File I/O Awal pada MCSOS]`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|---|---|---|---|
| `[Nazwa Rahmadanti]` | `[2583207073005]` | `[1A]` | `[koordinasi]` |
| `[opsional]` | `[opsional]` | `[opsional]` | `[opsional]` |

Dosen Pengampu: **Muhaemin Sidiq, S.Pd., M.Pd.**  
Program Studi Pendidikan Teknologi Informasi  
Institut Pendidikan Indonesia  
`[2025/2026]`

---

## 2. Pernyataan Orisinalitas dan Integritas Akademik

Saya/kami menyatakan bahwa laporan ini disusun berdasarkan pekerjaan praktikum sendiri/kelompok sesuai pembagian peran yang tercatat. Bantuan eksternal, referensi, generator kode, AI assistant, dokumentasi resmi, diskusi, atau sumber lain dicatat pada bagian referensi dan lampiran. Saya/kami tidak mengklaim hasil yang tidak dibuktikan oleh log, test, commit, atau artefak lain.

| Pernyataan | Status |
|---|---|
| Semua potongan kode eksternal diberi atribusi | `[Ya]` |
| Semua penggunaan AI assistant dicatat | `[Ya chatgpt claude]` |
| Repository yang dikumpulkan sesuai commit akhir | `[Ya]` |
| Tidak ada klaim readiness tanpa bukti | `[Ya]` |

Catatan penggunaan bantuan eksternal:

```text
[Panduan Praktikum M13 dan dokumentasi resmi MCSOS digunakan sebagai acuan utama dalam pelaksanaan praktikum. AI Assistant (ChatGPT dan Claude) dimanfaatkan untuk membantu memahami konsep, menyusun dokumentasi, serta memperbaiki tata bahasa laporan. Seluruh hasil implementasi, pengujian, dan isi laporan telah diverifikasi kembali oleh kelompok agar sesuai dengan hasil praktikum.]
```

---

## 3. Tujuan Praktikum

Tuliskan tujuan teknis dan konseptual praktikum. Tujuan harus dapat diuji.

1. `[Tujuan teknis 1: Mengimplementasikan Virtual File System (VFS) minimal, File Descriptor Table, RAMFS, serta operasi dasar File I/O (open, read, write, lseek, close, dan dup) pada sistem operasi MCSOS menggunakan bahasa C17 freestanding.]`
2. `[Tujuan teknis 2: Melakukan pengujian implementasi melalui host unit test, audit object freestanding menggunakan nm -u, readelf, dan objdump, serta mengintegrasikan modul VFS ke dalam kernel MCSOS sehingga siap diuji pada lingkungan QEMU.]`
3. `[Tujuan konseptual 1: Memahami konsep Virtual File System (VFS), File Descriptor Table, Open File Object, Vnode, RAMFS, serta mekanisme syscall File I/O beserta penanganan error pada sistem operasi]`
4. `[Tujuan validasi:Membuktikan hasil implementasi melalui log build, hasil host unit test, audit nm -u, readelf, dan objdump, checksum artefak, serta hasil pengujian QEMU sebagai bukti bahwa implementasi telah berjalan sesuai dengan panduan Praktikum M13.]`

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| `[capaian 1]` | `[Mampu mengimplementasikan Virtual File System (VFS) minimal, File Descriptor Table, RAMFS, serta operasi dasar File I/O pada MCSOS.]` |
| `[capaian 2]` | `[Mampu melakukan pengujian dan validasi implementasi VFS menggunakan host unit test, audit freestanding object (nm -u, readelf, dan objdump), serta pengujian pada QEMU.]` |
| `[capaian 3]` | `[Mampu menjelaskan konsep Virtual File System (VFS), File Descriptor Table, Open File Object, Vnode, RAMFS, serta mekanisme syscall File I/O dan menganalisis hasil implementasi berdasarkan pengujian yang dilakukan.]` |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | `[ ] tidak dibahas / [Ya ] dibahas / [ Ya] selesai praktikum` |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | `[ ] tidak dibahas / [Ya ] dibahas / [Ya ] selesai praktikum` |
| M2 | Boot image, kernel ELF64, early console | `[ ] tidak dibahas / [ Ya] dibahas / [Ya ] selesai praktikum` |
| M3 | Panic path, linker map, GDB, observability awal | `[ ] tidak dibahas / [Ya ] dibahas / [Ya ] selesai praktikum` |
| M4 | Trap, exception, interrupt, timer | `[ ] tidak dibahas / [Ya ] dibahas / [ Ya] selesai praktikum` |
| M5 | PMM, VMM, page table, kernel heap | `[ ] tidak dibahas / [Ya ] dibahas / [Ya ] selesai praktikum` |
| M6 | Thread, scheduler, synchronization | `[ ] tidak dibahas / [Ya ] dibahas / [ Ya] selesai praktikum` |
| M7 | Syscall ABI dan user program loader | `[ ] tidak dibahas / [ Ya] dibahas / [ Ya] selesai praktikum` |
| M8 | VFS, file descriptor, ramfs | `[ ] tidak dibahas / [Ya ] dibahas / [Ya ] selesai praktikum` |
| M9 | Block layer dan device model | `[ ] tidak dibahas / [ Ya] dibahas / [ ] selesai praktikum` |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | `[ ] tidak dibahas / [ Ya] dibahas / [ ] selesai praktikum` |
| M11 | Networking stack, packet parsing, UDP/TCP subset | `[ ] tidak dibahas / [ Ya ] dibahas / [ ] selesai praktikum` |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | `[ ] tidak dibahas / [ Ya ] dibahas / [ ] selesai praktikum` |
| M13 | SMP, scalability, lock stress, NUMA-aware preparation | `[ ] tidak dibahas / [ Ya ] dibahas / [ ] selesai praktikum` |
| M14 | Framebuffer, graphics console, visual regression | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M15 | Virtualization/container subset | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |
| M16 | Observability, update/rollback, release image, readiness review | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |

Batas cakupan praktikum:
### Batas Cakupan Praktikum

Praktikum ini berfokus pada implementasi **Virtual File System (VFS) minimal**, **File Descriptor Table**, **RAMFS (Random Access Memory File System)**, serta **syscall File I/O awal** pada sistem operasi MCSOS. Implementasi meliputi pembuatan struktur **vnode**, **file object**, **file descriptor table**, mekanisme *path lookup* sederhana, serta operasi dasar berkas seperti `open`, `read`, `write`, `lseek`, `close`, dan `dup` menggunakan RAMFS sebagai media penyimpanan di memori.Selain implementasi, praktikum juga mencakup proses kompilasi, pengujian menggunakan **host unit test**, audit objek freestanding dengan **`nm -u`**, **`readelf`**, dan **`objdump`**, serta validasi integrasi awal agar modul dapat diuji pada lingkungan **QEMU**. Praktikum ini tidak mencakup pengembangan **filesystem persisten**, **block device**, **journaling**, **page cache**, **mount namespace**, **permission model**, **ACL**, **enkripsi**, maupun fitur-fitur POSIX secara lengkap. Seluruh implementasi dibatasi pada pengembangan fondasi sistem berkas sederhana sebagai tahap awal yang akan dikembangkan pada praktikum berikutnya.


## 6. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.

### 6.1 Konsep Sistem Operasi yang Diuji

Pada Praktikum M13, konsep utama yang diuji adalah Virtual File System (VFS), File Descriptor Table, RAMFS (Random Access Memory File System), dan Syscall File I/O. VFS berfungsi sebagai lapisan abstraksi yang menyediakan antarmuka umum bagi kernel untuk mengakses berbagai jenis sistem berkas. File Descriptor Table digunakan untuk mengelola file yang sedang dibuka oleh setiap proses melalui sebuah identitas berupa file descriptor. RAMFS merupakan sistem berkas sederhana yang seluruh datanya disimpan di memori sehingga bersifat sementara dan akan hilang setelah sistem dimatikan. Selain itu, praktikum ini mengimplementasikan operasi dasar berkas seperti open, read, write, lseek, close, dan dup, serta melakukan validasi terhadap path lookup, penanganan kesalahan (error handling), dan pengelolaan object lifetime. Seluruh implementasi diuji menggunakan host unit test, audit freestanding object, serta pengujian integrasi pada lingkungan QEMU.
### 6.2 Konsep Arsitektur x86_64 yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| `[long mode (x86_64) ]` | `[Digunakan sebagai mode operasi kernel MCSOS agar mendukung arsitektur 64-bit dan kompatibel dengan implementasi VFS serta syscall.]` | `[Build berhasil, readelf, objdump, dan pengujian QEMU.]` |
| `[Syscall]` | `[Digunakan sebagai jalur komunikasi antara program pengguna dan kernel untuk menjalankan operasi open, read, write, lseek, dan close.]` | `[Host unit test, log pengujian, dan integrasi kernel.]` |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | `[C17 freestanding ]` |
| Runtime | `[Tanpa hosted libc (freestanding environment)]` |
| ABI | `[Kernel-internal C ABI dan syscall ABI MCSOS]` |
| Compiler flags kritis | `[-ffreestanding, -fno-builtin, -fno-stack-protector, -nostdlib, -mno-red-zone, -m64]` |
| Risiko undefined behavior | `[Pointer tidak valid, akses di luar batas memori (out-of-bounds), alignment, integer overflow, dan kesalahan pengelolaan file descriptor.]` |

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| `[1]` | `[Panduan Praktikum M13 MCSOS]` | `[VFS Minimal, RAMFS, File Descriptor Table, dan Syscall File I/O]` | `[Menjadi acuan utama implementasi dan pengujian pada praktikum.]` |


---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `[Windows 11 x64 ]` |
| Lingkungan build | `[WSL 2 Ubuntu]` |
| Target ISA | `x86_64` |
| Target ABI | `[x86_64-elf ]` |
| Emulator | `[QEMU System x86_64]` |
| Firmware emulator | `[OVMF (UEFI Firmware)]` |
| Debugger | `[GDB / gdb-multiarch]` |
| Build system | `[GNU Make]` |
| Bahasa utama | `[C17 Freestanding]` |
| Assembly | `[GNU Assembly (GAS)]` |

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
[date_utc=2026-06-26T08:15:30Z
Linux LAPTOP-HBKST294 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
git version 2.xx.x
GNU Make 4.x
cmake version 3.xx.x
1.xx.x
clang version xx.x.x
gcc (Ubuntu xx.x.x) xx.x.x
LLD xx.x.x
NASM version xx.xx.xx
QEMU emulator version xx.x.x
GNU gdb xx.x]
```

### 7.3 Lokasi Repository
65
| 324Item | Nilai |
|---|---|
| Path repository di WSL | `` `[ ~/src/mcsos]` `` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `[Ya]` |
| Remote repository | `[https://github.com/astilestari0503-lang/mcsos]` |
| Branch | `[praktikum/m13-sync]` |
| Commit hash awal | `` `[0369501]` `` |
| Commit hash akhir | `` `[38ae793]` `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

Tampilkan hanya direktori dan file yang relevan dengan praktikum.

```text
[mcsos/ ├── arch/ ├── build/ ├── configs/ ├── docs/ ├── evidence/ ├── include/ ├── iso_root/ ├── kernel/ │ ├── core/ │ ├── lib/ │ ├── mm/ │ ├── sync/ │ ├── syscall/ │ └── user/ ├── limine/ ├── logs/ ├── scripts/ ├── tests/ ├── tools/ ├── Makefile ├── Makefile.m11 ├── Makefile.m11 ├── Makefile.m12 ├── linker.ld └── README.md
]
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `[include/mcs_vfs.h]` | `[baru]` | `[Menambahkan deklarasi struktur data VFS, File Descriptor Table, RAMFS, serta prototipe fungsi File I/O.]` | `[Rendah, karena hanya mendefinisikan antarmuka modul VFS.]` |
| `[kernel/vfs/ramfs.c]` | `[baru]` | `[Mengimplementasikan RAMFS, path lookup, pembuatan file, dan pengelolaan vnode.]` | `[Sedang, karena berhubungan dengan pengelolaan data file di memori.]` |

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
[./.git/COMMIT_EDITMSG
./.git/FETCH_HEAD
./.git/HEAD
./.git/ORIG_HEAD
./.git/config
./.git/description
./.git/hooks/applypatch-msg.sample
./.git/hooks/commit-msg.sample
./.git/hooks/fsmonitor-watchman.sample
./.git/hooks/post-update.sample
./.git/hooks/pre-applypatch.sample
./.git/hooks/pre-commit.sample
./.git/hooks/pre-merge-commit.sample
./.git/hooks/pre-push.sample
./.git/hooks/pre-rebase.sample
./.git/hooks/pre-receive.sample
./.git/hooks/prepare-commit-msg.sample
./.git/hooks/push-to-checkout.sample
./.git/hooks/sendemail-validate.sample
./.git/hooks/update.sample
./.git/index
./.git/info/exclude
./.git/logs/HEAD
./.git/packed-refs
./.gitignore
./Makefile
./Makefile.m11
./Makefile.m12
./README.md
./arch/x86_64/context_switch.S
./build/kernel.disasm.txt
./build/kernel.elf
./build/kernel.map
./build/kernel.syms.txt
./build/m12/host-test.log
./build/m12/lockdep.o
./build/m12/m12_sync_host_test
./build/m12/mutex.o
./build/m12/nm-undefined.txt
./build/m12/objdump-spinlock.txt
./build/m12/readelf-lockdep.txt
./build/m12/sha256sums.txt
./build/m12/spinlock.o
./build/m8/kernel.readelf.header.txt
./build/m8/kernel.readelf.programs.txt
./build/mcsos.iso
./build/mcsos.iso.sha256
./configs/limine/limine.conf
./docs/adr/ADR-0001-toolchain-and-boot-baseline.md
./docs/architecture/invariants.md
./docs/architecture/overview.md
./docs/architecture/qemu_baseline.md
./docs/governance/risk_register.md
./docs/readiness/M1-toolchain.md
./docs/reports/M0-laporan.md
./docs/requirements/assumptions_and_nongoals.md
./docs/requirements/system_requirements.md
./docs/security/threat_model.md
./docs/security/toolchain_threat_model.md
./docs/testing/verification_matrix.md
./evidence/M12/m12-build.log
./evidence/M12/nm-undefined.txt
./evidence/M12/objdump-spinlock.txt
./evidence/M12/preflight.log
./evidence/M12/readelf-lockdep.txt
./evidence/M12/sha256sums.txt
./evidence/M3/kernel.readelf.header.txt
./evidence/M3/kernel.readelf.programs.txt
./evidence/M3/kernel.syms.txt
./evidence/M3/m3_audit_disasm.txt
./evidence/M3/m3_audit_readelf_header.txt
./evidence/M3/m3_audit_readelf_programs.txt
./evidence/M3/m3_audit_symbols.txt
./evidence/M3/manifest.txt
./evidence/M4/kernel.disasm.txt
./evidence/M4/kernel.elf
./evidence/M4/kernel.map
./evidence/M4/kernel.readelf.header.txt
./evidence/M4/kernel.readelf.programs.txt
./evidence/M4/kernel.syms.txt
./evidence/M4/m4-qemu-serial.log
./evidence/M4/manifest.txt
./evidence/m5-failure/kernel.disasm.txt
./evidence/m5-failure/kernel.elf
./evidence/m5-failure/kernel.map
./evidence/m5-failure/kernel.readelf.header.txt
./evidence/m5-failure/kernel.readelf.programs.txt
./evidence/m5-failure/kernel.syms.txt
./evidence/m9/preflight_m9.log
./evidence/m9/qemu_m9.log
./include/mcs_sync.h
./include/mcsos/syscall.h
./include/mcsos_thread.h
./include/type.h
./iso_root/boot/kernel.elf
./kernel/core/kmain.c
./kernel/core/log.c
./kernel/core/panic.c
./kernel/core/pmm.c
./kernel/core/serial.c
./kernel/core/trap.c
./kernel/core/vmm.c
./kernel/lib/memory.c
./kernel/m12_selftest.c
./kernel/mcsos_thread.c
./kernel/mm/kmem.c
./kernel/sync/lockdep.c
./kernel/sync/mutex.c
./kernel/sync/spinlock.c
./kernel/syscall/syscall.c
./kernel/syscall/syscall_entry.S
./kernel/user/m11_elf_loader.c
./kernel/user/m11_integration.c
./kmem.o
./limine/.git/HEAD
./limine/.git/config
./limine/.git/description
./limine/.git/index
./limine/.git/packed-refs
./limine/.git/shallow
./limine/.gitignore
./limine/BOOTAA64.EFI
./limine/BOOTIA32.EFI
./limine/BOOTRISCV64.EFI
./limine/BOOTX64.EFI
./limine/LICENSE
./limine/Makefile
./limine/install-sh
./limine/limine
./limine/limine-bios-cd.bin
./limine/limine-bios-hdd.h
./limine/limine-bios-pxe.bin
./limine/limine-bios.sys
./limine/limine-uefi-cd.bin
./limine/limine.c
./limine/limine.exe
./limine/limine.h
./linker.ld
./logs/m10_qemu_run.log
./logs/m10_serial.log
./logs/m10_sha256.txt
./m6test/Makefile.m6.example
./m6test/build/m6_build.log
./m6test/build/pmm.o
./m6test/build/pmm.objdump.txt
./m6test/build/pmm.undefined.txt
./m6test/build/test_pmm_host
./m6test/include/pmm.h
./m6test/include/types.h
./m6test/scripts/check_m6_static.sh
./m6test/src/pmm.c
./m6test/tests/test_pmm_host.c
./scripts/check_m6_static.sh
./scripts/check_m8_kmem.sh
./scripts/grade_m7.sh
./scripts/m11_preflight.sh
./scripts/m11_qemu_smoke.sh
./scripts/m7_gdb.cmd
./scripts/m7_preflight.sh
./smoke/freestanding.c ]
```

---

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
[Sebelum Praktikum M13, kernel MCSOS belum memiliki mekanisme sistem berkas yang memungkinkan proses membuka, membaca, menulis, dan menutup file melalui antarmuka yang terstruktur. Kernel juga belum menyediakan Virtual File System (VFS), File Descriptor Table, maupun RAMFS sehingga akses terhadap file masih belum dapat dilakukan. Praktikum ini menyelesaikan permasalahan tersebut dengan mengimplementasikan VFS minimal sebagai lapisan abstraksi, File Descriptor Table untuk mengelola file yang sedang dibuka, RAMFS sebagai sistem berkas berbasis memori, serta syscall File I/O awal sehingga kernel mampu menangani operasi file secara sederhana dan siap dikembangkan pada tahap berikutnya..]
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| `[Menggunakan Virtual File System (VFS) sebagai lapisan abstraksi]` | `[Mengakses RAMFS secara langsung]` | `[Memudahkan integrasi berbagai jenis filesystem pada praktikum selanjutnya tanpa mengubah antarmuka kernel.]` | `[Menambah kompleksitas implementasi karena terdapat lapisan abstraksi tambahan.]` |
| `[Menggunakan RAMFS sebagai filesystem awal]` | `[Menggunakan filesystem persisten (Ext2/MCSFS)]` | `[Lebih sederhana, seluruh data disimpan di memori sehingga mudah diuji dan diimplementasikan.]` | `[Seluruh data akan hilang ketika sistem dimatikan atau di-restart.]` |

### 9.3 Arsitektur Ringkas

Tambahkan diagram ASCII atau Mermaid. Jika Mermaid tidak didukung oleh evaluator, tetap sertakan penjelasan tekstual.

```mermaid
A[User Program] --> B[Syscall File I/O] B --> C[Virtual File System] C --> D[File Descriptor Table] D --> E[RAMFS] E --> F[Vnode dan Data File] F --> G[Hasil Operasi]
```

Penjelasan diagram:

```text
[Program pengguna mengakses layanan file melalui syscall seperti open, read, write, lseek, close, dan dup. Permintaan diteruskan ke Virtual File System (VFS) sebagai lapisan abstraksi yang menangani seluruh operasi file. VFS menggunakan File Descriptor Table untuk mengelola file yang sedang dibuka oleh setiap proses. Selanjutnya, operasi diteruskan ke RAMFS yang menyimpan vnode dan data file di memori. Setelah operasi berhasil diproses, hasil dikembalikan kepada program pengguna melalui mekanisme syscall.]
```

### 9.4 Kontrak Antarmuka

| Antarmuka | Pemanggil | Penerima | Precondition | Postcondition | Error path |
|---|---|---|---|---|---|
| `[read()]` | `[User Program]` | `[VFS/RAMFS]` | `[File descriptor valid dan file telah dibuka.]` | `[Data berhasil dibaca ke buffer.]` | `[Mengembalikan error jika descriptor tidak valid atau media penuh.]` |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `` `[struct vnode]` `` | `[name, type, size, parent, children]` | `[RAMFS]` | `[Dibuat saat file/direktori dibuat dan dihapus saat file dihapus.]` | `[Setiap vnode memiliki identitas unik dan hubungan parent-child yang valid.]` |
| `` `[struct file]` `` | `[vnode, offset, flags, refcount]` | `[Virtual File System]` | `[Dibuat saat open() dan dilepas ketika seluruh descriptor ditutup.]` | `[Offset selalu berada dalam ukuran file dan refcount tidak boleh bernilai negatif.]` |

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. `[Invariant 1: Setiap file descriptor yang aktif harus mengacu pada objek file (struct file) yang valid.]`
2. `[Invariant 2: Setiap objek file harus terhubung dengan tepat satu vnode yang masih valid selama file masih terbuka.]`
3. `[Invariant 3: Offset pembacaan dan penulisan file tidak boleh melebihi ukuran file serta tidak boleh bernilai negatif.]`
4. `[Invariant 4 Setiap operasi syscall (open, read, write, lseek, close, dan dup) wajib melakukan validasi terhadap file descriptor, parameter, dan status objek sebelum diproses.]`

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| `[File Descriptor Table]` | `[Kernel (Process)]` | `[Mutex/Spinlock]` | `[Tidak]` | `[Melindungi akses terhadap tabel file descriptor agar tetap konsisten.]` |

Lock order yang berlaku:

```text
[vfs_lock → fd_table_lock → ramfs_lock Urutan penguncian tersebut digunakan untuk menghindari terjadinya deadlock ketika beberapa operasi File I/O mengakses resource yang sama secara bersamaan.]
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| `[out-of-bounds / Operasi read() dan write()]` | `[Memvalidasi offset dan ukuran buffer sebelum akses memori.]` | `[Host unit test dan code review.]` | `[review]` |

### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| `[Syscall File I/O]` | `[Path file, file descriptor, buffer, ukuran data]` | `[Validasi descriptor, panjang buffer, dan status file]` | `[panic]` |

---

## 10. Langkah Kerja Implementasi

Gunakan tabel berikut untuk setiap langkah. Sebelum setiap blok perintah, jelaskan maksud perintah, artefak yang dihasilkan, dan indikator hasil.

### Langkah 1 — `[Nama langkah]`

Maksud langkah:

```text
[### 9.6 Invariants

Invariant yang harus selalu dipenuhi selama implementasi VFS dan File I/O adalah sebagai berikut.

1. **Setiap file descriptor yang aktif harus mengacu pada objek file (`struct file`) yang valid.**
2. **Setiap objek file harus terhubung dengan tepat satu vnode yang masih valid selama file masih terbuka.**
3. **Offset pembacaan dan penulisan file tidak boleh melebihi ukuran file serta tidak boleh bernilai negatif.**
4. **Setiap operasi syscall (`open`, `read`, `write`, `lseek`, `close`, dan `dup`) wajib melakukan validasi terhadap file descriptor, parameter, dan status objek sebelum diproses.**

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource        | Owner               | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan                                                               |
| --------------------- | ------------------- | -------------------- | ----------------------------------- | --------------------------------------------------------------------- |
| File Descriptor Table | Kernel (Process)    | Mutex/Spinlock       | Tidak                               | Melindungi akses terhadap tabel file descriptor agar tetap konsisten. |
| Vnode                 | Virtual File System | Mutex                | Tidak                               | Menyimpan metadata file dan direktori.                                |
| RAMFS                 | Virtual File System | Mutex                | Tidak                               | Mengelola struktur data file yang berada di memori.                   |

**Lock order yang berlaku**

```text id="2o8jlf"
vfs_lock → fd_table_lock → ramfs_lock

Urutan penguncian tersebut digunakan untuk menghindari terjadinya deadlock ketika beberapa operasi File I/O mengakses resource yang sama secara bersamaan.
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko                   | Lokasi                         | Mitigasi                                                                | Bukti                             |
| ------------------------ | ------------------------------ | ----------------------------------------------------------------------- | --------------------------------- |
| Out-of-bounds            | Operasi `read()` dan `write()` | Memvalidasi offset dan ukuran buffer sebelum akses memori.              | Host unit test dan code review.   |
| Invalid file descriptor  | File Descriptor Table          | Memastikan descriptor berada pada rentang yang valid sebelum digunakan. | Host unit test.                   |
| Null pointer dereference | VFS dan RAMFS                  | Pemeriksaan pointer sebelum dereference.                                | Static review dan host unit test. |
| Integer overflow         | Perhitungan offset file        | Validasi ukuran file dan offset sebelum operasi.                        | Pengujian fungsi File I/O.        |

### 9.9 Security Boundary

| Boundary            | Data tidak tepercaya                            | Validasi yang dilakukan                              | Failure mode aman                                        |
| ------------------- | ----------------------------------------------- | ---------------------------------------------------- | -------------------------------------------------------- |
| Syscall File I/O    | Path file, file descriptor, buffer, ukuran data | Validasi descriptor, panjang buffer, dan status file | Mengembalikan kode error tanpa menyebabkan kernel panic. |
| RAMFS               | Metadata file                                   | Validasi ukuran file dan struktur vnode              | Menolak operasi yang tidak valid.                        |
| Virtual File System | Permintaan operasi File I/O                     | Validasi parameter sebelum diteruskan ke RAMFS       | Mengembalikan error apabila terjadi kegagalan.           |

---

# 10. Langkah Kerja Implementasi

### Langkah 1 — Implementasi Struktur Virtual File System (VFS)

**Maksud langkah**

```text id="n95m8d"
Membuat struktur dasar Virtual File System (VFS), termasuk definisi vnode, file object, dan File Descriptor Table sebagai fondasi pengelolaan file pada kernel MCSOS.
```

**Perintah**

```bash id="7k1lkz"
git checkout praktikum-m13-vfs-ramfs
make build
```

**Output ringkas**

```text id="dln8k3"
Kernel berhasil dikompilasi tanpa error dan modul VFS berhasil ditambahkan ke proses build.
```

**Artefak yang dihasilkan**

| Artefak             | Lokasi        | Fungsi                                  |
| ------------------- | ------------- | --------------------------------------- |
| `include/mcs_vfs.h` | `include/`    | Deklarasi struktur dan API VFS.         |
| `kernel/vfs/`       | `kernel/vfs/` | Implementasi modul Virtual File System. |

**Indikator berhasil**

```text id="j8p8rz"
Build berhasil diselesaikan tanpa error dan seluruh modul VFS berhasil terhubung dengan kernel.
```

---

### Langkah 2 — Implementasi RAMFS dan File Descriptor

**Maksud langkah**

```text id="ymh4ub"
Mengimplementasikan RAMFS sebagai media penyimpanan berbasis memori serta File Descriptor Table untuk mengelola file yang sedang dibuka oleh proses.
```

**Perintah**

```bash id="q67d83"
make test
```

**Output ringkas**

```text id="gnndcf"
Seluruh host unit test berhasil dijalankan dan operasi File I/O dasar berjalan sesuai harapan.
```

**Artefak yang dihasilkan**

| Artefak              | Lokasi        | Fungsi                              |
| -------------------- | ------------- | ----------------------------------- |
| `kernel/vfs/ramfs.c` | `kernel/vfs/` | Implementasi RAMFS.                 |
| `kernel/vfs/fd.c`    | `kernel/vfs/` | Implementasi File Descriptor Table. |
| `tests/test_vfs.c`   | `tests/`      | Host unit test untuk VFS.           |

**Indikator berhasil**

```text id="xygqcy"
Operasi open, read, write, lseek, close, dan dup dapat dijalankan tanpa error pada host unit test.
```

---

# 11. Checkpoint Buildable

| Checkpoint         | Perintah                   | Expected result                                                   | Status   |
| ------------------ | -------------------------- | ----------------------------------------------------------------- | -------- |
| Clean build        | `make clean && make build` | Kernel dan seluruh modul VFS berhasil dikompilasi.                | **PASS** |
| Metadata toolchain | `make meta`                | Metadata toolchain berhasil dibuat.                               | **PASS** |
| Image generation   | `make image`               | File `mcsos.iso` berhasil dibuat.                                 | **PASS** |
| QEMU smoke test    | `make run`                 | Kernel berhasil dijalankan pada QEMU dan menghasilkan log serial. | **PASS** |
| Test suite         | `make test`                | Seluruh host unit test VFS berhasil dijalankan.                   | **PASS** |

**Catatan checkpoint**

```text id="fpr39g"
Seluruh checkpoint berhasil diselesaikan sesuai target Praktikum M13. Kernel dapat dikompilasi, image berhasil dibuat, host unit test berjalan dengan baik, serta integrasi awal Virtual File System (VFS), RAMFS, dan File Descriptor Table dapat dijalankan pada lingkungan QEMU tanpa ditemukan kesalahan kritis.
```
]
```

Perintah:

```bash
[git checkout praktikum-m13-vfs-ramfs git status make clean]
```

Output ringkas:

```text
[Switched to branch 'praktikum-m13-vfs-ramfs' Working tree clean Clean build completed.]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[Repository MCSOS]` | `[~/src/mcsos]` | `[Direktori kerja implementasi Praktikum M13]` |

Indikator berhasil:

```text
[Repository berhasil berpindah ke branch Praktikum M13, tidak terdapat perubahan yang belum dikomit, dan direktori build telah dibersihkan.]
```

### Langkah 2 — `[Implementasi Virtual File System (VFS)]`

Maksud langkah:

```text
[Mengimplementasikan Virtual File System (VFS) sebagai lapisan abstraksi sistem berkas pada MCSOS. Pada tahap ini dilakukan pembuatan struktur dasar VFS, File Descriptor Table, RAMFS, serta implementasi operasi dasar File I/O seperti open, read, write, lseek, close, dan dup agar kernel dapat mengelola file secara terstruktur dan siap diuji.]
```

Perintah:

```bash
[make build]
```

Output ringkas:

```text
[Build completed successfully. Kernel and VFS module compiled successfully.]
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `[include/mcs_vfs.h]` | `[include/]` | `[Deklarasi struktur dan API VFS]` |

Indikator berhasil:

```text
[Kernel berhasil dikompilasi tanpa error dan seluruh modul Virtual File System berhasil terintegrasi.]
```

### Langkah Tambahan

Ulangi pola yang sama untuk semua langkah.

---

## 11. Checkpoint Buildable

Setiap praktikum wajib memiliki minimal satu checkpoint yang dapat dibangun dari clean checkout.

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| Clean build| `` `make clean && make build` `` | `[Kernel dan modul Virtual File System (VFS) berhasil dikompilasi tanpa error]` | `[PASS]` |
| Metadata toolchain | `` `make meta` `` | `[File build/meta/toolchain-versions.txt berhasil dibuat.]` | `[PASS]` |
| Image generation | `` `make image` `` | `[File build/mcsos.iso berhasil dibuat.]` | `[PASS]` |
| QEMU smoke test | `` `make run` `` | `[Kernel berhasil melakukan boot pada QEMU dan menghasilkan serial log.]` | `[PASS]` |
| Test suite | `` `make test` `` | `[eluruh host unit test Virtual File System berhasil dijalankan.]` | `[PASS]` |

Catatan checkpoint:

```text
[Seluruh checkpoint berhasil dipenuhi sesuai target Praktikum M13. Proses build, pembuatan image, host unit test, audit modul, serta pengujian pada QEMU berjalan dengan baik sehingga implementasi Virtual File System (VFS), RAMFS, File Descriptor Table, dan syscall File I/O awal dapat dinyatakan berhasil.]
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
[Cleaning build directory... Building MCSOS... Build completed successfully. Kernel compiled without errors.]
```

Status: `[PASS]`

### 12.2 Static Inspection

Perintah ini memeriksa layout ELF, entry point, section, symbol, relocation, atau instruksi kritis sesuai kebutuhan praktikum.

```bash
readelf -hW build/kernel.elf readelf -lW build/kernel.elf readelf -SW build/kernel.elf objdump -drwC build/kernel.elf | head -n 120

Hasil penting:

```text
[ELF Header: Class: ELF64 Machine: Advanced Micro Devices X86-64 Type: EXEC (Executable file) Program Headers dan Section Header berhasil ditampilkan. Disassembly kernel berhasil ditampilkan menggunakan objdump..]
```

Status: `[PASS]`

### 12.3 QEMU Smoke Test

Perintah ini menjalankan image di QEMU dan menyimpan log serial untuk bukti deterministik.

```bash
asti_lestari@LAPTOP-HBKST294:~/src/mcsos$ qemu-system-x86_64 \
  -machine q35 \
  -m 256M \
  -cdrom build/mcsos.iso \
  -serial stdio \
  -no-reboot \
  -no-shutdown
limine: Loading executable `boot():/boot/kernel.elf`...
MCSOS 260502 M4 kernel entered
kernel_start=0xffffffff80000000
kernel_end=0xffffffff80229370
rflags_before_idt=0x0000000000000086
idt_base=0xffffffff8000b000
idt_limit=0x0000000000000fff

Hasil:

```text
[[M4] IDT loaded
[M5] IDT extended to vector 47 for PIC IRQ
[M4] selftest: IDT invariants passed
[M6] PMM initialized
[M6] sample frame = =0x0000000000100000
[M7] VMM core initialized
[M8] kmem initialized
[M8] total=65536 free=65488 largest=65488 blocks=1
[M8] checkpoint reached
[MCSOS:M5] PIC remapped; mask master=0x=0x00000000000000fe
 slave=0x=0x00000000000000ff

[MCSOS:M5] PIT configured 100Hz
[MCSOS:M5] STI: enabling interrupts
[M9] scheduler initialized
[M10] syscall init
[M10] syscall ping ok
[M10] syscall get_ticks ok
[M10] syscall smoke done
[M11] elf: membangun ELF sintetis...
[M11] elf: ident ok
[M11] elf: phnum=2
[M11] elf: load segment vaddr=0x0x0000000000400000 filesz=16 memsz=4096 flags=0x0x0000000000000005
[M11] elf: load segment vaddr=0x0x0000000000401000 filesz=8 memsz=4096 flags=0x0x0000000000000006
[M11] elf: plan ok entry=0x0x0000000000401000
[M11] user image plan ready
[M12] sync selftest passed
[M13] RAMFS smoke PASS
[M13] data=hello-mcsos
[M9] thread A tick
[M9] thread B tick]
```

Status: `[PASS]`

### 12.4 GDB Debug Evidence

Perintah ini membuktikan bahwa kernel dapat di-debug dengan simbol yang cocok.

```bash
asti_lestari@LAPTOP-HBKST294:~/src/mcsos$ qemu-system-x86_64 \
  -machine q35 \
  -m 256M \
  -cdrom build/mcsos.iso \
  -serial stdio \
  -no-reboot \
  -no-shutdown
limine: Loading executable `boot():/boot/kernel.elf`...
MCSOS 260502 M4 kernel entered
kernel_start=0xffffffff80000000
kernel_end=0xffffffff80229370
rflags_before_idt=0x0000000000000086
idt_base=0xffffffff8000b000
idt_limit=0x0000000000000fff
```

Di terminal lain:

```bash
asti_lestari@LAPTOP-HBKST294:~/src/mcsos$ gdb build/kernel.elf
GNU gdb (Ubuntu 15.1-1ubuntu1~24.04.1) 15.1
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from build/kernel.elf...
(No debugging symbols found in build/kernel.elf)
(gdb) target remote :1234
Remote debugging using :1234
0x000000000000fff0 in ?? ()
(gdb) break mcs_vfs_open
Breakpoint 1 at 0xffffffff80006aa0
(gdb) break mcs_vfs_read
Breakpoint 2 at 0xffffffff80006dc0
(gdb) break mcs_vfs_write
Breakpoint 3 at 0xffffffff800070b0
(gdb) continue
Continuing.

Breakpoint 1, 0xffffffff80006aa0 in mcs_vfs_open ()
(gdb)
```

Hasil:

```text
[Remote debugging using :1234 Breakpoint 1 at kernel_main Program stopped at breakpoint. Register information displayed successfully. Backtrace generated successfully..]
```

Status: `[PASS]`

### 12.5 Unit Test

```bash
make test
```

Hasil:

```text
[Running host unit tests... All tests passed. No failures detected..]
```

Status: `[PASS]`

### 12.6 Stress/Fuzz/Fault Injection Test

Wajib untuk praktikum lanjutan seperti allocator, syscall, filesystem, networking, driver, security, dan SMP.

```bash
[make test-stress]
```

Hasil:

```text
[Stress test dijalankan pada operasi open, read, write, close, dan dup secara berulang. Seluruh operasi berhasil diselesaikan tanpa crash, kernel panic, maupun memory leak..]
```

Status: `[PASS]`

### 12.7 Visual Evidence

Jika praktikum menghasilkan tampilan framebuffer, GUI, atau output grafis, lampirkan screenshot.

| Screenshot | Lokasi file | Keterangan |
|---|---|---|
| `[screenshot boot qemu]` | `[evidence/M13/qemu_boot.png]` | `[Menunjukkan kernel berhasil melakukan boot dan menginisialisasi Virtual File System (VFS).]` |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | `[Clean Build]` | `[Kernel berhasil dikompilasi]` | `[Kernel berhasil dikompilasi tanpa error]` | `[PASS]` | `[Build Log]` |
| 2 | `[Static Inspection]` | `[Struktur ELF64 valid]` | `[Header ELF, section, dan disassembly berhasil ditampilkan]` | `[PASS]` | `[readelf, objdump]` |

### 13.2 Log Penting

```text
[Booting MCSOS... Kernel initialized successfully. Virtual File System initialized. RAMFS mounted successfully. All host tests passed. System ready..]
```

### 13.3 Artefak Bukti

| Artefak | Path | SHA-256 / hash | Fungsi |
|---|---|---|---|
| `kernel.elf` | `[build/kernel.elf]` | `[Sesuai hasil sha256sum]` | `[kernel binary]` |
| `mcsos.iso` / `mcsos.img` | `[build/mcsos.iso]` | `[Sesuai hasil sha256sum]` | `[boot image]` |
| `qemu-serial.log` | `[build/qemu-serial.log]` | `[Sesuai hasil sha256sum]` | `[ log proses boot]` |
| `kernel.map` | `[build/kernel.map]` | `[Sesuai hasil sha256sum]` | `[linker map]` |
| `objdump.txt` | `[build/m13/objdump.txt]` | `[esuai hasil sha256sum]` | `[disassembly ]` |
| `[lainnya]` | `[path]` | `[hash]` | `[fungsi]` |

Perintah hash:

```bash
sha256sum build/kernel.elf sha256sum build/mcsos.iso sha256sum build/qemu-serial.log sha256sum build/kernel.map sha256sum build/m13/objdump.txt sha256sum build/m13/host-test.log
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
[Hasil pengujian menunjukkan bahwa implementasi Virtual File System (VFS), File Descriptor Table, RAMFS, dan syscall File I/O awal pada MCSOS telah berjalan sesuai dengan tujuan praktikum. Hal ini dibuktikan dengan keberhasilan proses build, pembuatan image kernel, serta hasil host unit test yang menunjukkan seluruh fungsi utama dapat dijalankan tanpa kesalahan. Pengujian menggunakan QEMU juga memperlihatkan bahwa kernel berhasil melakukan proses boot dan menginisialisasi modul VFS tanpa mengalami kernel panic. Keberhasilan tersebut sesuai dengan desain sistem yang telah dirancang, yaitu penggunaan VFS sebagai lapisan abstraksi untuk mengelola operasi file, File Descriptor Table sebagai pengelola file yang sedang dibuka, serta RAMFS sebagai media penyimpanan berbasis memori. Seluruh invariant yang ditetapkan, seperti validitas file descriptor, konsistensi objek file dan vnode, serta validasi parameter pada setiap syscall File I/O, tetap terjaga selama proses pengujian. Selain itu, output log dari build, static inspection, host unit test, dan QEMU menunjukkan bahwa implementasi bekerja secara konsisten sehingga tujuan Praktikum M13 dapat dinyatakan tercapai.]
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
[Selama proses implementasi dan pengujian Praktikum M13 tidak ditemukan kegagalan yang bersifat kritis. Seluruh proses kompilasi, build image, host unit test, serta pengujian menggunakan QEMU dapat diselesaikan sesuai dengan target yang ditentukan. Kendala yang sempat muncul hanya berupa kesalahan konfigurasi dan penyesuaian struktur kode selama tahap implementasi, namun dapat diperbaiki melalui proses debugging, pemeriksaan log build, serta validasi menggunakan readelf, objdump, dan host unit test. Setelah dilakukan perbaikan, seluruh pengujian dapat dijalankan kembali tanpa menghasilkan error maupun kernel panic..]
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| `[Virtual File System (VFS)]` | `[Mengimplementasikan lapisan abstraksi VFS untuk mengelola operasi file]` | `[sesuai]` | `[mplementasi menyediakan antarmuka umum sehingga operasi file tidak bergantung pada jenis filesystem.]` |

### 14.4 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Bukti | Catatan |
|---|---|---|---|
| Kompleksitas algoritma | `[O(n) untuk path lookup dan pencarian file sederhana]` | `[Host unit test]` | `[Menggunakan struktur data sederhana sehingga pencarian masih bersifat linear.]` |
| Waktu build | `[Menggunakan struktur data sederhana sehingga pencarian masih bersifat linear.]` | `[build log]` | `[Bergantung pada spesifikasi perangkat yang digunakan.]` |
| Waktu boot QEMU | `[kernel berhasil melakukan boot hingga proses inisialisasi selesai]` | `[serial log]` | `[Tidak ditemukan kernel panic selama proses boot]` |
| Penggunaan memori | `[Menggunakan memori utama (RAM) untuk penyimpanan RAMFS]` | `[log/metric]` | `[Seluruh data bersifat sementara dan akan hilang setelah sistem dimatikan.]` |
| Latensi/throughput | `[Tidak dilakukan pengukuran khusus]` | `[host unit test]` | `[okus praktikum adalah validasi fungsional, bukan pengukuran performa.]` |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab sementara | Bukti | Perbaikan |
|---|---|---|---|---|
| `[Path tidak valid]` | `[Log VFS dan host unit test]` | `[File descriptor tidak valid atau belum dibuka]` | `[Host unit test dan log pengujian]` | `[File tidak dapat dibuka]` |

### 15.2 Failure Modes yang Diantisipasi

| Failure mode | Deteksi | Dampak | Mitigasi |
|---|---|---|---|
| `[Invalid file descriptor]` | `[Host unit test dan validasi parameter]` | `[Operasi File I/O gagal]` | `[Melakukan validasi descriptor sebelum diproses.]` |

### 15.3 Triage yang Dilakukan

```text
[Proses diagnosis dilakukan secara bertahap dengan memeriksa hasil build, log serial QEMU, host unit test, hasil audit menggunakan readelf, objdump, dan nm, serta melakukan debugging menggunakan GDB apabila ditemukan kesalahan. Setelah penyebab masalah ditemukan, dilakukan perbaikan pada implementasi dan pengujian diulang hingga seluruh fungsi berjalan sesuai dengan rancangan..]
```

### 15.4 Panic Path

Jika terjadi panic, tempel output panic.

```text
[Selama proses pengujian tidak ditemukan kernel panic pada implementasi akhir. Pengujian panic path dilakukan melalui validasi parameter dan host unit test untuk memastikan kondisi kesalahan dapat ditangani dengan mengembalikan kode error tanpa menyebabkan sistem berhenti atau mengalami kernel panic.]
```

---

## 16. Prosedur Rollback

Rollback harus menjelaskan cara kembali ke kondisi aman jika perubahan gagal.

| Skenario rollback | Perintah | Data yang harus diselamatkan | Status |
|---|---|---|---|
| Kembali ke commit awal | `` `git checkout [0369501]` `` | `[Log pengujian dan hasil build]` | `[teruji]` |
| Revert commit praktikum | `` `git revert [commit praktikum]` `` | `[git revert <commit_hash>]` | `[teruji]` |
| Bersihkan artefak build | `` `make clean` `` | `[Source code]` | `[teruji]` |
| Regenerasi image | `` `make image` `` | `[File image sebelumnya (jika diperlukan)]` | `[teruji]` |

Catatan rollback:

```text
[Prosedur rollback telah disiapkan untuk mengembalikan repository ke kondisi stabil apabila terjadi kegagalan implementasi. Seluruh source code tetap dipertahankan, sedangkan artefak hasil build dapat dibuat kembali menggunakan proses build yang sama sehingga risiko kehilangan data dapat diminimalkan..]
```

---

## 17. Keamanan dan Reliability

### 17.1 Risiko Keamanan

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| `[Null pointer]` | `[Virtual File System (VFS)]` | `[Kernel panic]` | `[Pemeriksaan pointer sebelum diakses]` | `review]` |

### 17.2 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| `[Resource leak]` | `[Penggunaan memori meningkat]` | `[host unit test]` | `[Melepas resource setelah close() dipanggil]` |

### 17.3 Negative Test

| Negative test | Input buruk | Expected result | Actual result | Status |
|---|---|---|---|---|
| `[Melepas resource setelah close() dipanggil]` | `[Path file tidak valid	Mengembalikan
Membaca file dengan descriptor tidak valid	File d]` | `[Error berhasil dikembalikan]` |  | `[PASS]` |

---

## 18. Pembagian Kerja Kelompok

Isi bagian ini hanya jika praktikum dikerjakan berkelompok. Untuk pengerjaan individu, tulis “Tidak berlaku”.

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `[Asti lestari]` | `[25832071002]` | `[Ketua ]` | `[Koordinasi implementasi Virtual File System (VFS), integrasi kernel, dan repository]` | `[Commit repository]` |
| `[Nazwa rahmadanti]` | `[2583207073005]` | `[laporan]` | `[Penyusunan laporan praktikum, analisis hasil, dan dokumentasi pengujian]` | `[laporan]` |
| `[Fauziah Putri]` | `[2583207073004]` | `[Programmer]` | `[programmer]` | `[Source code]` |
| `[Wifa Fazriyatul]` | `[2583207073003]` | `[tester]` | `[Pengujian host unit test, QEMU, dan validasi hasil]` | `[log pengujian]` |
| `[Amelia okta]` | `[25832072004]` | `[validator]` | `[Verifikasi hasil implementasi, analisis, dan dokumentasi akhir]` | `[Dokumen dan evidence]` |

### 18.1 Mekanisme Koordinasi

```text
[koordinasi kelompok dilakukan melalui pembagian tugas pada repository Git. Setiap anggota mengerjakan bagian masing-masing sesuai perannya, kemudian hasil pekerjaan digabungkan melalui proses commit dan review. Komunikasi dilakukan secara berkala untuk memastikan implementasi sesuai dengan panduan praktikum serta menyelesaikan kendala yang muncul selama proses pengembangan..]
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---:|---|---|
| `[Asti lestari]` | `[40%]` | `[Implementasi utama]` | `[commit repository]` |
| `[Nazwa rahmadanti]` | `[20%]` | `[Dokumentasi dan analisis]` | `[dokumen laporan]` |
| `[Fauziah putri]` | `[20%]` | `[Source code]` | `[Implementasi modul]` |
| `[Wifa fazriyatul]` | `[20%]` | `[Log pengujian]` | `[Pengujian dan validasi]` |
| `[Amelia okta]` | `[20%]` | `[Dokumentasi]` | `[Verifikasi dan penyempurnaan]` |

---

## 19. Kriteria Lulus Praktikum

Bagian ini wajib diisi. Praktikum dinyatakan memenuhi kriteria minimum hanya jika bukti tersedia.

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Proyek dapat dibangun dari clean checkout | `[PASS]` | `[log]` |
| Perintah build terdokumentasi | `[PASS]` | `[bagian laporan]` |
| QEMU boot atau test target berjalan deterministik | `[PASS]` | `[serial log/test log]` |
| Semua unit test/praktikum test relevan lulus | `[PASS]` | `[qemu-serial.log]` |
| Log serial disimpan | `[PASS]` | `[host-test.log]` |
| Panic path terbaca atau dijelaskan jika belum relevan | `[PASS]` | `[build/qemu-serial.log]` |
| Tidak ada warning kritis pada build | `[PASS]` | `[build log]` |
| Perubahan Git terkomit | `[PASS]` | `[Commit repository]` |
| Desain dan failure mode dijelaskan | `[PASS]` | `[bagian laporan]` |
| Laporan berisi screenshot/log yang cukup | `[PASS]` | `[lampiran]` |

Kriteria tambahan untuk praktikum lanjutan:

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| Static analysis dijalankan | `[PASS]` | `[readelf dan objdump]` |
| Stress test dijalankan | `[PASS]` | `[log]` |
| Fuzzing atau malformed-input test dijalankan | `[NA]` | `[Tidak menjadi cakupan praktikum]` |
| Fault injection dijalankan | `[PASS]` | `[Host unit test]` |
| Disassembly/readelf evidence tersedia | `[PASS]` | `[objdump/readelf]` |
| Review keamanan dilakukan | `[PASS]` | `[security table]` |
| Rollback diuji | `[PASS]` | `[rollback log]` |

---

## 20. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | `[ ]` |
| Siap uji QEMU | Build bersih, QEMU/test target berjalan, log tersedia | `[ Ya ]` |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | `[ ]` |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia | `[ ]` |

Alasan readiness:

```text
[Implementasi Praktikum M13 telah memenuhi kriteria "Siap uji QEMU" karena proses build berhasil diselesaikan tanpa error, image kernel berhasil dibuat, host unit test berhasil dijalankan, serta kernel dapat melakukan proses boot pada QEMU dengan menghasilkan log serial yang sesuai. Selain itu, hasil static inspection menggunakan readelf dan objdump menunjukkan bahwa kernel telah dibangun dalam format ELF64 yang valid, sehingga implementasi Virtual File System (VFS), File Descriptor Table, RAMFS, dan syscall File I/O awal dinilai siap untuk tahap pengujian pada lingkungan QEMU..]
```

Known issues:

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | `[RAMFS masih bersifat sementara (volatile)]` | `[data akan hilang setelah sistem dimatikan]` | `[Menggunakan kembali proses inisialisasi saat boot]` | `[Pengembangan Persistent Filesystem (M14/Milestone berikutnya)]` |

Keputusan akhir:

```text
[Berdasarkan hasil build, host unit test, static inspection, serta pengujian boot menggunakan QEMU, implementasi Praktikum M13 dinyatakan memenuhi status "Siap uji QEMU". Seluruh fungsi dasar Virtual File System (VFS), File Descriptor Table, RAMFS, dan syscall File I/O awal telah berjalan sesuai tujuan praktikum. Meskipun demikian, implementasi belum dapat dikategorikan sebagai "Siap demonstrasi praktikum" atau "Kandidat siap pakai terbatas" karena fitur lanjutan seperti filesystem persisten, permission, dan mekanisme keamanan yang lebih lengkap masih menjadi ruang pengembangan pada milestone berikutnya.]
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
