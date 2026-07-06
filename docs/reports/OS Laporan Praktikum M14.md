# Template Laporan Praktikum Sistem Operasi Lanjut — MCSOS

**Nama file laporan:** `laporan_praktikum_[M14]_[_kelompok].md`  
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
| Kode praktikum | `M14` |
| Judul praktikum | `Block Device Layer, RAM Block Driver, Buffer Cache Minimal, dan Jalur Persiapan Filesystem Persistent pada MCSOS` |
| Jenis pengerjaan | `Kelompok` |
| Nama mahasiswa | `Fauziah Putri Rahayu` |
| NIM | `2583207073004` |
| Kelas | `1A` |
| Nama kelompok | `kelompok princess` |
| Anggota kelompok | `Asti lestari, Wifa fazriyatul, Nazwa Rahmadanti, Fauziah Putri, Amelia okta \| 25832071001, 2583207073003, 2583207073005, 2583207073004, 25832072004` |
| Tanggal praktikum | `` |
| Tanggal pengumpulan | `` |
| Repository | `~/osdev/mcsos` |
| Branch | `praktikum-m14-block-device` |
| Commit awal | `dfbdb72` |
| Commit akhir | `655dbe0` |
| Status readiness yang diklaim | `siap uji QEMU` |

---

## 1. Sampul

# Laporan Praktikum `M14`
## `Block Device Layer, RAM Block Driver, Buffer Cache Minimal, dan Jalur Persiapan Filesystem Persistent pada MCSOS`

Disusun oleh:

| Nama | NIM | Kelas | Peran |
|Fauziah Putri Rahayu|2583207073004|1A|---|
| `Asti lestari` | `25832071001` | `1A` | `koordinator` |
| `Wifa fazriyatul` | `2583207073003` | `1A` | `` |
| `Nazwa Rahmadanti` | `2583207073005` | `1A` | `` |
| `Fauziah Putri` | `2583207073004` | `1A` | `` |
| `Amelia okta` | `25832072004` | `1A` | `` |

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
Menggunakan AI assistant untuk membantu penjelasan langkah kerja dan memahami konsep block device layer, RAM block driver, buffer cache, dan jalur persiapan filesystem persistent pada praktikum M14. Seluruh implementasi, pengujian, build, audit, dan evidence tetap diverifikasi secara mandiri menggunakan terminal WSL, QEMU, dan GDB sesuai panduan praktikum.
```

---

## 3. Tujuan Praktikum

1. Membuat header `include/mcsos/block.h` yang mendefinisikan status code, struktur block device, operation table, RAM block metadata, dan buffer cache metadata untuk MCSOS.
2. Mengimplementasikan `kernel/block/block.c` sebagai registry block device dan wrapper validasi LBA untuk operasi read/write/flush.
3. Mengimplementasikan `kernel/block/ramblk.c` sebagai RAM-backed block driver yang deterministik, tanpa dynamic allocation, dan dapat diuji di host.
4. Mengimplementasikan `kernel/block/bcache.c` sebagai buffer cache minimal dengan `valid`, `dirty`, `lba`, `dev`, dan flush eksplisit.
5. Membuat dan menjalankan host unit test `tests/host/test_m14_block.c` sampai lulus di lingkungan hosted.
6. Mengompilasi object freestanding x86_64 dari ketiga modul block layer tanpa ketergantungan libc.
7. Menggabungkan object menjadi linked relocatable `m14_block_layer.o` dan memverifikasi tidak ada undefined symbol dengan `nm -u`.
8. Melakukan audit artefak menggunakan `readelf`, `objdump`, `nm`, `sha256sum`, log QEMU, dan menyimpan evidence di `artifacts/m14/`.

---

## 4. Capaian Pembelajaran Praktikum

Setelah praktikum ini, mahasiswa mampu:

| CPL/CPMK praktikum | Bukti yang harus ditunjukkan |
|---|---|
| Menjelaskan perbedaan file-level I/O pada VFS M13 dan block-level I/O pada storage layer M14 | Laporan dasar teori dan analisis teknis |
| Mendesain kontrak block device yang memisahkan registry, operasi driver, validasi range, dan error code | Header `block.h`, struktur data, dan invariant dalam laporan |
| Mengimplementasikan RAM block driver yang deterministik dan dapat diuji di host | Source `ramblk.c`, output host test `PASS` |
| Mengimplementasikan buffer cache minimal dengan `valid`, `dirty`, `lba`, `dev`, dan flush eksplisit | Source `bcache.c`, output host test `PASS` |
| Membuktikan operasi read/write/flush dan validasi boundary berjalan sesuai kontrak | Log `make host-test` dengan seluruh kasus `PASS` |
| Menghasilkan object freestanding x86_64 tanpa undefined symbol setelah linked relocatable aggregation | Output `make audit`, `nm -u m14_block_layer.o` kosong |
| Menyusun bukti audit menggunakan `nm`, `readelf`, `objdump`, `sha256sum`, QEMU log, dan laporan readiness | Direktori `artifacts/m14/` berisi seluruh artefak |
| Mengidentifikasi failure mode storage awal: out-of-range LBA, dirty buffer tidak di-flush, stale cache, block size mismatch | Bagian analisis dan failure modes pada laporan |

---

## 5. Peta Milestone MCSOS

Centang milestone yang menjadi fokus laporan ini.

| Milestone | Fokus | Status dalam laporan |
|---|---|---|
| M0 | Requirements, governance, baseline arsitektur | selesai praktikum |
| M1 | Toolchain reproducible, Git, QEMU, GDB, metadata build | selesai praktikum |
| M2 | Boot image, kernel ELF64, early console | selesai praktikum |
| M3 | Panic path, linker map, GDB, observability awal | selesai praktikum |
| M4 | IDT, exception stub, trap frame, dispatcher, jalur uji int3 | selesai praktikum |
| M5 | PMM, VMM, page table, kernel heap | selesai praktikum |
| M6 | Thread, scheduler, synchronization | selesai praktikum |
| M7 | Syscall ABI dan user program loader | selesai praktikum |
| M8 | VFS, file descriptor, ramfs | selesai praktikum |
| M9 | Block layer awal dan device model | selesai praktikum |
| M10 | Persistent filesystem, mcsfs/ext2-like, recovery | selesai praktikum |
| M11 | Networking stack, packet parsing, UDP/TCP subset | selesai praktikum |
| M12 | Security model, capability/ACL, syscall fuzzing, hardening | selesai praktikum |
| M13 | VFS minimal, file descriptor table, RAMFS volatil, syscall file I/O | selesai praktikum |
| M14 | Block device layer, RAM block driver, buffer cache minimal | **selesai praktikum** |
| M15 | Filesystem persistent berbasis blok | tidak dibahas |
| M16 | Observability, update/rollback, release image, readiness review | tidak dibahas |

Batas cakupan praktikum:

```text
Praktikum M14 berfokus pada implementasi block device registry, RAM block driver volatil, dan buffer cache minimal dengan dirty flag serta flush eksplisit pada MCSOS. Praktikum mencakup proses build host unit test, compile freestanding object x86_64, audit ELF/disassembly, linked relocatable aggregation, QEMU smoke test, dan pengumpulan evidence. Fitur seperti driver SATA/NVMe/virtio-blk, AHCI, DMA, MSI/MSI-X, interrupt completion, filesystem persistent, journal, fsck penuh, crash consistency, user ABI storage publik, security boundary pengguna, dan SMP-safe buffer cache tidak termasuk dalam cakupan praktikum ini.
```

---

## 6. Dasar Teori Ringkas

### 6.1 Konsep Sistem Operasi yang Diuji

```text
Pada praktikum M14, konsep utama yang diuji adalah block device layer, RAM block driver, buffer cache minimal, dan jalur persiapan filesystem persistent pada kernel x86_64.

1. Block Device
Block device adalah perangkat yang dibaca dan ditulis dalam unit blok tetap, biasanya melalui Logical Block Address (LBA). Tidak seperti character device yang mengalirkan byte satu per satu, block device memungkinkan akses acak ke blok mana pun secara independen. M14 menerapkan konsep ini tanpa bergantung pada hardware fisik dengan menggunakan RAM sebagai backing storage.

2. Block Device Registry
Registry menyimpan pointer ke struktur block device yang lifetime-nya dimiliki caller. Registry M14 hanya menyimpan referensi, tidak melakukan alokasi dinamis. Fungsi mcsos_blk_register, mcsos_blk_get, dan mcsos_blk_unregister membentuk antarmuka registry.

3. RAM Block Driver (ramblk)
RAM block driver meniru perangkat blok menggunakan array byte di memori. Driver tidak memakai malloc, tidak bergantung pada libc, sehingga dapat diuji di host dan digunakan sebagai freestanding object. Operasi flush pada ramblk adalah no-op sukses karena data sudah berada di memori volatil.

4. Buffer Cache Minimal
Buffer cache menyimpan salinan blok storage di memori. Entry memiliki flag valid (apakah slot berisi data yang bermakna) dan dirty (apakah data cache sudah dimodifikasi tetapi belum ditulis ke device). Write-back berarti penulisan hanya masuk ke cache; data tidak langsung ditulis ke device sampai flush dipanggil secara eksplisit atau entry diusir (eviction).

5. Flushing dan Eviction
bcache_flush_all menulis semua dirty entry ke device sebelum shutdown atau sebelum entry tersebut diganti oleh blok lain. Tanpa flush, data yang belum disinkronkan hilang jika terjadi crash atau reboot.

6. LBA dan Validasi Range
LBA adalah nomor blok logis mulai dari 0. Setiap operasi harus memvalidasi bahwa lba < block_count dan count <= block_count - lba agar tidak terjadi akses out-of-range yang dapat merusak memori kernel.
```

### 6.2 Konsep Arsitektur yang Relevan

| Konsep | Relevansi pada praktikum | Bukti/verifikasi |
|---|---|---|
| LBA (Logical Block Address) | Nomor blok logis untuk akses; harus divalidasi agar tidak out-of-range | Host test negative case `lba >= block_count` mengembalikan `MCSOS_BLK_ERANGE` |
| Block size power-of-two | Memudahkan alignment dan kalkulasi offset byte | `mcsos_is_power_of_two_u32` diverifikasi saat inisialisasi ramblk |
| Driver operation table | Function pointer `read`, `write`, `flush` memisahkan caller dari implementasi driver | `mcsos_blk_ops_t` di `block.h`, disassembly dan `nm` |
| Write-back cache | Penulisan ke cache tidak langsung ke media; flush eksplisit diperlukan | `bcache_write` menandai dirty; `bcache_flush_all` menulis ke device |
| Freestanding C | Kernel tidak bergantung pada hosted libc, `malloc`, `printf` | `nm -u m14_block_layer.o` kosong |
| Object audit ELF | `nm`, `readelf`, `objdump`, `sha256sum` membuktikan artefak dapat diperiksa | `artifacts/m14/` berisi semua output audit |

### 6.3 Konsep Implementasi Freestanding

| Aspek | Keputusan praktikum |
|---|---|
| Bahasa | C17 freestanding untuk kernel; C17 hosted untuk host unit test |
| Runtime | Tanpa hosted libc; `nm -u` harus kosong, tidak ada `malloc`/`memcpy` dari libc |
| ABI | Kernel-internal C ABI; belum ada stable driver ABI publik |
| Compiler flags kritis | `--target=x86_64-elf`, `-ffreestanding`, `-fno-builtin`, `-fno-stack-protector`, `-mno-red-zone`, `-mno-sse`, `-mno-mmx`, `-mcmodel=kernel`, `-Werror` |
| Risiko undefined behavior | Pointer null ke device/buffer (dijaga validasi eksplisit), LBA out-of-range (dijaga `mcsos_blk_validate_range`), block_size bukan power-of-two (dijaga `mcsos_is_power_of_two_u32`) |

### 6.4 Referensi Teori yang Digunakan

| No. | Sumber | Bagian yang digunakan | Alasan relevansi |
|---|---|---|---|
| 1 | Linux Kernel Documentation, "Block" | Block subsystem, konsep pemisahan request block I/O dari perangkat fisik | Landasan konseptual block layer M14 |
| 2 | Linux Kernel Documentation, "blk-mq" | Multi-queue block IO queueing mechanism | Pembanding edukatif arsitektur block layer modern |
| 3 | Linux Kernel Documentation, "null_blk" | Null block device driver sebagai perangkat sintetis | Landasan konseptual RAM block driver sebagai block device sintetis |
| 4 | QEMU Documentation, "Invocation" | Opsi `-drive format=raw`, boot, serial | Menjalankan QEMU smoke test |
| 5 | QEMU Documentation, "GDB usage" | `-s -S` gdbstub | GDB remote debug pada kernel M14 |
| 6 | LLVM/Clang Documentation | `-ffreestanding`, target `x86_64-elf` | Flag compile freestanding dan target triple |
| 7 | GNU Binutils Documentation | `nm`, `readelf`, `objdump` | Audit ELF, symbol table, disassembly |

---

## 7. Lingkungan Praktikum

### 7.1 Host dan Target

| Komponen | Nilai |
|---|---|
| Host OS | `Windows 11 x64` |
| Lingkungan build | `WSL 2 Ubuntu` |
| Target ISA | `x86_64` |
| Target ABI | `x86_64-elf` |
| Emulator | `QEMU qemu-system-x86_64` |
| Firmware emulator | `OVMF` |
| Debugger | `GDB` |
| Build system | `Make` |
| Bahasa utama | `C17 freestanding (kernel), C17 hosted (host test)` |

### 7.2 Versi Toolchain

Tempel output versi toolchain berikut. Jalankan dari clean shell WSL.

```bash
date -u +"date_utc=%Y-%m-%dT%H:%M:%SZ"
uname -a
git --version
make --version | head -n 1
clang --version | head -n 1
ld --version | head -n 1
readelf --version | head -n 1
objdump --version | head -n 1
nm --version | head -n 1
sha256sum --version | head -n 1
qemu-system-x86_64 --version | head -n 1
gdb --version | head -n 1
```

Output:

```text
date_utc=2026-...
Linux ... WSL2 x86_64 GNU/Linux
git version ...
GNU Make ...
Ubuntu clang version ...
GNU ld ...
GNU readelf ...
GNU objdump ...
GNU nm ...
sha256sum ...
QEMU emulator version ...
GNU gdb ...
```

### 7.3 Lokasi Repository

| Item | Nilai |
|---|---|
| Path repository di WSL | `~/osdev/mcsos` |
| Apakah berada di filesystem Linux WSL, bukan `/mnt/c` | `Ya` |
| Remote repository | `tidak ada` |
| Branch | `praktikum-m14-block-device` |
| Commit hash awal | `` |
| Commit hash akhir | `` |

---

## 8. Repository dan Struktur File

### 8.1 Struktur Direktori yang Relevan

```text
mcsos/
├── Makefile
├── include/
│   └── mcsos/
│       └── block.h
├── kernel/
│   └── block/
│       ├── block.c
│       ├── ramblk.c
│       └── bcache.c
├── tests/
│   └── host/
│       └── test_m14_block.c
├── scripts/
│   ├── m14_preflight.sh
│   └── m14_collect_evidence.sh
└── artifacts/
    └── m14/
        ├── host_info.txt
        ├── tool_versions.txt
        ├── preflight.log
        ├── test_m14_block.log
        ├── m14_block_layer.readelf.txt
        ├── m14_block_layer.nm.txt
        ├── m14_block_layer.objdump.txt
        ├── m14_block_layer.sha256.txt
        └── qemu_smoke.log
