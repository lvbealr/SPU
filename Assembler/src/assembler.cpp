#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "colorPrint.h"
#include "assembler.h"
#include "cmds.h"
#include "labels.h"
#include "customWarning.h"

// TODO ITS DEBUG MACRO AND CONSTANT
// TODO ON_DEBUG
static const size_t MAX_TRASH_BUFFER_SIZE = 100;

#define CLEAN_INPUT_LINE(stream) {               \
    char trash[MAX_TRASH_BUFFER_SIZE] = {};      \
    printf("Clean Buffer...\n");                 \
    fscanf(stream, "%[^\r\n]%*1[\r\n]", trash);  \
    printf("Trash: [%s]\n", trash);              \
    *trash = {};                                 \
}

#define CREATE_CLEAR_COMMAND(dest, src) {        \
    char *ptrToSymbol = strchr(src, ';');        \
                                                 \
    if (ptrToSymbol) {                           \
        strncpy(dest, src, ptrToSymbol - src);   \
    }                                            \
                                                 \
    else {                                       \
        strcpy(dest, src);                       \
    }                                            \
}


// TODO ОЧЕНЬ ПЛОХО
#define CHECK_IF_COMMANDS_USE_FSTREAM() {        \
    if (customStrcmp(command, "PUSH") &&         \
        customStrcmp(command, "POP" ) &&         \
        customStrcmp(command, "CALL") &&         \
        customStrcmp(command, "JA"  ) &&         \
        customStrcmp(command, "JAE" ) &&         \
        customStrcmp(command, "JB"  ) &&         \
        customStrcmp(command, "JBE" ) &&         \
        customStrcmp(command, "JE"  ) &&         \
        customStrcmp(command, "JNE" ) &&         \
        customStrcmp(command, "JMP" )) {         \
                                                 \
        CLEAN_INPUT_LINE(inCommands);            \
    }                                            \
}

typedef int valueType; // TODO NOT USED

static       int   customStrcmp      (char *firstString, char *secondString);
static const char *parseConsole      (int argc, char *argv[]);
static       void  run               (int commands[], int argc, char *argv[]);
static       void  initializeCommands(int commands[], const char *asmFileName);
static       void  initializeExeFile (int commands[]);

static       int   setLabel          (Label *labels, char *command, int ip);
static       void  goThroughLabels   (Label LABELS[], int commands[]);
static       void  putLabelAddress   (Label LABELS[], int *commands, int *ip, FILE *inCommands);

static       int   getArg            (const char *inputLine, int commands[], int ip);
static       int   getArgSD          (int *commands, char *registerName, int commandType, int argument, int ip);
static       int   getArgD           (int *commands, int commandType, int argument, int ip);
static       int   getArgS           (int *commands, char *registerName, int commandType, int ip);
static       int   setArg            (FILE *inCommands, int *commands, Label *labels, int ip, int type);
static       int   setArgPushPop     (FILE *inCommands, int *commands, int ip, int type);
static       int   isRegister        (const char *string);

static int customStrcmp(char *firstString, char *secondString) {
    customWarning(firstString  != NULL, 1);
    customWarning(secondString != NULL, 1);

    size_t firstLen     = strlen(firstString),
           secondLen    = strlen(secondString);

    char *firstStr  = (char *)calloc(firstLen,  sizeof(char));
    char *secondStr = (char *)calloc(secondLen, sizeof(char));

    strcpy(firstStr,  firstString);
    strcpy(secondStr, secondString);

    *(firstStr  + firstLen)  = EOF;
    *(secondStr + secondLen) = EOF;

    size_t firstIndex   = 0, secondIndex   = 0;
    int    firstElement = 0, secondElement = 0;

    int asciiDiff = 0;

    #define FREE_() {     \
        free(firstStr);   \
        free(secondStr);  \
        firstStr  = NULL; \
        secondStr = NULL; \
    }

    while (asciiDiff == 0) {
        firstElement  = firstStr[firstIndex];
        secondElement = secondStr[secondIndex];

        if (firstElement == '\n' || firstElement == EOF) {
            FREE_();
            return (secondElement == '\n' || secondElement == EOF) ? 0 : -1;
        }

        if (secondElement == '\n' || secondElement == EOF) {
            FREE_();
            return 1;
        }

        if (isalpha(firstElement) || firstElement == ' ' || firstElement == '\'') {
            if (isalpha(secondElement) || secondElement == ' ' || secondElement == '\'') {
                asciiDiff += tolower(firstElement) - tolower(secondElement);
                firstIndex++; secondIndex++;
            }

            else {
                secondIndex++;
            }
        }

        else {
            firstIndex++;
        }
    }

    FREE_();

    #undef FREE_()

    return asciiDiff;
}

static const char *parseConsole(int argc, char *argv[]) {
    if (argc > 1) {
        return argv[--argc];
    }

    else {
        return "asmExamples/defaultAsm.asm";
    }
}

static void run(int commands[], int argc, char *argv[]) {
    customWarning(commands != NULL, void(1));

    const char *asmFileName = parseConsole(argc, argv);

    initializeCommands(commands, asmFileName);
    initializeExeFile (commands);
}

