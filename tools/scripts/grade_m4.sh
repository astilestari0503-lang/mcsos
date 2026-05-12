#!/usr/bin/env bash

set -euo pipefail

score=0

make clean >/dev/null

if make >/dev/null; then
    score=$((score + 60))
else
    echo "M4_LOCAL_SCORE=$score/100"
    exit 1
fi

if [[ -f build/kernel.elf ]]; then
    if tools/scripts/m4_audit_elf.sh build/kernel.elf >/dev/null; then
        score=$((score + 20))
    fi
fi

if [[ -f build/m4-qemu-serial.log ]]; then
    if grep -q '\[M4\]' build/m4-qemu-serial.log; then
        score=$((score + 10))
    fi
fi

if [[ -f evidence/M4/manifest.txt ]]; then
    score=$((score + 10))
fi

echo "M4_LOCAL_SCORE=$score/100"
