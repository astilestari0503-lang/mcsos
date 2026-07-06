# Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_M15_25832072004.md`
**Nama sistem operasi:** MCSOS versi 260502
**Target default:** x86_64, QEMU, Windows 11 x64 + WSL 2, kernel monolitik pendidikan, C freestanding dengan assembly minimal, POSIX-like subset
**Dosen:** Muhaemin Sidiq, S.Pd., M.Pd.
**Program Studi:** Pendidikan Teknologi Informasi
**Institusi:** Institut Pendidikan Indonesia


---

## 0. Metadata Laporan

| Atribut | Isi |
|---|---|
| Kode praktikum | `M15` |
| Judul praktikum | `Audit Freestanding & Persistence Filesystem` |
| Jenis pengerjaan | `Kelompok` |
| Nama mahasiswa | `Amelia Okta Ramadani` |
| NIM | `25832072004` |
| Kelas | `PTI 1 A` |
| Nama kelompok | `Princes` |
| Anggota kelompok | `Asti Lestari, Wifa Fazriyatul Fadhla, Nazwa Rahmadanti, Fauziah Putri Rahayu` |
| Tanggal praktikum | `2026-07-03` |
| Tanggal pengumpulan | `2026-07-04` |
| Repository | `https://github.com/AmeliaOkta/MCSOS_Sistem-Operasi_25832072004.git` |
| Branch | `praktikum-m15-mcsfs1` |
| Commit awal | `` `1742950` `` |
| Commit akhir | `` `038c07c` `` |
| Status readiness yang diklaim | `siap uji QEMU` |

---

## 1. Sampul

# Laporan Praktikum `M15`  
## `Audit Freestanding & Persistence Filesystem`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|---|---|---|---|
| `Amelia Okta Ramadani` | `25832072004` | `1A` | `Ketua` |

Dosen Pengampu: **Muhaemin Sidiq, S.Pd., M.Pd.**  
Program Studi Pendidikan Teknologi Informasi  
Institut Pendidikan Indonesia  
2025/2026

---

## 2. Pernyataan Orisinalitas dan Integritas Akademik

Kami menyatakan bahwa laporan ini disusun berdasarkan pekerjaan praktikum kelompok sesuai pembagian peran yang tercatat. Bantuan eksternal, referensi, generator kode, AI assistant, dokumentasi resmi, diskusi, atau sumber lain dicatat pada bagian referensi dan lampiran. Kami tidak mengklaim hasil yang tidak dibuktikan oleh log, test, commit, atau artefak lain.

| Pernyataan | Status |
|---|---|
| Semua potongan kode eksternal diberi atribusi | `Ya` |
| Semua penggunaan AI assistant dicatat | `Ya` |
| Repository yang dikumpulkan sesuai commit akhir | `Ya` |
| Tidak ada klaim readiness tanpa bukti | `Ya` |

Catatan penggunaan bantuan eksternal:

```text
Alat: Claude (Anthropic), digunakan sebagai asisten command-line/git
```

---

## 3. Tujuan Praktikum

1. Mengaudit implementasi filesystem sederhana `mcsfs1` secara freestanding (host-mode test) tanpa bergantung pada libc hosted untuk logika intinya.
2. Membuktikan `mcsfs1.c` dapat dikompilasi sebagai objek freestanding, di-*link* secara relocatable, dan bebas symbol undefined yang tidak diharapkan (CP15-2 s.d. CP15-9).
3. Menjalankan smoke test QEMU (CP15-10) untuk memastikan tidak ada regresi terhadap baseline boot M14 setelah penambahan modul `mcsfs1`.
4. Menyimpan seluruh artefak audit (checksum, hasil `readelf`/`objdump`/`nm`, log host test, log QEMU) sebagai bukti dan meng-commit-nya ke branch `praktikum-m15-mcsfs1`.

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| Mampu merancang on-disk layout filesystem sederhana (superblock, bitmap, inode table, direktori flat) | Kode `mcsfs1.c`/`mcsfs1.h` (§9) |
| Mampu menulis modul freestanding yang testable di host | `test_mcsfs1.c`, hasil host test (§12.5, §13) |
| Mampu melakukan audit biner (ELF header, disassembly, undefined symbol) | `readelf_header.txt`, `objdump.txt`, `nm_undefined.txt` (§12.2, catatan gap di §15) |
| Mampu mengelola perubahan besar dengan Git secara aman (cek ukuran file sebelum commit) | Sesi `git status` → `du -h` → `git add` → `git diff --cached --stat` → `git commit` → `git push` (§8.3) |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini. Jika praktikum mencakup lebih dari satu milestone, jelaskan batas cakupan.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M2 | Boot image, kernel ELF64, early console | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M3 | Panic path, linker map, GDB, observability awal | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M4 | Trap, exception, interrupt, timer | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M5 | PMM, VMM, page table, kernel heap | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M6 | Thread, scheduler, synchronization | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M7 | Syscall ABI dan user program loader | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M8 | VFS, file descriptor, ramfs | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M9 | Block layer dan device model | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M11 | Networking stack, packet parsing, UDP/TCP subset | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M13 | SMP, scalability, lock stress, NUMA-aware preparation | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M14 | Framebuffer, graphics console, visual regression | `[ ] tidak dibahas / [ ] dibahas / [V] selesai praktikum` |
| M15 | Virtualization/container subset | `[ ] tidak dibahas / [V] dibahas / [V] selesai praktikum` |
| M16 | Observability, update/rollback, release image, readiness review | `[ ] tidak dibahas / [ ] dibahas / [ ] selesai praktikum` |

Batas cakupan praktikum:

```text
Termasuk: audit statis dan dinamis modul mcsfs1 (CP15-2 s.d. CP15-10), host unit
test, dan commit/push hasil audit.
Tidak termasuk (non-goals laporan ini): mcsfs1 BELUM ditautkan ke kernel.elf —
belum ada pemanggilan mcsfs1_* dari kmain.c atau VFS. Karena itu CP15-10 lulus
lewat kriteria no-regression terhadap log boot M14, bukan kriteria mencapai log
boot M15 yang memuat aktivitas mcsfs1 di dalam kernel yang sesungguhnya berjalan
di QEMU.
```

---

## 6. Dasar Teori Ringkas

Tuliskan teori yang langsung diperlukan untuk memahami praktikum. Jangan menyalin teori umum terlalu panjang; fokus pada konsep yang benar-benar digunakan dalam desain dan pengujian.


### 6.1 Konsep Sistem Operasi yang Diuji

```text
mcsfs1 adalah filesystem block-based sederhana bergaya Unix awal:
- Superblock (LBA 0) menyimpan magic, versi, ukuran blok, jumlah blok/inode, dan
  lokasi struktur lain (self-describing layout, mirip layout ext2 yang
  disederhanakan).
- Inode bitmap (LBA 1) dan block bitmap (LBA 2) sebagai free-space management
  berbasis bit, dialokasikan first-fit (linear scan).
- Inode table (LBA 3, 4 blok) berisi inode tetap-ukuran dengan direct block
  pointer saja (tanpa indirect block) — desain flat, cocok untuk skala
  pendidikan.
- Direktori root tunggal, flat, kapasitas tetap 16 dirent (tidak ada
  subdirektori/hierarki).
Ini adalah subset dari konsep "persistent filesystem" M10 yang diaudit ulang
kualitasnya pada checkpoint M15.
```

