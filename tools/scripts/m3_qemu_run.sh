#!/usr/bin/env bash

set -Eeuo pipefail

ISO="${1:-build/mcsos.iso}"
LOG="${2:-build/m4_serial.log}"

TIMEOUT_SEC="${MCSOS_QEMU_TIMEOUT:-8}"

OVMF_CODE="${OVMF_CODE:-/usr/share/ovmf/OVMF.fd}"

fail() {
    echo "FAIL: $*" >&2
    exit 1
}

test -f "$ISO" \
    || fail "ISO tidak ditemukan: $ISO"

command -v qemu-system-x86_64 >/dev/null 2>&1 \
    || fail "qemu-system-x86_64 tidak ditemukan"

test -f "$OVMF_CODE" \
    || fail "OVMF_CODE tidak ditemukan: $OVMF_CODE"

mkdir -p "$(dirname "$LOG")"

rm -f "$LOG"

timeout "$TIMEOUT_SEC" \
qemu-system-x86_64 \
    -machine q35 \
    -m 256M \
    -smp 1 \
    -cpu qemu64 \
    -bios "$OVMF_CODE" \
    -cdrom "$ISO" \
    -boot d \
    -serial file:"$LOG" \
    -display none \
    -no-reboot \
    -no-shutdown \
    || true

cat "$LOG"

echo "PASS: QEMU smoke test selesai"
