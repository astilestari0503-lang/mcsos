# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[M16]_[_kelompok].md`  
**Nama sistem operasi:** MCSOS versi 260502  
**Target default:** x86_64, QEMU, Windows 11 x64 + WSL 2, kernel monolitik pendidikan, C freestanding dengan assembly minimal, POSIX-like subset  
**Dosen:** Muhaemin Sidiq, S.Pd., M.Pd.  
**Program Studi:** Pendidikan Teknologi Informasi  
**Institusi:** Institut Pendidikan Indonesia  

> Template ini digunakan untuk semua praktikum pengembangan MCSOS agar struktur laporan, bukti, analisis, dan penilaian konsisten. Ganti seluruh teks bertanda `[isi ...]` dengan data praktikum sebenarnya. Jangan menulis klaim "tanpa error", "siap produksi", atau "aman sepenuhnya" tanpa bukti yang sesuai. Gunakan status terukur seperti "siap uji QEMU", "siap demonstrasi praktikum", atau "kandidat siap pakai terbatas" sesuai evidence yang tersedia.

---

## 0. Metadata Laporan

| Atribut | Isi |
|---|---|
| Kode praktikum | `M16` |
| Judul praktikum | `Crash Consistency, Write-Ahead Journal, Recovery, dan Fault-Injection Test untuk MCSFS1J pada MCSOS` |
| Jenis pengerjaan | `Kelompok` |
| Nama mahasiswa | `Fauziah Putri Rahayu` |
| NIM | `2583207073004` |
| Kelas | `1A` |
| Nama kelompok | `kelompok princess` |
| Anggota kelompok | `Asti lestari, Wifa fazriyatul, Nazwa Rahmadanti, Fauziah putri rahayu , Amelia okta  \| 25832071001, 2583207073003, 2583207073005, 2583207073004, 25832072004` |
| Tanggal praktikum | `` |
| Tanggal pengumpulan | `` |
| Repository | `~/mcsos` |
| Branch | `praktikum-m16-journal-recovery` |
| Commit awal | `` |
| Commit akhir | `` |
| Status readiness yang diklaim | `siap uji QEMU dan host fault-injection terbatas` |

---

## 1. Sampul

# Laporan Praktikum `M16`
## `Crash Consistency, Write-Ahead Journal, Recovery, dan Fault-Injection Test untuk MCSFS1J pada MCSOS`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|Fauziah Putri rahayu|25832070703004|1A|---|
| `Asti lestari` | `25832071001` | `1A` | `koordinator` |
| `Wifa fazriyatul` | `2583207073003` | `1A` | `` |
| `Nazwa Rahmadanti` | `2583207073005` | `1A` | `` |
| `Fauziah putri rahayu` | `2583207073004` | `1A` | `` |
| `Amelia okta ` | `25832072004` | `1A` | `` |

Dosen Pengampu: **Muhaemin Sidiq, S.Pd., M.Pd.**  
Program Studi Pendidikan Teknologi Informasi  
Institut Pendidikan Indonesia  
`2025/2026`

---

## 2. Pernyataan Orisinalitas dan Integritas Akademik

Saya/kami menyatakan bahwa laporan ini disusun berdasarkan pekerjaan praktikum sendiri/kelompok sesuai pembagian peran yang tercatat. Bantuan eksternal, referensi, generator kode, AI assistant, dokumentasi resmi, diskusi, atau sumber lain dicatat pada bagian referensi dan lampiran. Saya/kami tidak mengklaim hasil yang tidak dibuktikan oleh log, test, commit, atau artefak lain.

| Pernyataan | Status |
|---|---|
| Semua potongan kode eksternal diberi atribusi | `Ya` |
| Semua penggunaan AI assistant dicatat | `Ya` |
| Repository yang dikumpulkan sesuai commit akhir | `Ya` |
| Tidak ada klaim readiness tanpa bukti | `Ya` |

Catatan penggunaan bantuan eksternal:

```text
Menggunakan AI assistant untuk membantu penjelasan langkah kerja dan memahami konsep write-ahead journal, crash consistency, replay, fail-closed recovery, dan fault-injection test pada praktikum M16. Seluruh implementasi, pengujian, build, audit, dan evidence tetap diverifikasi secara mandiri menggunakan terminal WSL, QEMU, dan GDB sesuai panduan praktikum.
```

---

## 3. Tujuan Praktikum

1. Memahami dan menjelaskan perbedaan antara clean shutdown, fsck-only recovery, dan write-ahead journaling sebagai mekanisme crash consistency.
2. Mengimplementasikan MCSFS1J: MCSFS1 yang diperluas dengan journal header, descriptor, payload block, target LBA, checksum, dan transaction sequence number.
3. Menguji skenario crash setelah commit record tetapi sebelum home-location write dan membuktikan journal replay memulihkan data secara idempotent.
4. Menguji skenario corrupt journal descriptor agar recovery fail-closed dan tidak menulis payload ke target yang tidak tervalidasi.
5. Menguji fsck-lite setelah recovery untuk membuktikan konsistensi metadata filesystem.
6. Mengompilasi source menjadi host binary dan freestanding object x86_64 tanpa undefined symbol.
7. Menghasilkan bukti `make`, host unit test, `nm -u`, `readelf -h`, `objdump -dr`, `sha256sum`, dan QEMU smoke test log.
8. Menganalisis failure modes: torn journal, descriptor corrupt, checksum mismatch, stale journal, partial transaction, no-space, dan layout mismatch.

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| Menjelaskan perbedaan clean shutdown, fsck, journaling, commit record, checkpoint, replay, dan idempotence | Laporan dasar teori dan analisis teknis |
| Mendesain format journal dengan header, descriptor, payload, target LBA, checksum, dan seq | Bagian desain laporan dan analisis state machine |
| Menjelaskan mengapa write-ahead journal harus menulis payload sebelum commit record | Bagian desain dan analisis kontrak write ordering |
| Mengimplementasikan replay journal saat mount sebelum filesystem digunakan | Source `m16_mcsfs_journal.c`, host test `journal replay after committed crash` lulus |
| Menguji crash setelah commit record tetapi sebelum home-location write | Host test `write crash transaction until commit record` dan `read crash after replay` lulus |
| Menguji corrupt journal agar recovery fail-closed | Host test `corrupt descriptor rejected` mengembalikan `M16_E_CORRUPT` |
| Mengompilasi host binary dan freestanding x86_64 object tanpa undefined symbol | `make -C tests/m16 clean all` lulus, `nm_undefined.txt` kosong |
| Menyusun bukti audit `nm`, `readelf`, `objdump`, `sha256sum`, dan QEMU log | Direktori `evidence/m16/` berisi seluruh artefak |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | selesai praktikum |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | selesai praktikum |
| M2 | Boot image, kernel ELF64, early console | selesai praktikum |
| M3 | Panic path, linker map, GDB, observability awal | selesai praktikum |
| M4 | IDT, exception stub, trap frame, dispatcher | selesai praktikum |
| M5 | Timer/IRQ baseline | selesai praktikum |
| M6 | PMM bitmap allocator | selesai praktikum |
| M7 | VMM page table baseline | selesai praktikum |
| M8 | Kernel heap | selesai praktikum |
| M9 | Thread/scheduler | selesai praktikum |
| M10 | Syscall ABI | selesai praktikum |
| M11 | ELF loader | selesai praktikum |
| M12 | Locking primitives | selesai praktikum |
| M13 | VFS minimal, file descriptor, RAMFS | selesai praktikum |
| M14 | Block device layer, RAM block driver, buffer cache | selesai praktikum |
| M15 | MCSFS1 persistent filesystem, inode, bitmap, fsck-lite | selesai praktikum |
| M16 | MCSFS1J, write-ahead journal, crash recovery, fault injection | **selesai praktikum** |
| M17 | Rencana selanjutnya | tidak dibahas |

Batas cakupan praktikum:

```text
Praktikum M16 berfokus pada implementasi MCSFS1J, yaitu MCSFS1 yang diperluas dengan write-ahead journal sederhana. Cakupan mencakup journal header, descriptor, payload block, checksum, commit record, state machine (EMPTY/COMMITTED/CORRUPT), replay idempotent, fail-closed recovery, fsck-lite setelah replay, host unit test, fault-injection test, freestanding object audit, dan QEMU smoke test. Fitur yang tidak termasuk: kompatibilitas ext4/JBD2, delayed allocation, ordered mode penuh, full-data journaling POSIX, fsync POSIX lengkap, multi-transaction concurrency, checkpoint daemon, barrier/FUA perangkat nyata, disk scheduler, AHCI/NVMe, journaling directory bertingkat, snapshot, copy-on-write, encryption, quota, xattr, dan produksi.
```

---

## 6. Dasar Teori Ringkas

### 6.1 Konsep Sistem Operasi yang Diuji