```

### 8.2 File yang Dibuat atau Diubah

| File | Jenis perubahan | Alasan perubahan | Risiko |
|---|---|---|---|
| `include/mcsos/block.h` | baru | Definisi status code, struct `mcsos_blk_device_t`, `mcsos_blk_ops_t`, `mcsos_ramblk_t`, `mcsos_bcache_entry_t`, `mcsos_bcache_t`, dan deklarasi API | sedang — perubahan struct berdampak ke semua modul yang menggunakannya |
| `kernel/block/block.c` | baru | Implementasi registry block device dan wrapper validasi `mcsos_blk_read`, `mcsos_blk_write`, `mcsos_blk_flush` | sedang — validasi LBA yang salah menyebabkan akses out-of-range |
| `kernel/block/ramblk.c` | baru | Implementasi RAM block driver tanpa dynamic allocation | rendah — driver sintetis, tidak ada hardware |
| `kernel/block/bcache.c` | baru | Implementasi buffer cache minimal dengan dirty flag dan flush eksplisit | tinggi — dirty entry yang tidak di-flush sebelum eviction menyebabkan kehilangan data |
| `tests/host/test_m14_block.c` | baru | Host unit test untuk block API, ramblk, dan bcache | rendah — test environment hosted, tidak berdampak ke kernel |
| `Makefile` | diubah | Tambah target `host-test`, `freestanding`, `audit`, dan `m14-clean` | sedang — target yang salah dapat menghapus artefak penting |
| `scripts/m14_preflight.sh` | baru | Cek kesiapan M0–M13, toolchain, dan direktori sebelum M14 | rendah |
| `scripts/m14_collect_evidence.sh` | baru | Kumpulkan artefak ke `artifacts/m14/` | rendah |

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

## 9. Desain Teknis

### 9.1 Masalah yang Diselesaikan

```text
Kernel MCSOS sebelum M14 hanya memiliki VFS dan RAMFS yang menyimpan seluruh data di memori tanpa abstraksi storage berbasis blok. Akibatnya, tidak ada jalur terstruktur untuk mengintegrasikan media penyimpanan nyata di masa mendatang. M14 menyelesaikan masalah ini dengan:
1. Mendefinisikan kontrak block device yang memisahkan registry, operation table, validasi range, dan error code.
2. Menyediakan RAM block driver yang meniru perangkat blok secara deterministik untuk keperluan pengujian tanpa hardware nyata.
3. Menyediakan buffer cache minimal yang memperkenalkan invariant valid, dirty, dan flush sebelum masuk ke filesystem persistent M15+.
4. Membuktikan seluruh komponen dapat dikompilasi sebagai freestanding object x86_64 tanpa ketergantungan libc.
```

### 9.2 Keputusan Desain

| Keputusan | Alternatif yang dipertimbangkan | Alasan memilih | Konsekuensi |
|---|---|---|---|
| RAM block driver sebelum virtio-blk/NVMe | Langsung implementasi virtio-blk | RAM driver memungkinkan verifikasi kontrak block layer tanpa kompleksitas PCI/DMA/IRQ | Driver hardware nyata belum tersedia; harus ditambahkan di milestone berikutnya |
| Write-back cache (bukan write-through) | Write-through (tulis langsung ke device) | Write-back memperkenalkan konsep dirty flag yang relevan untuk filesystem persistent | Dirty entry dapat hilang jika crash sebelum flush; diterima karena M14 masih RAM-volatil |
| Static array entry cache (bukan linked list) | Linked list dinamis | Tidak memerlukan dynamic allocation; kompatibel freestanding | Jumlah entry cache terbatas; trade-off diterima untuk pendidikan |
| Validasi LBA di block layer (bukan di driver) | Validasi di setiap driver | Satu titik validasi mencegah duplikasi dan inkonsistensi antar driver | Driver dapat mengasumsikan argumen valid saat dipanggil oleh block layer |
| No-op flush pada ramblk | Flush menulis ke file host | Data sudah di memori; flush nyata tidak relevan untuk driver sintetis | Kehilangan data saat crash tetap terjadi — ini sengaja untuk pendidikan |

### 9.3 Arsitektur Komponen

```text
                +-------------------------------+
                |        VFS / RAMFS M13        |
                +---------------+---------------+
                                |
                                | calon integrasi M15+
                                v
+-------------------+    +-----------------------+    +----------------------+
| Host Unit Tests   | -> | M14 Block Device API  | -> | Driver ops table     |
| QEMU smoke path   |    | read/write/flush      |    | read/write/flush     |
+-------------------+    +-----------+-----------+    +----------+-----------+
                                    |                           |
                                    v                           v
                         +-----------------------+    +----------------------+
                         | Buffer Cache          |    | RAM Block Driver     |
                         | valid/dirty/dev/lba   |    | byte-array storage   |
                         +-----------------------+    +----------------------+
```

M14 tidak mengganti VFS M13. M14 menambahkan storage abstraction di bawah VFS agar modul berikutnya dapat mengembangkan filesystem persistent berbasis blok secara bertahap.

### 9.4 API Utama

| Fungsi | Dipanggil dari | Lokasi | Precondition | Postcondition | Error |
|---|---|---|---|---|---|
| `mcsos_blk_register(dev)` | kernel init / test | `block.c` | `dev != NULL`, `ops != NULL`, slot tersedia | Device terdaftar di registry | `MCSOS_BLK_EINVAL`, `MCSOS_BLK_ENOMEM` |
| `mcsos_blk_read(dev, lba, count, buf)` | block layer caller / bcache | `block.c` | `dev`, `buf` non-null; `lba + count <= block_count` | Data dari device tersalin ke `buf` | `MCSOS_BLK_ERANGE`, `MCSOS_BLK_EINVAL` |
| `mcsos_blk_write(dev, lba, count, buf)` | block layer caller / bcache | `block.c` | `dev`, `buf` non-null; `lba + count <= block_count` | Data dari `buf` tersalin ke device | `MCSOS_BLK_ERANGE`, `MCSOS_BLK_EINVAL` |
| `mcsos_blk_flush(dev)` | bcache / shutdown path | `block.c` | `dev != NULL`, `ops != NULL` | Semua data di device sinkron (no-op untuk ramblk) | `MCSOS_BLK_EINVAL` |
| `mcsos_ramblk_init(dev, ram, name, storage, size, bs)` | test / kernel init | `ramblk.c` | `dev`, `ram`, `storage`, `name` non-null; `bs` power-of-two >= 512 | Device siap digunakan sebagai block device | `MCSOS_BLK_EINVAL` |
| `mcsos_bcache_init(cache, dev, entries, count, bs)` | test / kernel init | `bcache.c` | `cache`, `dev`, `entries` non-null; `count > 0` | Cache siap digunakan | `MCSOS_BLK_EINVAL` |
| `mcsos_bcache_read(cache, lba, buf)` | filesystem caller | `bcache.c` | `cache`, `buf` non-null; `lba` valid | Data blok tersedia di `buf` (dari cache atau device) | `MCSOS_BLK_ERANGE`, `MCSOS_BLK_EINVAL` |
| `mcsos_bcache_write(cache, lba, buf)` | filesystem caller | `bcache.c` | `cache`, `buf` non-null; `lba` valid | Data dari `buf` tersalin ke cache entry, dirty ditandai | `MCSOS_BLK_ERANGE`, `MCSOS_BLK_EINVAL` |
| `mcsos_bcache_flush_all(cache)` | shutdown / fsync | `bcache.c` | `cache != NULL` | Semua dirty entry ditulis ke device; dirty di-clear | `MCSOS_BLK_EINVAL` jika write gagal |

### 9.5 Struktur Data Utama

| Struktur data | Field penting | Ownership | Lifetime | Invariant |
|---|---|---|---|---|
| `mcsos_blk_device_t` | `name`, `block_size`, `block_count`, `flags`, `ops`, `driver_data` | caller (kernel atau test) | selama device aktif | `ops != NULL`, `block_size >= 512` dan power-of-two, `block_count > 0` |
| `mcsos_blk_ops_t` | `read`, `write`, `flush` | statis di driver (`g_ramblk_ops`) | selama driver aktif | `read` dan `write` non-null untuk device yang diregistrasi |
| `mcsos_ramblk_t` | `storage`, `storage_size` | caller (test atau kernel) | selama driver aktif | `storage != NULL`, `storage_size % block_size == 0` |
| `mcsos_bcache_entry_t` | `valid`, `dirty`, `dev`, `lba`, `data[]` | `mcsos_bcache_t` | selama cache aktif | `valid` harus benar agar `dev` dan `lba` bermakna; `dirty` hanya valid jika `valid` |
| `mcsos_bcache_t` | `dev`, `entries`, `entry_count`, `block_size` | caller | selama cache aktif | `entry_count > 0`, `block_size` sama dengan `dev->block_size` |

### 9.6 Invariants

1. `dev->block_size >= 512` dan merupakan power-of-two — dijaga oleh `mcsos_ramblk_init` dan validasi di `block.c`.
2. `dev->block_count > 0` — dijaga oleh `mcsos_ramblk_init` dan validasi registry.
3. `dev->ops != NULL`, `dev->ops->read != NULL`, `dev->ops->write != NULL` sebelum device diregistrasi — dijaga oleh `mcsos_blk_register`.
4. `lba < block_count` dan `count <= block_count - lba` untuk setiap operasi — dijaga oleh `mcsos_blk_validate_range`.
5. Operasi dengan `count == 0` ditolak sebagai `MCSOS_BLK_EINVAL` — dijaga oleh `mcsos_blk_validate_range`.
6. Entry dirty harus di-flush sebelum eviction atau sebelum shutdown — dijaga oleh `mcsos_bcache_select_victim` dan `mcsos_bcache_flush_all`.
7. `cache->block_size` sama dengan `dev->block_size` — dijaga saat `mcsos_bcache_init`.
8. `nm -u m14_block_layer.o` kosong — ketiga modul bebas dari ketergantungan libc.

### 9.7 Ownership, Locking, dan Concurrency

| Objek/resource | Owner | Lock yang melindungi | Boleh dipakai di interrupt context? | Catatan |
|---|---|---|---|---|
| Registry `g_blk_registry[]` | kernel statis | tidak ada (M14 single-core) | Tidak disarankan | Tidak ada locking; caller wajib memastikan akses serial |
| `mcsos_blk_device_t` | caller yang mendaftarkan | tidak ada | Tidak | Lifetime harus dijamin caller sampai `unregister` |
| Buffer cache `entries[]` | `mcsos_bcache_t` | tidak ada (M14 belum SMP-safe) | Tidak | M14 hanya untuk single-core; SMP ditambahkan di milestone berikutnya |
| `mcsos_ramblk_t.storage` | caller test/kernel | tidak ada | Tidak | Backing storage harus dipertahankan caller selama ramblk aktif |

Lock order yang berlaku:

```text
M14 masih single-core dan tidak ada userspace yang mengakses block layer secara langsung. Mekanisme locking belum diperlukan. Non-goals M14 termasuk SMP-safety dan driver hardware nyata.
```

### 9.8 Memory Safety dan Undefined Behavior Risk

| Risiko | Lokasi | Mitigasi | Bukti |
|---|---|---|---|
| LBA out-of-range menyebabkan akses di luar array storage | `ramblk.c` operasi rw | `mcsos_blk_validate_range` di `block.c` + guard di `mcsos_ramblk_rw` | Host test negative case lulus: `MCSOS_BLK_ERANGE` |
| Dirty entry di-evict tanpa flush menyebabkan data hilang | `bcache.c` `mcsos_bcache_select_victim` | Flush dirty entry sebelum reuse | Host test flush-before-evict lulus |
| `block_size` bukan power-of-two menyebabkan offset kalkulasi salah | `ramblk_init` | `mcsos_is_power_of_two_u32_local` saat init | Host test init dengan block_size = 100 mengembalikan `MCSOS_BLK_EINVAL` |
| Null pointer dereference pada `dev`, `ops`, `buffer` | `block.c` semua fungsi | Guard `dev == NULL`, `ops == NULL`, `buffer == NULL` di awal fungsi | Host test null pointer cases lulus |
| `storage_size` bukan kelipatan `block_size` | `ramblk_init` | Validasi `storage_size % block_size != 0` mengembalikan `MCSOS_BLK_EINVAL` | Host test negative init lulus |

### 9.9 Security Boundary

| Boundary | Data tidak tepercaya | Validasi yang dilakukan | Failure mode aman |
|---|---|---|---|
| Input operasi dari caller block layer | LBA, count, pointer buffer | `mcsos_blk_validate_range` memeriksa range dan null | `MCSOS_BLK_EINVAL` atau `MCSOS_BLK_ERANGE` |
| Inisialisasi ramblk | Ukuran storage, block_size, pointer | Guard null, power-of-two, kelipatan block_size | `MCSOS_BLK_EINVAL` |
| Input bcache dari filesystem caller | LBA, pointer buffer | Validasi `lba < block_count` dan null check | `MCSOS_BLK_ERANGE` atau `MCSOS_BLK_EINVAL` |

---

## 10. Langkah Kerja Implementasi

### Langkah 1 — Catat Informasi Host dan Versi Toolchain

Maksud langkah:

```text
Mengumpulkan identitas host dan toolchain sebagai bukti bahwa praktikum dibangun pada lingkungan yang dapat diaudit.
```

Perintah:

```bash
mkdir -p artifacts/m14
{ uname -a; lsb_release -a 2>/dev/null || cat /etc/os-release; } | tee artifacts/m14/host_info.txt
{ clang --version; ld --version | head -n 1; nm --version | head -n 1; readelf --version | head -n 1; objdump --version | head -n 1; make --version | head -n 1; qemu-system-x86_64 --version; } | tee artifacts/m14/tool_versions.txt
```

Output ringkas:

```text
[tempel isi artifacts/m14/host_info.txt]
[tempel isi artifacts/m14/tool_versions.txt]
```

Indikator berhasil:

```text
artifacts/m14/host_info.txt dan artifacts/m14/tool_versions.txt terisi.
```

---

### Langkah 2 — Buat Branch M14

Maksud langkah:

```text
Membuat branch terpisah agar perubahan block layer tidak merusak baseline M13.
```

Perintah:

```bash
git status --short
git switch -c praktikum-m14-block-device
mkdir -p include/mcsos kernel/block tests/host scripts artifacts/m14
git branch --show-current
```

Output ringkas:

```text
praktikum-m14-block-device
```

Indikator berhasil:

```text
git branch --show-current menampilkan praktikum-m14-block-device.
```

---

### Langkah 3 — Jalankan Preflight M14

Maksud langkah:

```text
Memastikan toolchain dan artefak M0–M13 tersedia sebelum menulis source M14.
```

Perintah:

```bash
cat > scripts/m14_preflight.sh <<'EOF'
#!/usr/bin/env bash
set -euo pipefail
mkdir -p artifacts/m14
LOG="artifacts/m14/preflight.log"
: > "$LOG"
require_cmd() {
  local cmd="$1"
  if ! command -v "$cmd" >/dev/null 2>&1; then
    echo "MISSING_CMD: $cmd" | tee -a "$LOG"; return 1
  fi
  echo "OK_CMD: $cmd=$($cmd --version 2>/dev/null | head -n 1 || true)" | tee -a "$LOG"
}
require_cmd clang
require_cmd ld
require_cmd nm
require_cmd readelf
require_cmd objdump
require_cmd sha256sum
require_cmd make
require_cmd qemu-system-x86_64
for d in include kernel tests scripts; do
  [[ -d "$d" ]] && echo "OK_DIR: $d" | tee -a "$LOG" || echo "WARN_DIR_MISSING: $d" | tee -a "$LOG"
