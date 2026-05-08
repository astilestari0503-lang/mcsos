#!/usr/bin/env bash
set -euo pipefail

ISO="build/mcsos.iso"

if [ ! -f "$ISO" ]; then
    echo "ERROR: ISO tidak ditemukan. Jalankan make image."
    exit 1
fi

qemu-system-x86_64 \
    -machine q35 \
    -cpu qemu64 \
    -m 512M \
    -cdrom "$ISO" \
    -serial stdio \
    -display none
