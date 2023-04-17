

;----------------------------------------------
; draws a frame using 9-simbol string
;----------------------------------------------
; entry:   dh, dl   - coords of top-left
;          ch, cl   - w, h
;          si       - color
;         [bp], ... - simbols for the frame
; exit:    none
; destr:   ax, bx, cx, dx, di
;----------------------------------------------

drawaframe proc

    loadvideoesbx

    mov ax, 80d; calculating offset
    mul dl
    add al, dh
    mov di, 2d;
    mul di
    mov bx, ax; bx = start offset

    mov al, ch; width
    xor ch, ch
    mov cl, cl; counter


    mov di, cx; save cx

    mov dh, [bp];   left
    mov dl, [bp+1]; mid
    mov ah, [bp+2]; right
    mov al, al;     width
    call drawaline

    mov cx, di

    @@next:
        mov di, cx; save cx

        mov dh, [bp+3]; left
        mov dl, [bp+4]; mid
        mov ah, [bp+5]; right
        mov al, al;     width
        call drawaline

        mov cx, di

    loop @@next

    mov dh, [bp+6]; left
    mov dl, [bp+7]; mid
    mov ah, [bp+8]; right
    mov al, al;     width
    call drawaline


    ret
    endp

;----------------------------------------------

;----------------------------------------------
; draws a line
;----------------------------------------------
; entry:   dh, dl - left, mid ascii
;          ah, al - right ascii, width
;          bx     - start offset
;          si     - color
; exit:    bx = <next line start>
; assumes: ES = vidmem addr
; destr:   ah, bx, cx
;----------------------------------------------

drawaline proc

    mov ES:[bx],   dh
    mov ES:[bx+1], si; color
    add bx, 2

    xor ch, ch
    mov cl, al
    sub cl, 2d; крайние
    @@next:
        mov ES:[bx],   dl
        mov ES:[bx+1], si; color
        add bx, 2

    loop @@next

    mov ES:[bx],   ah
    mov ES:[bx+1], si; color
    add bx, 2

    xor ah, ah
    sub bx, ax
    sub bx, ax
    add bx, 160d

    ret
    endp

;----------------------------------------------