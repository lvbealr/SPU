IN ; ZALUPKA
POP AX; ZALUPKA
; ZALUPKA
CALL fact;ZALUPKA
; ZALUPKA
PUSH BX; ZALUPKA
OUT; ZALUPKA
; ZALUPKA
HLT; HUINYA; ZALUPKA
; ZALUPKA
fact:; ZALUPKA
    PUSH AX; ZALUPKA
    PUSH 1; ZALUPKA
    JNE NEXT:; ZALUPKA
        PUSH 1; ZALUPKA
        POP BX; ZALUPKA
        RET; ZALUPKA
    NEXT:; ZALUPKA
; ZALUPKA
    PUSH AX; ZALUPKA
; ZALUPKA
    PUSH AX; ZALUPKA
    PUSH 1; ZALUPKA
    SUB; ZALUPKA
    POP AX; ZALUPKA
; ZALUPKA
    CALL fact; ZALUPKA
; ZALUPKA
    PUSH BX; ZALUPKA
    MUL; ZALUPKA
    POP BX; ZALUPKA
; ZALUPKA
    RET; ZALUPKA



