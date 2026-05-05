#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
OUT="$ROOT/build/proof"
SRC="$ROOT/tests/toolchain/freestanding_probe.c"

mkdir -p "$OUT"

# Flag compilation untuk freestanding kernel
CFLAGS=(
    --target=x86_64-unknown-elf
    -std=c17
    -ffreestanding
    -fno-stack-protector
    -fno-pic
    -mno-red-zone
    -mno-mmx
    -mno-sse
    -mno-sse2
    -Wall
    -Wextra
    -Werror
    -O2
    -c
)

# Kompilasi menjadi object file
echo "Compiling $SRC..."
clang "${CFLAGS[@]}" "$SRC" -o "$OUT/freestanding_probe.o"
echo "OK: Object file created: $OUT/freestanding_probe.o"

# Link menjadi ELF executable
echo "Linking ELF..."
ld.lld \
    -m elf_x86_64 \
    -nostdlib \
    --entry=mcsos_toolchain_probe \
    -Ttext=0xffffffff80000000 \
    -o "$OUT/freestanding_probe.elf" \
    "$OUT/freestanding_probe.o"
echo "OK: ELF file created: $OUT/freestanding_probe.elf"

# Inspeksi ELF object header
echo "Reading object header..."
readelf -hW "$OUT/freestanding_probe.o" | tee "$OUT/readelf-objectheader.txt"

# Inspeksi ELF executable header
echo "Reading ELF header..."
readelf -hW "$OUT/freestanding_probe.elf" | tee "$OUT/readelf-header.txt"

# Inspeksi ELF sections
echo "Reading ELF sections..."
readelf -SW "$OUT/freestanding_probe.elf" | tee "$OUT/readelf-sections.txt"

# Disassembly object file
echo "Disassembling object..."
objdump -drwC "$OUT/freestanding_probe.o" | tee "$OUT/objdump-disassembly.txt"

# Cek undefined symbols
echo "Checking undefined symbols..."
nm -u "$OUT/freestanding_probe.elf" | tee "$OUT/nm-undefined.txt"

# File type report
echo "Checking file types..."
file "$OUT/freestanding_probe.o" "$OUT/freestanding_probe.elf" | tee "$OUT/file-type.txt"

# Validasi: tidak boleh ada undefined symbols
if [ -s "$OUT/nm-undefined.txt" ]; then
    echo "ERROR: undefined symbols detected in freestanding ELF" >&2
    cat "$OUT/nm-undefined.txt" >&2
    exit 1
fi

echo "OK: freestanding x86_64 ELF proof generated successfully"
