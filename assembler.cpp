#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "customWarning/customWarning.h"

typedef int valueType;

static const size_t MAX_CODE_SIZE = 200;

struct codeArray {
    size_t       capacity;
    size_t       size;
    valueType   *data;
};

static void codeArrayInitialize(codeArray *ARRAY) {
    customWarning(ARRAY != NULL, (void) 1);

    ARRAY->capacity = MAX_CODE_SIZE;
    ARRAY->size     = 0;
    ARRAY->data     = (valueType *)calloc(ARRAY->capacity, sizeof(valueType));

    customWarning(ARRAY->data != NULL, (void) 1);
}

static void codeArrayDestruct(codeArray *ARRAY) {
    customWarning(ARRAY != NULL, (void) 1);

    ARRAY->capacity = 0;
    ARRAY->size     = 0;
    free(ARRAY->data);
    ARRAY->data     = NULL;
}

static void codeArrayExpand(codeArray *ARRAY) {
    size_t newCapacity = ARRAY->capacity * 2;

    ARRAY->data = (valueType *)realloc(ARRAY->data, sizeof(valueType) * newCapacity);

    customWarning(ARRAY->data != NULL, (void) 1);

    ARRAY->capacity *= 2;
}

static void codeArrayPush(codeArray *ARRAY, valueType value) {
    customWarning(ARRAY != NULL, (void) 1);

    if (ARRAY->size >= ARRAY->capacity) {
        codeArrayExpand(ARRAY);
    }

    ARRAY->data[ARRAY->size++] = value;
}

enum cmdSPU {
    CMD_PUSH = 1,
    CMD_POP  = 2,
    CMD_ADD  = 3,
    CMD_SUB  = 4,
    CMD_MUL  = 5,
    CMD_DIV  = 6,
    CMD_IN   = 7,
    CMD_OUT  = 8,
    CMD_DMP  = 9,
    CMD_HLT  = 10
};

int main() {
    codeArray CODE = {};
    codeArrayInitialize(&CODE);

    FILE  *in                  = fopen("asmCode.asm", "r");
    char   word[100]           = {};

    size_t ip                  =  0; 

    while (fscanf(in, "%s", word) != EOF) {
        if (strcmp(word, "push") == 0) {
            codeArrayPush(&CODE, CMD_PUSH);
            continue;
        }

        if (strcmp(word, "pop") == 0) {
            codeArrayPush(&CODE, CMD_POP);
            continue;
        }

        if (strcmp(word, "add") == 0) {
            codeArrayPush(&CODE, CMD_ADD);
            continue;
        }

        if (strcmp(word, "sub") == 0) {
            codeArrayPush(&CODE, CMD_SUB);
            continue;
        }

        if (strcmp(word, "mul") == 0) {
            codeArrayPush(&CODE, CMD_MUL);
            continue;
        }

        if (strcmp(word, "div") == 0) {
            codeArrayPush(&CODE, CMD_DIV);
            continue;
        }

        if (strcmp(word, "in") == 0) {
            codeArrayPush(&CODE, CMD_IN);
            continue;
        }

        if (strcmp(word, "out") == 0) {
            codeArrayPush(&CODE, CMD_OUT);
            continue;
        }

        if (strcmp(word, "dmp") == 0) {
            codeArrayPush(&CODE, CMD_DMP);
            continue;
        }

        if (strcmp(word, "hlt") == 0) {
            codeArrayPush(&CODE, CMD_HLT);
            continue;
        }

        codeArrayPush(&CODE, atoi(word));
    }   

    fclose(in);

    size_t sizeValueType = sizeof(valueType);

    FILE *out = fopen("SPU_code.bin", "wb");
    fwrite(&CODE.size, sizeof(size_t), 1, out);
    fwrite(&sizeValueType, sizeof(size_t), 1, out);
    fwrite(CODE.data, sizeof(valueType), CODE.size, out);
    fclose(out);

    codeArrayDestruct(&CODE);
}