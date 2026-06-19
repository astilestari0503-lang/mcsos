.RECIPEPREFIX := >
SHELL := /usr/bin/env bash

BUILD_DIR := build

KERNEL := $(BUILD_DIR)/kernel.elf
BP_KERNEL := $(BUILD_DIR)/kernel.breakpoint.elf
PANIC_KERNEL := $(BUILD_DIR)/kernel.panic.elf

MAP := $(BUILD_DIR)/kernel.map
BP_MAP := $(BUILD_DIR)/kernel.breakpoint.map
PANIC_MAP := $(BUILD_DIR)/kernel.panic.map

DISASM := $(BUILD_DIR)/kernel.disasm.txt
SYMS := $(BUILD_DIR)/kernel.syms.txt

CC := clang
LD := ld.lld
OBJDUMP := objdump
READELF := readelf
NM := nm

COMMON_CFLAGS := \
--target=x86_64-unknown-none-elf \
-std=c17 \
-ffreestanding \
-fno-builtin \
-fno-stack-protector \
-fno-stack-check \
-fno-pic \
-fno-pie \
-fno-lto \
-m64 \
-march=x86-64 \
-mabi=sysv \
-mno-red-zone \
-mno-mmx \
-mno-sse \
-mno-sse2 \
-mcmodel=kernel \
-Wall \
-Wextra \
-Werror \
-Ikernel/arch/x86_64/include \
-Ikernel/include \
-Iinclude

COMMON_ASFLAGS := \
--target=x86_64-unknown-none-elf \
-ffreestanding \
-fno-pic \
-fno-pie \
-m64

CFLAGS := $(COMMON_CFLAGS)

ASFLAGS := $(COMMON_ASFLAGS)

BP_CFLAGS := \
$(COMMON_CFLAGS) \
-DMCSOS_M4_TRIGGER_BREAKPOINT=1

PANIC_CFLAGS := \
$(COMMON_CFLAGS) \
-DMCSOS_M4_TRIGGER_PANIC=1

LDFLAGS := \
-nostdlib \
-static \
-z max-page-size=0x1000 \
-T linker.ld

SRC_C := $(shell find kernel -name '*.c' | LC_ALL=C sort)

SRC_S := $(shell find kernel -name '*.S' | LC_ALL=C sort)

OBJ := \
$(patsubst %.c,$(BUILD_DIR)/normal/%.o,$(SRC_C)) \
$(patsubst %.S,$(BUILD_DIR)/normal/%.o,$(SRC_S))

BP_OBJ := \
$(patsubst %.c,$(BUILD_DIR)/breakpoint/%.o,$(SRC_C)) \
$(patsubst %.S,$(BUILD_DIR)/breakpoint/%.o,$(SRC_S))

PANIC_OBJ := \
$(patsubst %.c,$(BUILD_DIR)/panic/%.o,$(SRC_C)) \
$(patsubst %.S,$(BUILD_DIR)/panic/%.o,$(SRC_S))

.PHONY: all build breakpoint panic inspect audit clean distclean

all: build inspect

build: $(KERNEL)

breakpoint: $(BP_KERNEL)

panic: $(PANIC_KERNEL)

$(BUILD_DIR)/normal/%.o: %.c
>mkdir -p $(dir $@)
>$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/normal/%.o: %.S
>mkdir -p $(dir $@)
>$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/breakpoint/%.o: %.c
>mkdir -p $(dir $@)
>$(CC) $(BP_CFLAGS) -c $< -o $@

$(BUILD_DIR)/breakpoint/%.o: %.S
>mkdir -p $(dir $@)
>$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/panic/%.o: %.c
>mkdir -p $(dir $@)
>$(CC) $(PANIC_CFLAGS) -c $< -o $@

$(BUILD_DIR)/panic/%.o: %.S
>mkdir -p $(dir $@)
>$(CC) $(ASFLAGS) -c $< -o $@