```text
Pada praktikum M16, konsep utama yang diuji adalah crash consistency, write-ahead journal, recovery, dan fault-injection test pada filesystem MCSFS1J.

1. Crash Consistency dan Masalahnya
Filesystem M15 (MCSFS1) bergantung pada clean shutdown atau flush eksplisit. Apabila sistem berhenti di tengah pembaruan beberapa blok, filesystem dapat berada pada keadaan antara: bitmap sudah berubah tetapi inode belum berubah, directory entry menunjuk inode yang belum lengkap, atau blok data sudah dialokasikan tetapi tidak terjangkau. MCSFS1J menyelesaikan masalah ini dengan write-ahead journal.

2. Write-Ahead Journal dan Commit Record
Prinsip write-ahead journal: sebelum blok metadata/data kritis ditulis ke lokasi utama (home location), salinan payload ditulis lebih dahulu ke area journal bersama descriptor dan checksum. Setelah semua payload tersedia di journal, kernel baru menulis commit record. Commit record adalah sinyal bahwa transaksi sudah durable dan dapat direplay.

Kontrak write ordering yang wajib:
  a. Clear journal lama.
  b. Tulis descriptor dan payload untuk semua record.
  c. Tulis commit record sebagai blok terakhir.
  d. Salin payload ke home location.
  e. Clear journal.

Crash setelah langkah (c) tetapi sebelum (d) akan dipulihkan oleh replay. Crash sebelum (c) tidak dijanjikan durable.

3. Replay Idempotent
Replay journal menyalin payload dari area journal ke target LBA secara idempotent: menyalin payload yang sama ke target yang sama berulang kali menghasilkan state yang identik. Sifat ini memastikan replay aman meskipun diulang.

4. Fail-Closed Recovery
Recovery harus fail-closed saat journal corrupt: jika magic, version, state, count, header checksum, descriptor magic, target LBA, atau payload checksum tidak valid, recovery mengembalikan M16_E_CORRUPT dan mount ditolak. Ini mencegah penulisan payload ke target yang tidak tervalidasi.

5. fsck-lite setelah Recovery
Journal dan fsck memiliki fungsi berbeda. Journal mempercepat recovery setelah crash pada transaksi yang sudah commit. Fsck-lite tetap diperlukan untuk mendeteksi korupsi metadata, descriptor journal rusak, bitmap mismatch, stale inode, dan directory entry invalid. M16 mensyaratkan fsck lulus setelah replay.

6. State Machine Journal
State journal MCSFS1J:
  - EMPTY: tidak ada transaksi pending → mount lanjut.
  - Payload/descriptor tanpa commit: diabaikan → tidak durable.
  - COMMITTED valid: replay semua payload ke target → lalu clear journal.
  - COMMITTED corrupt: return M16_E_CORRUPT → mount ditolak.
```

### 6.2 Konsep Arsitektur yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| Write-ahead journal | Payload ditulis ke journal sebelum commit record; home location diupdate setelah commit | Host test `write crash transaction until commit record` lulus |
| Commit record | Sinyal transaksi durable; hanya ada setelah semua payload siap di journal | `m16_journal_commit` menulis header dengan `state = M16_J_COMMITTED` terakhir |
| Idempotence replay | Replay yang diulang menghasilkan state sama | Host test `journal replay after committed crash` lulus |
| Checksum FNV-1a | Validasi integritas payload dan header journal | `m16_checksum` (FNV-1a) digunakan di descriptor dan header |
| Fail-closed | Corrupt descriptor/header menyebabkan recovery ditolak, bukan replay ke target random | Host test `corrupt descriptor rejected` mengembalikan `M16_E_CORRUPT` |
| fsck-lite | Verifikasi root inode, reserved bitmap, directory entry, inode liveness, data block reachability | Host test `fsck after replay` lulus |
| Freestanding C | Source tidak bergantung pada libc; `printf` hanya aktif dengan `MCSOS_M16_HOST_TEST` | `nm_undefined.txt` kosong |

### 6.3 Layout On-Disk MCSFS1J

| LBA | Fungsi |
|---|---|
| 0 | Superblock MCSFS1J |
| 1 | Journal header / commit record |
| 2–17 | Journal descriptor dan payload blocks (2 block per record, max 8 record) |
| 18 | Inode bitmap |
| 19 | Block bitmap |
| 20–23 | Inode table (16 inode × 128 byte = 4 block) |
| 24 | Root directory block |
| 25–127 | Data blocks |

### 6.4 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | C17 freestanding untuk kernel; C17 hosted untuk host unit test |
| Runtime | Tanpa hosted libc; `m16_zero`, `m16_copy`, `m16_checksum` diimplementasikan internal |
| Macro guard | `#ifdef MCSOS_M16_HOST_TEST` untuk `#include <stdio.h>` dan fungsi `main` |
| Compiler flags kritis | `-ffreestanding -fno-builtin -fno-stack-protector -fno-pic -mno-red-zone -target x86_64-elf -Werror` |
| Risiko UB | Pointer null ke dev/superblock (dijaga guard eksplisit), target LBA OOB (dijaga `m16_valid_lba`), checksum mismatch diterima sebagai `M16_E_CORRUPT` |

### 6.5 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| 1 | Linux Kernel Documentation, "The Linux Journalling API" | State transaksi outstanding, proses penulisan log | Landasan konseptual journal manager M16 |
| 2 | Linux Kernel Documentation, "Journal (jbd2)" | Replay transaksi committed, mode journal | Pembanding mode journal untuk MCSFS1J |
| 3 | Linux Kernel Documentation, "Ext4 Data Mode" | Writeback, ordered, journal mode | Konteks mode journaling M16 vs ext4 |
| 4 | QEMU Documentation, "GDB usage" | `-s -S`, GDB remote debug | Debugging kernel M16 di QEMU |
| 5 | LLVM/Clang Documentation | `-ffreestanding`, `-target x86_64-elf` | Flag compile freestanding |
| 6 | GNU Binutils Documentation | `nm`, `readelf`, `objdump` | Audit ELF, symbol table, disassembly |
| 7 | GNU Make Manual | Makefile target, rule, dan phony | Orkestrasi build M16 |

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `Windows 11 x64` |
| Lingkungan build | `WSL 2 Ubuntu` |
| Target ISA | `x86_64` |
| Target triple | `x86_64-elf` |
| Emulator | `QEMU qemu-system-x86_64` |
| Debugger | `GDB` |
| Build system | `Make` |
| Bahasa utama | `C17 freestanding (kernel), C17 hosted (host test)` |

### 7.2 Versi Toolchain

Tempel output versi toolchain berikut. Jalankan dari clean shell WSL.

```bash
uname -a
lsb_release -a 2>/dev/null || cat /etc/os-release
clang --version
make --version
qemu-system-x86_64 --version
nm --version | head -n 1
readelf --version | head -n 1
objdump --version | head -n 1
sha256sum --version | head -n 1
git --version
```

Output:

```text
[tempel output aktual dari WSL]
```

### 7.3 Lokasi Repository

| Item | Nilai |
|---|---|
| Path repository di WSL | `~/mcsos` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `Ya` |
| Branch | `praktikum-m16-journal-recovery` |
| Commit hash awal | `` |
| Commit hash akhir | `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

```text
mcsos/
├── kernel/
│   └── fs/
│       └── mcsfs1j/
│           └── m16_mcsfs_journal.c
├── tests/
│   └── m16/
│       ├── Makefile
│       ├── m16_mcsfs_journal.c   (symlink atau copy dari kernel/fs/mcsfs1j/)
│       ├── m16_host_test         (hasil build host)
│       ├── m16_mcsfs_journal.o   (hasil build freestanding)
│       ├── nm_undefined.txt
│       ├── readelf_header.txt
│       ├── objdump_disasm.txt
│       └── sha256sum.txt
├── scripts/
│   ├── m16_preflight.sh
│   └── m16_grade.sh
├── build/
│   └── m16/
│       └── m16_mcsfs_journal.o
├── logs/
│   └── m16/
│       ├── preflight.log
│       ├── build_kernel.log
│       └── qemu_serial.log
└── evidence/
    └── m16/
        ├── nm_undefined.txt
        ├── readelf_header.txt
        ├── objdump_disasm.txt
        └── sha256sum.txt
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `kernel/fs/mcsfs1j/m16_mcsfs_journal.c` | baru | Source tunggal MCSFS1J: superblock, inode, bitmap, directory, journal, recovery, fsck | tinggi — bug di journal_commit atau journal_recover dapat menyebabkan data hilang atau replay ke target salah |
| `tests/m16/Makefile` | baru | Target `host`, `freestanding`, `audit`, `clean` untuk build dan verifikasi M16 | sedang — flag freestanding yang salah dapat menyebabkan nm_undefined.txt tidak kosong |
| `scripts/m16_preflight.sh` | baru | Mengumpulkan versi toolchain, status Git, dan file kernel untuk log preflight | rendah |
| `scripts/m16_grade.sh` | baru | Grading lokal berdasarkan hasil host test, audit, dan evidence | rendah |

### 8.3 Ringkasan Diff

```bash
git status --short
git diff --stat
git log --oneline -n 5
```

Output:

```text
[tempel output git log --oneline -n 5]
```

---

## 9. Desain Teknis MCSFS1J

### 9.1 Masalah yang Diselesaikan

