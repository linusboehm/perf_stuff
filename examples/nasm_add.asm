global add_numbers
section .text

add_numbers:
    mov rax, rdi    ; move first argument into rax
    %rep 40
        add rax, rsi    ; add second argument
    %endrep
    ret