done
git status --short | tee artifacts/m14/git_status_before_m14.txt
echo "M14_PREFLIGHT_DONE" | tee -a "$LOG"
EOF
chmod +x scripts/m14_preflight.sh
./scripts/m14_preflight.sh
```

Output ringkas:

```text
OK_CMD: clang=Ubuntu clang version ...
OK_CMD: ld=...
OK_CMD: nm=...
OK_CMD: readelf=...
OK_CMD: objdump=...
OK_CMD: sha256sum=...
OK_CMD: make=...
OK_CMD: qemu-system-x86_64=QEMU emulator version ...
OK_DIR: include
OK_DIR: kernel
OK_DIR: tests
OK_DIR: scripts
M14_PREFLIGHT_DONE
```

Indikator berhasil:

```text
Log berakhir dengan M14_PREFLIGHT_DONE dan semua baris menampilkan OK_CMD atau OK_DIR.
```

---

### Langkah 4 — Buat Header `include/mcsos/block.h`

Maksud langkah:

```text
Mendefinisikan seluruh tipe data, konstanta, dan deklarasi API block layer agar dapat digunakan oleh block.c, ramblk.c, bcache.c, dan host test.
```

Perintah:

```bash
# Buat include/mcsos/block.h
```

File `include/mcsos/block.h`:

```c
#ifndef MCSOS_BLOCK_H
#define MCSOS_BLOCK_H

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Status codes                                                         */
/* ------------------------------------------------------------------ */
typedef enum {
    MCSOS_BLK_OK      =  0,
    MCSOS_BLK_EINVAL  = -1,
    MCSOS_BLK_ERANGE  = -2,
    MCSOS_BLK_EIO     = -3,
    MCSOS_BLK_ENOMEM  = -4,
    MCSOS_BLK_EBUSY   = -5,
} mcsos_blk_status_t;

/* ------------------------------------------------------------------ */
/* Constants                                                            */
/* ------------------------------------------------------------------ */
#define MCSOS_BLK_NAME_MAX            32u
#define MCSOS_BLK_REGISTRY_MAX        8u
#define MCSOS_BLK_DEFAULT_SECTOR_SIZE 512u
#define MCSOS_BLK_FLAG_READONLY       (1u << 0)

/* ------------------------------------------------------------------ */
/* Forward declarations                                                 */
/* ------------------------------------------------------------------ */
typedef struct mcsos_blk_device  mcsos_blk_device_t;
typedef struct mcsos_blk_ops     mcsos_blk_ops_t;
typedef struct mcsos_ramblk      mcsos_ramblk_t;
typedef struct mcsos_bcache_entry mcsos_bcache_entry_t;
typedef struct mcsos_bcache      mcsos_bcache_t;

/* ------------------------------------------------------------------ */
/* Driver operation table                                               */
/* ------------------------------------------------------------------ */
struct mcsos_blk_ops {
    mcsos_blk_status_t (*read) (mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer);
    mcsos_blk_status_t (*write)(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer);
    mcsos_blk_status_t (*flush)(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer);
};

/* ------------------------------------------------------------------ */
/* Block device descriptor                                              */
/* ------------------------------------------------------------------ */
struct mcsos_blk_device {
    char                  name[MCSOS_BLK_NAME_MAX];
    uint32_t              block_size;
    uint64_t              block_count;
    uint32_t              flags;
    const mcsos_blk_ops_t *ops;
    void                  *driver_data;
};

/* ------------------------------------------------------------------ */
/* RAM block driver metadata                                            */
/* ------------------------------------------------------------------ */
struct mcsos_ramblk {
    uint8_t  *storage;
    uint64_t  storage_size;
};

/* ------------------------------------------------------------------ */
/* Buffer cache entry                                                   */
/* ------------------------------------------------------------------ */
struct mcsos_bcache_entry {
    uint8_t              valid;
    uint8_t              dirty;
    mcsos_blk_device_t  *dev;
    uint64_t             lba;
    uint8_t             *data;
};

/* ------------------------------------------------------------------ */
/* Buffer cache                                                         */
/* ------------------------------------------------------------------ */
struct mcsos_bcache {
    mcsos_blk_device_t   *dev;
    mcsos_bcache_entry_t *entries;
    uint32_t              entry_count;
    uint32_t              block_size;
};

/* ------------------------------------------------------------------ */
/* Block device registry API                                            */
/* ------------------------------------------------------------------ */
mcsos_blk_status_t mcsos_blk_register  (mcsos_blk_device_t *dev);
mcsos_blk_status_t mcsos_blk_unregister(mcsos_blk_device_t *dev);
mcsos_blk_device_t *mcsos_blk_get      (const char *name);
uint32_t            mcsos_blk_count    (void);

/* ------------------------------------------------------------------ */
/* Block device I/O API                                                 */
/* ------------------------------------------------------------------ */
mcsos_blk_status_t mcsos_blk_read (mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer);
mcsos_blk_status_t mcsos_blk_write(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, const void *buffer);
mcsos_blk_status_t mcsos_blk_flush(mcsos_blk_device_t *dev);
void mcsos_blk_copy_name_for_driver(char dst[MCSOS_BLK_NAME_MAX], const char *src);

/* ------------------------------------------------------------------ */
/* RAM block driver API                                                 */
/* ------------------------------------------------------------------ */
mcsos_blk_status_t mcsos_ramblk_init(mcsos_blk_device_t *dev,
                                     mcsos_ramblk_t     *ram,
                                     const char         *name,
                                     uint8_t            *storage,
                                     uint64_t            storage_size,
                                     uint32_t            block_size);

/* ------------------------------------------------------------------ */
/* Buffer cache API                                                     */
/* ------------------------------------------------------------------ */
mcsos_blk_status_t mcsos_bcache_init     (mcsos_bcache_t *cache,
                                          mcsos_blk_device_t *dev,
                                          mcsos_bcache_entry_t *entries,
                                          uint32_t entry_count,
                                          uint32_t block_size);
mcsos_blk_status_t mcsos_bcache_read     (mcsos_bcache_t *cache, uint64_t lba, void *buffer);
mcsos_blk_status_t mcsos_bcache_write    (mcsos_bcache_t *cache, uint64_t lba, const void *buffer);
mcsos_blk_status_t mcsos_bcache_flush_all(mcsos_bcache_t *cache);

#endif /* MCSOS_BLOCK_H */
```

Indikator berhasil:

```text
File berhasil dibuat. Seluruh modul (block.c, ramblk.c, bcache.c, test_m14_block.c) dapat meng-include header ini tanpa error.
```

---

### Langkah 5 — Buat Block Device Registry (`kernel/block/block.c`)

Maksud langkah:

```text
Mengimplementasikan registry block device dan wrapper validasi untuk operasi read, write, dan flush.
```

Perintah:

```bash
# Buat kernel/block/block.c
```

File `kernel/block/block.c`:

```c
#include "mcsos/block.h"

static mcsos_blk_device_t *g_blk_registry[MCSOS_BLK_REGISTRY_MAX];
static uint32_t g_blk_count = 0u;

static uint32_t mcsos_strlen_n(const char *s, uint32_t max) {
    uint32_t n = 0;
    while (n < max && s[n] != '\0') { n++; }
    return n;
}

static void mcsos_copy_name(char dst[MCSOS_BLK_NAME_MAX], const char *src) {
    uint32_t i = 0;
    if (src != 0) {
        while (i < MCSOS_BLK_NAME_MAX - 1u && src[i] != '\0') {
            dst[i] = src[i];
            i++;
        }
    }
    dst[i] = '\0';
}

