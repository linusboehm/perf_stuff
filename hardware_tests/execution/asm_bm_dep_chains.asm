global ASM_add
global ASM_mov_add1
global ASM_mov_add2
global ASM_mov_add3

section .text

ASM_add:
    %rep 4000
      add rcx, 1   ; rcx += 1
    %endrep
    ret

ASM_mov_add1:
    %rep 4000
      mov rdi, rcx ; rdi = rcx
      add rcx, 1   ; rcx += 1
    %endrep
    ret

ASM_mov_add2:
    %rep 4000
      mov rcx, rcx ; rcx = rcx
      add rcx, 1   ; rcx += 1
    %endrep
    ret

ASM_mov_add3:
    %rep 4000
      mov rcx, rax ; rcx = rax
      add rcx, 1   ; rcx += 1
    %endrep
    ret