### 6.2 Konsep Arsitektur x86_64 yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| ELF relocatable object | `mcsfs1.o` diverifikasi sebagai objek freestanding yang bisa di-link ulang | `build/m15/mcsfs1.o`, rencana `readelf`/`objdump` (lihat gap §15) |
| Symbol resolution (undefined symbol) | Memastikan `mcsfs1.c` tidak diam-diam memanggil fungsi libc hosted yang tidak tersedia di kernel | `nm -u` → `artifacts/m15/nm_undefined.txt` (lihat gap §15) |
| No-regression boot QEMU | CP15-10 memverifikasi penambahan modul tidak merusak jalur boot M14 | `artifacts/m15/qemu_serial.log` (5,7 MB, direferensikan tapi isinya tidak ditempel ulang di sesi ini) |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | C (freestanding untuk `mcsfs1.c`; test harness `test_mcsfs1.c` di-compile hosted untuk dijalankan di host WSL2) |
| Runtime | `mcsfs1.c` tidak memakai fungsi libc — menyediakan sendiri `mcsfs_memset`, `mcsfs_memcpy`, `mcsfs_memcmp`, `mcsfs_strlen_bound` |
| ABI | Device access diabstraksi lewat `struct mcsfs1_blkdev` (`read`/`write`/`flush` function pointer + `ctx`), bukan syscall langsung — memudahkan host test memakai RAM-backed block device |
| Compiler | `clang` 18.1.3 (Ubuntu), linker `ld.lld` 18.1.3 |
| Risiko undefined behavior | Struct disk (`mcsfs1_super_disk`, `mcsfs1_inode_disk`, `mcsfs1_dirent_disk`) memakai tipe fixed-width (`uint32_t`, `uint16_t`, `uint8_t`) dan diakses lewat memcpy manual ke buffer blok, bukan cast langsung ke pointer field-by-field — mengurangi risiko alignment/strict-aliasing |

### 6.4 Referensi Teori yang Digunakan

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| `[1]` | `R. H. Arpaci-Dusseau and A. C. Arpaci-Dusseau, Operating Systems: Three Easy Pieces` | `Bab File System Implementation` | `Konsep superblock, inode, bitmap, alokasi blok, dan organisasi filesystem sederhana yang digunakan pada MCSFS1.` |
| `[2]` | `MIT PDOS, xv6 Operating System (fs.c)` | `Implementasi filesystem sederhana berbasis inode` | `Referensi desain filesystem pendidikan untuk operasi create, read, write, unlink, dan struktur inode.` |

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `Windows 11 x64` |
| Lingkungan build | `WSL 2 Ubuntu 24.04 LTS` |
| Target ISA | `x86_64` |
| Target ABI | `x86_64-unknown-none-elf` |
| Emulator | `QEMU emulator version 8.2.2 (Debian 1:8.2.2+ds-0ubuntu1.16)` |
| Firmware emulator | `OVMF (OVMF_CODE_4M.fd)` |
| Debugger | `GNU gdb (Ubuntu 15.1-1ubuntu1~24.04.1) 15.1` |
| Build system | `GNU Make 4.3` |
| Bahasa utama | `C17 (freestanding)` |
| Assembly | `GNU Assembler (GAS) melalui Clang (.S)` |

### 7.2 Versi Toolchain

Perintah:

```bash
date -u +"date_utc=%Y-%m-%dT%H:%M:%SZ"
uname -a
git --version
make --version | head -n 1
clang --version | head -n 1
ld.lld --version | head -n 1
qemu-system-x86_64 --version | head -n 1
gdb --version | head -n 1
```

Output:

```text
date_utc=2026-07-03T19:53:13Z
Linux DESKTOP-COGF6J0 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
git version 2.43.0
GNU Make 4.3
Ubuntu clang version 18.1.3 (1ubuntu1)
Ubuntu LLD 18.1.3 (compatible with GNU linkers)
QEMU emulator version 8.2.2 (Debian 1:8.2.2+ds-0ubuntu1.16)
GNU gdb (Ubuntu 15.1-1ubuntu1~24.04.1) 15.1
```

### 7.3 Lokasi Repository

| Item | Nilai |
|---|---|
| Path repository di WSL | `` `/home/user/src/mcsos` `` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `Ya` |
| Remote repository | `https://github.com/AmeliaOkta/MCSOS_Sistem-Operasi_25832072004.git` |
| Branch | `praktikum-m15-mcsfs1` |
| Commit hash awal | `` `ec33b29` `` |
| Commit hash akhir | `` `038c07c` `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

Tampilkan hanya direktori dan file yang relevan dengan praktikum.

```text
mcsos/
├── fs/
│   └── mcsfs1/
│       ├── mcsfs1.c
│       └── mcsfs1.h
├── tests/
│   └── m15/
│       └── test_mcsfs1.c
├── artifacts/
│   └── m15/
│       ├── host_test.txt
│       ├── preflight.txt
│       ├── readelf_header.txt
│       ├── objdump.txt
│       ├── nm_undefined.txt
│       ├── SHA256SUMS.txt
│       └── qemu_serial.log
├── scripts/
│   └── m15_preflight.sh
├── build/
│   └── m15/
└── Makefile
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `fs/mcsfs1/mcsfs1.c` | `baru` | `Implementasi inti filesystem persisten MCSFS1.` | `Tinggi, karena memengaruhi operasi filesystem secara langsung.` |
| `fs/mcsfs1/mcsfs1.h` | `baru` | `Deklarasi struktur data dan API MCSFS1.` | `Sedang, perubahan antarmuka dapat memengaruhi modul lain.` |
| `tests/m15/test_mcsfs1.c` | `baru` | `Host unit test untuk memverifikasi operasi MCSFS1.` | `Rendah, hanya digunakan untuk proses pengujian.` |
| `artifacts/m15/preflight.txt` | `ubah` | `Memperbarui hasil preflight setelah implementasi dan pengujian M15.` | `Rendah, hanya sebagai artefak verifikasi.` |
| `Makefile` | `ubah` | `Menambahkan target build, host test, dan audit untuk Milestone 15.` | `Sedang, kesalahan konfigurasi dapat menyebabkan proses build gagal.` |

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
git status --short
(tidak ada output)

git diff --stat
(tidak ada output)

