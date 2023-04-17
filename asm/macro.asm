

VIDMEM equ 0b800h

;----------------------------------
; loads ES with video seg addr
;----------------------------------
; entry:    none
; exit:     ES = 0b800h
; destroys: bp; самая важная часть!
;----------------------------------
loadvideoesbp macro

    nop
    mov bp, VIDMEM
    mov ES, bp
    nop

    endm
;----------------------------------

;----------------------------------
; loads ES with video seg addr
;----------------------------------
; entry:    none
; exit:     ES = 0b800h
; destroys: bx; самая важная часть!
;----------------------------------
loadvideoesbx macro

    nop
    mov bx, VIDMEM
    mov ES, bx
    nop

    endm
;----------------------------------

;----------------------------------------------
; exit to dos
;----------------------------------------------
; entry: n/a
; exit:  n/a
; destr: n/a
;----------------------------------------------
myexit macro

    nop
    mov ax, 4c00h
    int 21h
    nop

    endm
;----------------------------------------------

;----------------------------------------------
; exit to dos, stay residented
;----------------------------------------------
; entry: n/a
; exit:  n/a
; destr: n/a
;----------------------------------------------
myexitresident macro

    nop
    mov ax, 3100h
    int 21h
    nop

    endm
;----------------------------------------------

;----------------------------------------------
; loads dx with needed memory to stay residented up to main:
;----------------------------------------------
; entry: n/a
; exit:  n/a
; destr: n/a
;----------------------------------------------
loaddxresident macro

    nop
    mov dx, offset main
    shr dx, 4
    add dx, 1
    nop

    endm
;----------------------------------------------


;----------------------------------------------
; loads bp with calculated offset in vidmem
;----------------------------------------------
; entry: dh:dl - coords
; exit:  none
; destr: ax, bp
;----------------------------------------------

calculateoffsetbp macro

    mov ax, 160d; calculating offset
    mul dl
    mov bp, dx
    shr bp, 8
    add ax, bp
    add ax, bp

    mov bp, ax; bp = needed

    endm
;----------------------------------------------

;----------------------------------------------
; loads bx with calculated offset in vidmem
;----------------------------------------------
; entry: dh:dl - coords
; exit:  none
; destr: ax, bx (controlled)
;----------------------------------------------

calculateoffsetbx macro

    mov ax, 160d; calculating offset
    mul dl
    xor bh, bh
    mov bl, dh
    add ax, bx
    add ax, bx

    mov bx, ax; bp = needed

    endm
;----------------------------------------------



;----------------------------------------------
; loads ds register with cs register value
; saves old ds value
;----------------------------------------------
; entry: none
; exit:  ds = cs
; destr: none
;----------------------------------------------

oldds  dw 0
buffer dw 0

movdscs macro
    nop

    mov cs:oldds, ds

    mov cs:buffer, cs
    mov ds, cs:buffer 

    nop
    endm
;----------------------------------------------


;----------------------------------------------
; checks movedflag, jumps to end09
;----------------------------------------------
; entry: none
; exit:  none
; destr: none
;----------------------------------------------

checkmovedflag macro
    nop

    cmp byte ptr movedflag, 1
    je exit09

    nop
    endm
;----------------------------------------------