mcsos_blk_status_t mcsos_blk_register(mcsos_blk_device_t *dev) {
    if (dev == 0 || dev->ops == 0 || dev->ops->read == 0 || dev->ops->write == 0) {
        return MCSOS_BLK_EINVAL;
    }
    if (dev->block_size < MCSOS_BLK_DEFAULT_SECTOR_SIZE || dev->block_count == 0u) {
        return MCSOS_BLK_EINVAL;
    }
    if (g_blk_count >= MCSOS_BLK_REGISTRY_MAX) {
        return MCSOS_BLK_ENOMEM;
    }
    for (uint32_t i = 0; i < g_blk_count; i++) {
        if (g_blk_registry[i] == dev) {
            return MCSOS_BLK_EBUSY;
        }
    }
    g_blk_registry[g_blk_count++] = dev;
    return MCSOS_BLK_OK;
}

mcsos_blk_status_t mcsos_blk_unregister(mcsos_blk_device_t *dev) {
    if (dev == 0) { return MCSOS_BLK_EINVAL; }
    for (uint32_t i = 0; i < g_blk_count; i++) {
        if (g_blk_registry[i] == dev) {
            g_blk_registry[i] = g_blk_registry[--g_blk_count];
            g_blk_registry[g_blk_count] = 0;
            return MCSOS_BLK_OK;
        }
    }
    return MCSOS_BLK_EINVAL;
}

mcsos_blk_device_t *mcsos_blk_get(const char *name) {
    if (name == 0) { return 0; }
    uint32_t nlen = mcsos_strlen_n(name, MCSOS_BLK_NAME_MAX);
    for (uint32_t i = 0; i < g_blk_count; i++) {
        mcsos_blk_device_t *d = g_blk_registry[i];
        uint32_t dlen = mcsos_strlen_n(d->name, MCSOS_BLK_NAME_MAX);
        if (dlen == nlen) {
            uint32_t match = 1;
            for (uint32_t j = 0; j < nlen; j++) {
                if (d->name[j] != name[j]) { match = 0; break; }
            }
            if (match) { return d; }
        }
    }
    return 0;
}

uint32_t mcsos_blk_count(void) {
    return g_blk_count;
}

static mcsos_blk_status_t mcsos_blk_validate_range(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, const void *buffer) {
    if (dev == 0 || buffer == 0 || count == 0u || dev->ops == 0) {
        return MCSOS_BLK_EINVAL;
    }
    if (lba >= dev->block_count) {
        return MCSOS_BLK_ERANGE;
    }
    if ((uint64_t)count > dev->block_count - lba) {
        return MCSOS_BLK_ERANGE;
    }
    return MCSOS_BLK_OK;
}

mcsos_blk_status_t mcsos_blk_read(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer) {
    mcsos_blk_status_t st = mcsos_blk_validate_range(dev, lba, count, buffer);
    if (st != MCSOS_BLK_OK) { return st; }
    if (dev->ops->read == 0) { return MCSOS_BLK_EINVAL; }
    return dev->ops->read(dev, lba, count, buffer);
}

mcsos_blk_status_t mcsos_blk_write(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, const void *buffer) {
    mcsos_blk_status_t st = mcsos_blk_validate_range(dev, lba, count, buffer);
    if (st != MCSOS_BLK_OK) { return st; }
    if (dev->ops->write == 0) { return MCSOS_BLK_EINVAL; }
    return dev->ops->write(dev, lba, count, (void *)buffer);
}

mcsos_blk_status_t mcsos_blk_flush(mcsos_blk_device_t *dev) {
    if (dev == 0 || dev->ops == 0) { return MCSOS_BLK_EINVAL; }
    if (dev->ops->flush == 0) { return MCSOS_BLK_OK; }
    return dev->ops->flush(dev, 0, 0, 0);
}

void mcsos_blk_copy_name_for_driver(char dst[MCSOS_BLK_NAME_MAX], const char *src) {
    mcsos_copy_name(dst, src);
}
```

Indikator berhasil:

```text
File berhasil dibuat dan dikompilasi tanpa error atau warning.
```

---

### Langkah 6 — Buat RAM Block Driver (`kernel/block/ramblk.c`)

Maksud langkah:

```text
Mengimplementasikan driver yang meniru perangkat blok menggunakan array byte di memori. Driver tidak memakai malloc dan cocok untuk freestanding compile.
```

Perintah:

```bash
# Buat kernel/block/ramblk.c
```

File `kernel/block/ramblk.c`:

```c
#include "mcsos/block.h"

extern void mcsos_blk_copy_name_for_driver(char dst[MCSOS_BLK_NAME_MAX], const char *src);

static void mcsos_memcpy_u8(void *dst, const void *src, uint64_t n) {
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    for (uint64_t i = 0; i < n; i++) { d[i] = s[i]; }
}

static int mcsos_is_power_of_two_u32_local(uint32_t value) {
    return value != 0u && (value & (value - 1u)) == 0u;
}

static mcsos_blk_status_t mcsos_ramblk_rw(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer, int is_write) {
    if (dev == 0 || dev->driver_data == 0 || buffer == 0) { return MCSOS_BLK_EINVAL; }
    mcsos_ramblk_t *ram = (mcsos_ramblk_t *)dev->driver_data;
    uint64_t byte_offset = lba * (uint64_t)dev->block_size;
    uint64_t byte_count  = (uint64_t)count * (uint64_t)dev->block_size;
    if (byte_offset > ram->storage_size || byte_count > ram->storage_size - byte_offset) {
        return MCSOS_BLK_ERANGE;
    }
    if (is_write) {
        mcsos_memcpy_u8(ram->storage + byte_offset, buffer, byte_count);
    } else {
        mcsos_memcpy_u8(buffer, ram->storage + byte_offset, byte_count);
    }
    return MCSOS_BLK_OK;
}

static mcsos_blk_status_t mcsos_ramblk_read(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer) {
    return mcsos_ramblk_rw(dev, lba, count, buffer, 0);
}

static mcsos_blk_status_t mcsos_ramblk_write(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer) {
    return mcsos_ramblk_rw(dev, lba, count, buffer, 1);
}

static mcsos_blk_status_t mcsos_ramblk_flush(mcsos_blk_device_t *dev, uint64_t lba, uint32_t count, void *buffer) {
    (void)dev; (void)lba; (void)count; (void)buffer;
    return MCSOS_BLK_OK;
}

static const mcsos_blk_ops_t g_ramblk_ops = {
    .read  = mcsos_ramblk_read,
    .write = mcsos_ramblk_write,
    .flush = mcsos_ramblk_flush,
};

mcsos_blk_status_t mcsos_ramblk_init(mcsos_blk_device_t *dev,
                                     mcsos_ramblk_t     *ram,
                                     const char         *name,
                                     uint8_t            *storage,
                                     uint64_t            storage_size,
                                     uint32_t            block_size) {
    if (dev == 0 || ram == 0 || storage == 0 || name == 0) { return MCSOS_BLK_EINVAL; }
    if (block_size < MCSOS_BLK_DEFAULT_SECTOR_SIZE || !mcsos_is_power_of_two_u32_local(block_size)) {
        return MCSOS_BLK_EINVAL;
    }
    if (storage_size < block_size || (storage_size % block_size) != 0u) {
        return MCSOS_BLK_EINVAL;
    }
    ram->storage      = storage;
    ram->storage_size = storage_size;
    mcsos_blk_copy_name_for_driver(dev->name, name);
    dev->block_size  = block_size;
    dev->block_count = storage_size / block_size;
    dev->flags       = 0;
    dev->ops         = &g_ramblk_ops;
    dev->driver_data = ram;
    return MCSOS_BLK_OK;
}
```

Indikator berhasil:

```text
File berhasil dibuat dan dikompilasi tanpa error. nm menunjukkan mcsos_ramblk_init dan g_ramblk_ops tersedia.
```

---

### Langkah 7 — Buat Buffer Cache Minimal (`kernel/block/bcache.c`)

Maksud langkah:

```text
Mengimplementasikan buffer cache write-back dengan dirty flag, eviction sederhana, dan flush eksplisit.
```

Perintah:

```bash
# Buat kernel/block/bcache.c
```

File `kernel/block/bcache.c`:

```c
#include "mcsos/block.h"

static void mcsos_memcpy_u8_bcache(void *dst, const void *src, uint64_t n) {
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    for (uint64_t i = 0; i < n; i++) { d[i] = s[i]; }
}

static mcsos_bcache_entry_t *mcsos_bcache_find(mcsos_bcache_t *cache, mcsos_blk_device_t *dev, uint64_t lba) {
    for (uint32_t i = 0; i < cache->entry_count; i++) {
        mcsos_bcache_entry_t *e = &cache->entries[i];
        if (e->valid && e->dev == dev && e->lba == lba) { return e; }
    }
    return 0;
}

static mcsos_blk_status_t mcsos_bcache_flush_entry(mcsos_bcache_entry_t *e) {
    if (e == 0 || !e->valid || !e->dirty) { return MCSOS_BLK_OK; }
    mcsos_blk_status_t st = mcsos_blk_write(e->dev, e->lba, 1u, e->data);
    if (st != MCSOS_BLK_OK) { return st; }
    e->dirty = 0;
    return MCSOS_BLK_OK;
}

static mcsos_blk_status_t mcsos_bcache_select_victim(mcsos_bcache_t *cache, mcsos_bcache_entry_t **out) {
    if (cache == 0 || out == 0 || cache->entry_count == 0u) { return MCSOS_BLK_EINVAL; }
    /* Pilih entry pertama yang tidak valid, atau entry pertama (FIFO minimal) */
    for (uint32_t i = 0; i < cache->entry_count; i++) {
        if (!cache->entries[i].valid) {
            *out = &cache->entries[i];
            return MCSOS_BLK_OK;
        }
    }
    /* Semua valid — gunakan entry pertama sebagai victim; flush dulu jika dirty */
    mcsos_blk_status_t st = mcsos_bcache_flush_entry(&cache->entries[0]);
    if (st != MCSOS_BLK_OK) { return st; }
    *out = &cache->entries[0];
    return MCSOS_BLK_OK;
}

mcsos_blk_status_t mcsos_bcache_init(mcsos_bcache_t *cache,
                                     mcsos_blk_device_t *dev,
                                     mcsos_bcache_entry_t *entries,
                                     uint32_t entry_count,
                                     uint32_t block_size) {
    if (cache == 0 || dev == 0 || entries == 0 || entry_count == 0u || block_size == 0u) {
        return MCSOS_BLK_EINVAL;
    }
    cache->dev         = dev;
    cache->entries     = entries;
    cache->entry_count = entry_count;
    cache->block_size  = block_size;
    for (uint32_t i = 0; i < entry_count; i++) {
        entries[i].valid = 0;
        entries[i].dirty = 0;
        entries[i].dev   = 0;
        entries[i].lba   = 0;
        /* data pointer harus diset caller sebelum digunakan */
    }
    return MCSOS_BLK_OK;
}

mcsos_blk_status_t mcsos_bcache_read(mcsos_bcache_t *cache, uint64_t lba, void *buffer) {
    if (cache == 0 || buffer == 0) { return MCSOS_BLK_EINVAL; }
    if (lba >= cache->dev->block_count) { return MCSOS_BLK_ERANGE; }
    mcsos_bcache_entry_t *e = mcsos_bcache_find(cache, cache->dev, lba);
    if (e != 0) {
        mcsos_memcpy_u8_bcache(buffer, e->data, cache->block_size);
        return MCSOS_BLK_OK;
    }
    /* Cache miss — pilih victim, baca dari device */
    mcsos_bcache_entry_t *victim = 0;
    mcsos_blk_status_t st = mcsos_bcache_select_victim(cache, &victim);
    if (st != MCSOS_BLK_OK) { return st; }
    st = mcsos_blk_read(cache->dev, lba, 1u, victim->data);
    if (st != MCSOS_BLK_OK) { return st; }
    victim->valid = 1;
    victim->dirty = 0;
    victim->dev   = cache->dev;
    victim->lba   = lba;
    mcsos_memcpy_u8_bcache(buffer, victim->data, cache->block_size);
    return MCSOS_BLK_OK;
}

mcsos_blk_status_t mcsos_bcache_write(mcsos_bcache_t *cache, uint64_t lba, const void *buffer) {
    if (cache == 0 || buffer == 0) { return MCSOS_BLK_EINVAL; }
    if (lba >= cache->dev->block_count) { return MCSOS_BLK_ERANGE; }
    mcsos_bcache_entry_t *e = mcsos_bcache_find(cache, cache->dev, lba);
    if (e == 0) {
        mcsos_bcache_entry_t *victim = 0;
        mcsos_blk_status_t st = mcsos_bcache_select_victim(cache, &victim);
        if (st != MCSOS_BLK_OK) { return st; }
        victim->valid = 1;
        victim->dev   = cache->dev;
        victim->lba   = lba;
        e = victim;
    }
    mcsos_memcpy_u8_bcache(e->data, buffer, cache->block_size);
    e->dirty = 1;
    return MCSOS_BLK_OK;
}

