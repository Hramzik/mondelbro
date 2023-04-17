

;----------------------------------------------
; writes the text in entered place (| = \n)
;----------------------------------------------
; entry:   dh, dl - coords of top-left
;          cx     - len
;          si     - color
;          di     - first text byte offset
; exit:    none
; destr:   ax, bx, bp, cx, di
;----------------------------------------------
writetext proc
    loadvideoesbx

    mov ax, 80; calculating offset
    mul dl
    add al, dh
    mov bp, 2;
    mul bp
    mov bx, ax; bx = start

    xor bp, bp; written on the line
    mov dx, si; color


    @@writechar:
        mov al,     [di]
        cmp al, ENDL; cmp to '|'
        je @@newline

        @@oldline:
            mov ES:[bx],   al
            mov ES:[bx+1], dl; color
            jmp @@end

        @@newline:
            mov ax, 80
            sub ax, bp; written on the line
            mov dh, 2
            mul dh
            add bx, ax; to the next line start
            xor bp, bp; 0 symbols written on the line
            sub bx, 2;  bc of @@end
            sub bp, 1;  bc of @@end
            jmp @@end

        @@end:
        add di, 1; next char
        add bx, 2; next space

        add bp, 1; written on the line
        loop @@writechar

    ret
    endp

