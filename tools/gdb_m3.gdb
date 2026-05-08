set pagination off
set confirm off

file build/kernel.elf

target remote localhost:1234

break kernel_main
break kernel_panic_at

continue

info registers
bt
disassemble /m kernel_main
