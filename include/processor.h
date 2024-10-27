#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <cstdlib>

#include "../Stack/stack.h"
// TODO include

const int    REGISTERS_NUMBER = 5;
const size_t VIDEO_COLUMNS    = 30;
const size_t VIDEO_ROWS       = 30;
const size_t RAM_SIZE         = VIDEO_ROWS * VIDEO_COLUMNS;

struct Processor {
  stack *STACK;
  stack *RETURN_ADDRESS;
  int    commands[MAX_CMDS_SIZE];
  int    RAM[RAM_SIZE];
  int    registers[REGISTERS_NUMBER];
  int    ip;
};

#endif // PROCESSOR_H_