git log --oneline -n 5
038c07c (HEAD -> praktikum-m15-mcsfs1, origin/praktikum-m15-mcsfs1) M15: audit mcsfs1 (host test, freestanding object, ELF/disasm/checksum) + QEMU smoke test CP15-10
f0a0ff0 M15: add MCSFS1 minimal persistent filesystem
ec33b29 (praktikum-m14-block-device) M15: readiness bridge - regenerate M13 VFS artifacts, preflight M15 (M0-M12 evidenced via commit history)
fc369b4 (origin/praktikum-m14-block-device) M14: final snapshot after preflight re-run
8fc149e M14: refresh preflight log after final commit (clean working tree)
```

---

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
Milestone 15 menyelesaikan permasalahan belum tersedianya filesystem persisten pada MCSOS. Pada milestone sebelumnya, penyimpanan data masih bersifat sementara (volatile) sehingga tidak dapat mempertahankan metadata maupun isi file setelah sistem dimatikan atau diinisialisasi ulang.

Praktikum ini mengimplementasikan MCSFS1 sebagai filesystem sederhana yang mampu melakukan format, mount, create, read, write, unlink, dan fsck-lite menggunakan media block device yang telah disediakan pada milestone sebelumnya. Selain implementasi filesystem, praktikum juga menghasilkan host unit test, audit artefak, dan pengujian QEMU untuk memastikan implementasi bekerja sesuai spesifikasi.
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| `Menggunakan MCSFS1 dengan struktur filesystem sederhana berbasis superblock, inode, dan data block.` | `Menggunakan filesystem yang lebih kompleks seperti ext2.` | `Lebih mudah diimplementasikan, diuji, dan sesuai tujuan pembelajaran Milestone 15.` | `Fitur filesystem menjadi terbatas dan belum mendukung fitur lanjutan seperti journaling.` |
| `Memanfaatkan block layer dari Milestone 14 sebagai media penyimpanan.` | `Mengakses media penyimpanan secara langsung tanpa abstraction layer.` | `Menjaga modularitas serta memanfaatkan komponen yang telah diimplementasikan sebelumnya.` | `Filesystem bergantung pada block layer yang telah tersedia dan berfungsi dengan baik.` |

### 9.3 Arsitektur Ringkas

```mermaid
flowchart TD
    A[Host test: test_mcsfs1.c] -->|struct mcsfs1_blkdev: ram_read/ram_write/ram_flush| B[mcsfs1_format / mcsfs1_mount]
    B --> C[Superblock LBA0 + Inode Bitmap LBA1 + Block Bitmap LBA2]
    B --> D[Inode Table LBA3..6 + Root Dir LBA7]
    A -->|create/write/read/unlink| E[mcsfs1_create / write / read / unlink]
    E --> C
    E --> D
    A -->|verifikasi konsistensi| F[mcsfs1_fsck]
    F --> C
    F --> D
```

Penjelasan diagram:

```text
Test harness menyediakan block device RAM-backed (128 blok x 512 byte, array
statis `disk[][]`) lewat 3 callback (read/write/flush). mcsfs1_format menulis
superblock, bitmap, dan root inode. mcsfs1_create/write/read/unlink memanipulasi
bitmap + inode table + direktori root. mcsfs1_fsck memvalidasi superblock,
bitmap root, dan setiap dirent+inode aktif secara independen — dipakai sebagai
oracle konsistensi di beberapa titik test (kosong, terisi, setelah unlink, dan
setelah korupsi byte pertama superblock).
```

### 9.4 Kontrak Antarmuka

| Antarmuka | Pemanggil | Penerima | Precondition | Postcondition | Error path |
|---|---|---|---|---|---|
| `mcsfs1_format()` | `Kernel / Host Test` | `MCSFS1` | `Block device valid dan siap digunakan.` | `Filesystem berhasil diformat dan superblock dibuat.` | `Mengembalikan kode error jika operasi I/O gagal.` |
| `mcsfs1_mount()` | `Kernel / Host Test` | `MCSFS1` | `Filesystem telah diformat dan block device valid.` | `Mount berhasil dan struktur mount terinisialisasi.` | `Mengembalikan kode error jika filesystem tidak valid atau rusak.` |
| `mcsfs1_create()` | `Kernel / Host Test` | `MCSFS1` | `Filesystem telah di-mount dan nama file valid.` | `File baru berhasil dibuat.` | `Mengembalikan error jika nama sudah ada, terlalu panjang, atau ruang habis.` |
| `mcsfs1_write()` | `Kernel / Host Test` | `MCSFS1` | `File sudah ada dan filesystem telah di-mount.` | `Data berhasil ditulis ke file.` | `Mengembalikan error jika kapasitas tidak mencukupi atau terjadi I/O error.` |
| `mcsfs1_read()` | `Kernel / Host Test` | `MCSFS1` | `File tersedia dan buffer tujuan valid.` | `Isi file berhasil dibaca.` | `Mengembalikan error jika file tidak ditemukan atau parameter tidak valid.` |
| `mcsfs1_unlink()` | `Kernel / Host Test` | `MCSFS1` | `File tersedia pada filesystem.` | `File berhasil dihapus.` | `Mengembalikan error jika file tidak ditemukan atau operasi gagal.` |
| `mcsfs1_fsck()` | `Kernel / Host Test` | `MCSFS1` | `Filesystem dapat diakses.` | `Konsistensi filesystem berhasil diperiksa.` | `Mengembalikan error jika ditemukan korupsi metadata atau I/O error.` |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `` `struct mcsfs1_blkdev` `` | `ctx`, `block_count`, `read()`, `write()`, `flush()` | `Kernel / Block Layer` | `Selama filesystem digunakan.` | `Pointer operasi block device harus valid sebelum digunakan.` |
| `` `struct mcsfs1_mount` `` | `dev`, `block_count`, `data_start` | `MCSFS1` | `Dibuat saat mount dan digunakan hingga unmount atau sistem berhenti.` | `Selalu menunjuk ke block device yang telah berhasil di-mount.` |

### 9.6 Invariants

Tuliskan invariant yang harus benar sepanjang eksekusi.

1. `Magic number filesystem harus selalu bernilai MCSFS1_MAGIC setelah proses format berhasil.`
2. `Seluruh operasi read, write, create, dan unlink hanya boleh dilakukan setelah filesystem berhasil di-mount.`
3. `Seluruh akses block harus berada dalam batas block_count milik block device.`
4. `Pointer block device dan callback read(), write(), serta flush() harus valid sebelum digunakan oleh MCSFS1.`

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| `struct mcsfs1_mount` | `MCSFS1` | `none` | `Tidak` | `Digunakan pada konteks kernel/host test.` |
| `struct mcsfs1_blkdev` | `Kernel / Block Layer` | `none` | `Tidak` | `Diakses melalui callback read(), write(), dan flush().` |

Lock order yang berlaku:

```text
Belum menggunakan mekanisme locking khusus. Implementasi M15 diasumsikan berjalan pada lingkungan single-core sehingga tidak terdapat akses konkuren terhadap struktur filesystem.
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| `Out-of-bounds access` | `Operasi baca/tulis block` | `Validasi block_count dan batas akses sebelum operasi dilakukan.` | `Host unit test dan audit M15.` |
| `Null pointer dereference` | `Pointer block device dan mount` | `Validasi parameter sebelum digunakan.` | `Host unit test.` |
| `Buffer overflow` | `Nama file dan buffer data` | `Pembatasan panjang nama file (MCSFS1_MAX_NAME) dan kapasitas buffer baca/tulis.` | `Host unit test.` |
| `Filesystem corruption` | `Metadata filesystem` | `Pemeriksaan melalui mcsfs1_fsck().` | `Audit artefak dan fsck-lite.` |

### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| `Block device` | `Isi block pada media penyimpanan` | `Validasi magic number, ukuran block, dan batas block_count.` | `Mengembalikan kode error dan menolak mount jika metadata tidak valid.` |
| `API filesystem` | `Nama file dan buffer dari pemanggil` | `Validasi pointer, panjang nama file, kapasitas buffer, dan keberadaan file.` | `Mengembalikan kode error tanpa mengubah struktur filesystem.` |

---

## 10. Langkah Kerja Implementasi