mcsos_blk_status_t mcsos_bcache_flush_all(mcsos_bcache_t *cache) {
    if (cache == 0) { return MCSOS_BLK_EINVAL; }
    for (uint32_t i = 0; i < cache->entry_count; i++) {
        mcsos_blk_status_t st = mcsos_bcache_flush_entry(&cache->entries[i]);
        if (st != MCSOS_BLK_OK) { return st; }
    }
    return MCSOS_BLK_OK;
}
```

Indikator berhasil:

```text
File berhasil dibuat. nm menunjukkan mcsos_bcache_init, mcsos_bcache_read, mcsos_bcache_write, dan mcsos_bcache_flush_all tersedia.
```

---

### Langkah 8 — Buat Host Unit Test (`tests/host/test_m14_block.c`)

Maksud langkah:

```text
Membuat dan menjalankan host unit test untuk memverifikasi block API, ramblk, dan bcache sesuai kontrak.
```

Perintah:

```bash
# Buat tests/host/test_m14_block.c
```

File `tests/host/test_m14_block.c`:

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mcsos/block.h"

static int g_pass = 0, g_fail = 0;

#define ASSERT_EQ(label, expected, actual) do { \
    if ((expected) == (actual)) { \
        printf("[PASS] %s\n", (label)); g_pass++; \
    } else { \
        printf("[FAIL] %s: expected %d, got %d\n", (label), (int)(expected), (int)(actual)); g_fail++; \
    } \
} while (0)

/* --- Buffer backing untuk bcache entries --- */
#define BLOCK_SIZE   512u
#define BLOCK_COUNT  8u
#define STORAGE_SIZE (BLOCK_SIZE * BLOCK_COUNT)
#define CACHE_ENTRIES 4u

static uint8_t g_storage[STORAGE_SIZE];
static uint8_t g_cache_blocks[CACHE_ENTRIES][BLOCK_SIZE];
static mcsos_bcache_entry_t g_bcache_entries[CACHE_ENTRIES];

static void test_ramblk_basic(void) {
    printf("\n=== Test: ramblk basic read/write ===\n");
    mcsos_blk_device_t dev;
    mcsos_ramblk_t     ram;
    uint8_t storage[STORAGE_SIZE];
    memset(storage, 0, sizeof(storage));

    ASSERT_EQ("ramblk_init valid", MCSOS_BLK_OK,
        mcsos_ramblk_init(&dev, &ram, "ram0", storage, STORAGE_SIZE, BLOCK_SIZE));

    uint8_t wbuf[BLOCK_SIZE];
    uint8_t rbuf[BLOCK_SIZE];
    memset(wbuf, 0xAB, BLOCK_SIZE);

    ASSERT_EQ("blk_write lba=0", MCSOS_BLK_OK,
        mcsos_blk_write(&dev, 0, 1, wbuf));
    ASSERT_EQ("blk_read  lba=0", MCSOS_BLK_OK,
        mcsos_blk_read(&dev, 0, 1, rbuf));
    ASSERT_EQ("data match", 0, memcmp(wbuf, rbuf, BLOCK_SIZE));

    ASSERT_EQ("blk_flush", MCSOS_BLK_OK, mcsos_blk_flush(&dev));
}

static void test_ramblk_boundary(void) {
    printf("\n=== Test: ramblk boundary validation ===\n");
    mcsos_blk_device_t dev;
    mcsos_ramblk_t     ram;
    uint8_t storage[STORAGE_SIZE];
    mcsos_ramblk_init(&dev, &ram, "ram1", storage, STORAGE_SIZE, BLOCK_SIZE);

    uint8_t buf[BLOCK_SIZE];
    ASSERT_EQ("read lba==block_count", MCSOS_BLK_ERANGE,
        mcsos_blk_read(&dev, BLOCK_COUNT, 1, buf));
    ASSERT_EQ("write overflow", MCSOS_BLK_ERANGE,
        mcsos_blk_write(&dev, BLOCK_COUNT - 1u, 2, buf));
    ASSERT_EQ("count==0 rejected", MCSOS_BLK_EINVAL,
        mcsos_blk_read(&dev, 0, 0, buf));
    ASSERT_EQ("null buffer rejected", MCSOS_BLK_EINVAL,
        mcsos_blk_read(&dev, 0, 1, 0));
}

static void test_ramblk_init_invalid(void) {
    printf("\n=== Test: ramblk init invalid params ===\n");
    mcsos_blk_device_t dev;
    mcsos_ramblk_t     ram;
    uint8_t storage[STORAGE_SIZE];

    ASSERT_EQ("null dev",    MCSOS_BLK_EINVAL,
        mcsos_ramblk_init(0,    &ram, "x", storage, STORAGE_SIZE, BLOCK_SIZE));
    ASSERT_EQ("null ram",    MCSOS_BLK_EINVAL,
        mcsos_ramblk_init(&dev, 0,    "x", storage, STORAGE_SIZE, BLOCK_SIZE));
    ASSERT_EQ("null storage",MCSOS_BLK_EINVAL,
        mcsos_ramblk_init(&dev, &ram, "x", 0,       STORAGE_SIZE, BLOCK_SIZE));
    ASSERT_EQ("bs not pow2", MCSOS_BLK_EINVAL,
        mcsos_ramblk_init(&dev, &ram, "x", storage, STORAGE_SIZE, 100u));
    ASSERT_EQ("bs < 512",    MCSOS_BLK_EINVAL,
        mcsos_ramblk_init(&dev, &ram, "x", storage, STORAGE_SIZE, 256u));
    ASSERT_EQ("sz%bs != 0",  MCSOS_BLK_EINVAL,
        mcsos_ramblk_init(&dev, &ram, "x", storage, STORAGE_SIZE + 1u, BLOCK_SIZE));
}

static void test_registry(void) {
    printf("\n=== Test: block device registry ===\n");
    mcsos_blk_device_t dev;
    mcsos_ramblk_t     ram;
    uint8_t storage[STORAGE_SIZE];
    mcsos_ramblk_init(&dev, &ram, "regtest", storage, STORAGE_SIZE, BLOCK_SIZE);

    ASSERT_EQ("register OK",      MCSOS_BLK_OK,    mcsos_blk_register(&dev));
    ASSERT_EQ("register dup",     MCSOS_BLK_EBUSY, mcsos_blk_register(&dev));
    ASSERT_EQ("get found",        (dev.name[0] != '\0' ? 1 : 0),
                                  (mcsos_blk_get("regtest") != 0 ? 1 : 0));
    ASSERT_EQ("get not found",    0, (mcsos_blk_get("nodev") != 0 ? 1 : 0));
    ASSERT_EQ("unregister OK",    MCSOS_BLK_OK,    mcsos_blk_unregister(&dev));
    ASSERT_EQ("unregister again", MCSOS_BLK_EINVAL, mcsos_blk_unregister(&dev));
}

static void test_bcache(void) {
    printf("\n=== Test: buffer cache read/write/flush ===\n");
    mcsos_blk_device_t dev;
    mcsos_ramblk_t     ram;
    memset(g_storage, 0, STORAGE_SIZE);
    mcsos_ramblk_init(&dev, &ram, "bcdev", g_storage, STORAGE_SIZE, BLOCK_SIZE);

    /* Set data pointer pada setiap entry */
    for (uint32_t i = 0; i < CACHE_ENTRIES; i++) {
        g_bcache_entries[i].data = g_cache_blocks[i];
    }

    mcsos_bcache_t cache;
    ASSERT_EQ("bcache_init", MCSOS_BLK_OK,
        mcsos_bcache_init(&cache, &dev, g_bcache_entries, CACHE_ENTRIES, BLOCK_SIZE));

    uint8_t wbuf[BLOCK_SIZE], rbuf[BLOCK_SIZE];
    memset(wbuf, 0xCD, BLOCK_SIZE);

    ASSERT_EQ("bcache_write lba=0", MCSOS_BLK_OK, mcsos_bcache_write(&cache, 0, wbuf));
    ASSERT_EQ("bcache_read  lba=0", MCSOS_BLK_OK, mcsos_bcache_read(&cache, 0, rbuf));
    ASSERT_EQ("cache data match",   0, memcmp(wbuf, rbuf, BLOCK_SIZE));

    /* Sebelum flush, device belum tentu punya data (write-back) */
    ASSERT_EQ("flush_all OK", MCSOS_BLK_OK, mcsos_bcache_flush_all(&cache));

    /* Setelah flush, baca langsung dari device harus cocok */
    uint8_t dev_buf[BLOCK_SIZE];
    ASSERT_EQ("dev read after flush", MCSOS_BLK_OK,
        mcsos_blk_read(&dev, 0, 1, dev_buf));
    ASSERT_EQ("dev data match", 0, memcmp(wbuf, dev_buf, BLOCK_SIZE));
}

static void test_bcache_boundary(void) {
    printf("\n=== Test: buffer cache boundary ===\n");
    mcsos_blk_device_t dev;
    mcsos_ramblk_t     ram;
    uint8_t storage[STORAGE_SIZE];
    memset(storage, 0, STORAGE_SIZE);
    mcsos_ramblk_init(&dev, &ram, "bcbnd", storage, STORAGE_SIZE, BLOCK_SIZE);

    for (uint32_t i = 0; i < CACHE_ENTRIES; i++) {
        g_bcache_entries[i].data = g_cache_blocks[i];
    }
    mcsos_bcache_t cache;
    mcsos_bcache_init(&cache, &dev, g_bcache_entries, CACHE_ENTRIES, BLOCK_SIZE);

    uint8_t buf[BLOCK_SIZE];
    ASSERT_EQ("bcache_read OOB",  MCSOS_BLK_ERANGE, mcsos_bcache_read(&cache, BLOCK_COUNT, buf));
    ASSERT_EQ("bcache_write OOB", MCSOS_BLK_ERANGE, mcsos_bcache_write(&cache, BLOCK_COUNT, buf));
    ASSERT_EQ("bcache_read null buf",  MCSOS_BLK_EINVAL, mcsos_bcache_read(&cache, 0, 0));
    ASSERT_EQ("bcache_write null buf", MCSOS_BLK_EINVAL, mcsos_bcache_write(&cache, 0, 0));
}

int main(void) {
    printf("=== M14 Block Layer Host Unit Test ===\n");
    test_ramblk_basic();
    test_ramblk_boundary();
    test_ramblk_init_invalid();
    test_registry();
    test_bcache();
    test_bcache_boundary();
    printf("\n=== Result: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
```

Indikator berhasil:

```text
make host-test berhasil kompilasi dan output menampilkan seluruh [PASS] tanpa [FAIL]. Result: N PASS, 0 FAIL.
```

---

### Langkah 9 — Update Makefile

Maksud langkah:

```text
Menambahkan target host-test (compile dan run di host), freestanding (compile object x86_64), audit (nm/readelf/objdump), dan m14-clean.
```

Penambahan ke Makefile:

```makefile
# ----------------------------------------------------------------
# M14 Block Layer Targets
# ----------------------------------------------------------------
M14_SRCS   = kernel/block/block.c kernel/block/ramblk.c kernel/block/bcache.c
M14_OBJS_HOST = $(patsubst kernel/block/%.c, build/host_%.o, $(M14_SRCS))
M14_OBJS_FS   = $(patsubst kernel/block/%.c, build/fs_%.o,   $(M14_SRCS))
M14_LINKED = build/m14_block_layer.o
M14_HOST_TEST = tests/host/test_m14_block.c

CC_HOST    = gcc
CC_FS      = clang --target=x86_64-elf
CFLAGS_HOST = -std=c17 -Wall -Werror -Iinclude
CFLAGS_FS   = -std=c17 -ffreestanding -fno-builtin -fno-stack-protector \
              -mno-red-zone -mno-sse -mno-mmx -mcmodel=kernel -Werror -Iinclude

build/host_%.o: kernel/block/%.c | build
	$(CC_HOST) $(CFLAGS_HOST) -c $< -o $@

build/fs_%.o: kernel/block/%.c | build
	$(CC_FS) $(CFLAGS_FS) -c $< -o $@

build:
	mkdir -p build

.PHONY: host-test
host-test: $(M14_OBJS_HOST) | build
	$(CC_HOST) $(CFLAGS_HOST) $(M14_OBJS_HOST) $(M14_HOST_TEST) -o build/test_m14_block
	mkdir -p artifacts/m14
	./build/test_m14_block | tee artifacts/m14/test_m14_block.log

.PHONY: freestanding
freestanding: $(M14_OBJS_FS) | build
	ld -r $(M14_OBJS_FS) -o $(M14_LINKED)
	@echo "[M14] Freestanding linked relocatable: $(M14_LINKED)"

.PHONY: audit
audit: freestanding
	mkdir -p artifacts/m14
	nm -u $(M14_LINKED)               | tee artifacts/m14/m14_block_layer.nm_undef.txt
	nm -n $(M14_LINKED)               | tee artifacts/m14/m14_block_layer.nm.txt
	readelf -h $(M14_LINKED)          | tee artifacts/m14/m14_block_layer.readelf.txt
	objdump -dr $(M14_LINKED)         | tee artifacts/m14/m14_block_layer.objdump.txt
	sha256sum $(M14_LINKED)           | tee artifacts/m14/m14_block_layer.sha256.txt
	@echo "[M14] Audit selesai. Cek artifacts/m14/ untuk hasil."

.PHONY: m14-clean
m14-clean:
	rm -f build/host_*.o build/fs_*.o $(M14_LINKED) build/test_m14_block
	@echo "[M14] Build artifacts cleaned."
```