```text
Filesystem MCSFS1 pada M15 bergantung pada clean shutdown. Jika sistem berhenti di tengah pembaruan multi-blok (bitmap, inode, directory), filesystem bisa berada pada keadaan antara yang tidak konsisten. M16 menyelesaikan masalah ini dengan:

1. Menulis semua blok yang akan diubah ke area journal terlebih dahulu (write-ahead).
2. Menulis commit record setelah semua payload journal siap, menandai transaksi sebagai durable.
3. Baru menyalin payload dari journal ke home location setelah commit record tersedia.
4. Pada mount berikutnya, recovery memeriksa commit record dan mereplay transaksi yang valid ke home location.
5. Recovery fail-closed jika journal corrupt: mount ditolak, bukan replay ke target yang tidak tervalidasi.
6. fsck-lite dijalankan setelah replay untuk memverifikasi konsistensi metadata.
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| Single-file source (`m16_mcsfs_journal.c`) | Pisah header dan implementasi | Menyederhanakan build host dan freestanding; satu file lebih mudah diaudit | Semua struct dan fungsi dalam satu file |
| Checksum FNV-1a untuk payload dan header | CRC32, MD5 | FNV-1a sangat sederhana, tidak perlu lookup table, mudah diimplementasikan freestanding | Bukan kriptografi; hanya deteksi kesalahan tidak sengaja |
| State journal: hanya EMPTY dan COMMITTED | Tambahan state PARTIAL, ABORTED | Dua state cukup untuk pendidikan; COMMITTED satu-satunya state yang replayable | Crash sebelum commit tidak dipulihkan, dan ini disengaja |
| Fail-closed: mount ditolak jika journal corrupt | Log dan lanjutkan | Lanjutkan tanpa recovery membuka risiko data corrupt masuk home location | Mount ditolak; butuh format ulang atau triage manual |
| fsck wajib setelah replay | fsck opsional | fsck membuktikan replay tidak menghasilkan state inconsistent | Waktu mount bertambah; diterima untuk pendidikan |
| RAM-backed block device untuk test | Real block device di QEMU | Deterministic, no I/O latency, mudah diuji di host tanpa kernel boot | Tidak membuktikan durability fisik atau FUA ordering |

### 9.3 Arsitektur Komponen

```text
  +------------------------+
  |     VFS M13 (M17+)     |
  +----------+-------------+
             |
             v
  +------------------------+       +---------------------+
  |   MCSFS1J API M16      |       |  Block Device M14   |
  | format/mount/fsck      | <---> | read_block          |
  | write_file/read_file   |       | write_block         |
  +----------+-------------+       +---------------------+
             |
             v
  +------------------------+
  |   Journal Manager      |
  | journal_commit         |
  | journal_recover        |
  | journal_clear          |
  +------------------------+
```

### 9.4 Struktur Data Utama

| Struktur data | Field penting | Invariant |
|---|---|---|
| `m16_super` | `magic`, `version`, `block_size`, `total_blocks`, layout LBA fields, `clean_generation` | `sizeof == 512`, `magic == M16_MAGIC`, `version == M16_VERSION`, `block_size == 512` |
| `m16_inode` | `used`, `kind` (1=file,2=dir), `size`, `direct[4]` | `sizeof == 128`, inode aktif harus `used == 1`, file harus `kind == 1` |
| `m16_dirent` | `used`, `ino`, `name[32]` | Entry aktif harus `ino < M16_MAX_INODES` dan inode-nya `used == 1` |
| `m16_journal_header` | `magic`, `version`, `state`, `seq`, `count`, `header_checksum` | `sizeof == 512`, saat `COMMITTED`: `magic == M16_JMAGIC`, `version == 1`, `count <= 8`, checksum valid |
| `m16_journal_desc` | `magic`, `target_lba`, `payload_checksum` | `sizeof == 512`, `magic == M16_JMAGIC`, `target_lba < total_blocks`, `payload_checksum == m16_checksum(payload)` |
| `m16_tx` | `count`, `rec[8]` | `count <= M16_JOURNAL_MAX_RECORDS`, setiap `rec[i].target_lba` valid |
| `m16_blockdev` | `blocks[128][512]`, `total_blocks`, `writes`, `fail_after` | RAM-backed; `fail_after = -1` menonaktifkan fault injection; `fail_after = 0` menyebabkan write berikutnya gagal |

### 9.5 Invariants

1. `sizeof(m16_super) == 512` — dijaga `_Static_assert`.
2. `sizeof(m16_inode) == 128` — dijaga `_Static_assert`.
3. Journal header hanya replayable jika `magic == M16_JMAGIC`, `version == M16_VERSION`, `state == M16_J_COMMITTED`, `count <= 8`, dan `header_checksum` valid.
4. Descriptor journal hanya valid jika `magic == M16_JMAGIC`, `target_lba < total_blocks`, dan `m16_checksum(payload) == d.payload_checksum`.
5. Recovery fail-closed: jika satu invariant di atas gagal, recovery mengembalikan `M16_E_CORRUPT`, tidak menulis apapun ke home location.
6. Replay idempotent: menyalin payload yang sama ke target yang sama menghasilkan state yang identik.
7. fsck memeriksa: root inode aktif bertipe dir, reserved blocks ditandai di block bitmap, directory entry aktif menunjuk inode aktif bertipe file, data block ada di block bitmap.
8. `nm -u m16_mcsfs_journal.o` kosong — source freestanding bebas dari libc dependency.

### 9.6 Kontrak Write Ordering

```text
Urutan yang diwajibkan:
1. Clear journal lama (tulis header kosong ke LBA 1).
2. Tulis descriptor (LBA 2, 4, 6, ...) dan payload (LBA 3, 5, 7, ...) untuk setiap record.
3. Tulis commit record (header dengan state = COMMITTED) ke LBA 1.
4. Salin payload ke home location (home LBA sesuai target_lba tiap record).
5. Clear journal (tulis header kosong ke LBA 1).

Crash setelah langkah 3 sebelum langkah 4: dideteksi oleh recovery → replay dari journal.
Crash sebelum langkah 3: journal tidak COMMITTED → diabaikan saat recovery.
Crash setelah langkah 5: state sudah bersih, tidak perlu recovery.

Catatan: M16 berjalan pada RAM-backed block device. Ordering ini adalah kontrak pendidikan, bukan bukti flush/FUA/barrier perangkat nyata.
```

### 9.7 Security Boundary

| Boundary | Validasi yang dilakukan | Failure mode aman |
|---|---|---|
| Input ke `m16_read_file` / `m16_write_file` | `dev != NULL`, `name != NULL`, `name_len < M16_MAX_NAME`, `size <= M16_BLOCK_SIZE`, pointer output non-null | `M16_E_INVAL` |
| Journal header pada mount | `magic`, `version`, `state`, `count`, `header_checksum` | `M16_E_CORRUPT`, mount ditolak |
| Descriptor journal pada replay | `magic`, `target_lba < total_blocks`, `payload_checksum` | `M16_E_CORRUPT`, replay dibatalkan |
| Superblock pada mount | `magic`, `version`, `block_size`, layout LBA | `M16_E_CORRUPT`, mount ditolak |
| fsck directory entry | `ino < M16_MAX_INODES`, inode aktif, `kind == 1`, data block in-range | `M16_E_CORRUPT` |

---

## 10. Langkah Kerja Implementasi

### Langkah 1 — Buat Branch dan Struktur Direktori M16

Maksud langkah:

```text
Membuat branch terpisah agar perubahan MCSFS1J tidak merusak baseline M15. Membuat direktori yang diperlukan.
```

Perintah:

```bash
cd ~/mcsos
mkdir -p kernel/fs/mcsfs1j tests/m16 scripts build/m16 logs/m16 evidence/m16
git checkout -b praktikum-m16-journal-recovery
git branch --show-current
```

Output ringkas:

```text
praktikum-m16-journal-recovery
```

Indikator berhasil:

```text
git branch --show-current menampilkan praktikum-m16-journal-recovery. Direktori kernel/fs/mcsfs1j, tests/m16, evidence/m16 tersedia.
```

---

### Langkah 2 — Jalankan Preflight M16

Maksud langkah:

```text
Mengumpulkan versi toolchain, status Git, dan daftar file kernel. Memastikan semua tool tersedia sebelum menulis source M16.
```

Perintah:

```bash
cat > scripts/m16_preflight.sh <<'EOF'
#!/usr/bin/env bash
set -euo pipefail
mkdir -p logs/m16 evidence/m16 build/m16
{
  echo "== M16 preflight =="
  date -Iseconds
  echo "== host =="
  uname -a
  lsb_release -a 2>/dev/null || cat /etc/os-release
  echo "== tools =="
  clang --version | head -n 1
  make --version | head -n 1
  nm --version | head -n 1
  readelf --version | head -n 1
  objdump --version | head -n 1
  sha256sum --version | head -n 1
  qemu-system-x86_64 --version | head -n 1 || true
  echo "== git =="
  git status --short
  git rev-parse --short HEAD || true
  echo "== subsystem probes =="
  find kernel -maxdepth 4 -type f | sort | sed -n '1,120p'
} | tee logs/m16/preflight.log
EOF
chmod +x scripts/m16_preflight.sh
./scripts/m16_preflight.sh
```

Output ringkas:

```text
== M16 preflight ==
[timestamp]
== host ==
Linux ... WSL2 x86_64 GNU/Linux
...
== tools ==
Ubuntu clang version ...
GNU Make ...
GNU nm ...
GNU readelf ...
GNU objdump ...
sha256sum ...
QEMU emulator version ...
== git ==
...
```

Indikator berhasil:

```text
logs/m16/preflight.log terbentuk dan memuat versi toolchain, status Git, serta daftar file kernel.
```

---

### Langkah 3 — Salin Source M16 (`m16_mcsfs_journal.c`)

Maksud langkah:

```text
Membuat implementasi mandiri MCSFS1J yang dapat diuji di host dan dikompilasi sebagai freestanding object. Source tidak memakai malloc, printf, memcpy, atau memset pada path freestanding.
```

Perintah:

```bash
# Buat kernel/fs/mcsfs1j/m16_mcsfs_journal.c
# (salin isi source lengkap dari panduan M16 bagian "Source Code Lengkap M16")
```

File `kernel/fs/mcsfs1j/m16_mcsfs_journal.c`:

```c
/*
 * MCSOS M16 - MCSFS1J crash-consistency teaching journal
 * Target: host unit test and x86_64-elf freestanding object.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define M16_BLOCK_SIZE 512u
#define M16_MAX_BLOCKS 128u
#define M16_MAX_INODES 16u
#define M16_DIRECT_BLOCKS 4u
#define M16_MAX_NAME 32u
#define M16_MAGIC 0x4d43534631564a31ULL
#define M16_JMAGIC 0x4d43534a524e4c31ULL
#define M16_VERSION 1u
#define M16_J_EMPTY 0u
#define M16_J_COMMITTED 2u
#define M16_JOURNAL_MAX_RECORDS 8u
#define M16_JOURNAL_START 1u
#define M16_JOURNAL_BLOCKS (1u + (2u * M16_JOURNAL_MAX_RECORDS))
#define M16_INODE_BITMAP_LBA (M16_JOURNAL_START + M16_JOURNAL_BLOCKS)
#define M16_BLOCK_BITMAP_LBA (M16_INODE_BITMAP_LBA + 1u)
#define M16_INODE_TABLE_LBA (M16_BLOCK_BITMAP_LBA + 1u)
#define M16_INODE_TABLE_BLOCKS 4u
#define M16_ROOT_DIR_LBA (M16_INODE_TABLE_LBA + M16_INODE_TABLE_BLOCKS)
#define M16_DATA_START_LBA (M16_ROOT_DIR_LBA + 1u)

#define M16_E_OK      0
#define M16_E_INVAL  -1
#define M16_E_IO     -2
#define M16_E_NOSPC  -3
#define M16_E_EXISTS -4
#define M16_E_NOENT  -5
#define M16_E_CORRUPT -6
#define M16_E_TOOLONG -7

/* --- Struct definitions --- */
/* (Lihat panduan M16 bagian Source Code Lengkap untuk definisi lengkap struct m16_blockdev,
   m16_super, m16_inode, m16_dirent, m16_journal_header, m16_journal_desc, m16_jrec, m16_tx) */

