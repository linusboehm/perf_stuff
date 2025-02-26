global ASM_add
global ASM_mov_add

section .text

ASM_add:
    %rep 4000
      add rcx, 1   ; rcx += 1
    %endrep
    ret

ASM_mov_add:
    %rep 4000
      mov rcx, rax ; rcx = rax
      add rcx, 1   ; rcx += 1
    %endrep
    ret

;;; objdump
; 48 89 c1       mov    rcx,rax
; 48 83 c1 01    add    rcx,0x1