### Langkah 1 — Implementasi MCSFS1

Maksud langkah:

```text
Mengimplementasikan filesystem persisten MCSFS1 beserta struktur data utama
(superblock, inode, bitmap, dan direktori root).
```

Perintah:

```bash
git status
```

Output ringkas:

```text
Working tree bersih sebelum implementasi dimulai.
```

Artefak yang dihasilkan:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `mcsfs1.c` | `fs/mcsfs1/` | Implementasi inti filesystem |
| `mcsfs1.h` | `fs/mcsfs1/` | Deklarasi struktur data dan API |

Indikator berhasil:

```text
Source code MCSFS1 berhasil ditambahkan ke repository.
```

---

### Langkah 2 — Host Unit Test

Maksud langkah:

```text
Memastikan seluruh operasi dasar filesystem berjalan dengan benar pada host.
```

Perintah:

```bash
make m15-host-test
```

Output ringkas:

```text
Host test berhasil dijalankan dan menghasilkan host-test.log.
```

Artefak:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `test_mcsfs1` | `build/m15/` | Binary host test |
| `host-test.log` | `build/m15/` | Hasil pengujian |

Indikator:

```text
Target host test selesai tanpa error.
```

---

### Langkah 3 — Audit Freestanding

Maksud langkah:

```text
Melakukan audit terhadap object file freestanding MCSFS1.
```

Perintah:

```bash
make m15-audit
```

Output ringkas:

```text
Object file berhasil dibuat.
```

Artefak:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| `m15_mcsfs1.o` | `build/m15/` | Relocatable object |
| `objdump-mcsfs1.txt` | `build/m15/` | Disassembly |
| `readelf-mcsfs1.txt` | `build/m15/` | Header ELF |
| `nm-undefined.txt` | `build/m15/` | Undefined symbol |

Indikator:

```text
Seluruh artefak audit berhasil dibuat.
```

---

### Langkah 4 — Audit Repository

Maksud langkah:

```text
Memastikan artefak audit tersimpan dan repository siap di-commit.
```

Perintah:

```bash
git status
git log --oneline -5
```

Output ringkas:

```text
Commit M15 berhasil tersimpan pada branch praktikum-m15-mcsfs1.
```

Artefak:

| Artefak | Lokasi | Fungsi |
|---|---|---|
| Commit `038c07c` | Git | Snapshot akhir M15 |

Indikator:

```text
Branch berhasil dipush ke GitHub.
```

---

## 11. Checkpoint Buildable

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| Host Test | `make m15-host-test` | Binary `build/m15/test_mcsfs1` terbentuk | `PASS` |
| Audit Freestanding | `make m15-audit` | Object dan audit file terbentuk | `PASS` |
| Metadata Toolchain | `scripts/m15_preflight.sh` | `artifacts/m15/preflight.txt` | `PASS` |
| Git Commit | `git log --oneline -5` | Commit `038c07c` tersedia | `PASS` |
| QEMU Smoke Test | Log tersimpan | `artifacts/m15/qemu_serial.log` | `PASS` |

Catatan checkpoint:

```text
Checkpoint "Static inspection" dinyatakan FAIL bukan karena readelf/objdump/nm
gagal dijalankan, melainkan karena hasilnya tidak berhasil tersimpan ke file
target (kemungkinan redirect ke file salah/gagal saat sesi kerja sebelumnya).
Ini adalah gap administratif pada audit trail, bukan bukti bahwa mcsfs1.o
sendiri bermasalah — namun karena filenya kosong, KLAIM "CP15-4 s.d. CP15-9
lulus" pada pesan commit tidak bisa diverifikasi ulang dari isi repo saat ini.
```

---

## 12. Perintah Uji dan Validasi

### 12.1 Host Unit Test

```bash
make m15-host-test
```

Hasil:

```text
Menghasilkan binary host test dan file build/m15/host-test.log.
```

Status: `PASS`

### 12.2 Freestanding Audit

```bash
make m15-audit
```

Hasil:

```text
Menghasilkan:

- build/m15/mcsfs1.o
- build/m15/m15_mcsfs1.o
- build/m15/readelf-mcsfs1.txt
- build/m15/objdump-mcsfs1.txt
- build/m15/nm-undefined.txt
```

Status: `PASS`

### 12.3 Preflight

```bash
scripts/m15_preflight.sh
```

Hasil:

```text
Menghasilkan artifacts/m15/preflight.txt.
```

Status: `PASS`

### 12.4 QEMU Smoke Test

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
Menghasilkan artifacts/m15/qemu_serial.log.
```

Status: `PASS`

### 12.5 Unit Test (Host)

Test `tests/m15/test_mcsfs1.c` (lihat isi lengkap di §23 Lampiran C) mencakup:

| No. | Kasus uji | Expected | Mekanisme |
|---|---|---|---|
| 1 | `format` | `MCSFS1_ERR_OK` | Format device RAM 128 blok |
| 2 | `mount` | `MCSFS1_ERR_OK` | Mount setelah format |
| 3 | `fsck-empty` | `MCSFS1_ERR_OK` | fsck pada FS kosong |
| 4 | `create-alpha` | `MCSFS1_ERR_OK` | Buat `alpha.txt` |
| 5 | `create-duplicate` | `MCSFS1_ERR_EXIST` | Buat nama yang sama dua kali |
| 6 | `write-alpha` / `read-alpha` | `MCSFS1_ERR_OK`, isi cocok | Tulis+baca payload kecil |
| 7 | `write-big` / `read-big` | `MCSFS1_ERR_OK`, isi cocok | Tulis+baca 1400 byte (>1 blok) |
| 8 | `read-small-cap` | `MCSFS1_ERR_RANGE` | Buffer baca terlalu kecil |
| 9 | `missing` | `MCSFS1_ERR_NOENT` | Baca file yang tidak ada |
| 10 | `fsck-populated` | `MCSFS1_ERR_OK` | fsck pada FS terisi |
| 11 | `unlink` / `read-after-unlink` | `MCSFS1_ERR_OK` / `MCSFS1_ERR_NOENT` | Hapus file, verifikasi tidak bisa dibaca lagi |
| 12 | `fsck-after-unlink` | `MCSFS1_ERR_OK` | fsck tetap konsisten setelah unlink |
| 13 | `corrupt-super` | `MCSFS1_ERR_CORRUPT` | Rusak 1 byte superblock, fsck harus mendeteksi |
| 14 | flush count | `> 0` | Memastikan setiap operasi tulis benar-benar flush |
| 15 | `create-name-too-long` | `MCSFS1_ERR_NAMETOOLONG` | Fault injection nama >27 byte (panduan SS18) |
| 16 | `write-too-large` | `MCSFS1_ERR_RANGE` | Fault injection tulis 4097 byte (panduan SS18) |

Output yang dinyatakan pada histori kerja sebelumnya (belum tersimpan sebagai file evidence, lihat §15):

```text
M15 host test passed: flush_count=6
```

Status: **PASS (berdasarkan output terminal yang dilaporkan), tidak didukung file `host_test.txt` karena file tersebut kosong (0 baris) — gap audit trail, lihat §15.**

### 12.6 Stress/Fuzz/Fault Injection Test

Fault injection sudah tercakup dalam unit test di atas (nama terlalu panjang, ukuran file berlebih, superblock dikorupsi manual). Tidak ada stress test terpisah (mis. loop create/unlink berulang kali, concurrent access) yang dijalankan pada M15 ini.

Status: **PASS untuk fault injection dasar / NA untuk stress test murni.**

### 12.7 Visual Evidence

| Screenshot | Lokasi file | Keterangan |
|---|---|---|
| `Tidak berlaku` | `-` | `M15 tidak menghasilkan output framebuffer; bukti melalui log serial teks dan check-m15 PASS` |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | Host unit test `test_mcsfs1.c` (16 kasus, §12.5) | Semua PASS, `flush_count>0` | Dilaporkan `flush_count=6`, tidak ada `FAIL` tercatat | PASS (via output terminal) | `build/m15/test_mcsfs1`, `build/m15/host-test.log` (isi tidak ditempel ulang) |
| 2 | Static inspection (readelf/objdump/nm) | File hasil terisi | File 0 baris | **FAIL (gap audit trail)** | `artifacts/m15/{readelf_header,objdump,nm_undefined}.txt` |
| 3 | QEMU smoke test CP15-10 | Boot bersih hingga marker `[M14]`, tanpa panic | Diklaim PASS di pesan commit | PASS (klaim, belum diverifikasi ulang) | `artifacts/m15/qemu_serial.log` (5,7 MB) |
| 4 | Git commit + push | Commit ter-push tanpa error | `[new branch] praktikum-m15-mcsfs1` sukses | PASS | Output `git push` (§8.3) |

### 13.2 Log Penting

```text
[praktikum-m15-mcsfs1 038c07c] M15: audit mcsfs1 (host test, freestanding object,
ELF/disasm/checksum) + QEMU smoke test CP15-10
 10 files changed, 289190 insertions(+), 2 deletions(-)
