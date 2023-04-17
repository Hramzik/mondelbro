

;----------------------------------------------
; loads new isr21h, saves old one
;----------------------------------------------
; entry:   n/a
; exit:    none
; destr:   ax, bx, es, si, di
;----------------------------------------------

;old21h dd 0 at myisr21h.asm


loadnew21h proc

@@start:

    push ds

@@saveold:

    mov ax, 0
    mov ds, ax
    mov si, 21h*4
    mov ax, cs
    mov es, ax
    mov di, offset old21h
    cld
    movsw
    movsw; load old isr21 address to old21h

@@loadnew:

    mov ax, 0
    mov es, ax
    mov bx, 21h*4
    mov ax, offset new21h
    mov es:[bx],   ax; load new isr21 address to 0:[21h*4]
    mov es:[bx+2], cs; load new isr21 segment

@@end:

    pop ds

    ret
    endp
;----------------------------------------------

