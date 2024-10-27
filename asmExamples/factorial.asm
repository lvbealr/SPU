IN
POP AX

CALL FACT:

PUSH BX
OUT

HLT

FACT:
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

    CALL FACT:

    PUSH BX
    MUL
    POP BX

    RET
