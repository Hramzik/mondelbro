

;----------------------------------------------
; draws a centered frame with text
;----------------------------------------------
; entry:   cx       - len
;          si       - color
;          di       - number of the style
;         [bp], ... - simbols for the frame (style 0)
;          bx       - first text byte offset
; exit:    none
; destr:   ax, bx, bp, cx, di
;----------------------------------------------

centerall proc

    push cx
    push si
    call countsize
    pop si
    pop cx

    push cx; <------------------
    mov ax, 80
    sub al, dh
    mov cl, 2
    div cl

    mov ch, dh; width
    add ch, 2; a bit wider
    mov dh, al; x-cord
    sub dh, 1; a bit wider **

    mov ax, 25
    sub al, dl
    mov cl, 2
    div cl

    mov cl, dl; height
    ;add cl, 2; a bit wider; NOT, because top and bottom lines does not count!
    mov dl, al; y-cord
    sub dl, 1; a bit wider **

;   atually drawing: 

    push bx
    push dx
    call drawstyled
    pop dx
    pop bx

    pop cx; <------------------
    add dh, 1; **
    add dl, 1; **
    mov di, bx
    call writetext


    ret
    endp

;----------------------------------------------

;----------------------------------------------
; counts width and height of the string
;----------------------------------------------
; entry:   cx       - len
;          es:bx    - first text byte offset
; exit:    dh - width
;          dl - height
; destr:   ah, cx, si
;----------------------------------------------

countsize proc

    xor ah, ah; cur width
    xor dh, dh; max width
    xor dl, dl; max height
    add dl, 1
    add dh, 1; bug fix


    xor si, si
    @@count:
        cmp es:[bx+si], byte ptr ENDL; cmp to '|'
        je @@endl

        @@symbol:
            add ah, 1; current width
            jmp @@next

        @@endl:
            add dl, 1; max height

            cmp ah, dh
            jg  @@newmaxwidth
            jmp @@widthtozero

            @@newmaxwidth:
                mov dh, ah
                jmp @@widthtozero

            @@widthtozero:
                xor ah, ah

        @@next:
            add si, 1

    loop @@count


    cmp ah, dh; final compare
    jle  @@end

    mov dh, ah


    @@end:
    ret
    endp