global loadGDTR64
global reloadSegment64

section .text

loadGDTR64:
    lgdt [rdi]
    ret

reloadSegment64:
    push rdi
    lea rax, [rel .reloadCS]
    push rax
    retfq
    .reloadCS:
        mov ax, si
        mov ds, ax
        mov es, ax,
        mov fs, ax,
        mov gs, ax
        mov ss, ax
        ret