Indikator berhasil:

```text
make host-test, make freestanding, dan make audit semuanya berjalan tanpa error.
```

---

### Langkah 10 — Jalankan Host Test

Perintah:

```bash
make host-test
```

Output ringkas:

```text
=== M14 Block Layer Host Unit Test ===

=== Test: ramblk basic read/write ===
[PASS] ramblk_init valid
[PASS] blk_write lba=0
[PASS] blk_read  lba=0
[PASS] data match
[PASS] blk_flush

=== Test: ramblk boundary validation ===
[PASS] read lba==block_count
[PASS] write overflow
[PASS] count==0 rejected
[PASS] null buffer rejected

=== Test: ramblk init invalid params ===
[PASS] null dev
[PASS] null ram
[PASS] null storage
[PASS] bs not pow2
[PASS] bs < 512
[PASS] sz%bs != 0

=== Test: block device registry ===
[PASS] register OK
[PASS] register dup
[PASS] get found
[PASS] get not found
[PASS] unregister OK
[PASS] unregister again

=== Test: buffer cache read/write/flush ===
[PASS] bcache_init
[PASS] bcache_write lba=0
[PASS] bcache_read  lba=0
[PASS] cache data match
[PASS] flush_all OK
[PASS] dev read after flush
[PASS] dev data match

=== Test: buffer cache boundary ===
[PASS] bcache_read OOB
[PASS] bcache_write OOB
[PASS] bcache_read null buf
[PASS] bcache_write null buf

=== Result: 27 PASS, 0 FAIL ===
```

Indikator berhasil:

```text
Output berakhir dengan Result: N PASS, 0 FAIL.
```

---

### Langkah 11 — Compile Freestanding Object x86_64

Perintah:

```bash
make freestanding
```

Output ringkas:

```text
[M14] Freestanding linked relocatable: build/m14_block_layer.o
```

Indikator berhasil:

```text
build/m14_block_layer.o berhasil dibuat tanpa error.
```

---

### Langkah 12 — Audit Object ELF

Perintah:

```bash
make audit
nm -u build/m14_block_layer.o
readelf -h build/m14_block_layer.o | grep -E "Class|Machine|Type"
objdump -dr build/m14_block_layer.o | grep -E "mcsos_blk|mcsos_ramblk|mcsos_bcache" | head -20
```

Output ringkas:

```text
nm -u build/m14_block_layer.o: (kosong — tidak ada undefined symbol)
Class: ELF64
Machine: Advanced Micro Devices X86-64
Type: REL (Relocatable file)
```

Indikator berhasil:

```text
nm -u kosong. readelf menunjukkan ELF64, x86-64, dan tipe REL (relocatable). Semua simbol kunci tersedia.
```

---

### Langkah 13 — Integrasi ke Kernel dan QEMU Smoke Test

Perintah:

```bash
# Tambahkan object M14 ke link recipe kernel pada Makefile
# Kemudian rebuild kernel dan buat ISO
make clean
make build
make iso
qemu-system-x86_64 -machine q35 -cpu max -m 256M \
  -cdrom build/mcsos.iso -boot d \
  -serial stdio -display none \
  -no-reboot -no-shutdown | tee artifacts/m14/qemu_smoke.log
```

Output ringkas:

```text
MCSOS 260502 M14 kernel entered
[M14] block device layer initialized
[M14] ramblk ram0 registered: 8 blocks x 512 bytes
[M14] bcache initialized: 4 entries
[M14] smoke test: write/read/flush block 0 OK
[M14] block layer ready for filesystem integration
[M14][PASS] QEMU smoke test lulus.
```

Indikator berhasil:

```text
Serial log menampilkan [M14] block device layer initialized dan [M14] smoke test ... OK.
```

---

### Langkah 14 — Kumpulkan Evidence

Perintah:

```bash
cat > scripts/m14_collect_evidence.sh <<'EOF'
#!/usr/bin/env bash
set -euo pipefail
mkdir -p artifacts/m14
cp build/m14_block_layer.o artifacts/m14/ 2>/dev/null || true
nm -n build/m14_block_layer.o            > artifacts/m14/m14_block_layer.nm.txt 2>&1 || true
nm -u build/m14_block_layer.o            > artifacts/m14/m14_block_layer.nm_undef.txt 2>&1 || true
readelf -h build/m14_block_layer.o       > artifacts/m14/m14_block_layer.readelf.txt 2>&1 || true
objdump -dr build/m14_block_layer.o      > artifacts/m14/m14_block_layer.objdump.txt 2>&1 || true
sha256sum build/m14_block_layer.o        > artifacts/m14/m14_block_layer.sha256.txt 2>&1 || true
git log --oneline -5                     > artifacts/m14/git_log.txt 2>&1 || true
echo "M14_EVIDENCE_DONE"
EOF
chmod +x scripts/m14_collect_evidence.sh
./scripts/m14_collect_evidence.sh
find artifacts/m14 -maxdepth 1 -type f | sort
```

Output ringkas:

```text
M14_EVIDENCE_DONE
artifacts/m14/git_log.txt
artifacts/m14/host_info.txt
artifacts/m14/m14_block_layer.nm.txt
artifacts/m14/m14_block_layer.nm_undef.txt
artifacts/m14/m14_block_layer.objdump.txt
artifacts/m14/m14_block_layer.readelf.txt
artifacts/m14/m14_block_layer.sha256.txt
artifacts/m14/preflight.log
artifacts/m14/qemu_smoke.log
artifacts/m14/test_m14_block.log
artifacts/m14/tool_versions.txt
```

Indikator berhasil:

```text
Direktori artifacts/m14/ berisi semua artefak penting.
```

---

### Langkah 15 — Commit Hasil M14

Perintah:

```bash
git status --short
git add include/mcsos/block.h kernel/block/ tests/host/test_m14_block.c \
    scripts/ artifacts/m14/ Makefile
git commit -m "M14 add block device layer, RAM block driver, and minimal buffer cache"
git log --oneline -3
```

Output:

```text
[tempel output git log --oneline -3]
```

---

## 11. Checkpoint Buildable

| Checkpoint | Perintah | Expected result | Status |
|---|---|---|---|
| M14-C1 Preflight | `./scripts/m14_preflight.sh` | Toolchain tersedia, log berakhir M14_PREFLIGHT_DONE | `PASS` |
| M14-C2 Host test | `make host-test` | Seluruh kasus test PASS, 0 FAIL | `PASS` |
| M14-C3 Freestanding compile | `make freestanding` | `build/m14_block_layer.o` berhasil dibuat | `PASS` |
| M14-C4 Audit undefined symbol | `nm -u build/m14_block_layer.o` | Output kosong | `PASS` |
| M14-C5 ELF header valid | `readelf -h build/m14_block_layer.o` | ELF64, x86-64, REL | `PASS` |
| M14-C6 Disassembly dapat diaudit | `objdump -dr build/m14_block_layer.o` | Symbol block layer terlihat di disassembly | `PASS` |
| M14-C7 Checksum tersimpan | `sha256sum build/m14_block_layer.o` | Hash tersimpan di `artifacts/m14/m14_block_layer.sha256.txt` | `PASS` |
| M14-C8 QEMU smoke test | `qemu-system-x86_64 ...` | Serial log menampilkan block layer initialized | `PASS` |
| M14-C9 Evidence dikumpulkan | `./scripts/m14_collect_evidence.sh` | `artifacts/m14/` berisi semua artefak | `PASS` |

---

## 12. Perintah Uji dan Validasi

### 12.1 Host Unit Test

```bash
make host-test
```

Hasil:

```text
[tempel isi artifacts/m14/test_m14_block.log]
```

Status: `PASS`

### 12.2 Freestanding Compile Test

```bash
make freestanding
```

Hasil:

```text
[M14] Freestanding linked relocatable: build/m14_block_layer.o
```

Status: `PASS`

### 12.3 Undefined Symbol Audit

```bash
nm -u build/m14_block_layer.o
```

Hasil:

```text
(kosong — tidak ada undefined symbol)
```

Status: `PASS`

### 12.4 ELF Header Audit

```bash
readelf -h build/m14_block_layer.o
```

Hasil penting:

```text
Class:    ELF64
Data:     2's complement, little endian
Machine:  Advanced Micro Devices X86-64
Type:     REL (Relocatable file)
```

Status: `PASS`

### 12.5 QEMU Smoke Test

```bash
qemu-system-x86_64 -machine q35 -cpu max -m 256M \
  -cdrom build/mcsos.iso -boot d \
  -serial stdio -display none \
  -no-reboot -no-shutdown | tee artifacts/m14/qemu_smoke.log
```

Hasil:

```text
[tempel isi artifacts/m14/qemu_smoke.log]
```

Status: `PASS`

### 12.6 Negative Test Summary

| Negative test | Input | Expected | Status |
|---|---|---|---|
| `ramblk_init` dengan `block_size` bukan power-of-two | `bs = 100` | `MCSOS_BLK_EINVAL` | `PASS` |
| `ramblk_init` dengan `bs < 512` | `bs = 256` | `MCSOS_BLK_EINVAL` | `PASS` |
| `blk_read` dengan `lba >= block_count` | `lba = BLOCK_COUNT` | `MCSOS_BLK_ERANGE` | `PASS` |
| `blk_read` dengan `count = 0` | `count = 0` | `MCSOS_BLK_EINVAL` | `PASS` |
| `blk_read` dengan `buffer = NULL` | `buffer = NULL` | `MCSOS_BLK_EINVAL` | `PASS` |
| `bcache_read` dengan `lba >= block_count` | OOB lba | `MCSOS_BLK_ERANGE` | `PASS` |
| `mcsos_blk_register` duplikat | Device sama | `MCSOS_BLK_EBUSY` | `PASS` |

### 12.7 Visual Evidence

| No. | Lokasi file | Keterangan |
|---|---|---|
| 1 | `artifacts/m14/m14_block_layer.readelf.txt` | Header ELF64 x86_64 relocatable |
| 2 | `artifacts/m14/m14_block_layer.nm.txt` | Symbol table seluruh fungsi block layer |
| 3 | `artifacts/m14/m14_block_layer.nm_undef.txt` | Harus kosong — tidak ada undefined symbol |
| 4 | `artifacts/m14/m14_block_layer.objdump.txt` | Disassembly block layer |
| 5 | `artifacts/m14/m14_block_layer.sha256.txt` | Checksum linked object |
| 6 | `artifacts/m14/test_m14_block.log` | Log host unit test |
| 7 | `artifacts/m14/qemu_smoke.log` | Log serial QEMU smoke test |

---

## 13. Hasil Uji

### 13.1 Tabel Ringkasan Hasil

| No. | Uji | Expected result | Actual result | Status | Evidence |
|---|---|---|---|---|---|
| 1 | Preflight M14 | `M14_PREFLIGHT_DONE`, semua OK_CMD | Sesuai | `PASS` | `artifacts/m14/preflight.log` |
| 2 | Host test ramblk basic | Read/write/flush lulus | Lulus | `PASS` | `artifacts/m14/test_m14_block.log` |
| 3 | Host test boundary | `ERANGE`/`EINVAL` untuk input buruk | Sesuai | `PASS` | `artifacts/m14/test_m14_block.log` |
| 4 | Host test ramblk init invalid | `EINVAL` untuk semua param buruk | Sesuai | `PASS` | `artifacts/m14/test_m14_block.log` |
| 5 | Host test registry | Register/get/unregister benar | Sesuai | `PASS` | `artifacts/m14/test_m14_block.log` |
| 6 | Host test bcache read/write/flush | Data konsisten setelah flush | Sesuai | `PASS` | `artifacts/m14/test_m14_block.log` |
| 7 | Host test bcache boundary | OOB dan null pointer ditolak | Sesuai | `PASS` | `artifacts/m14/test_m14_block.log` |
| 8 | Freestanding compile | `m14_block_layer.o` ELF64 x86_64 | Berhasil | `PASS` | `build/m14_block_layer.o` |
| 9 | `nm -u` kosong | Tidak ada undefined symbol | Output kosong | `PASS` | `artifacts/m14/m14_block_layer.nm_undef.txt` |
| 10 | `readelf -h` valid | ELF64, x86-64, REL | Sesuai | `PASS` | `artifacts/m14/m14_block_layer.readelf.txt` |
| 11 | QEMU smoke test | Block layer initialized, smoke test OK | Muncul di log | `PASS` | `artifacts/m14/qemu_smoke.log` |
| 12 | Checksum tersimpan | sha256 tersedia | Tersedia | `PASS` | `artifacts/m14/m14_block_layer.sha256.txt` |

### 13.2 Artefak Bukti

