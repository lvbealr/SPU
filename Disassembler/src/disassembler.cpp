#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

#include "textStruct.h"
#include "cmds.h"

int main(int argc, const char *argv[]) {
  // TODO fileName
  const char *fileName = "../../build/SPU_code.txt";
  // const char *outputFileName = "dasm_code.txt";

  textData text = {};

  textDataInitialize(fileName, &text);

  int    cmd   = 0;
  size_t index = 0;

  // КОМАНДЫ БЕЗ АРГУМЕНТОВ (ИХ ПРОСТО ПЕЧАТАТЬ, СДВИГАТЬ ИНДЕКС НА 1):
  // HLT, ADD, SUB, MUL, DIV, IN, OUT, DRAW, RET, SQRT,

  // КОМАНДЫ С АРГУМЕНТАМИ (ИХ НУЖНО ВЫЧИСЛЯТЬ ЧЕРЕЗ КУЧУ ПРОВЕРОК)

  // PUSH & POP \\
  \\ __________ //

  // PUSH (PUSH | REG, PUSH | IMMED, (PUSH | IMMED) | REG)
  // PUSH | REG: сдвигаем index, считываем первую букву регистра (A = 1, B = 2, ...) И ПИШЕМ РЕГИСТР
  // PUSH | IMMED: сдвигаем index, считываем аргумент
  // PUSH | IMMED | REG cдвигаем индекс, считываем аргумент!! (число), сдвигаем индекс, считываем первую букву регистра,
  // записываем без разницы в каком порядке [AX + 5] или наоборот

  // JMP, JA, JAE, JB, JBE, JE, JNE, CALL \\
  \\ ___________________________________  //

  // сдвигаем индекс, считываем название метки (точнее ее индекс (адрес инициализации))
  // можно дополнять запись в выходной файл как call LABEL_<index> и LABEL_<index>:
  // когда потом в очередной раз будем считывать команду, индекс метки по хорошему не должен совпасть ни с чем другим
  // и тогда печатаем LABEL_<считанное_число>:

  // ОТДЕЛЬНО ОБРАБОТАТЬ [CX], [5]

  // TODO ЧЕ ДЕЛАТЬ С ИНИЦИАЛИЗАЦИЕЙ МЕТОК? ПРЫЖКИ И CALL ОТРАБАТЫВАЮТ НОРМАЛЬНО, НО НЕ ПОНЯТНО ГДЕ ЭТА МЕТКА ОПИСЫВАЕТСЯ
  // TODO МБ ПОПРОБОВАТЬ СОХРАНЯТЬ АДРЕСА МЕТОК В АССЕМБЛЕРНЫЙ ФАЙЛ А В ПРОЦЕ ИХ УБИРАТЬ

  while (sscanf(text.lineArray[index].linePointer, "%d", &cmd) && cmd != -2) { // TODO MAGIC
    if (cmd == (PUSH | REG)) {
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &cmd);
      printf("PUSH %cX\n", cmd + 'A' - 1); // DO BETTER NO MAGIC
    }

    else if (cmd == (POP | REG)) {
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &cmd);
      printf("POP %cX\n", cmd + 'A' - 1); // DO BETTER NO MAGIC
    }

    else if (cmd == (PUSH | IMMED)) {
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &cmd);
      printf("PUSH %d\n", cmd);
    }

    else if (cmd == (POP | IMMED)) {
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &cmd);
      printf("POP %d\n", cmd);
    }

    else if (cmd == ((PUSH | IMMED) | REG)) {
      int number   = 0;
      int reg      = 0;
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &number);
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &reg);
      printf("PUSH [%cX + %d]\n", reg + 'A' - 1, number);
    }

    else if (cmd == ((POP | IMMED) | REG)) {
      int number   = 0;
      int reg      = 0;
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &number);
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &reg);
      printf("POP [%cX + %d]\n", reg + 'A' - 1, number);
    }

    else if (cmd == ((MEM | REG) | PUSH)) {
      int reg = 0;
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &reg);
      printf("PUSH [%cX]\n", reg + 'A' - 1);
    }

    else if (cmd == ((MEM | REG) | POP)) {
      int reg = 0;
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &reg);
      printf("POP [%cX]\n", reg + 'A' - 1);
    }

    else if (cmd == ((MEM | IMMED) | PUSH)) {
      int number = 0;
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &number);
      printf("PUSH [%d]\n", number);
    }

    else if (cmd == ((MEM | IMMED) | POP)) {
      int number = 0;
      index++;
      sscanf(text.lineArray[index].linePointer, "%d", &number);
      printf("POP [%d]\n", number);
    }

    else if ((JMP <= cmd) && (cmd <= JNE)) {
      index++;
      int label = 0;
      sscanf(text.lineArray[index].linePointer, "%d", &label);
      printf("%d LABEL_%d:\n", cmd, label); // обернуть в макрос, пока что код команды
    }

    else if (cmd == CALL) {
      index++;
      int label = 0;
      sscanf(text.lineArray[index].linePointer, "%d", &label);
      printf("CALL LABEL_%d\n", label);
    }

    else {
      printf("%d\n", cmd);
    }

    index++;
  }

  textDataDestruct(&text);
}