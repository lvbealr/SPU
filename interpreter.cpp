#include <cstdio>

#include "Stack/stack.h"
#include "Stack/privateStack.h"
#include "customWarning/customWarning.h"

extern stack* STACK_PTR;

typedef int valueType;

struct codeArray {
    size_t size;
    size_t sizeValueType;
    int   *data;
};

void codeArrayInitialize(codeArray *ARRAY, size_t codeSize, size_t sizeValueType) {
    customWarning(ARRAY != NULL, (void) 1);

    ARRAY->size          = codeSize;
    ARRAY->sizeValueType = sizeValueType;
    ARRAY->data          = (int *)calloc(ARRAY->size, sizeValueType);

    customWarning(ARRAY->data != NULL, (void) 1);
}

void codeArrayDestruct(codeArray *ARRAY) {
    customWarning(ARRAY != NULL, (void) 1);

    ARRAY->size = 0;
    free(ARRAY->data);
    ARRAY->data = NULL;
}

void printMyStack(stack *STACK) {
    customWarning(STACK != NULL, (void) 1);

    for (size_t index = 0; index < STACK->size; index++) {
        printf("%d ", STACK->data[index]);
    }

    printf("\n");
}

int main() {
    stack *STACK = createPrivateStack();

    initializePrivateStack(STACK, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    FILE *in             = fopen("SPU_code.bin", "rb");
    size_t codeSize      = 0;
    size_t sizeValueType = 0;

    fread(&codeSize,      sizeof(size_t), 1, in);
    fread(&sizeValueType, sizeof(size_t), 1, in);

    codeArray CODE = {};
    codeArrayInitialize(&CODE, codeSize, sizeValueType);
    fread(CODE.data, sizeValueType, codeSize, in);

    int ip = 0;

    while (1) {
        int cmd = *(CODE.data + ip);
        
        if (cmd == 1) {
            valueType arg = *(CODE.data + ++ip);
            stackPush(STACK, arg);
            ++ip;
            continue;
        }

        if (cmd == 2) {
            valueType arg = 0;
            stackPop(STACK, &arg);
            ++ip;
            continue;
        }

        if (cmd == 3) {
            valueType first  = 0;
            valueType second = 0;
            stackPop(STACK, &first);
            stackPop(STACK, &second);
            stackPush(STACK, first + second);
            ++ip;
            continue;
        }

        if (cmd == 4) {
            valueType first  = 0;
            valueType second = 0;
            stackPop(STACK, &first);
            stackPop(STACK, &second);
            stackPush(STACK, second - first);
            ++ip;
            continue;
        }
        
        if (cmd == 5) {
            valueType arg = 0;
            stackPush(STACK, stackPop(STACK, &arg) * stackPop(STACK, &arg));
            ++ip;
            continue;
        }

        if (cmd == 6) {
            valueType first  = 0;
            valueType second = 0;
            stackPop(STACK, &first);
            stackPop(STACK, &second);
            stackPush(STACK, second / first);
            ++ip;
            continue;
        }

        if (cmd == 7) {
            // ! FORGOT
            ++ip;
            continue;
        }

        if (cmd == 8) {
            printMyStack(STACK);
            ++ip;
            continue;
        }

        if (cmd == 9) {
            // TODO
            ++ip;
            continue;
        }

        if (cmd == 10) {
            break;
        }

        printf("Syntax Error: [COMMAND] %d\n", cmd);
        break;
    }

    codeArrayDestruct(&CODE);
    fclose(in);
}