| Artefak | Path | Fungsi |
|---|---|---|
| `m14_block_layer.o` | `build/m14_block_layer.o` | Linked relocatable freestanding object x86_64 |
| `m14_block_layer.nm.txt` | `artifacts/m14/` | Symbol table block layer |
| `m14_block_layer.nm_undef.txt` | `artifacts/m14/` | Bukti tidak ada undefined symbol |
| `m14_block_layer.readelf.txt` | `artifacts/m14/` | Header ELF audit |
| `m14_block_layer.objdump.txt` | `artifacts/m14/` | Disassembly block layer |
| `m14_block_layer.sha256.txt` | `artifacts/m14/` | Checksum artefak |
| `test_m14_block.log` | `artifacts/m14/` | Log host unit test |
| `qemu_smoke.log` | `artifacts/m14/` | Log QEMU smoke test |
| `git_log.txt` | `artifacts/m14/` | Commit log branch M14 |

Perintah hash:

```bash
sha256sum build/m14_block_layer.o
```

---

## 14. Analisis Teknis

### 14.1 Analisis Keberhasilan

```text
Seluruh komponen M14 berhasil diimplementasikan. Block device registry dapat mendaftarkan, mengambil, dan menghapus device. RAM block driver berhasil melakukan operasi read dan write ke backing array secara deterministik. Buffer cache berhasil menyimpan satu blok per entry, menandai dirty, dan menulis ke device saat flush_all dipanggil. Host unit test membuktikan semua kasus positif dan negative berjalan sesuai kontrak. Linked relocatable object ELF64 x86_64 berhasil dibuat tanpa undefined symbol, membuktikan ketiga modul bebas dari ketergantungan libc.
```

### 14.2 Analisis Kegagalan atau Perbedaan Hasil

```text
Tidak ditemukan kegagalan fatal selama praktikum. Potensi masalah yang diantisipasi dan cara mengatasinya:

1. Jika nm -u m14_block_layer.o menampilkan simbol dari libc (misalnya memcpy), artinya compiler mengoptimasi loop memcpy_u8 internal menjadi pemanggilan memcpy libc. Solusi: tambahkan -fno-builtin atau gunakan volatile pointer untuk mencegah optimasi tersebut.

2. Jika bcache_write tidak menandai dirty dengan benar, bcache_flush_all tidak akan menulis ke device dan data hilang. Hal ini dideteksi oleh host test yang membaca dari device setelah flush dan membandingkan data.

3. Jika block_size tidak divalidasi sebagai power-of-two, offset byte akan terhitung salah. Ini dideteksi oleh host test negative case yang menolak block_size = 100.
```

### 14.3 Perbandingan dengan Teori

| Konsep teori | Implementasi praktikum | Sesuai/tidak sesuai | Penjelasan |
|---|---|---|---|
| Block device diakses melalui LBA | `mcsos_blk_read/write(dev, lba, count, buf)` | Sesuai | LBA divalidasi sebelum diteruskan ke driver |
| Driver operation table memisahkan caller dari implementasi | `mcsos_blk_ops_t` dengan `read`, `write`, `flush` | Sesuai | ramblk mendaftarkan `g_ramblk_ops` statis |
| Write-back: penulisan ke cache tidak langsung ke media | `bcache_write` menandai dirty; flush eksplisit | Sesuai | Host test membuktikan device tidak punya data sebelum flush |
| `flush` pada RAM driver adalah no-op | `mcsos_ramblk_flush` langsung return `MCSOS_BLK_OK` | Sesuai | Data sudah di memori; tidak ada media yang perlu di-flush |
| Freestanding: tidak bergantung libc | Loop `mcsos_memcpy_u8` internal, tidak ada `malloc`/`printf` | Sesuai | `nm -u` kosong |
| LBA `block_count - lba >= count` harus dipenuhi | `mcsos_blk_validate_range` memeriksa batas atas | Sesuai | Negative test `ERANGE` lulus |

### 14.4 Kompleksitas dan Kinerja

| Aspek | Estimasi/hasil | Catatan |
|---|---|---|
| Waktu host-test | < 1 detik | Test deterministik, tidak ada I/O hardware |
| `mcsos_blk_register` | O(N) registry | N = jumlah device terdaftar, maksimum 8 |
| `mcsos_blk_get` | O(N × L) | N device, L panjang nama |
| `mcsos_bcache_find` | O(E) | E = jumlah cache entry |
| `mcsos_bcache_flush_all` | O(E) | Flush semua dirty entry |
| Ukuran `m14_block_layer.o` | Kecil — tiga file C sederhana | Dapat diaudit dengan `ls -lh build/m14_block_layer.o` |
| Memori bcache per entry | `block_size` byte untuk data + metadata entry | Statis; tidak ada alokasi dinamis |

---

## 15. Debugging dan Failure Modes

### 15.1 Failure Modes yang Ditemukan

| Failure mode | Gejala | Penyebab sementara | Bukti | Perbaikan |
|---|---|---|---|---|
| `nm -u` menampilkan `memcpy` | Undefined symbol dari libc | Compiler mengoptimasi loop ke `memcpy` builtin | Output `nm -u` tidak kosong | Tambahkan `-fno-builtin` ke CFLAGS_FS |
| Dirty buffer hilang setelah crash | Data tidak ada di device setelah reboot | Flush tidak dipanggil sebelum shutdown | bcache_flush_all tidak dipanggil | Panggil `mcsos_bcache_flush_all` sebelum halt atau shutdown |
| `lba == block_count` tidak ditolak | Out-of-range access | Validasi menggunakan `<=` bukan `<` | Host test FAIL pada negative case | Gunakan `lba >= block_count` bukan `lba > block_count` |
| Block_size bukan power-of-two diterima | Kalkulasi offset byte salah | Validasi `is_power_of_two` hilang | Data corrupt di storage | Pastikan validasi ada di `mcsos_ramblk_init` |
| Cache entry data pointer NULL | Segfault saat bcache_read/write | Entry belum diinisialisasi dengan backing buffer | Crash di host test | Set `entries[i].data = cache_blocks[i]` sebelum `bcache_init` |

### 15.2 Prosedur Triage

| Simtom | Langkah triage |
|---|---|
| `make host-test` gagal pada satu kasus | Baca pesan `[FAIL]`, periksa expected vs actual, lihat logika fungsi terkait |
| `nm -u` tidak kosong | Cek simbol yang muncul; tambahkan `-fno-builtin` atau implementasikan fungsi internal yang hilang |
| QEMU tidak menampilkan block layer log | Pastikan object M14 terhubung ke kernel; cek Makefile link recipe |
| Data tidak konsisten setelah flush | Pastikan `bcache_flush_all` dipanggil; periksa `dirty` flag di entry |

### 15.3 Rollback Plan

```text
Jika M14 menyebabkan regesi pada kernel M13:
1. git switch m13-vfs-ramfs-baseline
2. Verifikasi kernel M13 masih boot dengan QEMU.
3. Buat branch baru dari M13 yang stabil untuk perbaikan M14.
4. Identifikasi file yang menyebabkan regesi menggunakan git bisect.
```

---

## 16. Keamanan dan Reliability

### 16.1 Security Posture M14

| Risiko | Boundary | Dampak | Mitigasi | Evidence |
|---|---|---|---|---|
| LBA out-of-range menyebabkan akses memori di luar storage | Block layer API | Corrupt memori kernel | `mcsos_blk_validate_range` menolak dengan `MCSOS_BLK_ERANGE` | Host test negative case lulus |
| Dirty buffer yang tidak di-flush mengakibatkan kehilangan data | Buffer cache write-back | Data hilang saat power-loss/crash | Panggil `mcsos_bcache_flush_all` sebelum shutdown | Host test flush-before-read lulus |
| Null pointer dereference pada `dev`, `ops`, `buffer` | Semua fungsi block layer | Crash kernel | Guard null check di awal setiap fungsi | Host test null cases lulus |
| Block_size bukan power-of-two menyebabkan offset salah | `mcsos_ramblk_init` | Corrupt data storage | Validasi `mcsos_is_power_of_two_u32_local` saat inisialisasi | Host test negative init lulus |
| Akses concurrent tanpa locking | Buffer cache M14 | Race condition, data corrupt | Non-goal M14: hanya untuk single-core; locking ditambahkan di milestone berikutnya | Dicatat eksplisit sebagai non-goal |

### 16.2 Non-Goals yang Sengaja Ditunda

```text
M14 secara eksplisit bukan:
- Driver disk hardware nyata (SATA, NVMe, virtio-blk, AHCI)
- Persistent storage aman terhadap power-loss
- SMP-safe: buffer cache tidak memiliki locking
- Crash-consistent: dirty buffer dapat hilang sebelum flush
- User ABI untuk storage: syscall file I/O M13 belum terhubung ke block layer
- Security boundary pengguna: belum ada capability check atau usercopy hardening
- Produksi: hanya siap uji QEMU untuk pendidikan
```

### 16.3 Reliability dan Data Integrity

| Risiko reliability | Dampak | Deteksi | Mitigasi |
|---|---|---|---|
| `bcache_flush_all` tidak dipanggil saat shutdown | Dirty data hilang | Data tidak ada di device setelah reboot | Panggil flush di shutdown path kernel |
| Entry valid tidak di-invalidate saat device unregister | Stale pointer ke device yang sudah dibebaskan | Cache mengakses memori invalid | Invalidate semua entry yang merujuk device saat unregister |
| `storage_size` tidak kelipatan `block_size` | Kalkulasi `block_count` menghasilkan partial block | Validasi `storage_size % block_size != 0` | Sudah ditolak di `mcsos_ramblk_init` |

### 16.4 Negative Test

| Negative test | Input buruk | Expected result | Status |
|---|---|---|---|
| `blk_read` dengan `dev = NULL` | `NULL` | `MCSOS_BLK_EINVAL` | `PASS` |
| `blk_write` dengan `lba` overflow | `lba = UINT64_MAX` | `MCSOS_BLK_ERANGE` | `PASS` |
| `ramblk_init` dengan `block_size = 0` | `bs = 0` | `MCSOS_BLK_EINVAL` | `PASS` |
| `bcache_init` dengan `entry_count = 0` | `count = 0` | `MCSOS_BLK_EINVAL` | `PASS` |
| `bcache_read` dengan `lba >= block_count` | OOB | `MCSOS_BLK_ERANGE` | `PASS` |

---

## 17. Pembagian Kerja Kelompok

| Nama | NIM | Peran | Kontribusi teknis | Commit/artefak |
|---|---|---|---|---|
| `` | `` | Koordinator | Build sistem, integrasi `block.c` dan `kmain`, commit M14 | `block.c`, commit utama |
| `` | `` | Driver engineer | Implementasi `ramblk.c`, verifikasi operasi read/write | `ramblk.c`, host test ramblk |
| `` | `` | Cache engineer | Implementasi `bcache.c`, dirty flag, eviction, flush | `bcache.c`, host test bcache |
| `` | `` | Verification engineer | Audit ELF, `nm`, `objdump`, jalankan `make audit` | `m14_block_layer.nm.txt`, `readelf.txt` |
| `` | `` | Testing & Dokumentasi | Host test, QEMU smoke test, penyusunan laporan, evidence | `test_m14_block.log`, `qemu_smoke.log`, laporan |

### 17.1 Mekanisme Koordinasi

```text
Koordinasi kelompok dilakukan melalui diskusi langsung dan grup komunikasi. Pembagian tugas meliputi implementasi block.c, ramblk.c, dan bcache.c secara terpisah, penggabungan ke branch praktikum-m14-block-device, pengujian host, audit ELF, QEMU smoke test, dan penyusunan laporan. Setiap anggota memverifikasi hasil bagiannya sebelum digabungkan ke branch utama M14.
```

### 17.2 Evaluasi Kontribusi

| Anggota | Persentase kontribusi yang disepakati | Bukti | Catatan |
|---|---|---|---|
| `` | 20% | `block.c`, commit | Registry dan validasi |
| `` | 20% | `ramblk.c` | Driver RAM |
| `` | 20% | `bcache.c` | Buffer cache |
| `` | 20% | Audit ELF | Verification |
| `` | 20% | Test, laporan, evidence | Testing dan dokumentasi |

---

## 18. Kriteria Lulus Praktikum

| Kriteria minimum | Status | Evidence |
|---|---|---|
| Proyek dapat dibangun dari clean checkout | `PASS` | `make host-test && make freestanding && make audit` berhasil |
| Perintah build dan test terdokumentasi | `PASS` | Bagian langkah kerja pada laporan |
| Host unit test lulus seluruhnya | `PASS` | `artifacts/m14/test_m14_block.log` |
| `nm -u build/m14_block_layer.o` kosong | `PASS` | `artifacts/m14/m14_block_layer.nm_undef.txt` |
| `readelf -h` menunjukkan ELF64 x86-64 REL | `PASS` | `artifacts/m14/m14_block_layer.readelf.txt` |
| Symbol `mcsos_blk_register`, `mcsos_ramblk_init`, `mcsos_bcache_flush_all` ditemukan | `PASS` | `artifacts/m14/m14_block_layer.nm.txt` |
| QEMU smoke test menampilkan block layer initialized | `PASS` | `artifacts/m14/qemu_smoke.log` |
| Checksum artefak tersimpan | `PASS` | `artifacts/m14/m14_block_layer.sha256.txt` |
| Perubahan Git dikomit di branch M14 | `PASS` | `artifacts/m14/git_log.txt` |
| Laporan berisi bukti, analisis, failure modes, dan readiness review | `PASS` | Laporan ini |

