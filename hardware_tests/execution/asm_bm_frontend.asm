global ASM_add
global ASM_nop_short
global ASM_nop_short_loop
global ASM_nop_long
global ASM_nop_long_loop

section .text

; objdump -Mintel -d ./build/gcc-release/hardware_tests/execution/perf_stuff.asm_bm_frontend | grep "add\s*rax,0x9" | wc -l
; front end and backend have to be defined before this (with python interpreter)
; one binary with just this, then objdump
ASM_add:
    mov rax, rdi    ; move first argument into rax
    %rep 1000000
        add rax, 9    ; add second argument
    %endrep
    ret

; this comes out to 0.25 cycles per instruction -> 4 instructions are decoded per clock cycle
; ---> frontend can do multiple instructions at once
ASM_nop_short:
    %rep 1000000
        nop
    %endrep
    ret

ASM_nop_short_loop:
.loop:
    nop
    dec rdi
    jnz .loop
    ret

; same amount of backend work (none)
; https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
; page: 1392
ASM_nop_long:
    %rep 1000000
        db 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
    %endrep
    ret

ASM_nop_long_loop:
.loop:
    db 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
    dec rdi
    jnz .loop
    ret
