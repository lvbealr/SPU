IN          ; comment
POP AX

CALL fact   ; one more comment

PUSH BX
OUT

HLT

fact:
    PUSH AX
    PUSH 1
    JNE NEXT:
        PUSH 1
        POP BX
        RET
    NEXT:

    PUSH AX

    PUSH AX
    PUSH 1
    SUB
    POP AX

    CALL fact

    PUSH BX
    MUL
    POP BX
    RET



