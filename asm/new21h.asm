

;----------------------------------------------
; if ah == 0a, sets return_ip to 01a5
; always calls old 21h
;----------------------------------------------
; entry:   n/a
; exit:    none
; destr:   none
;----------------------------------------------

new21h proc

@@check:

    cmp ah, 0ah
    jne @@wrongax
    jmp @@start

@@wrongax:

    db 0eah; jmp far, to old isr

old21h dd 0

@@start:

    push bx

@@main:

    mov bx, sp
    mov ss:[bx + (1) * 2], 01a5h

@@exit:

    pop bx

    jmp @@wrongax
    endp

;----------------------------------------------



