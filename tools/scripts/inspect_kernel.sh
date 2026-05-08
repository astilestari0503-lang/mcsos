#!/usr/bin/env bash
set -euo pipefail

KERNEL="build/kernel.elf"
OUTDIR="build/inspect"

if [ ! -f "$KERNEL" ]; then
    echo "ERROR: kernel tidak ditemukan. Jalankan make build."
    exit 1
fi

mkdir -p "$OUTDIR"

readelf -h "$KERNEL" > "$OUTDIR/readelf_header.txt"
readelf -l "$KERNEL" > "$OUTDIR/readelf_program_headers.txt"
nm -n "$KERNEL" > "$OUTDIR/nm_symbols.txt"

echo "OK: inspect selesai di $OUTDIR"
