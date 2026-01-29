section .data
    test_str db "TESTING", 0

section .text
    global _start

_start:
    mov rax, 60
    mov rdi, 0
    syscall