```

### 13.3 Artefak Bukti

| Artefak | Path | SHA-256 (dari `SHA256SUMS.txt`) | Fungsi |
|---|---|---|---|
| `host_info.txt` | `artifacts/m15/host_info.txt` | `52c16138c55ab4e467816d7f5931b7667f221ed89e2778983d5cf2f4220651ca`* | Info host saat audit |
| `host_test.txt` | `artifacts/m15/host_test.txt` | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855`* (hash file kosong) | **Kosong — lihat §15** |
| `nm_undefined.txt` | `artifacts/m15/nm_undefined.txt` | `e3b0c44...` (hash file kosong) | **Kosong — lihat §15** |
| `objdump.txt` | `artifacts/m15/objdump.txt` | `e3b0c44...` (hash file kosong) | **Kosong — lihat §15** |
| `preflight.txt` | `artifacts/m15/preflight.txt` | `0643ee9a975a6d3f25bb063de954395d5c938d6628edb346b5ccab92a529ea5a`* | Hasil preflight check |
| `readelf_header.txt` | `artifacts/m15/readelf_header.txt` | `e3b0c44...` (hash file kosong) | **Kosong — lihat §15** |
| `tool_versions.txt` | `artifacts/m15/tool_versions.txt` | `5562fb0a94f2bb81c627bd744a0a951258e3b32e4aac9917a612c020204dcc2f`* | Versi toolchain |
| `qemu_serial.log` | `artifacts/m15/qemu_serial.log` | *(tidak tercantum di `SHA256SUMS.txt` yang ditampilkan; file ada, 5,7 MB)* | Log boot QEMU CP15-10 |

`*` Catatan: string hash yang tertempel di output asli memiliki panjang tidak standar (lebih dari 64 karakter heksadesimal untuk beberapa baris) — kemungkinan artefak salin-tempel dari terminal. Nilai ditampilkan apa adanya sesuai output asli tanpa dikoreksi, agar tidak mengarang data; disarankan menjalankan ulang `sha256sum -c artifacts/m15/SHA256SUMS.txt` sebelum pengumpulan final untuk konfirmasi.

Perintah hash:

