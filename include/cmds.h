#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <cstdlib>

const size_t MAX_CMDS_SIZE = 512;
const size_t MAX_CMD_SIZE  = 32;

#define CMD_(name, cmdCode, ...) name = cmdCode,

enum Commands {
  END_OF_COMMANDS = -2,
  #include "cmd_generator.h"
};

enum Bits {
  MEM   = 32,
  REG   = 64,
  IMMED = 128
};

#undef CMD_


#endif // COMMANDS_H_