$(KERNEL): $(OBJ) linker.ld
>mkdir -p $(BUILD_DIR)
>$(LD) $(LDFLAGS) -Map=$(MAP) -o $@ $(OBJ)

$(BP_KERNEL): $(BP_OBJ) linker.ld
>mkdir -p $(BUILD_DIR)
>$(LD) $(LDFLAGS) -Map=$(BP_MAP) -o $@ $(BP_OBJ)

$(PANIC_KERNEL): $(PANIC_OBJ) linker.ld
>mkdir -p $(BUILD_DIR)
>$(LD) $(LDFLAGS) -Map=$(PANIC_MAP) -o $@ $(PANIC_OBJ)

inspect: $(KERNEL)
>mkdir -p $(BUILD_DIR)
#>tools/scripts/m4_audit_elf.sh build/kernel.elf
>$(READELF) -h $(KERNEL) > $(BUILD_DIR)/kernel.readelf.header.txt
>$(READELF) -l $(KERNEL) > $(BUILD_DIR)/kernel.readelf.programs.txt
>$(NM) -n $(KERNEL) > $(SYMS)
>$(OBJDUMP) -d -Mintel $(KERNEL) > $(DISASM)
>grep -q 'ELF64' $(BUILD_DIR)/kernel.readelf.header.txt
>grep -q 'Machine:[[:space:]]*Advanced Micro Devices X86-64' $(BUILD_DIR)/kernel.readelf.header.txt
>grep -q 'kmain' $(SYMS)
>grep -q 'x86_64_idt_init' $(SYMS)
>grep -q 'x86_64_trap_dispatch' $(SYMS)
>grep -q 'pic_remap' $(SYMS)
>grep -q 'pit_configure_hz' $(SYMS)
>grep -q 'timer_on_irq0' $(SYMS)
>grep -q 'iretq' $(DISASM)
>grep -q 'lidt' $(DISASM)
>grep -q 'sti' $(DISASM)
>grep -q 'hlt' $(DISASM)

audit: inspect breakpoint panic
>! $(NM) -u $(KERNEL) | grep .
>! $(NM) -u $(BP_KERNEL) | grep .
>! $(NM) -u $(PANIC_KERNEL) | grep .
>$(READELF) -S $(KERNEL) | grep -q '.text'
>$(READELF) -S $(KERNEL) | grep -q '.rodata'
grade: all
>grep -q 'isr_stub_32' build/kernel.syms.txt
>grep -q 'pic_remap' build/kernel.syms.txt
>grep -q 'pit_configure_hz' build/kernel.syms.txt
>grep -q 'timer_on_irq0' build/kernel.syms.txt
>grep -q 'x86_64_trap_dispatch' build/kernel.syms.txt
>@echo "M5 static grade: PASS"

m7-host-test:
>mkdir -p build
>cc -std=c17 \
>-DMCSOS_HOST_TEST \
>-Ikernel/include \
>kernel/core/vmm.c \
>tests/test_vmm_host.c \
>-o build/test_vmm_host
check: m7-host-test
>./build/test_vmm_host
clean:
>rm -rf $(BUILD_DIR)

distclean: clean
>rm -rf iso_root limine

CC ?= clang
CFLAGS_COMMON := -std=c17 -Wall -Wextra -Werror -Ikernel/include \
-Iinclude
CFLAGS_KERNEL := $(CFLAGS_COMMON) -ffreestanding -fno-builtin -fno-stack-protector -mno-red-zone
BUILD_DIR := build/m8

.PHONY: m8-clean m8-kmem-host-test m8-kmem-freestanding m8-audit m8-all

m8-clean:
>$(RM) -r $(BUILD_DIR)

$(BUILD_DIR):
>mkdir -p $(BUILD_DIR)

m8-kmem-freestanding: | $(BUILD_DIR)
>$(CC) $(CFLAGS_KERNEL) -c kernel/mm/kmem.c -o $(BUILD_DIR)/kmem.freestanding.o

