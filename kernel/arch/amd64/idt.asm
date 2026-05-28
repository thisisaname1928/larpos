global loadIDT64
global isrFuncTab
extern exceptionHandler

section .text

loadIDT64:
    cli
    lidt [rdi]
    sti
    ret

%macro ISR_FUNC 1
ISR_FUNC_%+%1:
    push rdi
    push rsi
    mov rdi, %1
    lea rsi, [rsp + 16]

    push rbp
    sub rsp, 8

    call exceptionHandler

    add rsp, 8
    pop rbp
    pop rsi
    pop rdi
    iretq
%endmacro

%macro ISR_FUNC_W_ECODE 1
ISR_FUNC_%+%1:
    push rdi
    push rsi
    mov rdi, %1
    lea rsi, [rsp + 16]

    push rbp
    sub rsp, 8

    call exceptionHandler

    add rsp, 8
    pop rbp
    pop rsi
    pop rdi
    add rsp, 16
    iretq
%endmacro

ISR_FUNC 0
ISR_FUNC 1
ISR_FUNC 2
ISR_FUNC 3
ISR_FUNC 4
ISR_FUNC 5
ISR_FUNC 6
ISR_FUNC 7
ISR_FUNC_W_ECODE 8
ISR_FUNC 9
ISR_FUNC_W_ECODE 10
ISR_FUNC_W_ECODE 11
ISR_FUNC_W_ECODE 12
ISR_FUNC_W_ECODE 13
ISR_FUNC_W_ECODE 14
ISR_FUNC 15
ISR_FUNC 16
ISR_FUNC_W_ECODE 17
ISR_FUNC 18
ISR_FUNC 19
ISR_FUNC 20
ISR_FUNC_W_ECODE 21
ISR_FUNC 22
ISR_FUNC 23
ISR_FUNC 24
ISR_FUNC 25
ISR_FUNC 26
ISR_FUNC 27
ISR_FUNC 28
ISR_FUNC_W_ECODE 29
ISR_FUNC_W_ECODE 30
ISR_FUNC 31


isrFuncTab:
%assign i 0 
%rep    32 
    dq ISR_FUNC_%+i
%assign i i+1 
%endrep
