global ASM_add
global ASM_add_loop
global ASM_add_loop_unrolled
global ASM_nop_short
global ASM_nop_short_loop
global ASM_nop_long
global ASM_nop_long_loop

section .text

ASM_add:
    xor rax, rax    ; set rax to zero
    %rep 100000
        add rax, 9  ; add
    %endrep
    ret

ASM_add_loop:
    xor rax, rax    ; set rax to zero
.loop:
    add rax, 8
    dec rdi
    jnz .loop
    ret

ASM_nop_short:
    %rep 100000
        nop
    %endrep
    ret

ASM_nop_short_loop:
.loop:
    nop
    dec rdi
    jnz .loop
    ret

; https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html (page: 1392)
ASM_nop_long:
    %rep 100000
        db 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00 ; 9-byte nop encoding
    %endrep
    ret

ASM_nop_long_loop:
.loop:
    db 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
    dec rdi
    jnz .loop
    ret

ASM_add_loop_unrolled:
    xor rax, rax    ; set rax to zero
    mov rdi, 2500
    mov r9, 123
.loop:
    %rep 40
      add rax, r9
      add rsi, r9
      add r10, r9
      add r11, r9
      ; add rdx, r9
    %endrep
    dec rdi
    jnz .loop
    ret
