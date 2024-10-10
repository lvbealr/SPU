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
            CODE.data[ip++] = CMD_PUSH;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "pop") == 0) {
            CODE.data[ip++] = CMD_POP;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "add") == 0) {
            CODE.data[ip++] = CMD_ADD;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "sub") == 0) {
            CODE.data[ip++] = CMD_SUB;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "mul") == 0) {
            CODE.data[ip++] = CMD_MUL;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "div") == 0) {
            CODE.data[ip++] = CMD_DIV;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "in") == 0) {
            CODE.data[ip++] = CMD_IN;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "out") == 0) {
            CODE.data[ip++] = CMD_OUT;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "dmp") == 0) {
            CODE.data[ip++] = CMD_DMP;
            CODE.size++;
            continue;
        }

        if (strcmp(word, "hlt") == 0) {
            CODE.data[ip++] = CMD_HLT;
            CODE.size++;
            continue;
        }

        CODE.data[ip++]     = atoi(word); // TODO myTypeConversion or ???
        CODE.size++;
    }   

    fclose(in);

    FILE *out = fopen("SPU_code.bin", "wb");
    fwrite(CODE.data, sizeof(valueType), CODE.size, out);
    fclose(out);

    codeArrayDestruct(&CODE);
}