.286
.model tiny
.code
org 100h
locals @@
jumps

start:
    jmp main

include macro.asm
include lockf.asm

main:

    call enterpass

    myexit

end start