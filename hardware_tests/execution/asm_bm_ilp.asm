global ASM_mov1
global ASM_mov2
global ASM_mov3
global ASM_mov_add

section .text

ASM_mov1:
align 64
.loop:
    mov rax, [rsi]
    sub rdi, 1
    jnle .loop
    ret

ASM_mov2:
align 64
.loop:
    mov rax, [rsi]
    mov rax, [rsi]
    sub rdi, 1
    jnle .loop
    ret

ASM_mov3:
align 64
.loop:
    mov rax, [rsi]
    mov rax, [rsi]
    mov rax, [rsi]
    sub rdi, 1
    jnle .loop
    ret

; can slot another add in -> https://agner.org/optimize/
ASM_mov_add:
align 64
.loop:
    mov rax, [rsi]
    mov rax, [rsi]
    add r8, 3
    sub rdi, 1
    jnle .loop
    ret