m8-kmem-host-test: | $(BUILD_DIR)
>$(CC) $(CFLAGS_COMMON) tests/test_kmem.c kernel/mm/kmem.c -o $(BUILD_DIR)/test_kmem
>./$(BUILD_DIR)/test_kmem | tee $(BUILD_DIR)/test_kmem.log

m8-audit: m8-kmem-freestanding
>nm -u $(BUILD_DIR)/kmem.freestanding.o | tee $(BUILD_DIR)/nm_u.txt
>test ! -s $(BUILD_DIR)/nm_u.txt
>readelf -h $(BUILD_DIR)/kmem.freestanding.o > $(BUILD_DIR)/readelf_h.txt
>objdump -dr $(BUILD_DIR)/kmem.freestanding.o > $(BUILD_DIR)/kmem.objdump.txt

m8-all: m8-kmem-host-test m8-audit

CC := clang
LD := ld.lld
OBJDUMP ?= objdump
READELF ?= readelf
NM ?= nm
SHA256SUM ?= sha256sum

BUILD := build/m9
CFLAGS_HOST := -std=c17 -Wall -Wextra -Werror -DMCSOS_HOST_TEST -Iinclude
CFLAGS_KERNEL := -target x86_64-unknown-none-elf -std=c17 -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Werror -Iinclude
ASFLAGS_KERNEL := -target x86_64-unknown-none-elf -ffreestanding -fno-stack-protector -fno-pic -mno-red-zone

.PHONY: m9-all m9-host-test m9-freestanding m9-audit m9-clean

m9-all: m9-host-test m9-freestanding m9-audit

$(BUILD):
>mkdir -p $(BUILD)

m9-host-test: $(BUILD)
>$(CC) $(CFLAGS_HOST) tests/test_scheduler.c kernel/mcsos_thread.c -o $(BUILD)/m9_host_test
>$(BUILD)/m9_host_test | tee $(BUILD)/test_scheduler.log

m9-freestanding: $(BUILD)
>$(CC) $(CFLAGS_KERNEL) -c kernel/mcsos_thread.c -o $(BUILD)/mcsos_thread.freestanding.o
>$(CC) $(ASFLAGS_KERNEL) -c arch/x86_64/context_switch.S -o $(BUILD)/context_switch.o
>$(LD) -r $(BUILD)/mcsos_thread.freestanding.o $(BUILD)/context_switch.o -o $(BUILD)/m9_scheduler_combined.o

m9-audit: m9-freestanding
>$(NM) -u $(BUILD)/m9_scheduler_combined.o | tee $(BUILD)/nm_undefined.log
>$(READELF) -h $(BUILD)/m9_scheduler_combined.o | tee $(BUILD)/readelf_header.log
>$(OBJDUMP) -d $(BUILD)/m9_scheduler_combined.o | grep -E 'mcsos_context_switch|jmp|ret|hlt' | tee $(BUILD)/objdump_key.log
>$(SHA256SUM) $(BUILD)/m9_host_test $(BUILD)/m9_scheduler_combined.o | tee $(BUILD)/sha256.log

m9-clean:
>rm -rf $(BUILD)

# ==================================================
# M10 SYSCALL
# ==================================================

HOST_CC ?= clang
SHA256SUM ?= sha256sum

M10_BUILD := build/m10

M10_HOST_CFLAGS := \
-std=c17 \
-Wall \
-Wextra \
-Werror \
-Iinclude

M10_KERNEL_CFLAGS := \
-target x86_64-unknown-none-elf \
-std=c17 \
-ffreestanding \
-fno-stack-protector \
-fno-builtin \
-mno-red-zone \
-Wall \
-Wextra \
-Werror \
-Iinclude

.PHONY: \
m10-all \
m10-host-test \
m10-freestanding \
m10-audit \
m10-clean

