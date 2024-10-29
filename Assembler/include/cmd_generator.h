CMD_(HLT, -1,
{
  return 0;
})

CMD_(PUSH, 0,
{
  PROCESSOR.ip = push(&PROCESSOR);

  continue;
})

CMD_(POP, 1, {
  PROCESSOR.ip = pop(&PROCESSOR);

  continue;
})

CMD_(ADD, 2,
{
  int firstValue  = 0;
  int secondValue = 0;
  stackPop (PROCESSOR.STACK, &firstValue);
  stackPop (PROCESSOR.STACK, &secondValue);
  stackPush(PROCESSOR.STACK, firstValue + secondValue);

  continue;
})

CMD_(SUB, 3,
{
  int firstValue  = 0;
  int secondValue = 0;
  stackPop (PROCESSOR.STACK, &firstValue);
  stackPop (PROCESSOR.STACK, &secondValue);
  stackPush(PROCESSOR.STACK, secondValue - firstValue);

  continue;
})

CMD_(MUL, 4,
{
  int firstValue  = 0;
  int secondValue = 0;
  stackPop (PROCESSOR.STACK, &firstValue);
  stackPop (PROCESSOR.STACK, &secondValue);
  stackPush(PROCESSOR.STACK, firstValue * secondValue);

  continue;
})

CMD_(DIV, 5,
{
  int firstValue  = 0;
  int secondValue = 0;
  stackPop (PROCESSOR.STACK, &firstValue);
  stackPop (PROCESSOR.STACK, &secondValue);
  stackPush(PROCESSOR.STACK, secondValue / firstValue);

  continue;
})

CMD_(IN, 6,
{
  int value = 0;

  scanf    ("%d", &value);
  stackPush(PROCESSOR.STACK, value);

  continue;
})

CMD_(OUT, 7,
{
  int value = 0;
  stackPop(PROCESSOR.STACK, &value);
  printf  ("%d\n", value);

  continue;
})

CMD_(JMP, 8,
{
  PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];

  continue;
})

CMD_(JA, 9,
{
  int firstValue  = 0;
  int secondValue = 0;

  stackPop(PROCESSOR.STACK, &secondValue);
  stackPop(PROCESSOR.STACK, &firstValue);

  if (firstValue > secondValue) {
    PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];
  }

  else {
    PROCESSOR.ip++;
  }

  continue;
})

CMD_(JAE, 10,
{
  int firstValue  = 0;
  int secondValue = 0;

  stackPop(PROCESSOR.STACK, &secondValue);
  stackPop(PROCESSOR.STACK, &firstValue);

  if (firstValue >= secondValue) {
    PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];
  }

  else {
    PROCESSOR.ip++;
  }

  continue;
})

CMD_(JB, 11,
{
  int firstValue  = 0;
  int secondValue = 0;

  stackPop(PROCESSOR.STACK, &secondValue);
  stackPop(PROCESSOR.STACK, &firstValue);

  if (firstValue < secondValue) {
    PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];
  }

  else {
    PROCESSOR.ip++;
  }

  continue;
})

CMD_(JBE, 12,
{
  int firstValue  = 0;
  int secondValue = 0;

  stackPop(PROCESSOR.STACK, &secondValue);
  stackPop(PROCESSOR.STACK, &firstValue);

  if (firstValue <= secondValue) {
    PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];
  }

  else {
    PROCESSOR.ip++;
  }

  continue;
})

CMD_(JE, 13,
{
  int firstValue  = 0;
  int secondValue = 0;

  stackPop(PROCESSOR.STACK, &secondValue);
  stackPop(PROCESSOR.STACK, &firstValue);

  if (firstValue == secondValue) {
    PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];
  }

  else {
    PROCESSOR.ip++;
  }

  continue;
})

CMD_(JNE, 14,
{
  int firstValue  = 0;
  int secondValue = 0;

  stackPop(PROCESSOR.STACK, &secondValue);
  stackPop(PROCESSOR.STACK, &firstValue);

  if (firstValue != secondValue) {
    PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];
  }

  else {
    PROCESSOR.ip++;
  }

  continue;
})

CMD_(DRAW, 15,
{
  for (size_t row = 0; row < VIDEO_ROWS; row++) {
    for (size_t column = 0; column < VIDEO_COLUMNS; column++) {
      printf(" %c", PROCESSOR.RAM[column + row * VIDEO_ROWS]);
    }

    printf("\n");
  }

  continue;
})

CMD_(CALL, 16,
{
  stackPush(PROCESSOR.RETURN_ADDRESS, PROCESSOR.ip + 1);
  PROCESSOR.ip = PROCESSOR.commands[PROCESSOR.ip];

  continue;
})

CMD_(RET, 17,
{
  stackPop(PROCESSOR.RETURN_ADDRESS, &PROCESSOR.ip);

  continue;
})

CMD_(SQRT, 18,
{
  int value = 0;
  stackPop(PROCESSOR.STACK, &value);

  value  = (int)sqrt((double)value * 100);
  value /= 10;

  stackPush(PROCESSOR.STACK, value);

  continue;
})