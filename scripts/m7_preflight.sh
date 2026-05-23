#!/usr/bin/env bash
set -euo pipefail

echo "[M7] preflight check"

need_cmd() {
    command -v "$1" >/dev/null 2>&1 || {
        echo "[FAIL] command tidak ditemukan: $1"
        exit 1
    }
    echo "[OK] command: $1"
}

need_file() {
    [ -f "$1" ] || {
        echo "[FAIL] file tidak ada: $1"
        exit 1
    }
    echo "[OK] file: $1"
}

need_dir() {
    [ -d "$1" ] || {
        echo "[FAIL] direktori tidak ada: $1"
        exit 1
    }
    echo "[OK] dir: $1"
}

need_cmd git
need_cmd make
need_cmd clang
need_cmd qemu-system-x86_64
need_cmd gdb

need_dir kernel
need_dir tests
need_dir scripts

need_file kernel/core/pmm.c
need_file kernel/include/mcsos/kernel/pmm.h

echo "[M7] preflight sukses"
