;----------------------------------------------
; gets passw string
;----------------------------------------------
; entry: none 
; exit:  none
; destr: ax, dx 
;----------------------------------------------

enterpass proc

    mov ax, 0a00h;
    mov dx, offset passbuffer;
    int 21h;

    ret
    endp
;----------------------------------------------

;----------------------------------------------
; checks password
;----------------------------------------------
; entry: passbuffer - set by enterpass
; exit:  none
; destr: 
;----------------------------------------------

passbuffer db 50, 0, 10 dup (0)
etalon     dw 3b18h

checkpass proc

    mov bp, offset passbuffer
    xor ch, ch
    mov cl, [bp + 1]
    add bp, 2;

    mov dx, 420; current

    @@next:

        mov ax, dx;
        mov bl, [bp];
        mul bl;
        mov dx, ax;

        add dx, [bp]


        add bp, 1
        loop @@next


    cmp dx, word ptr etalon
    je  @@good
    jmp @@bad

    @@good:
        mov si, 1
        jmp @@end

    @@bad:
        mov si, 0;
        jmp @@end


    @@end:
    ret
    endp
;----------------------------------------------

;----------------------------------------------
; writes good msg
;----------------------------------------------
; entry: none 
; exit:  none
; destr: 
;----------------------------------------------

GOODCLR equ 004eh
GOODSTYLE   equ 9
goodmsg db "access granted"

drawgood proc

    mov cx, offset drawgood - offset goodmsg
    mov si, GOODCLR
    mov di, GOODSTYLE
    mov bx, offset goodmsg

    call centerall

    ret
    endp
;----------------------------------------------


;----------------------------------------------
; writes bad msg
;----------------------------------------------
; entry: none 
; exit:  none
; destr: 
;----------------------------------------------

BADCLR equ 0004h
BADSTYLE   equ 9
badmsg db "access denied"

drawbad proc

    mov cx, offset drawbad - offset badmsg
    mov si, BADCLR
    mov di, BADSTYLE
    mov bx, offset badmsg

    call centerall

    ret
    endp
;----------------------------------------------

;----------------------------------------------
; writes needed msg
;----------------------------------------------
; entry: si = 0 / 1
; exit:  none
; destr: 
;----------------------------------------------

writemsg proc

    cmp si, 1
    je  @@good
    jmp @@bad

    @@good:
        call drawgood
        jmp @@end

    @@bad:
        call drawbad
        jmp @@end


    @@end:
    ret
    endp
;----------------------------------------------