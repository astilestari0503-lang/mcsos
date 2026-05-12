set pagination off
set confirm off
set disassembly-flavor intel

file build/kernel.elf

set architecture i386:x86-64

target remote localhost:1234

break kmain
break x86_64_idt_init
break x86_64_trap_dispatch

continue