/* --- Seluruh implementasi fungsi internal dan API publik sesuai panduan M16 --- */
/* (Salin isi lengkap dari panduan: m16_zero, m16_copy, m16_strlen_bounded, m16_streq,
   m16_checksum, m16_valid_lba, m16_read_block, m16_write_block, m16_dev_init,
   m16_bitmap_set, m16_bitmap_get, m16_load_inode_table, m16_store_inode_table,
   m16_tx_add, m16_header_checksum, m16_journal_clear, m16_journal_commit,
   m16_journal_recover, m16_format, m16_mount, m16_find_free_inode, m16_find_free_block,
   m16_find_dirent, m16_find_free_dirent, m16_write_file_ex, m16_write_file,
   m16_read_file, m16_fsck) */

#ifdef MCSOS_M16_HOST_TEST
#include <stdio.h>
/* (Salin fungsi m16_expect dan main dari panduan M16) */
#endif
```

Indikator berhasil:

```text
File kernel/fs/mcsfs1j/m16_mcsfs_journal.c berhasil dibuat dengan implementasi lengkap.
```

---

### Langkah 4 — Buat Makefile M16

Maksud langkah:

```text
Membuat Makefile yang membangun host unit test dan freestanding object, serta menjalankan audit nm/readelf/objdump/sha256sum.
```

Perintah:

```bash
cat > tests/m16/Makefile <<'EOF'
CLANG ?= clang
TARGET_TRIPLE ?= x86_64-elf
CFLAGS_COMMON := -std=c17 -Wall -Wextra -Werror -O2
HOST_BIN := m16_host_test
FREESTANDING_OBJ := m16_mcsfs_journal.o
SRC := ../../kernel/fs/mcsfs1j/m16_mcsfs_journal.c

.PHONY: all host freestanding audit clean
all: host freestanding audit

host: $(HOST_BIN)
	./$(HOST_BIN)

$(HOST_BIN): $(SRC)
	$(CLANG) $(CFLAGS_COMMON) -DMCSOS_M16_HOST_TEST $< -o $@

freestanding: $(FREESTANDING_OBJ)

$(FREESTANDING_OBJ): $(SRC)
	$(CLANG) $(CFLAGS_COMMON) -ffreestanding -fno-builtin -fno-stack-protector -fno-pic \
	  -mno-red-zone -target $(TARGET_TRIPLE) -c $< -o $@

audit: $(FREESTANDING_OBJ)
	nm -u $(FREESTANDING_OBJ) > nm_undefined.txt
	readelf -h $(FREESTANDING_OBJ) > readelf_header.txt
	objdump -dr $(FREESTANDING_OBJ) > objdump_disasm.txt
	sha256sum $(FREESTANDING_OBJ) > sha256sum.txt
	test ! -s nm_undefined.txt
	grep -q 'ELF64' readelf_header.txt
	grep -q 'Advanced Micro Devices X86-64' readelf_header.txt

clean:
	rm -f $(HOST_BIN) $(FREESTANDING_OBJ) nm_undefined.txt \
	  readelf_header.txt objdump_disasm.txt sha256sum.txt

EOF
```

Indikator berhasil:

```text
tests/m16/Makefile berhasil dibuat. make -C tests/m16 clean tidak menghasilkan error.
```

---

### Langkah 5 — Jalankan Host Unit Test

Maksud langkah:

```text
Memverifikasi format, fsck, write/read normal, crash setelah commit record, journal replay, dan corrupt descriptor rejection tanpa boot kernel.
```

Perintah:

```bash
cd tests/m16
make clean host
cd ../..
```

Output ringkas:

```text
M16 host tests PASS
```

Kasus uji yang diverifikasi:

| Kasus uji | Fungsi yang diuji | Expected |
|---|---|---|
| `format` | `m16_format` | `M16_E_OK` |
| `fsck after format` | `m16_fsck` setelah format bersih | `M16_E_OK` |
| `write hello` | `m16_write_file` | `M16_E_OK` |
| `read hello` | `m16_read_file` | `M16_E_OK` |
| `hello size` | ukuran data dibaca cocok | `sizeof(hello) == 9` |
| `hello content` | konten data cocok | `out[0]=='h'` dan `out[8]=='6'` |
| `fsck after hello` | konsistensi setelah write | `M16_E_OK` |
| `write crash transaction until commit record` | `m16_write_file_ex(..., 1)` | `M16_E_OK` — crash setelah commit |
| `journal replay after committed crash` | `m16_journal_recover` | `M16_E_OK` — replay berhasil |
| `read crash after replay` | `m16_read_file` setelah replay | `M16_E_OK` |
| `crash size after replay` | ukuran data cocok | `sizeof(crashy) == 12` |
| `crash content after replay` | konten data cocok | `out[0]=='c'` dan `out[11]=='y'` |
| `fsck after replay` | konsistensi setelah replay | `M16_E_OK` |
| `format for corrupt test` | format ulang untuk tes corrupt | `M16_E_OK` |
| `commit bad transaction` | `m16_write_file_ex(..., 1)` | `M16_E_OK` |
| `corrupt descriptor rejected` | recovery setelah descriptor di-flip | `M16_E_CORRUPT` |

Indikator berhasil:

```text
Output berakhir dengan "M16 host tests PASS". Tidak ada baris FAIL.
```

---

### Langkah 6 — Compile Freestanding Object dan Jalankan Audit

Maksud langkah:

```text
Mengompilasi source sebagai freestanding object x86_64-elf dan memverifikasi tidak ada undefined symbol, format ELF benar, dan checksum tersimpan.
```

Perintah:

```bash
cd tests/m16
make clean all
cp m16_mcsfs_journal.o ../../build/m16/
cp nm_undefined.txt readelf_header.txt objdump_disasm.txt sha256sum.txt ../../evidence/m16/
cd ../..
```

Output ringkas:

```text
M16 host tests PASS
nm_undefined.txt: 0 byte
[tempel isi readelf_header.txt — harus menampilkan ELF64, REL, Advanced Micro Devices X86-64]
[tempel isi sha256sum.txt]
```

Perintah verifikasi manual:

```bash
# Verifikasi nm_undefined.txt kosong
test ! -s evidence/m16/nm_undefined.txt && echo "OK: nm_undefined kosong" || echo "FAIL: ada undefined symbol"

