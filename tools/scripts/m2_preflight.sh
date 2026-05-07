#!/usr/bin/env bash
set -euo pipefail

echo "== M2 preflight MCSOS 260502 =="

pwd

required_cmds=(
git
make
clang
ld.lld
readelf
objdump
nm
qemu-system-x86_64
xorriso
python3
)

for c in "${required_cmds[@]}"; do
command -v "$c" >/dev/null
echo "OK command: $c -> $(command -v "$c")"
done

required_docs=(
docs/architecture/overview.md
docs/architecture/invariants.md
docs/security/threat_model.md
docs/testing/verification_matrix.md
)

for f in "${required_docs[@]}"; do
test -f "$f"
echo "OK M0 file: $f"
done

mkdir -p build/meta

if [ -f build/meta/toolchain-versions.txt ]; then
echo "OK M1 metadata: build/meta/toolchain-versions.txt"
else
echo "WARN: metadata M1 belum ada"
fi

echo "OK: M2 preflight passed"