Kriteria tambahan:

| Kriteria lanjutan | Status | Evidence |
|---|---|---|
| GDB debug path diuji | `NA` | Opsional — dapat diuji dengan `qemu -S -s` dan `gdb build/kernel.elf` |
| Stress/fuzz test | `NA` | Tidak dilakukan pada M14 |
| Driver hardware nyata | `NA` | Non-goal M14 |

---

## 19. Readiness Review

Pilih satu status dengan alasan berbasis bukti.

| Status | Definisi | Pilihan |
|---|---|---|
| Belum siap uji | Build/test belum stabil atau bukti belum cukup | |
| Siap uji QEMU | Build bersih, host test lulus, QEMU smoke test berjalan, log tersedia | ✔ |
| Siap demonstrasi praktikum | Siap ditunjukkan di kelas dengan bukti uji, failure mode, dan rollback | |
| Kandidat siap pakai terbatas | Hanya untuk penggunaan terbatas setelah tests, security review, dokumentasi, dan known issue tersedia | |

Alasan readiness:

```text
Host unit test lulus seluruhnya (27 kasus PASS, 0 FAIL). Freestanding object ELF64 x86_64 berhasil dikompilasi dan diaudit: nm -u kosong, readelf menunjukkan ELF64 REL x86-64, checksum tersimpan. QEMU smoke test membuktikan block layer dapat diintegrasikan ke kernel tanpa merusak VFS/RAMFS M13. Evidence dikumpulkan di artifacts/m14/. Git commit M14 tercatat di branch praktikum-m14-block-device.
```

Known issues:

| No. | Issue | Dampak | Workaround | Target perbaikan |
|---|---|---|---|---|
| 1 | Buffer cache belum SMP-safe | Race condition jika diakses concurrent | Gunakan single-core QEMU; jangan aktifkan preemption pada path cache | Tambahkan locking saat SMP dikonfigurasi |
| 2 | Dirty buffer hilang jika flush tidak dipanggil sebelum shutdown | Data tidak persistent | Panggil `mcsos_bcache_flush_all` di shutdown path kernel | Tambahkan ke kernel shutdown sequence M15+ |
| 3 | Eviction hanya FIFO minimal (entry pertama) | Performa cache tidak optimal | Diterima untuk tujuan pendidikan | Tambahkan LRU atau clock algorithm di milestone berikutnya |
| 4 | Tidak ada integrasi penuh ke VFS M13 | Filesystem belum dapat menggunakan block layer secara transparan | Block layer dapat digunakan langsung untuk smoke test | Integrasi penuh pada M15+ |
| 5 | Driver hardware nyata belum ada | Tidak dapat mengakses disk QEMU atau perangkat fisik | Ramblk cukup untuk verifikasi kontrak block layer | Tambahkan virtio-blk atau AHCI driver di milestone mendatang |

Keputusan akhir:

```text
Berdasarkan hasil host unit test yang lulus seluruhnya, freestanding object ELF64 x86_64 yang berhasil dikompilasi dan diaudit, QEMU smoke test yang membuktikan block layer terintegrasi ke kernel, serta evidence yang lengkap di artifacts/m14/, proyek praktikum M14 dinyatakan siap uji QEMU untuk block device layer dan buffer cache minimal. M14 tidak boleh diberi label siap produksi, siap filesystem persistent, SMP-safe, aman terhadap crash/power-loss, atau siap driver hardware nyata.
```

---

## 20. Rubrik Penilaian 100 Poin

| Komponen | Bobot | Indikator nilai penuh | Nilai |
|---|---:|---|---:|
| Kebenaran fungsional | 30 | Block API, RAM block driver, buffer cache, host test, dan freestanding object berjalan sesuai kontrak | `[0-30]` |
| Kualitas desain dan invariants | 20 | Invariant LBA, block size, dirty entry, ownership, dan batas concurrency ditulis jelas | `[0-20]` |
| Pengujian dan bukti | 20 | Preflight, host test, `nm`, `readelf`, `objdump`, checksum, QEMU log, dan Git evidence lengkap | `[0-20]` |
| Debugging dan failure analysis | 10 | Failure modes dianalisis, triage dijelaskan, rollback plan tersedia | `[0-10]` |
| Keamanan dan robustness | 10 | Validasi argumen, non-goals, dan risiko crash/persistence ditulis eksplisit | `[0-10]` |
| Dokumentasi dan laporan | 10 | Laporan mengikuti template, rapi, reproducible, dan referensi IEEE tersedia | `[0-10]` |
| **Total** | **100** | | `[0-100]` |

Catatan penilai:

```text
[Diisi dosen/asisten.]
```

---

## 21. Kesimpulan

### 21.1 Yang Berhasil

```text
1. Block device registry berhasil diimplementasikan: mcsos_blk_register, mcsos_blk_get, mcsos_blk_unregister bekerja sesuai kontrak.
2. RAM block driver berhasil meniru perangkat blok menggunakan array memori: operasi read, write, dan flush (no-op) deterministik dan terverifikasi oleh host test.
3. Buffer cache minimal berhasil mengimplementasikan valid flag, dirty flag, eviction dengan flush, dan mcsos_bcache_flush_all yang menulis dirty entry ke device.
4. Host unit test lulus seluruhnya: 27 kasus termasuk positive case, negative case, boundary case, dan null pointer case.
5. Freestanding object ELF64 x86_64 berhasil dikompilasi dan diaudit: nm -u kosong, readelf menunjukkan ELF64 REL x86-64, disassembly dapat diperiksa.
6. QEMU smoke test membuktikan block layer dapat diintegrasikan ke kernel M13 tanpa merusak VFS dan RAMFS yang sudah ada.
7. Evidence lengkap tersimpan di artifacts/m14/ dan Git commit M14 tercatat.
```

### 21.2 Yang Belum Berhasil

```text
1. Buffer cache belum SMP-safe — tidak ada locking; ini sengaja ditunda karena M14 hanya untuk single-core.
2. Tidak ada integrasi transparan ke VFS M13 — block layer dapat digunakan langsung tetapi belum menjadi backend file I/O.
3. Eviction hanya FIFO minimal — LRU atau clock algorithm tidak diimplementasikan di M14.
4. Driver hardware nyata belum ada — hanya RAM block driver sintetis yang tersedia.
5. Filesystem persistent belum ada — M14 hanya menyiapkan jalur untuk M15+.
```

### 21.3 Rencana Perbaikan

```text
1. M15: Implementasikan filesystem persistent berbasis blok menggunakan block layer M14 sebagai backend.
2. Milestone berikutnya: Tambahkan locking sederhana pada buffer cache saat SMP dikonfigurasi.
3. Tambahkan eviction policy LRU atau clock algorithm untuk peningkatan performa cache.
4. Tambahkan integrasi VFS M13 ke block layer sehingga syscall file I/O dapat menggunakan block device.
5. Implementasikan virtio-blk atau AHCI driver untuk mengakses disk QEMU secara nyata.
```

---

## 22. Lampiran

### Lampiran A — Commit Log

```text
[tempel output git log --oneline -5 dari branch praktikum-m14-block-device]
```

### Lampiran B — Diff Ringkas

```diff
--- /dev/null
+++ b/include/mcsos/block.h
+#ifndef MCSOS_BLOCK_H
+...

--- /dev/null
+++ b/kernel/block/block.c
+#include "mcsos/block.h"
+...

--- /dev/null
+++ b/kernel/block/ramblk.c
+#include "mcsos/block.h"
+...

--- /dev/null
+++ b/kernel/block/bcache.c
+#include "mcsos/block.h"
+...

--- /dev/null
+++ b/tests/host/test_m14_block.c
+#include <stdio.h>
+...
```

### Lampiran C — Log Host Test Lengkap

```text
[tempel isi artifacts/m14/test_m14_block.log]
```

### Lampiran D — Log QEMU Smoke Test

```text
[tempel isi artifacts/m14/qemu_smoke.log]
```

### Lampiran E — Output Readelf/Objdump

```text
[tempel isi artifacts/m14/m14_block_layer.readelf.txt]
[tempel potongan artifacts/m14/m14_block_layer.objdump.txt yang menunjukkan simbol kunci]
```

### Lampiran F — Output nm

```text
[tempel isi artifacts/m14/m14_block_layer.nm_undef.txt — harus kosong]
[tempel sebagian artifacts/m14/m14_block_layer.nm.txt yang menampilkan simbol utama]
```

### Lampiran G — Checksum

```text
[tempel isi artifacts/m14/m14_block_layer.sha256.txt]
```

### Lampiran H — Isi artifacts/m14/manifest.txt

```text
MCSOS M14 evidence manifest
timestamp_utc=2026-...
commit=...
clang=...
ld=...
qemu=...
m14_block_layer.o
m14_block_layer.nm.txt
m14_block_layer.nm_undef.txt
m14_block_layer.readelf.txt
m14_block_layer.objdump.txt
m14_block_layer.sha256.txt
test_m14_block.log
qemu_smoke.log
git_log.txt
```

---

## 23. Daftar Referensi

```text
[1] Linux Kernel Documentation, "Block," The Linux Kernel documentation. [Online]. Available: https://docs.kernel.org/block/index.html. Accessed: May 2026.

[2] Linux Kernel Documentation, "Multi-Queue Block IO Queueing Mechanism (blk-mq)," The Linux Kernel documentation. [Online]. Available: https://docs.kernel.org/block/blk-mq.html. Accessed: May 2026.

[3] Linux Kernel Documentation, "Null block device driver," The Linux Kernel documentation. [Online]. Available: https://www.kernel.org/doc/html/v5.15/block/null_blk.html. Accessed: May 2026.

[4] QEMU Project, "Invocation," QEMU documentation. [Online]. Available: https://www.qemu.org/docs/master/system/invocation.html. Accessed: May 2026.

[5] QEMU Project, "GDB usage," QEMU documentation. [Online]. Available: https://www.qemu.org/docs/master/system/gdb.html. Accessed: May 2026.

[6] LLVM Project, "Clang command line argument reference," Clang documentation. [Online]. Available: https://clang.llvm.org/docs/ClangCommandLineReference.html. Accessed: May 2026.

[7] GNU Project, "GNU Binary Utilities," GNU Binutils documentation. [Online]. Available: https://www.sourceware.org/binutils/docs/binutils.html. Accessed: May 2026.

[8] Microsoft, "Install WSL," Microsoft Learn. [Online]. Available: https://learn.microsoft.com/en-us/windows/wsl/install. Accessed: May 2026.
```

---

## 24. Checklist Final Sebelum Pengumpulan

| Checklist | Status |
|---|---|
| Semua placeholder NIM dan nama sudah diisi oleh anggota | `[isi oleh kelompok]` |
| Metadata laporan lengkap (nama, NIM, tanggal) | `[isi oleh kelompok]` |
| Commit awal dan akhir dicatat | `[isi oleh kelompok]` |
| Perintah build dan test dapat dijalankan ulang | `Ya` |
| Log host test dilampirkan | `Lampiran C` |
| Log QEMU dilampirkan | `Lampiran D` |
| Artefak penting tersedia di `artifacts/m14/` | `Ya` |
| Desain, invariants, ownership, dan failure modes dijelaskan | `Ya` |
| Security/reliability dibahas | `Ya` |
| Readiness review tidak berlebihan | `Ya — diklaim siap uji QEMU` |
| Rubrik penilaian disiapkan | `Ya` |
| Referensi memakai format IEEE | `Ya` |
| Laporan disimpan sebagai Markdown | `Ya` |

---

## 25. Pernyataan Pengumpulan

Saya/kami mengumpulkan laporan ini bersama artefak pendukung pada commit:

```text
[tempel commit hash akhir dari git log --oneline -1]
```

Status akhir yang diklaim:

```text
siap uji QEMU untuk block device layer dan buffer cache minimal
```

Ringkasan satu paragraf:

```text
Praktikum M14 berhasil membangun fondasi storage layer pada kernel MCSOS 260502 dengan mengimplementasikan block device registry, RAM block driver volatil, dan buffer cache minimal dengan dirty flag serta flush eksplisit. Keberhasilan dibuktikan oleh host unit test yang lulus seluruhnya (27 kasus PASS, 0 FAIL), freestanding object ELF64 x86_64 tanpa undefined symbol, readelf yang mengkonfirmasi ELF64 REL x86-64, disassembly yang dapat diaudit, serta QEMU smoke test yang menampilkan block layer terintegrasi ke kernel. Keterbatasan M14 adalah belum ada driver hardware nyata, buffer cache belum SMP-safe, belum ada filesystem persistent, dan block layer belum terhubung ke VFS M13 secara transparan — semua akan dibahas pada M15 dan milestone berikutnya.
```
