PUSH 0
PUSH 0
POP AX
POP BX

FIRST:

    PUSH AX
    PUSH -15
    ADD
    PUSH AX
    PUSH -15
    ADD
    MUL
    PUSH BX
    PUSH -15
    ADD
    PUSH BX
    PUSH -15
    ADD
    MUL
    ADD
    PUSH 25
    JA NEXT:

        PUSH 42
        PUSH BX
        PUSH 30
        MUL
        POP CX
        PUSH AX
        PUSH CX
        ADD
        POP CX
        POP [CX]

    NEXT:

    PUSH AX
    PUSH -15
    ADD
    PUSH AX
    PUSH -15
    ADD
    MUL
    PUSH BX
    PUSH -15
    ADD
    PUSH BX
    PUSH -15
    ADD
    MUL
    ADD
    PUSH 25
    JBE ELSENEXT:

        PUSH 46
        PUSH BX
        PUSH 30
        MUL
        POP CX
        PUSH AX
        PUSH CX
        ADD
        POP CX
        POP [CX]

    ELSENEXT:

    PUSH 1
    PUSH AX
    ADD
    POP AX

    PUSH AX
    PUSH 30
    JNE FIRST_IF:

        PUSH 1
        PUSH BX
        ADD
        POP BX

        PUSH 0
        POP AX

    FIRST_IF:

    PUSH BX
    PUSH 30
    MUL
    POP CX
    PUSH AX
    PUSH CX
    ADD
    PUSH 900
    JB FIRST:

DRAW

HLT
