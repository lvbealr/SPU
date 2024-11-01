in
in
in

pop cx
pop bx
pop ax

call quadratka

push cx
push ex
push fx
out
out
out

hlt

quadratka:
  push ax
  push 0

  jne next1:
    push bx
    push 0

    jne next2:
      push cx
      push 0

      jne next3:
        push 8
        pop cx
        push 0
        push 0
        pop ex
        pop fx

        ret

      next3:
        push -1
        pop cx
        push 0
        push 0
        pop fx
        pop ex

        ret

    next2:
      call linear
      ret

  next1:
    push bx
    push bx
    mul
    push 4
    push ax
    push cx
    mul
    mul
    sub
    pop dx
    push dx
    push 0

    jae next4:
      push -1
      pop cx
      push cx
      pop ex
      push cx
      pop fx
      ret

    next4:
      push dx
      sqrt
      pop dx

      push bx
      push -1
      mul
      push dx
      add
      push ax
      push 2
      mul
      div
      pop ex

      push bx
      push -1
      mul
      push dx
      sub
      push ax
      push 2
      mul
      div
      pop fx

      push ex
      push fx

      je next5:
        push 2
        pop cx

        ret

      next5:
        push 1
        pop cx

ret

linear:
  push -1
  push cx
  mul
  push bx
  div
  pop ex
  push ex
  pop fx
  push 1
  pop cx

ret