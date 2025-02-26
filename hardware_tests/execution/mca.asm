.intel_syntax

ASM_add_loop:
.loop:
# LLVM-MCA-BEGIN
    add rcx, 1
    add rcx, 1
    dec rdi
    jnz .loop
# LLVM-MCA-END
    ret

ASM_add_loop_xor:
.loop:
# LLVM-MCA-BEGIN
    add rcx, 1
    xor rcx, rcx
    add rcx, 1
    dec rdi
    jnz .loop
# LLVM-MCA-END
    ret

ASM_add_loop_mov:
.loop:
# LLVM-MCA-BEGIN
    mov rax, [rsi]
    add rcx, 1
    mov rax, [rsi]
    dec rdi
    jnz .loop
# LLVM-MCA-END
    ret