static void initializeCommands(int commands[], const char *asmFileName) {
    customWarning(commands != NULL, void (1));

    FILE *inCommands = fopen(asmFileName, "r");
    customWarning(inCommands != NULL, (void) 1);

    char command   [MAX_CMD_SIZE]  = {};
    int ip                         = 0;

    Label labels[MAX_LABEL_COUNT] = {};
    labelsInitialize(labels);

    #define CMD_(name, cmdCode, ...)                                 \
        if (customStrcmp(newCommand, #name) == 0) {                  \
            ip = setArg(inCommands, commands, labels, ip, name);     \
            continue;                                                \
        }

    while (1) {
        printf("===========================================\n"); // ! DEBUG

        char newCommand [MAX_CMD_SIZE] = {};

        if (fscanf(inCommands, "%s", command) == EOF) {
            break;
        }

        printf("Read Command: [%s]\n", command);

        if (!isalpha(*command)) {
            printf("Command Doesn't Start With A Letter!\n"); // ! DEBUG

            CLEAN_INPUT_LINE(inCommands);

            continue;
        }

        CHECK_IF_COMMANDS_USE_FSTREAM();

        CREATE_CLEAR_COMMAND(newCommand, command);

        printf("New Command After Delete ';': [%s]\n", newCommand); // ! DEBUG

        #include "cmd_generator.h"

        if (strchr(newCommand, ':') != NULL) {
            printf("Label: [%s]\n", newCommand); // ! DEBUG
            setLabel(labels, command, ip);
            continue;
        }

        customPrint(red, bold, bgDefault, "Syntax Error: %s\n", command);
        customWarning(NULL != NULL, (void) 1);
    }

    #undef CMD_

    commands[ip] = END_OF_COMMANDS;
    printf("END: [%d]\n", commands[ip]);

    fclose(inCommands);

    goThroughLabels(labels, commands);
    labelsDestruct (labels);
}

// TODO BINARY MODE
// TODO OUTPUT FILE BY CONSOLE
static void initializeExeFile(int commands[]) {
    FILE *exeCommands = fopen("build/SPU_code.txt", "w");
    customWarning(exeCommands != NULL, (void) 1);

    int ip = 0;

    while (1) {
        fprintf(exeCommands, "%d\n", commands[ip++]);

        if (commands[ip - 1] == END_OF_COMMANDS) {
            break;
        }
    }

    fclose(exeCommands);
}

// TODO MAGIC
static void goThroughLabels(Label LABELS[], int commands[]) {
    for (int labelIndex = 0; labelIndex < MAX_LABEL_COUNT; labelIndex++) {
        for (int jmpIndex = 0; jmpIndex < LABELS[labelIndex].jmpUsed; jmpIndex++) {
            commands[LABELS[labelIndex].jmpAddress[jmpIndex]] = LABELS[labelIndex].initAddress;
            printf("Label (goThroughLabels): [%s] || To .txt: [%d]\n", LABELS[labelIndex].labelName, LABELS[labelIndex].initAddress);

            if (LABELS[labelIndex].initAddress == -1) {
                customPrint  (red, bold, bgDefault, "Error: label isn't initialized\n");
                customWarning(NULL != NULL, (void) 1);
            }
        }
    }
}

static void putLabelAddress(Label LABELS[], int *commands, int *ip, FILE *inCommands) {
    char labelName   [MAX_LABEL_NAME_SIZE] = {};
    char newLabelName[MAX_LABEL_NAME_SIZE] = {};

    fscanf(inCommands, "%s", labelName);
    printf("Read Label (putLabelAddress): [%s]\n", labelName);    // ! DEBUG

    CREATE_CLEAR_COMMAND(newLabelName, labelName);

    printf("New Label After Delete Trash: [%s]\n", newLabelName); // ! DEBUG

    int labelAddress  = findLabelAddress(LABELS, newLabelName, *ip);

    CLEAN_INPUT_LINE(inCommands);
    commands[(*ip)++] = labelAddress;
    printf("Label (putLabelAddress): [%s], to .txt: [%d]\n", labelName, commands[(*ip)-1]);
}

static int isRegister(const char *string) {
    if ((customStrcmp((char *)string, "AX")) == 0 ||
        (customStrcmp((char *)string, "BX")) == 0 ||
        (customStrcmp((char *)string, "CX")) == 0 ||
        (customStrcmp((char *)string, "DX")) == 0 ||
        (customStrcmp((char *)string, "EX")) == 0 ||
        (customStrcmp((char *)string, "FX")) == 0) {
            return 1;
        }

    return 0;
}

static int getArg(const char *inputLine, int commands[], int ip) {
    int arg                              =  0;
    char registerName[MAX_REGISTER_SIZE] = {};

    int commandType = 0;

    if (strchr(inputLine, ']') != NULL) {
        commandType = MEM;
    }

    if (sscanf(inputLine, "%s + %d", registerName, &arg) == 2 ||
        sscanf(inputLine, "%d + %s", &arg, registerName) == 2) {
            printf("Argument Is [<REG> + <DIGIT>]: [%s + %d]\n", registerName, arg); // TODO FIX [%d + %s]

            return getArgSD(commands, registerName, commandType, arg, ip);
        }

    if (sscanf(inputLine, "%d", &arg) == 1) {
        printf("Argument Is Digit (getArg): [%d]\n", arg);
        return getArgD(commands, commandType, arg, ip);
    }

    if (sscanf(inputLine, "%s", registerName) == 1) {
        printf("Argument Is Register (getArg): [%s]\n", registerName);

        char newRegisterName[MAX_REGISTER_SIZE] = {};

        CREATE_CLEAR_COMMAND(newRegisterName, registerName);

        printf("New Register After Delete Trash: [%s]\n", newRegisterName);
        return getArgS(commands, newRegisterName, commandType, ip);
    }

    return ip;
}

static int setArg(FILE *inCommands, int *commands, Label *labels, int ip, int type) {
    if (type == PUSH || type == POP) {
        return setArgPushPop(inCommands, commands, ip, type);
    }

    else if ((type >= JMP && type <= JNE) || type == CALL) {
        commands[ip++] = type;
        printf("type (setArg), then putLabelAddress: [%d]\n", commands[ip-1]);
        putLabelAddress(labels, commands, &ip, inCommands);

        return ip;
    }

    else {
        commands[ip++] = type;
        printf("type (setArg): [%d]\n", commands[ip-1]);

        return ip;
    }
}


static int setLabel(Label *labels, char *command, int ip) {
    char labelName[MAX_LABEL_NAME_SIZE] = {};
    strcpy(labelName, command);

    int err = initializeLabelAddress(labels, labelName, ip);

    if (err == EXCEEDED_MAX_LABEL_COUNT) {
        customPrint  (red, bold, bgDefault, "Error: exceeded max label count\n");
        customWarning(NULL != NULL, 1);
    }

    if (err == DOUBLE_INITIALIZE) {
        customPrint  (red, bold, bgDefault, "Error: double label initialization\n");
        customWarning(NULL != NULL, 1);
    }

    return 0;
}

static int getArgSD(int *commands, char *registerName, int commandType, int argument, int ip){
    if (registerName[2] == ']'){
        registerName[2] = '\0';
    }
    if (!isRegister(registerName)){
        customPrint  (red, bold, bgDefault, "Error: not a register name\n");
        customWarning(NULL != NULL, 1);
    }

    printf("==============GET ARG============\n");
    commands[ip++] = (commandType | IMMED) | REG;
    printf("commandType | IMMED | REG: [%d]\n", commands[ip - 1]);
    commands[ip++] = argument;
    printf("argument: [%d]\n", commands[ip - 1]);
    commands[ip++] = registerName[0] - 'A' + 1;
    printf("first register letter: [%c]\n", commands[ip - 1]);

    return ip;
}

static int getArgD(int *commands, int commandType, int arg, int ip) {
    commands[ip++] = commandType | IMMED;
    printf("commandType | IMMED (getArgD): [%d]\n", commands[ip-1]);
    commands[ip++] = arg;
    printf("argument (getArgD): [%d]\n", commands[ip-1]);

    return ip;
}

static int getArgS(int *commands, char *registerName, int commandType, int ip) {
    if (registerName[2] == ']') {
        registerName[2] = '\0';
    }

    if (!isRegister(registerName)) {
        customPrint  (red, bold, bgDefault, "Error: not a register name\n");
        customWarning(NULL != NULL, 1);
    }

    commands[ip++] = commandType | REG;
    printf("commandType | REG (getArgS): [%d] | [%d] -> [%d]\n", commandType, REG, commands[ip - 1]);
    commands[ip++] = registerName[0] - 'A' + 1;
    printf("first register letter: [%d]\n", commands[ip - 1]);

    return ip;
}

static int setArgPushPop(FILE *inCommands, int *commands, int ip, int type) {
    int  commandIp     = ip;
    char inputLine[30] = {}; // TODO MAGIC

    fscanf(inCommands, "%[^\n]\n", inputLine);

    if (strchr(inputLine, '[') != NULL) {
        ip = getArg(strchr(inputLine, '[') + 1, commands, ip);

        if ((commands[commandIp] & IMMED) && ((commands[commandIp] & MEM) == 0) && type == POP) {
            customPrint  (red, bold, bgDefault, "Error: constant in pop\n");
            customWarning(NULL != NULL, 1);
        }

        commands[commandIp] |= type;
        printf("|= type (setArgPushPop): [%d]", commands[commandIp]);

        return ip;
    }

    ip = getArg(inputLine, commands, ip);
    commands[commandIp] |= type;
    printf("|= type (setArgPushPop) if no [: type = [%d], [%d]\n", type, commands[commandIp]);

    return ip;
}

int main(int argc, char *argv[]) {
    int commands[MAX_CMDS_SIZE] = {};

    run(commands, argc, argv);

    return 0;
}