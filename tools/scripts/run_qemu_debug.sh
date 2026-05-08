#!/usr/bin/env bash
set -euo pipefail

ISO="build/mcsos.iso"
LOG="build/qemu-serial.log"
OVMF_CODE="/usr/share/OVMF/OVMF_CODE.fd"

rm -f "$LOG"

qemu-system-x86_64 \
  -machine q35 \
  -cpu qemu64 \
  -m 512M \
  -serial "file:$LOG" \
  -display none \
  -monitor stdio \
  -no-reboot \
  -no-shutdown \
  -drive if=pflash,format=raw,readonly=on,file="$OVMF_CODE" \
  -cdrom "$ISO" \
  -s -S