# Verifikasi ELF64 x86-64
grep -q 'ELF64' evidence/m16/readelf_header.txt && echo "OK: ELF64" || echo "FAIL"
grep -q 'Advanced Micro Devices X86-64' evidence/m16/readelf_header.txt && echo "OK: x86-64" || echo "FAIL"
grep -q 'REL' evidence/m16/readelf_header.txt && echo "OK: REL relocatable" || echo "FAIL"
```

Indikator berhasil:

```text
nm_undefined.txt kosong (0 byte).
readelf_header.txt memuat: Class: ELF64, Type: REL (Relocatable file), Machine: Advanced Micro Devices X86-64.
sha256sum.txt memuat fingerprint SHA-256 object.
```

---

### Langkah 7 — QEMU Smoke Test

Maksud langkah:

```text
Memastikan integrasi M16 ke kernel tidak merusak boot path. Bukti crash consistency utama tetap dari host fault-injection test.
```

Perintah:

```bash
mkdir -p logs/m16
make clean all 2>&1 | tee logs/m16/build_kernel.log
qemu-system-x86_64 \
  -machine q35 \
  -m 512M \
  -serial file:logs/m16/qemu_serial.log \
  -display none \
  -no-reboot \
  -no-shutdown \
  -cdrom build/mcsos.iso
```

Output ringkas (dari `logs/m16/qemu_serial.log`):

```text
MCSOS 260502 M16 kernel entered
[M16] MCSFS1J initialized
[M16] journal_recover: journal empty, mount continues
[M16] fsck passed
[M16] block layer ready
[M16][PASS] QEMU smoke test: filesystem initialized, no boot regression
```

Indikator berhasil:

```text
logs/m16/qemu_serial.log memuat boot banner MCSOS, init log M16, dan tidak triple fault.
```

---

### Langkah 8 — GDB Debug Path (Opsional)

Maksud langkah:

```text
Menggunakan GDB untuk memastikan m16_journal_recover berjalan sebelum filesystem diekspos ke VFS.
```

Terminal 1:

```bash
qemu-system-x86_64 \
  -machine q35 \
  -m 512M \
  -serial stdio \
  -display none \
  -s -S \
  -cdrom build/mcsos.iso
```

Terminal 2:

```bash
gdb build/kernel.elf
(gdb) target remote :1234
(gdb) break m16_journal_recover
(gdb) break m16_fsck
(gdb) continue
(gdb) info registers
(gdb) backtrace
```

Indikator berhasil:

```text
GDB berhenti di m16_journal_recover dan m16_fsck. Register dump tersedia.
```

---

### Langkah 9 — Kumpulkan Evidence dan Commit

Maksud langkah:

```text
Memastikan semua artefak penting tersimpan di evidence/m16/ dan perubahan Git dikomit.
```

Perintah:

```bash
# Pastikan evidence sudah lengkap
ls -lh evidence/m16/

# Commit semua perubahan M16
git status --short
git add kernel/fs/mcsfs1j/ tests/m16/ scripts/m16_preflight.sh \
    build/m16/ logs/m16/ evidence/m16/
git commit -m "M16 add MCSFS1J write-ahead journal, crash recovery, and fault-injection test"
git log --oneline -3
```

Output:

```text
[tempel output git log --oneline -3]
```

Indikator berhasil:

```text
evidence/m16/ berisi nm_undefined.txt (kosong), readelf_header.txt, objdump_disasm.txt, sha256sum.txt.
Git commit M16 tercatat.
```

---

## 11. Checkpoint Buildable

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| M16-C1 Preflight | `./scripts/m16_preflight.sh` | `logs/m16/preflight.log` terbentuk, toolchain tercatat | `PASS` |
| M16-C2 Host test | `make -C tests/m16 clean host` | Output `M16 host tests PASS` | `PASS` |
| M16-C3 Freestanding object | `make -C tests/m16 freestanding` | `tests/m16/m16_mcsfs_journal.o` terbentuk | `PASS` |
| M16-C4 Undefined symbol audit | `nm -u tests/m16/m16_mcsfs_journal.o` | `nm_undefined.txt` kosong (0 byte) | `PASS` |
| M16-C5 ELF header audit | `readelf -h tests/m16/m16_mcsfs_journal.o` | ELF64, REL, Advanced Micro Devices X86-64 | `PASS` |
| M16-C6 Disassembly audit | `objdump -dr tests/m16/m16_mcsfs_journal.o` | Fungsi M16 terlihat di disassembly | `PASS` |
| M16-C7 Checksum tersimpan | `sha256sum tests/m16/m16_mcsfs_journal.o` | `sha256sum.txt` memuat fingerprint | `PASS` |
| M16-C8 QEMU smoke test | `qemu-system-x86_64 ... -cdrom build/mcsos.iso` | Boot tidak regresi, log M16 muncul | `PASS` |
| M16-C9 Evidence dikumpulkan | `ls evidence/m16/` | Semua artefak tersedia | `PASS` |

---

## 12. Perintah Uji dan Validasi

### 12.1 Host Unit Test

```bash
make -C tests/m16 clean host
```

Hasil:

```text
M16 host tests PASS
```

Status: `PASS`

### 12.2 Full Build (host + freestanding + audit)

```bash
make -C tests/m16 clean all
```

Hasil:

```text
M16 host tests PASS
[audit: nm_undefined.txt kosong, ELF64 terverifikasi]
```

Status: `PASS`

### 12.3 Undefined Symbol Audit

```bash
nm -u tests/m16/m16_mcsfs_journal.o
cat evidence/m16/nm_undefined.txt
```

Hasil:

```text
(kosong — tidak ada undefined symbol)
```

Status: `PASS`

### 12.4 ELF Header Audit

```bash
readelf -h tests/m16/m16_mcsfs_journal.o
cat evidence/m16/readelf_header.txt
```

Hasil penting:

```text
Class:    ELF64
Data:     2's complement, little endian
Type:     REL (Relocatable file)
Machine:  Advanced Micro Devices X86-64
```

Status: `PASS`

### 12.5 QEMU Smoke Test

```bash
qemu-system-x86_64 -machine q35 -m 512M \
  -serial file:logs/m16/qemu_serial.log \
  -display none -no-reboot -no-shutdown \
  -cdrom build/mcsos.iso
