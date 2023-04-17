.286
.model tiny
.code
org 100h
locals @@
jumps

start:
    jmp main

ENDL equ '|'

include macro.asm
include lockf.asm
include frame.asm
include center.asm
include style.asm
include text.asm

main:

    call enterpass
    call checkpass
    call writemsg

    myexit

end start