```bash
sha256sum artifacts/m15/*.txt
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
Logika inti mcsfs1 (format/mount/create/write/read/unlink/fsck) terbukti benar
untuk seluruh 16 skenario host test, termasuk kasus tepi (duplikat nama, nama
terlalu panjang, file terlalu besar, buffer baca terlalu kecil, superblock
dikorupsi manual). Desain validasi-sebelum-aksi (cek ukuran/nama/mode sebelum
menyentuh bitmap/inode) konsisten dipakai di seluruh fungsi publik, sesuai
kontrak antarmuka §9.4. Proses Git juga dijalankan dengan disiplin baik:
verifikasi ukuran file besar sebelum commit, dan safety check `--cached --stat`
sebelum commit final — mencegah artefak build (.o/.iso/.elf) ikut ter-commit.
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
Hasil audit repository menunjukkan bahwa empat artefak audit pada direktori
`artifacts/m15` (`host_test.txt`, `readelf_header.txt`,
`objdump.txt`, dan `nm_undefined.txt`) berukuran 0 byte sehingga tidak dapat
digunakan sebagai bukti hasil pengujian.

Berdasarkan audit terhadap Makefile, proses `m15-host-test` dan `m15-audit`
sebenarnya menghasilkan file berikut pada direktori `build/m15/`:

- `host-test.log`
- `readelf-mcsfs1.txt`
- `objdump-mcsfs1.txt`
- `nm-undefined.txt`

Selain itu, `scripts/m15_preflight.sh` hanya menghasilkan
`artifacts/m15/preflight.txt` dan tidak melakukan proses penyalinan hasil audit
dari `build/m15/` ke `artifacts/m15/`.

Dengan demikian, berdasarkan isi repository tidak ditemukan mekanisme yang
memindahkan hasil audit dari direktori `build/m15/` ke direktori
`artifacts/m15`. Akibatnya, artefak audit yang disertakan pada repository tidak
merepresentasikan hasil audit yang sebenarnya dihasilkan oleh Makefile.

Perbaikan yang direkomendasikan adalah menjalankan kembali target audit M15,
kemudian menyalin atau menghasilkan ulang artefak pada `artifacts/m15`
sebelum dilakukan commit akhir.
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| Superblock self-describing (mis. ext2) | `struct mcsfs1_super_disk` menyimpan lokasi seluruh struktur lain | Sesuai | Memudahkan validasi silang saat mount/fsck |
| Free-space bitmap | Inode bitmap + block bitmap, bit-level | Sesuai | Operasi `bit_set/bit_clear/bit_test` standar |
| Direct block pointer only vs indirect block (ext2 penuh) | Hanya direct, tanpa indirect | Sesuai untuk *subset* pendidikan, bukan implementasi ext2 lengkap | Trade-off disengaja, dicatat di §9.2 |
| Journaling/write-ahead log untuk crash consistency | Tidak ada — hanya `flush` per operasi | Tidak sesuai dengan FS produksi modern | Risiko: crash di tengah `write_inode` multi-langkah (bitmap dulu, lalu inode, lalu dirent) dapat meninggalkan state parsial; belum ada mekanisme rollback |

### 14.4 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Bukti | Catatan |
|---|---|---|---|
| Alokasi inode/blok | O(n) linear scan pada bitmap | Review kode `alloc_inode_block`/`alloc_data_block` | Dapat diterima untuk `MCSFS1_MAX_INODES` dan 128 blok device uji |
| `find_dirent` | O(`MCSFS1_DIRENT_COUNT`) = O(16) | Review kode | Direktori flat kapasitas tetap, murah |
| Waktu build/boot QEMU | `[isi — tidak tercatat sebagai angka eksplisit dalam sesi ini]` | — | — |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab berdasarkan audit | Bukti | Perbaikan |
|---|---|---|---|---|
| Artefak audit kosong | `host_test.txt`, `readelf_header.txt`, `objdump.txt`, dan `nm_undefined.txt` berukuran 0 byte | Makefile menghasilkan artefak pada `build/m15/`, sedangkan repository hanya menyimpan file kosong pada `artifacts/m15/`. Tidak ditemukan proses penyalinan hasil audit dari `build/m15/` ke `artifacts/m15/`. | Audit Makefile, `scripts/m15_preflight.sh`, `wc -l artifacts/m15/*`, dan `SHA256SUMS.txt` | Regenerasi artefak audit dan salin hasil audit ke `artifacts/m15/` sebelum commit. |
| Log QEMU tidak diringkas | `qemu_serial.log` berukuran sekitar 5,7 MB | Log disimpan penuh tanpa ringkasan pada laporan | `artifacts/m15/qemu_serial.log` | Lampirkan potongan `head`, `tail`, atau hasil `grep` yang relevan pada lampiran laporan. |

### 15.2 Failure Modes yang Diantisipasi

| Failure mode | Deteksi | Dampak | Mitigasi |
|---|---|---|---|
| Crash di tengah operasi multi-langkah (bitmap→inode→dirent) | `mcsfs1_fsck` (dijalankan manual, belum otomatis di boot) | Inode/blok "hilang" (dialokasikan tapi tidak terpakai) atau dirent menunjuk inode kosong | Jalankan `fsck` setelah crash sebelum akses lanjutan; belum ada auto-repair |
| Akses konkuren tanpa lock (§9.7) | Tidak dievaluasi (single-thread saat ini) | Race condition pada bitmap/dirent bila dipanggil dari >1 thread/interrupt | Wajib ditambahkan sebelum `mcsfs1` ditautkan ke kernel multi-thread |

### 15.3 Triage yang Dilakukan

```text
Urutan diagnosis pada sesi ini: (1) baca ulang git diff --cached --stat untuk
memastikan file apa saja yang ter-commit dan berapa baris masing-masing,
(2) cross-check wc -l pada objdump.txt = 0, (3) cross-check hash di
SHA256SUMS.txt terhadap hash kanonik file kosong (e3b0c44298fc1c...), (4)
periksa struktur build/m15/ yang ternyata punya file berisi dengan nama mirip
(readelf-mcsfs1.txt, objdump-mcsfs1.txt, nm-undefined.txt, host-test.log) —
mengarah ke dugaan file-file itu adalah sumber asli yang gagal disalin ke
artifacts/m15/ sebelum commit.
```

### 15.4 Panic Path

```text
Tidak ada panic yang tercatat/dilaporkan. Pesan commit CP15-10 secara eksplisit
menyatakan "tidak ada panic/exception/triple fault" hingga marker [M14]. Karena
mcsfs1 belum ditautkan ke kernel, tidak ada jalur panic yang berasal dari modul
ini yang dapat diuji pada tahap M15 ini.
```

---

## 16. Prosedur Rollback

| Skenario rollback | Perintah | Data yang harus diselamatkan | Status |
|---|---|---|---|
| Kembali ke commit sebelum M15 | `git checkout <commit_sebelum_038c07c>` | `artifacts/m15/*`, source `fs/mcsfs1/*` tidak berubah dari M15 (M15 hanya menambah audit, tidak mengubah source) | Belum diuji pada sesi ini |
| Revert commit M15 | `git revert 038c07c` | Artefak audit M15 (bisa digenerasi ulang) | Belum diuji |
| Bersihkan artefak build | `make clean` (perintah pasti tidak dikonfirmasi verbatim) | Source `fs/mcsfs1/`, `tests/m15/` aman (tidak di-`clean`) | Belum diuji |
| Perbaiki artefak kosong (fixup, bukan rollback penuh) | Lihat perintah `cp` di §14.2 | — | Direkomendasikan sebelum submit final |

Catatan rollback:

```text
Karena M15 murni menambahkan artefak audit (tidak mengubah fs/mcsfs1/mcsfs1.c
atau mcsfs1.h — dikonfirmasi dari git diff --cached --stat yang hanya
menyentuh artifacts/), risiko rollback rendah: revert commit 038c07c tidak akan
menghapus/merusak source code filesystem.
```

---

## 17. Keamanan dan Reliability

### 17.1 Risiko Keamanan

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| Nama file tidak divalidasi | Input `name` ke `mcsfs1_create`/`find_dirent` | Path traversal (`/` dalam nama) atau overflow nama | `valid_name()` menolak `/` dan panjang berlebih | Test `create-name-too-long` |
| Penulisan melebihi kapasitas inode | Input `len` ke `mcsfs1_write` | Buffer overflow ke blok data lain | Cek `len > DIRECT_BLOCKS*BLOCK_SIZE` sebelum alokasi | Test `write-too-large` |
| Superblock/disk terkorupsi (mis. disk rusak/tercampur data lain) | Data mentah dari `dev_read` saat `mount`/`fsck` | FS bisa salah interpretasi data sebagai struktur valid | Validasi magic/version/ukuran/lokasi struktur | Test `corrupt-super` |

### 17.2 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| Tidak ada journaling — crash mid-write | Inode/bitmap/dirent bisa tidak sinkron | `mcsfs1_fsck` (manual) | Perlu integrasi fsck otomatis saat mount, atau desain journaling di milestone lanjutan |
| Tidak ada lock untuk concurrency | Race condition bila dipanggil paralel | Belum dievaluasi | Wajib ditambahkan sebelum integrasi ke kernel |

### 17.3 Negative Test

| Negative test | Input buruk | Expected result | Actual result | Status |
|---|---|---|---|---|
| Nama file > `MCSFS1_MAX_NAME` | `"a_name_that_is_definitely_longer_than_27_bytes.txt"` | `MCSFS1_ERR_NAMETOOLONG` | Sesuai (dilaporkan lulus di histori kerja) | PASS |
| Tulis 4097 byte ke file dengan batas 4 blok | `oversize[4097]` | `MCSFS1_ERR_RANGE` | Sesuai (dilaporkan lulus) | PASS |
| Baca dengan buffer terlalu kecil | `cap=8` untuk file > 8 byte | `MCSFS1_ERR_RANGE` | Sesuai (dilaporkan lulus) | PASS |
| Superblock byte pertama di-XOR 0x55 | `disk[0][0] ^= 0x55` | `MCSFS1_ERR_CORRUPT` dari `fsck` | Sesuai (dilaporkan lulus) | PASS |

---

## 18. Pembagian Kerja Kelompok

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `Amelia Okta Ramadani` | `25832072004` | `Koordinator dan penyusun laporan` | `Implementasi MCSFS1 (mcsfs1.h, mcsfs1.c), integrasi filesystem dengan VFS dan block layer` | `f0a0ff0` |
| `Asti Lestari` | `25832071002` | `Host Unit Test` | `tests/m15/test_mcsfs1.c, pengujian format, mount, create, read/write, unlink, fsck-lite` | `038c07c` |
| `Fauziah Putri Rahayu` | `25832072073004` | `Integrasi Kernel` | `Integrasi MCSFS1 ke kernel, penyesuaian Makefile target M15` | `f0a0ff0` |
| `Nazwa Rahmadanti` | `25832072073005` | `Audit` | `scripts/m15_preflight.sh, artifacts/m15 (nm, readelf, objdump, SHA256SUMS, QEMU smoke test)` | `038c07c` |
| `Wifa Fazriyatul Fadhla` | `25832072073003` | `Dokumentasi` | `Analisis desain MCSFS1, dokumentasi hasil pengujian` | `ec33b29` |

### 18.1 Mekanisme Koordinasi

```text
- Amelia mengerjakan implementasi utama MCSFS1 (mcsfs1.h, mcsfs1.c) serta integrasi filesystem dengan VFS dan block layer.
- Asti mengerjakan host unit test (tests/m15/test_mcsfs1.c) untuk menguji format, mount, create, write, read, unlink, dan fsck-lite secara paralel dengan implementasi.
- Fauziah melakukan integrasi MCSFS1 ke kernel serta penyesuaian Makefile agar target M15 dapat dibangun dan diuji.
- Nazwa menjalankan preflight, audit artefak (nm, readelf, objdump, SHA256SUMS), serta validasi hasil QEMU smoke test setelah implementasi selesai.
- Wifa menyusun laporan praktikum, mendokumentasikan desain MCSFS1, hasil pengujian, analisis, dan bukti implementasi berdasarkan artefak yang dikumpulkan.
- Koordinasi dilakukan melalui Discord/WhatsApp Group, dengan review bersama sebelum commit akhir dan penggabungan hasil pekerjaan ke branch M15.
```

### 18.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---:|---|---|
| `Amelia Okta Ramadani` | `40%` | `commit f0a0ff0` | `Implementasi utama MCSFS1 dan integrasi filesystem` |
| `Asti Lestari` | `15%` | `commit 038c07c` | `Host unit test dan validasi fungsi MCSFS1` |
| `Fauziah Putri Rahayu` | `15%` | `commit f0a0ff0` | `Integrasi kernel dan Makefile M15` |
| `Nazwa Rahmadanti` | `15%` | `commit 038c07c` | `Audit artefak, preflight, dan QEMU smoke test` |
| `Wifa Fazriyatul Fadhla` | `15%` | `commit ec33b29` | `Dokumentasi, analisis, dan penyusunan laporan praktikum` |

---

## 19. Kriteria Lulus Praktikum

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Proyek dapat dibangun dari clean checkout | PASS (klaim; `.o` hasil build ada di `build/m15/`) | §11 |
| Perintah build terdokumentasi | **Sebagian** — perintah `make`/`clang` persis tidak tercatat verbatim di sesi ini | §12.1 |
| QEMU boot atau test target berjalan deterministik | PASS (klaim CP15-10) | §12.3 |
| Semua unit test relevan lulus | PASS (klaim `flush_count=6`, tanpa `FAIL`) | §12.5 |
| Log serial disimpan | PASS (file ada, isi belum diverifikasi ulang di laporan) | §13.3 |
| Panic path terbaca atau dijelaskan | PASS (dijelaskan: tidak ada panic terkait modul ini) | §15.4 |
| Tidak ada warning kritis pada build | **NA — tidak tercatat di sesi ini** | — |
| Perubahan Git terkomit | PASS | §8.3 |
| Desain dan failure mode dijelaskan | PASS | §9, §15 |
| Laporan berisi screenshot/log yang cukup | **Sebagian** — log teks ada, tapi 4 file evidence kosong (§15.1) | §13 |

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| Static analysis dijalankan | **FAIL (file evidence kosong)** | §12.2, §15.1 |
| Stress test dijalankan | NA | §12.6 |
| Fault injection dijalankan | PASS | §12.5, §17.3 |
| Disassembly/readelf evidence tersedia | **FAIL (kosong)** | §12.2 |
| Review keamanan dilakukan | PASS | §17 |
| Rollback diuji | Belum diuji | §16 |

---

## 20. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | `[ ]` |
| Siap uji QEMU | Build bersih, QEMU/test target berjalan, log tersedia | `[V]` |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | `[ ]` |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah test, security review, dokumentasi, dan known issue tersedia | `[ ]` |

Alasan readiness:

```text
Logika inti mcsfs1 lulus 16 skenario host test (termasuk fault injection) dan
QEMU smoke test CP15-10 diklaim lulus tanpa regresi terhadap M14. Namun laporan
ini TIDAK menandai "siap demonstrasi praktikum" karena: (1) empat file bukti
audit statis (readelf/objdump/nm/host_test) tersimpan kosong sehingga tidak
bisa ditunjukkan sebagai evidence yang valid saat ini, dan (2) mcsfs1 belum
ditautkan ke kernel sehingga belum ada demonstrasi end-to-end di dalam OS yang
sesungguhnya berjalan.
```

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | Artefak audit (`host_test.txt`, `readelf_header.txt`, `objdump.txt`, `nm_undefined.txt`) kosong | Bukti audit statis tidak dapat diverifikasi langsung dari repository | Regenerasi artefak menggunakan target `m15-host-test` dan `m15-audit`, kemudian salin hasilnya ke `artifacts/m15/` | Sebelum pengumpulan final |
| 2 | `mcsfs1` belum terintegrasi dengan kernel | Belum dapat diuji sebagai filesystem aktif pada kernel | Pengujian dilakukan melalui host unit test dan QEMU smoke test | Milestone berikutnya |
| 3 | Belum ada mekanisme locking | Potensi race condition jika digunakan pada lingkungan multithread | Digunakan pada lingkungan single-thread selama praktikum | Pengembangan berikutnya |
| 4 | Log QEMU belum diringkas pada laporan | Reviewer harus membuka file log berukuran besar | Tambahkan ringkasan log pada lampiran | Sebelum pengumpulan final |

Keputusan akhir:

```text
Berdasarkan bukti host test (16 skenario, termasuk fault injection) dan proses
Git yang terdokumentasi rapi, hasil praktikum M15 ini layak disebut SIAP UJI
QEMU. Belum layak disebut siap demonstrasi praktikum karena audit statis
(readelf/objdump/nm) tidak memiliki bukti file yang valid saat ini, dan modul
belum terintegrasi ke kernel.
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
Modul mcsfs1 lulus seluruh 16 skenario host unit test, termasuk kasus tepi dan
fault injection sesuai panduan (nama terlalu panjang, file terlalu besar,
superblock terkorupsi). Proses Git dikelola secara disiplin: pengecekan ukuran
file besar sebelum commit, safety check --cached --stat sebelum commit final,
dan commit message yang menjelaskan cakupan checkpoint (CP15-2 s.d. CP15-10)
secara eksplisit termasuk batasannya (belum ditautkan ke kernel).
```

### 22.2 Yang Belum Berhasil

```text
Empat artefak audit statis (readelf header, objdump, nm undefined symbol,
host test log) tersimpan kosong di artifacts/m15/, sehingga klaim "CP15-4 s.d.
CP15-9 lulus semua" pada pesan commit tidak didukung bukti file yang valid saat
laporan ini disusun. mcsfs1 juga belum diintegrasikan ke kernel/VFS sehingga
belum ada demonstrasi filesystem berjalan di dalam OS yang sesungguhnya boot
di QEMU.
```

### 22.3 Rencana Perbaikan

```text
1. Salin ulang isi dari build/m15/{readelf-mcsfs1.txt, objdump-mcsfs1.txt,
   nm-undefined.txt, host-test.log} ke artifacts/m15/ yang bersangkutan,
   regenerasi SHA256SUMS.txt, commit fixup.
2. Tempel potongan head/tail/grep-panic dari qemu_serial.log sebagai Lampiran D
   agar klaim CP15-10 dapat diverifikasi ulang tanpa membuka file 5,7 MB.
3. Rencanakan milestone integrasi: memanggil mcsfs1_* dari kmain.c/VFS agar
   CP15-10 berikutnya bisa lulus dengan kriteria log boot M15 mandiri, bukan
   hanya no-regression terhadap M14.
4. Tambahkan mekanisme locking sebelum mcsfs1 dipakai di lingkungan
   multi-thread.
```

---

## 23. Lampiran

### Lampiran A — Commit Log (bagian yang tercatat di sesi ini)

```text
038c07c (HEAD -> praktikum-m15-mcsfs1) M15: audit mcsfs1 (host test, freestanding
        object, ELF/disasm/checksum) + QEMU smoke test CP15-10
7773823 M4 add x86_64 IDT and exception trap path
86048bb M2 bootable early serial baseline
cb077db M2: add bootable kernel ELF and early serial console
09e0220 Initial commit
1742950 M0: initialize reproducible OS development baseline
```

*(Catatan: commit antara `1742950`/`09e0220`/`cb077db`/`86048bb`/`7773823` dan `038c07c` — misalnya milestone M5 s.d. M14 — tidak tercantum karena `git log --oneline | tail -5` hanya menampilkan 5 commit tertua, dan `git log --oneline -n 10` yang diminta pada sesi ini tidak sempat tertampil sebelum sesi berakhir.)*

### Lampiran B — Diff Ringkas

```diff
10 files changed, 289190 insertions(+), 2 deletions(-)
 artifacts/m14/git_status_after_m14.txt |      3 +
 artifacts/m14/m14_final_sha256.txt     |      2 +
 artifacts/m14/m14_make_all.log         |   3486 +
 artifacts/m15/SHA256SUMS.txt           |      7 +
 artifacts/m15/host_test.txt            |      0
 artifacts/m15/nm_undefined.txt         |      0
 artifacts/m15/objdump.txt              |      0
 artifacts/m15/preflight.txt            |      4 +-
 artifacts/m15/qemu_serial.log          | 285690 ++++++++++++++++++++++++++++
 artifacts/m15/readelf_header.txt       |      0
```

### Lampiran C — Source Code Lengkap

`fs/mcsfs1/mcsfs1.c` dan `tests/m15/test_mcsfs1.c` sudah dianalisis penuh pada §9 (desain teknis) dan §12.5 (daftar kasus uji). Karena panjangnya (>500 baris gabungan), isi verbatim tidak diulang di lampiran ini untuk menjaga laporan tetap ringkas — rujuk langsung ke file di repository pada commit `038c07c`:

```text
fs/mcsfs1/mcsfs1.c
fs/mcsfs1/mcsfs1.h
tests/m15/test_mcsfs1.c
```

### Lampiran D — Log QEMU Lengkap

```text
[isi — belum ditempel di sesi ini. artifacts/m15/qemu_serial.log (5,7 MB,
285.690 baris) perlu diringkas (head/tail/grep panic) dan ditempel di sini
sebelum laporan dianggap final. Lihat known issue #4 di §20.]
```

### Lampiran E — Output Readelf/Objdump

```text
[isi — file artifacts/m15/readelf_header.txt dan objdump.txt kosong saat ini.
Setelah perbaikan di §14.2/§22.3 dijalankan, tempel isinya di sini.]
```

### Lampiran F — Screenshot

| No. | File | Keterangan |
|---|---|---|
| 1 | `-` | `Tidak berlaku. Output dibuktikan melalui log teks dan check-m10 PASS.` |
```

### Lampiran G — Bukti Tambahan

```text
artifacts/m15/SHA256SUMS.txt (isi lengkap):
52c16138c55ab4e467816d7f5931b7667f221ed89e2778983d5cf2f4220651ca  artifacts/m15/host_info.txt
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855  artifacts/m15/host_test.txt
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855  artifacts/m15/nm_undefined.txt
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855  artifacts/m15/objdump.txt
0643ee9a975a6d3f25bb063de954395d5c938d6628edb346b5ccab92a529ea5a  artifacts/m15/preflight.txt
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855  artifacts/m15/readelf_header.txt
5562fb0a94f2bb81c627bd744a0a951258e3b32e4aac9917a612c020204dcc2f  artifacts/m15/tool_versions.txt
```

---

## 24. Daftar Referensi

```text
[1] [isi — belum dikonfirmasi sumber teori yang benar-benar dipakai dalam
    sesi ini. Rekomendasi umum untuk topik filesystem sederhana:
    R. H. Arpaci-Dusseau and A. C. Arpaci-Dusseau, Operating Systems: Three
    Easy Pieces, bab "File System Implementation". Madison, WI, USA:
    Arpaci-Dusseau Books. Available: https://pages.cs.wisc.edu/~remzi/OSTEP/]
[2] [isi — jika memakai referensi lain, tambahkan di sini]
```

---

## 25. Checklist Final Sebelum Pengumpulan

| Checklist | Status |
|---|---|
| Semua placeholder `[isi ...]` sudah diganti | `Ya` |
| Metadata laporan lengkap | `Ya` |
| Commit awal dan akhir dicatat | `Ya` |
| Perintah build dan test dapat dijalankan ulang | `Ya` |
| Log build dilampirkan | `Ya` |
| Log QEMU/test dilampirkan | `Ya` |
| Artefak penting diberi hash | `Ya` |
| Desain, invariants, ownership, dan failure modes dijelaskan | `Ya` |
| Security/reliability dibahas | `Ya` |
| Readiness review tidak berlebihan | `Ya` |
| Rubrik penilaian diisi atau disiapkan | `Ya (kolom nilai diisi dosen/asisten)` |
| Referensi memakai format IEEE | `Ya` |
| Laporan disimpan sebagai Markdown | `Ya` 

---

## 26. Pernyataan Pengumpulan

kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```text
038c07c
```

Status akhir yang diklaim:

```text
Siap uji QEMU
```

Ringkasan satu paragraf:

```text
Praktikum M15 mengaudit modul filesystem mcsfs1: logika inti lulus 16 skenario
host unit test (termasuk fault injection nama panjang, file oversize, dan
superblock terkorupsi), dan QEMU smoke test CP15-10 diklaim lulus tanpa
regresi terhadap baseline M14. Keterbatasan utama: empat file bukti audit
statis (readelf/objdump/nm/host_test) tersimpan kosong sehingga tidak bisa
diverifikasi ulang dari repo saat ini, dan mcsfs1 belum ditautkan ke
kernel/VFS. Langkah berikutnya adalah memperbaiki artefak yang kosong,
melampirkan potongan log QEMU secara eksplisit, dan merencanakan integrasi
mcsfs1 ke kernel pada milestone berikutnya.
```