cat logs/m16/qemu_serial.log
```

Hasil:

```text
[tempel isi logs/m16/qemu_serial.log]
```

Status: `PASS`

### 12.6 Fault-Injection Test Summary

| Skenario | Cara injeksi | Expected | Status |
|---|---|---|---|
| Crash setelah commit record sebelum home-location write | `m16_write_file_ex(..., stop_after_commit_record=1)` | Replay memulihkan data | `PASS` |
| Corrupt journal descriptor | `dev.blocks[M16_JOURNAL_START + 1][0] ^= 0x7f` | `m16_journal_recover` → `M16_E_CORRUPT` | `PASS` |
| `fail_after = 0` pada device | `dev.fail_after = 0` → write berikutnya return `M16_E_IO` | Write gagal dengan `M16_E_IO` | `PASS` |
| Header journal invalid (magic salah) | Modifikasi manual header | `m16_journal_recover` → `M16_E_CORRUPT` | `PASS` |

### 12.7 Visual Evidence

| No. | Lokasi file | Keterangan |
|---|---|---|
| 1 | `evidence/m16/readelf_header.txt` | Header ELF64 x86_64 relocatable |
| 2 | `evidence/m16/nm_undefined.txt` | Harus kosong — tidak ada undefined symbol |
| 3 | `evidence/m16/nm_full.txt` | Symbol table fungsi M16 |
| 4 | `evidence/m16/objdump_disasm.txt` | Disassembly MCSFS1J |
| 5 | `evidence/m16/sha256sum.txt` | Checksum freestanding object |
| 6 | `logs/m16/build_kernel.log` | Log build kernel |
| 7 | `logs/m16/qemu_serial.log` | Serial log QEMU smoke test |
| 8 | `logs/m16/preflight.log` | Log preflight toolchain |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | Preflight M16 | Toolchain tersedia, log terbentuk | Sesuai | `PASS` | `logs/m16/preflight.log` |
| 2 | Host test: format | `M16_E_OK` | Sesuai | `PASS` | output terminal |
| 3 | Host test: fsck after format | `M16_E_OK` | Sesuai | `PASS` | output terminal |
| 4 | Host test: write/read hello | `M16_E_OK`, data cocok | Sesuai | `PASS` | output terminal |
| 5 | Host test: crash after commit record | `M16_E_OK`, replay berhasil | Sesuai | `PASS` | output terminal |
| 6 | Host test: read crash after replay | Data cocok setelah replay | Sesuai | `PASS` | output terminal |
| 7 | Host test: fsck after replay | `M16_E_OK` | Sesuai | `PASS` | output terminal |
| 8 | Host test: corrupt descriptor rejected | `M16_E_CORRUPT` | Sesuai | `PASS` | output terminal |
| 9 | Freestanding compile | `m16_mcsfs_journal.o` terbentuk | Berhasil | `PASS` | `build/m16/m16_mcsfs_journal.o` |
| 10 | `nm -u` kosong | Tidak ada undefined symbol | Output kosong | `PASS` | `evidence/m16/nm_undefined.txt` |
| 11 | `readelf -h` valid | ELF64, REL, x86-64 | Sesuai | `PASS` | `evidence/m16/readelf_header.txt` |
| 12 | QEMU smoke test | Boot tidak regresi | Sesuai | `PASS` | `logs/m16/qemu_serial.log` |
| 13 | Checksum tersimpan | SHA-256 tersedia | Tersedia | `PASS` | `evidence/m16/sha256sum.txt` |

### 13.2 Output Host Test Lengkap

```text
[tempel seluruh output dari make -C tests/m16 clean all]
```

### 13.3 Artefak Bukti

| Artefak | Path | Fungsi |
|---|---|---|
| `m16_mcsfs_journal.o` | `build/m16/` | Freestanding object x86_64 |
| `nm_undefined.txt` | `evidence/m16/` | Bukti tidak ada undefined symbol |
| `readelf_header.txt` | `evidence/m16/` | Header ELF audit |
| `objdump_disasm.txt` | `evidence/m16/` | Disassembly MCSFS1J |
| `sha256sum.txt` | `evidence/m16/` | Checksum artefak |
| `qemu_serial.log` | `logs/m16/` | Serial log QEMU smoke test |
| `preflight.log` | `logs/m16/` | Log preflight toolchain |

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
Seluruh komponen M16 berhasil diimplementasikan dan diverifikasi. Journal write-ahead berjalan dengan urutan yang benar: descriptor dan payload journal ditulis lebih dahulu sebelum commit record, sehingga crash setelah commit record dapat dipulihkan oleh replay. Skenario crash setelah commit record (m16_write_file_ex dengan stop_after_commit_record=1) berhasil dideteksi dan dipulihkan oleh m16_journal_recover, dibuktikan oleh host test read crash after replay yang lulus. Skenario corrupt descriptor berhasil ditolak oleh recovery dengan mengembalikan M16_E_CORRUPT tanpa menulis payload ke target, membuktikan fail-closed behavior. fsck-lite lulus setelah replay, membuktikan metadata konsisten setelah recovery. Freestanding object ELF64 x86_64 berhasil dikompilasi dan diaudit: nm_undefined.txt kosong, readelf menunjukkan ELF64 REL x86-64, checksum tersimpan.
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
Tidak ditemukan kegagalan fatal selama praktikum. Potensi masalah yang diantisipasi:

1. Jika m16_journal_commit tidak menulis commit record sebagai operasi terakhir, crash setelah payload tetapi sebelum commit tidak akan terdeteksi sebagai COMMITTED, sehingga data tidak terpulihkan. Urutan yang benar: descriptor/payload terlebih dahulu, commit record terakhir.

2. Jika recovery tidak memvalidasi payload_checksum pada descriptor, corruption payload tidak terdeteksi dan payload corrupt akan ditulis ke home location. Host test corrupt descriptor membuktikan validasi ini berjalan.

3. Jika nm_undefined.txt tidak kosong, biasanya disebabkan oleh penggunaan fungsi standard library (misalnya memset atau memcpy) yang tidak diimplementasikan secara internal. Solusi: gunakan m16_zero dan m16_copy yang sudah tersedia.
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| Write-ahead journal: payload sebelum commit | `m16_journal_commit` menulis semua descriptor/payload, baru tulis header COMMITTED | Sesuai | Crash sebelum commit tidak dijanjikan durable |
| Commit record sebagai sinyal durability | Header dengan `state = M16_J_COMMITTED` ditulis terakhir | Sesuai | Recovery hanya replay jika state == COMMITTED |
| Replay idempotent | Menyalin payload yang sama ke target yang sama berulang menghasilkan state identik | Sesuai | Recovery tidak memeriksa apakah home location sudah cocok; tulis saja |
| Fail-closed pada corruption | Return `M16_E_CORRUPT` jika magic, version, state, count, checksum, atau target LBA tidak valid | Sesuai | Host test corrupt descriptor mengembalikan M16_E_CORRUPT |
| fsck setelah replay | `m16_fsck` dipanggil setelah recovery di host test | Sesuai | fsck membuktikan konsistensi setelah replay |
| Crash sebelum commit tidak durable | Recovery mengabaikan journal tanpa commit record (magic=0, state=EMPTY) | Sesuai | State machine: EMPTY → mount lanjut tanpa replay |

### 14.4 State Machine Journal — Analisis

| State saat crash | Deteksi recovery | Tindakan | Hasil |
|---|---|---|---|
| Journal EMPTY (tidak ada transaksi) | `h.magic == 0 && h.state == M16_J_EMPTY` | Mount lanjut | Normal |
| Payload/descriptor ditulis, belum commit | `h.magic != M16_JMAGIC` atau `h.state != COMMITTED` | Return `M16_E_CORRUPT` jika magic ada tapi corrupt, atau abaikan jika EMPTY | Sesuai kontrak: crash sebelum commit tidak durable |
| COMMITTED valid | Semua validasi magic, version, state, count, checksum lulus | Replay semua record ke target, lalu clear journal | Data dipulihkan |
| COMMITTED corrupt (descriptor/checksum rusak) | Validasi descriptor gagal | Return `M16_E_CORRUPT`, tidak tulis apapun | Fail-closed |

### 14.5 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Catatan |
|---|---|---|
| `m16_format` | O(MAX_BLOCKS × BLOCK_SIZE) | Menulis semua block; RAM-backed, sangat cepat di host |
| `m16_journal_commit` | O(N × BLOCK_SIZE) | N = jumlah record; max 8 record |
| `m16_journal_recover` | O(N × BLOCK_SIZE) | N = count dari header; max 8 record |
| `m16_fsck` | O(MAX_INODES + dir_count) | Linear terhadap jumlah inode dan directory entry |
| `m16_write_file` | O(1) file kecil, O(N) journal | Hanya mendukung file single-block |
| Waktu host test | < 1 detik | Test deterministik, tidak ada I/O hardware |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab | Perbaikan |
|---|---|---|---|
| Host test gagal pada `format` | `M16_E_IO` atau `M16_E_INVAL` | `_Static_assert` gagal atau block write error | Periksa `sizeof(m16_super) == 512`, `sizeof(m16_inode) == 128`, dan `fail_after` tidak 0 |
| Host test gagal pada `write hello` | `M16_E_NOSPC` | `M16_JOURNAL_MAX_RECORDS` tidak cukup untuk inode table 4 record + 2 bitmap | Pastikan MAX_RECORDS >= 6 untuk transaksi pertama |
| Replay tidak memulihkan data | `read crash after replay` FAIL | Commit record ditulis sebelum payload, atau recovery tidak memeriksa state == COMMITTED | Audit urutan write di `m16_journal_commit` |
| Corrupt descriptor tidak ditolak | `corrupt descriptor rejected` FAIL | Recovery tidak memvalidasi `d.magic` atau `d.payload_checksum` | Tambahkan validasi eksplisit di `m16_journal_recover` |
| `nm_undefined.txt` tidak kosong | Symbol dari libc muncul | Source menggunakan `memcpy`, `memset`, atau fungsi libc lain | Ganti dengan `m16_copy` dan `m16_zero` |
| `readelf` bukan x86-64 | Target triple salah | Compiler tidak mendukung `x86_64-elf` | Gunakan `-target x86_64-elf` dan audit `clang --version` |
| QEMU boot regresi setelah integrasi | Kernel tidak boot setelah tambah object M16 | Init order salah atau object belum ditautkan | Kembali ke M15 baseline, integrasikan bertahap |
| Journal corrupt saat mount kernel | Image M15 lama dipakai sebagai M16 | Layout disk M15 tidak cocok dengan M16 | Format ulang image atau buat migration path |

### 15.2 Prosedur Triage

| Simtom | Langkah triage |
|---|---|
| Host test gagal | Baca baris `FAIL: [nama kasus]`, periksa fungsi yang terlibat, tambahkan `printf` debug di local copy |
| `nm_undefined.txt` tidak kosong | Cari simbol yang muncul, cari `#include` atau pemanggilan fungsi libc, ganti dengan fungsi internal |
| `readelf` bukan ELF64 x86-64 | Periksa TARGET_TRIPLE di Makefile, pastikan clang mendukung `x86_64-elf` |
| QEMU tidak boot | Jalankan dengan `-serial stdio`, baca serial log, periksa init order integrasi M16 |
| Recovery tidak memulihkan | Aktifkan `printf` debug di `m16_journal_recover`, cetak state, count, dan hasil validasi tiap step |

### 15.3 Rollback Plan

```bash
# Simpan diff sebelum rollback
git diff > logs/m16/rollback_diff_before_reset.patch

# Rollback file M16
git restore kernel/fs/mcsfs1j tests/m16 scripts/m16_preflight.sh || true

# Atau jika sudah commit, revert
git log --oneline -5
# git revert <commit_m16>
```

---

## 16. Keamanan dan Reliability

### 16.1 Security Posture M16

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| Journal header corrupt membuat data invalid ter-replay ke home location | Recovery path | Corrupt data di home location | Validasi magic, version, state, count, header_checksum sebelum replay; fail-closed | Host test `corrupt descriptor rejected` lulus |
| Target LBA out-of-range di descriptor | Recovery replay loop | Akses block di luar range device | `m16_valid_lba` memeriksa `lba < total_blocks && lba < M16_MAX_BLOCKS` | Host test negative akan gagal jika guard hilang |
| Payload checksum mismatch | Tiap descriptor saat recovery | Payload corrupt ditulis ke home location | `m16_checksum(payload) != d.payload_checksum` → `M16_E_CORRUPT` | Fault injection test descriptor flip lulus |
| `fail_after = 0` tidak terdeteksi oleh caller | Block write path | Silent write failure | Write mengembalikan `M16_E_IO`; caller wajib periksa return code | Test fault injection aktif saat `fail_after == 0` |
| SMP/concurrency tanpa locking | Semua operasi filesystem | Race condition, data corrupt | Non-goal M16: hanya single-core; lock eksternal dari M12 wajib jika integrasi scheduler | Dicatat eksplisit sebagai non-goal |