$(M10_BUILD):
>mkdir -p $(M10_BUILD)

m10-host-test: $(M10_BUILD)
>$(HOST_CC) \
>$(M10_HOST_CFLAGS) \
>tests/test_syscall_host.c \
>kernel/syscall/syscall.c \
>-o $(M10_BUILD)/test_syscall_host

>$(M10_BUILD)/test_syscall_host

$(M10_BUILD)/syscall.o: \
kernel/syscall/syscall.c \
include/mcsos/syscall.h \
| $(M10_BUILD)

>$(CC) \
>$(M10_KERNEL_CFLAGS) \
>-c kernel/syscall/syscall.c \
>-o $@

$(M10_BUILD)/syscall_entry.o: \
kernel/syscall/syscall_entry.S \
| $(M10_BUILD)

>$(CC) \
>-target x86_64-unknown-none-elf \
>-c kernel/syscall/syscall_entry.S \
>-o $@

m10-freestanding: \
$(M10_BUILD)/syscall.o \
$(M10_BUILD)/syscall_entry.o

>$(LD) -r \
>$(M10_BUILD)/syscall.o \
>$(M10_BUILD)/syscall_entry.o \
>-o $(M10_BUILD)/m10_syscall_combined.o

m10-audit: m10-freestanding

>$(NM) -u \
>$(M10_BUILD)/m10_syscall_combined.o \
>| tee $(M10_BUILD)/nm_undefined.log

>$(READELF) -h \
>$(M10_BUILD)/m10_syscall_combined.o \
>| tee $(M10_BUILD)/readelf_header.log

>$(OBJDUMP) -dr \
>$(M10_BUILD)/m10_syscall_combined.o \
>| tee $(M10_BUILD)/objdump.log

>$(SHA256SUM) \
>$(M10_BUILD)/test_syscall_host \
>$(M10_BUILD)/m10_syscall_combined.o \
>| tee $(M10_BUILD)/sha256.log

m10-all: \
m10-host-test \
m10-freestanding \
m10-audit

m10-clean:
>rm -rf $(M10_BUILD)

test: m10-host-test m10-audit
>@echo "M10 tests passed"

CC ?= clang
HOST_CFLAGS := -std=c17 -Wall -Wextra -Werror -O2 -g
FREESTANDING_CFLAGS := -target x86_64-elf -std=c17 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Werror -O2 -g

.PHONY: m15-all

m15-all: artifacts/m15/test_mcsfs1 artifacts/m15/mcsfs1.o artifacts/m15/mcsfs1.rel.o
>./artifacts/m15/test_mcsfs1 | tee artifacts/m15/host_test.txt
>nm -u artifacts/m15/mcsfs1.rel.o | tee artifacts/m15/nm_undefined.txt
>test ! -s artifacts/m15/nm_undefined.txt
>readelf -h artifacts/m15/mcsfs1.rel.o | tee artifacts/m15/readelf_header.txt
>objdump -dr artifacts/m15/mcsfs1.rel.o | tee artifacts/m15/objdump.txt >/dev/null
>sha256sum artifacts/m15/* | tee artifacts/m15/SHA256SUMS.txt

artifacts/m15/test_mcsfs1: tests/m15/test_mcsfs1.c fs/mcsfs1/mcsfs1.c fs/mcsfs1/mcsfs1.h
>mkdir -p artifacts/m15
>$(CC) $(HOST_CFLAGS) -I. tests/m15/test_mcsfs1.c fs/mcsfs1/mcsfs1.c -o $@

artifacts/m15/mcsfs1.o: fs/mcsfs1/mcsfs1.c fs/mcsfs1/mcsfs1.h
>mkdir -p artifacts/m15
>$(CC) $(FREESTANDING_CFLAGS) -I. -c fs/mcsfs1/mcsfs1.c -o $@

artifacts/m15/mcsfs1.rel.o: artifacts/m15/mcsfs1.o
>ld -r $< -o $@
