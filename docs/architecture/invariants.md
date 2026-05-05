# MCSOS Toolchain and Environment Invariants

## M1 Invariants

1. **Repository Location**: Repository MCSOS harus berada di filesystem Linux WSL (contoh: `~/src/mcsos`), BUKAN di `/mnt/c` atau mount Windows lain.

2. **Generated Artifacts**: Semua generated artifact harus berada di direktori `build/` dan TIDAK boleh dikomit ke Git.

3. **Tool Availability**: Semua build tool wajib tersedia melalui PATH WSL dan tercatat pada `build/meta/toolchain-versions.txt`.

4. **Proof Object Format**: Proof object file (`freestanding_probe.o`) harus bertipe ELF64 x86_64 dan dihasilkan dengan compiler dalam mode freestanding.

5. **Undefined Symbols**: Proof ELF (`freestanding_probe.elf`) tidak boleh memiliki undefined symbol apapun.

6. **Freestanding Runtime**: Kompilasi kernel dan proof tidak boleh bergantung pada hosted libc, startup object host, dynamic linker, exception runtime, atau stack protector runtime host.

7. **Emulator Ready**: QEMU x86_64, machine q35, dan OVMF firmware harus terdeteksi dan tersedia sebelum M2 dimulai.

8. **Toolchain Audit**: Setiap perubahan toolchain, versi distro, atau konfigurasi WSL harus dicatat dalam readiness review.