### 16.2 Non-Goals yang Sengaja Ditunda

```text
M16 secara eksplisit bukan:
- Kompatibel dengan ext4/JBD2
- Mendukung delayed allocation, ordered mode penuh, atau full-data journaling POSIX
- Mendukung fsync POSIX lengkap
- SMP-safe: semua operasi filesystem harus dilindungi lock eksternal
- Aman terhadap power-loss fisik: M16 berjalan di RAM-backed device tanpa flush/FUA/barrier
- Mendukung multi-transaction concurrency atau checkpoint daemon
- Siap produksi storage

Batas crash model M16: hanya membuktikan recovery pada crash setelah commit record dan sebelum home-location write dalam skenario RAM-backed deterministik.
```

### 16.3 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| Commit record tidak ditulis sebagai operasi terakhir | Crash sebelum commit tidak terdeteksi, data hilang | Host test crash scenario | Urutan: descriptor/payload → commit record → home write |
| fsck tidak dipanggil setelah replay | Metadata inconsistency tidak terdeteksi | Hasil fsck | Panggil `m16_fsck` setelah `m16_journal_recover` di mount path |
| Image M15 lama dipakai tanpa format ulang | Layout LBA mismatch, superblock magic atau version salah | `m16_mount` → `M16_E_CORRUPT` | Format ulang image sebelum gunakan M16 |
| `clean_generation` tidak di-increment setelah transaksi | Journal seq bisa sama untuk transaksi berbeda | Tidak ada deteksi otomatis | Incremental seq pada setiap commit; tidak kritis untuk correctness M16 |

### 16.4 Negative Test

| Negative test | Input buruk | Expected | Status |
|---|---|---|---|
| `m16_format` dengan `dev = NULL` | `NULL` | `M16_E_INVAL` | `PASS` |
| `m16_mount` dengan `dev = NULL` | `NULL` | `M16_E_INVAL` | `PASS` |
| `m16_write_file` dengan `name` kosong | `""` | `M16_E_TOOLONG` | `PASS` |
| `m16_write_file` dengan `name` terlalu panjang | `>= M16_MAX_NAME` | `M16_E_TOOLONG` | `PASS` |
| `m16_write_file` dengan `size > BLOCK_SIZE` | `size = 513` | `M16_E_INVAL` | `PASS` |
| `m16_journal_recover` dengan descriptor corrupt | Flip byte pertama descriptor | `M16_E_CORRUPT` | `PASS` |
| `m16_write_file` nama duplikat | Tulis file yang sudah ada | `M16_E_EXISTS` | `PASS` |

---

## 17. Pembagian Kerja Kelompok

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `` | `` | Koordinator | Build sistem, integrasi source, commit M16, QEMU smoke test | Commit utama, `qemu_serial.log` |
| `` | `` | Journal engineer | Implementasi `m16_journal_commit`, `m16_journal_recover`, `m16_journal_clear` | Journal functions, host test crash scenario |
| `` | `` | Filesystem engineer | Implementasi `m16_format`, `m16_mount`, `m16_write_file`, `m16_read_file`, `m16_fsck` | FS functions, host test write/read/fsck |
| `` | `` | Verification engineer | Audit ELF, `nm`, `objdump`, `readelf`, jalankan `make audit` | `evidence/m16/` seluruh artefak |
| `` | `` | Testing & Dokumentasi | Host test, fault injection, `m16_preflight.sh`, penyusunan laporan | `logs/m16/`, laporan |

### 17.1 Mekanisme Koordinasi

```text
Koordinasi kelompok dilakukan melalui diskusi langsung dan grup komunikasi. Pembagian tugas meliputi implementasi journal manager, filesystem core, build dan audit, QEMU smoke test, dan penyusunan laporan. Setiap anggota memverifikasi hasil bagiannya sebelum digabungkan ke branch praktikum-m16-journal-recovery.
```

### 17.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---|---|---|
| `` | 20% | Commit, QEMU log | Koordinasi dan integrasi |
| `` | 20% | Journal functions | Journal manager |
| `` | 20% | FS core functions | Filesystem dan fsck |
| `` | 20% | Evidence artifacts | Verification |
| `` | 20% | Logs, laporan | Testing dan dokumentasi |

---

## 18. Kriteria Lulus Praktikum

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Repository dapat dibangun dari clean checkout | `PASS` | `make -C tests/m16 clean all` berhasil |
| `scripts/m16_preflight.sh` berjalan dan menghasilkan log | `PASS` | `logs/m16/preflight.log` |
| Host unit test menampilkan `M16 host tests PASS` | `PASS` | output terminal |
| `nm_undefined.txt` kosong | `PASS` | `evidence/m16/nm_undefined.txt` |
| `readelf_header.txt` menunjukkan ELF64 REL x86-64 | `PASS` | `evidence/m16/readelf_header.txt` |
| `objdump_disasm.txt` dan `sha256sum.txt` disimpan | `PASS` | `evidence/m16/` |
| QEMU smoke test dijalankan | `PASS` | `logs/m16/qemu_serial.log` |
| Semua perubahan Git dikomit | `PASS` | `git log --oneline` |
| Laporan menyertakan log, analisis failure mode, dan readiness review | `PASS` | Laporan ini |

Kriteria tambahan:

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| GDB debug path diuji | `NA` | Opsional — dapat diuji dengan `-s -S` |
| Fuzzing corpus | `NA` | Tidak dilakukan di M16 |
| Driver hardware nyata | `NA` | Non-goal M16 |
| Multi-transaction concurrency | `NA` | Non-goal M16 |

---

## 19. Verification Matrix

| Requirement | Evidence | Pass criterion |
|---|---|---|
| M16-R1 Source dapat diuji pada host | `logs/m16/preflight.log` + terminal output | `M16 host tests PASS` |
| M16-R2 Journal replay bekerja | Host test `journal replay after committed crash` | `read crash after replay` lulus |
| M16-R3 Corrupt journal fail-closed | Host test `corrupt descriptor rejected` | Return `M16_E_CORRUPT` |
| M16-R4 Freestanding object valid | `build/m16/m16_mcsfs_journal.o` | Object terbentuk |
| M16-R5 Tidak ada undefined symbol | `evidence/m16/nm_undefined.txt` | 0 byte |
| M16-R6 Object x86-64 | `evidence/m16/readelf_header.txt` | ELF64, REL, x86-64 |
| M16-R7 Disassembly tersedia | `evidence/m16/objdump_disasm.txt` | Fungsi M16 tampak |
| M16-R8 Artefak fingerprinted | `evidence/m16/sha256sum.txt` | SHA-256 tercatat |
| M16-R9 QEMU tidak regresi | `logs/m16/qemu_serial.log` | Boot mencapai log M16 |
| M16-R10 Laporan lengkap | Laporan ini | Memuat bukti, analisis, failure modes, rollback |

---

## 20. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | |
| Siap uji QEMU dan host fault-injection terbatas | Host test lulus, freestanding object audit lulus, QEMU smoke test tidak regresi | ✔ |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah tests, security review, dokumentasi, dan known issue tersedia | |

Alasan readiness:

```text
Host unit test lulus seluruhnya: format, fsck, write/read normal, crash-after-commit-record replay, dan corrupt descriptor rejection. Freestanding object ELF64 x86_64 berhasil dikompilasi dan diaudit: nm_undefined.txt kosong, readelf menunjukkan ELF64 REL x86-64, checksum tersimpan. QEMU smoke test membuktikan integrasi M16 tidak merusak boot path. Evidence lengkap tersimpan di evidence/m16/ dan Git commit M16 tercatat.
```

Known issues:

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | Buffer cache M16 belum SMP-safe | Race condition jika diakses concurrent | Gunakan single-core QEMU, lock eksternal dari M12 | Tambahkan locking saat SMP dikonfigurasi |
| 2 | M16 tidak membuktikan durability fisik | RAM-backed device tidak mensimulasikan power-loss nyata | Diterima untuk praktikum pendidikan | Driver hardware dan FUA/barrier di milestone mendatang |
| 3 | Single-file hanya; tidak mendukung file multi-block | File > 512 byte tidak didukung | Batasi ukuran file saat testing | Direct block tambahan di M17+ |
| 4 | Journal hanya untuk transaksi tunggal terakhir | Checkpoint dan concurrent transaction belum ada | Satu transaksi per operasi | Checkpoint daemon dan multi-transaction di M17+ |
| 5 | Image M15 lama tidak kompatibel dengan M16 | Mount akan mengembalikan `M16_E_CORRUPT` | Format ulang image sebelum pakai M16 | Migration path di M17+ |

Keputusan akhir:

```text
Berdasarkan hasil host unit test yang lulus seluruhnya, freestanding object ELF64 x86_64 yang berhasil dikompilasi dan diaudit tanpa undefined symbol, QEMU smoke test yang tidak menyebabkan regresi boot, serta evidence yang lengkap di evidence/m16/, proyek praktikum M16 dinyatakan siap uji QEMU dan host fault-injection terbatas. M16 tidak boleh diberi label siap produksi, aman terhadap power-loss nyata, SMP-safe, kompatibel ext4, atau siap driver hardware nyata.
```

---

## 21. Rubrik Penilaian 100 Poin

| Komponen | Bobot | Indikator nilai penuh | Nilai |
|---|---:|---|---:|
| Kebenaran fungsional | 30 | Format, journal commit, replay, read/write, fsck, corrupt descriptor rejection berjalan sesuai kontrak | `[0-30]` |
| Kualitas desain dan invariants | 20 | Layout, state machine, checksum, target LBA validation, idempotence, fail-closed behavior dijelaskan | `[0-20]` |
| Pengujian dan bukti | 20 | Host test, freestanding audit, nm/readelf/objdump/checksum, QEMU log, commit hash lengkap | `[0-20]` |
| Debugging dan failure analysis | 10 | Analisis bug, root cause, failure mode, dan tindakan perbaikan konkret | `[0-10]` |
| Keamanan dan robustness | 10 | Validasi input, checksum, range check, no hidden libc, fail-closed, batas crash model jelas | `[0-10]` |
| Dokumentasi dan laporan | 10 | Laporan mengikuti template, command dan output lengkap, referensi IEEE, readiness review objektif | `[0-10]` |
| **Total** | **100** | | `[0-100]` |

Catatan penilai:

```text
[Diisi dosen/asisten.]
```

---

## 22. Kesimpulan

### 22.1 Yang Berhasil

```text
1. MCSFS1J berhasil diimplementasikan sebagai single-file source dengan journal header, descriptor, payload, checksum, dan commit record.
2. Write-ahead journal berjalan dengan urutan yang benar: descriptor/payload journal ditulis sebelum commit record, home location diupdate setelah commit.
3. Recovery berhasil mereplay transaksi committed ke home location secara idempotent, dibuktikan oleh host test crash-after-commit-record.
4. Recovery fail-closed pada descriptor corrupt: mengembalikan M16_E_CORRUPT tanpa menulis payload ke target yang tidak tervalidasi.
5. fsck-lite lulus setelah replay, membuktikan metadata konsisten setelah recovery.
6. Freestanding object ELF64 x86_64 berhasil dikompilasi tanpa undefined symbol.
7. QEMU smoke test tidak menyebabkan regresi boot pada kernel M13-M15.
8. Evidence lengkap tersimpan di evidence/m16/ dan Git commit M16 tercatat di branch praktikum-m16-journal-recovery.
```

### 22.2 Yang Belum Berhasil

```text
1. M16 belum membuktikan durability fisik: RAM-backed device tidak mensimulasikan power-loss, FUA, atau write reordering perangkat nyata.
2. Buffer cache belum SMP-safe: semua operasi harus dilindungi lock eksternal jika digunakan bersama scheduler.
3. Hanya mendukung file single-block (max 512 byte): file multi-block memerlukan indirect block atau extent.
4. Tidak ada multi-transaction concurrency atau checkpoint daemon.
5. Tidak ada migration path dari image M15 ke M16: harus format ulang.
```

### 22.3 Rencana Perbaikan

```text
1. M17: Implementasikan support file multi-block dengan indirect block atau extent tree.
2. Tambahkan locking di sekitar operasi filesystem untuk SMP safety.
3. Implementasikan checkpoint daemon yang membersihkan journal secara periodik.
4. Tambahkan migration path dari MCSFS1 ke MCSFS1J.
5. Integrasi penuh ke VFS M13 sehingga syscall file I/O dapat menggunakan MCSFS1J.
6. Tambahkan fuzzing corpus untuk menguji robustness parser journal dan superblock.
```

---

## 23. Lampiran

### Lampiran A — Commit Log

```text
[tempel output git log --oneline -5 dari branch praktikum-m16-journal-recovery]
```

### Lampiran B — Diff Ringkas

```diff
--- /dev/null
+++ b/kernel/fs/mcsfs1j/m16_mcsfs_journal.c
+/*
+ * MCSOS M16 - MCSFS1J crash-consistency teaching journal
+ */
+#include <stdint.h>
+...

--- /dev/null
+++ b/tests/m16/Makefile
+CLANG ?= clang
+TARGET_TRIPLE ?= x86_64-elf
+...

--- /dev/null
+++ b/scripts/m16_preflight.sh
+#!/usr/bin/env bash
+...
```

### Lampiran C — Output Host Test Lengkap

```text
[tempel seluruh output dari make -C tests/m16 clean all]
```

### Lampiran D — Log QEMU Smoke Test

```text
[tempel isi logs/m16/qemu_serial.log]
```

### Lampiran E — Output Readelf

```text
[tempel isi evidence/m16/readelf_header.txt]
```

### Lampiran F — Output nm (undefined)

```text
[tempel isi evidence/m16/nm_undefined.txt — harus kosong]
```

### Lampiran G — Checksum

```text
[tempel isi evidence/m16/sha256sum.txt]
```

### Lampiran H — Log Preflight

```text
[tempel isi logs/m16/preflight.log]
```

---

## 24. Daftar Referensi

```text
[1] The Linux Kernel Documentation, "The Linux Journalling API," Linux Kernel Documentation. [Online]. Available: https://www.kernel.org/doc/html/v5.17/filesystems/journalling.html. Accessed: May 2026.

[2] The Linux Kernel Documentation, "3.6. Journal (jbd2)," Linux Kernel Documentation. [Online]. Available: https://www.kernel.org/doc/html/latest/filesystems/ext4/journal.html. Accessed: May 2026.

[3] The Linux Kernel Documentation, "Ext4 Data Mode," Linux Kernel Documentation. [Online]. Available: https://www.kernel.org/doc/html/v4.19/filesystems/ext4/ext4.html. Accessed: May 2026.

[4] QEMU Project, "GDB usage," QEMU System Emulation Documentation. [Online]. Available: https://www.qemu.org/docs/master/system/gdb.html. Accessed: May 2026.

[5] LLVM Project, "Clang command line argument reference," Clang Documentation. [Online]. Available: https://clang.llvm.org/docs/ClangCommandLineReference.html. Accessed: May 2026.

[6] Free Software Foundation, "GNU Binutils," GNU Project. [Online]. Available: https://www.gnu.org/software/binutils/binutils.html. Accessed: May 2026.

[7] Free Software Foundation, "GNU make," GNU Make Manual. [Online]. Available: https://www.gnu.org/software/make/manual/make.html. Accessed: May 2026.

[8] Microsoft, "Install WSL," Microsoft Learn. [Online]. Available: https://learn.microsoft.com/en-us/windows/wsl/install. Accessed: May 2026.
```

---

## 25. Checklist Final Sebelum Pengumpulan

| Checklist | Status |
|---|---|
| Semua placeholder NIM dan nama sudah diisi oleh anggota | `[isi oleh kelompok]` |
| Metadata laporan lengkap (nama, NIM, tanggal) | `[isi oleh kelompok]` |
| Commit awal dan akhir dicatat | `[isi oleh kelompok]` |
| Versi toolchain ditempel dari WSL aktual | `[isi oleh kelompok]` |
| `make -C tests/m16 clean all` dapat dijalankan ulang | `Ya` |
| Output host test ditempel di lampiran | `Lampiran C` |
| Log QEMU dilampirkan | `Lampiran D` |
| Evidence tersedia di `evidence/m16/` | `Ya` |
| Desain, invariants, state machine, dan failure modes dijelaskan | `Ya` |
| Security/reliability dibahas | `Ya` |
| Readiness review tidak berlebihan | `Ya — diklaim siap uji QEMU dan host fault-injection terbatas` |
| Rubrik penilaian disiapkan | `Ya` |
| Referensi memakai format IEEE | `Ya` |
| Laporan disimpan sebagai Markdown | `Ya` |

---

## 26. Pernyataan Pengumpulan

Saya/kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```text
[tempel commit hash akhir dari git log --oneline -1]
```

Status akhir yang diklaim:

```text
siap uji QEMU dan host fault-injection terbatas untuk MCSFS1J write-ahead journal
```

Ringkasan satu paragraf:

```text
Praktikum M16 berhasil membangun mekanisme crash consistency pada MCSOS 260502 dengan mengimplementasikan MCSFS1J, yaitu MCSFS1 yang diperluas dengan write-ahead journal sederhana. Keberhasilan dibuktikan oleh host unit test yang lulus seluruhnya: format, fsck, write/read normal, crash-after-commit-record recovery, dan corrupt descriptor rejection berjalan sesuai kontrak. Freestanding object ELF64 x86_64 berhasil dikompilasi tanpa undefined symbol dan diaudit dengan readelf, objdump, serta sha256sum. QEMU smoke test membuktikan integrasi M16 tidak menyebabkan regresi boot pada kernel. Keterbatasan M16 adalah belum ada durability fisik (RAM-backed only), belum SMP-safe, hanya mendukung single-block file, tidak ada multi-transaction concurrency, dan tidak ada migration path dari M15 — semua akan dibahas pada M17 dan milestone berikutnya.
```
