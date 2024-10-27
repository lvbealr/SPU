#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "../Stack/stack.h"
#include "../include/cmds.h"
#include "../include/processor.h"
#include "../Stack/privateStack.h"
#include "../customWarning/customWarning.h"

static int run     ();
static int readFile(FILE *fp, int *commands);
static int push    (Processor *PROCESSOR);
static int pop     (Processor *PROCESSOR);

static int run() {
    // TODO STACK FIELD BY MACRO
    // TODO STACK CREATING
    // stack *STACK     = createPrivateStack();
    // stack *RET_STACK = createPrivateStack();
    // initializePrivateStack(    STACK, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    // initializePrivateStack(RET_STACK, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    // TODO WTF IT IS ERROR

    stack *STACK     = createPrivateStack();
    initializePrivateStack(    STACK, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    stack *RET_STACK = createPrivateStack();
    initializePrivateStack(RET_STACK, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    Processor PROCESSOR = {.STACK          = STACK,
                           .RETURN_ADDRESS = RET_STACK,
                           .commands       = {},
                           .RAM            = {},
                           .registers      = {},
                           .ip             = 0};

    // TODO OPEN FILE BY CONSOLE
    FILE *fp = fopen("SPU_code.txt", "r");
    customWarning(fp != NULL, 1);

    readFile(fp, PROCESSOR.commands);

    #define CMD_(name, cmdCode, ...) \
    case(name): {                    \
        __VA_ARGS__                  \
    }

    while (1) {
        int command       = PROCESSOR.commands[PROCESSOR.ip++];
        int maskedCommand = command & 31; // TODO MAGIC
        if (command == HLT || command == END_OF_COMMANDS) {
            maskedCommand = HLT;
        }

        switch (maskedCommand) {
            #include "../include/cmd_generator.h"
        }
    }

    destructPrivateStack(STACK);
    destructPrivateStack(RET_STACK);

    PROCESSOR.STACK          = NULL;
    PROCESSOR.RETURN_ADDRESS = NULL;

    // TODO FUCKING MEMORY LEAKS IN STACK

    #undef CMD_
}

static int readFile(FILE *fp, int *commands) {
    customWarning(fp       != NULL, 1);
    customWarning(commands != NULL, 1);

    int command = 0;
    int ip      = 0;

    while (1) {
        fscanf(fp, "%d", &command);
        commands[ip++] = command;

        if (commands[ip - 1] == END_OF_COMMANDS) {
            break;
        }
    }

    return 0;
}

static int push(Processor *PROCESSOR) {
    customWarning(PROCESSOR != NULL, 1);

    int command = PROCESSOR->commands[PROCESSOR->ip - 1];

    if ((command & IMMED) && (command & REG)) {
        int firstValue  = PROCESSOR->commands[PROCESSOR->ip++];
        int secondValue = PROCESSOR->registers[PROCESSOR->commands[PROCESSOR->ip++]];

        if (command & MEM) {
            stackPush(PROCESSOR->STACK, PROCESSOR->RAM[firstValue + secondValue]);

            return PROCESSOR->ip;
        }

        stackPush(PROCESSOR->STACK, firstValue + secondValue);

        return PROCESSOR->ip;
    }

    if (command & IMMED) {
        int firstValue = PROCESSOR->commands[PROCESSOR->ip++];

        if (command & MEM) {
            stackPush(PROCESSOR->STACK, firstValue);

            return PROCESSOR->ip;
        }

        stackPush(PROCESSOR->STACK, firstValue);

        return PROCESSOR->ip;
    }

    if (command & REG) {
        int firstValue = PROCESSOR->registers[PROCESSOR->commands[PROCESSOR->ip++]];

        if (command & MEM) {
            stackPush(PROCESSOR->STACK, PROCESSOR->RAM[firstValue]);

            return PROCESSOR->ip;
        }

        stackPush(PROCESSOR->STACK, firstValue);

        return PROCESSOR->ip;
    }

    return PROCESSOR->ip;
}

static int pop(Processor *PROCESSOR) {
    customWarning(PROCESSOR != NULL, 1);

    int command = PROCESSOR->commands[PROCESSOR->ip - 1];

     if ((command & IMMED) && (command & REG)){
        int firstValue  = PROCESSOR->commands[PROCESSOR->ip++];
        int secondValue = PROCESSOR->registers[PROCESSOR->commands[PROCESSOR->ip++]];
        int ramValue    = 0;

        stackPop(PROCESSOR->STACK, &ramValue);
        PROCESSOR->RAM[firstValue + secondValue] = ramValue;

        return PROCESSOR->ip;
    }
    if (command & IMMED){
        int value     = PROCESSOR->commands[PROCESSOR->ip++];
        int ramValue = 0;

        stackPop(PROCESSOR->STACK, &ramValue);
        PROCESSOR->RAM[value] = ramValue;

        return PROCESSOR->ip;
    }
    if ((command & REG) && ((command & MEM) == 0)){
        int value = 0;

        stackPop(PROCESSOR->STACK, &value);
        PROCESSOR->registers[PROCESSOR->commands[PROCESSOR->ip++]] = value;

        return PROCESSOR->ip;
    }
    if ((command & REG)){
        int ramValue = 0;

        stackPop(PROCESSOR->STACK, &ramValue);
        PROCESSOR->RAM[PROCESSOR->registers[PROCESSOR->commands[PROCESSOR->ip++]]] = ramValue;

        return PROCESSOR->ip;
    }

    return PROCESSOR->ip;
}

int main() {
    run();

    return 0;
}