#!/usr/bin/env bash

set -euo pipefail

out="evidence/M4"

mkdir -p "$out"

cp -f build/kernel.elf "$out/" 2>/dev/null || true
cp -f build/kernel.map "$out/" 2>/dev/null || true

cp -f build/m4.syms.txt "$out/" 2>/dev/null || true
cp -f build/m4.disasm.txt "$out/" 2>/dev/null || true
cp -f build/m4.readelf.header.txt "$out/" 2>/dev/null || true
cp -f build/m4.readelf.programs.txt "$out/" 2>/dev/null || true
cp -f build/m4.readelf.sections.txt "$out/" 2>/dev/null || true

cp -f build/m4-qemu-serial.log "$out/" 2>/dev/null || true

{
    echo "MCSOS M4 evidence manifest"

    date -u +"timestamp_utc=%Y-%m-%dT%H:%M:%SZ"

    git rev-parse HEAD 2>/dev/null | sed 's/^/commit=/'

    clang --version | head -n 1 | sed 's/^/clang=/'

    ld.lld --version | head -n 1 | sed 's/^/lld=/'

    qemu-system-x86_64 --version 2>/dev/null | head -n 1 | sed 's/^/qemu=/'

    find "$out" -maxdepth 1 -type f -printf '%f\n' | LC_ALL=C sort

} > "$out/manifest.txt"

echo "[M4][PASS] Evidence dikumpulkan di $out"
