.intel_syntax

ASM_add_loop_xor:
.loop:
# LLVM-MCA-BEGIN
    add rcx, 1
    imul rcx, rcx
    dec rdi
    jnz .loop
# LLVM-MCA-END
    ret

; D : Instruction dispatched.
; e : Instruction executing.
; E : Instruction executed.
; R : Instruction retired.
; = : Instruction already dispatched, waiting to be executed.
; - : Instruction executed, waiting to be retired.
