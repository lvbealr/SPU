#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "colorPrint.h"
#include "assembler.h"
#include "cmds.h"
#include "labels.h"
#include "customWarning.h"

// TODO COMMANDS REGISTER (PUSH push)

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

    size_t firstLen = strlen(firstString), secondLen = strlen(secondString);
    size_t firstIndex   = 0, secondIndex   = 0;
    int    firstElement = 0, secondElement = 0;

    int asciiDiff = 0;

    while (asciiDiff == 0 && firstIndex < firstLen && secondIndex < secondLen) {
        firstElement  = firstString[firstIndex];
        secondElement = secondString[secondIndex];

        if (firstElement == '\n' || firstElement == EOF) {
            return (secondElement == '\n' || secondElement == EOF) ? 0 : -1;
        }

        if (secondElement == '\n' || secondElement == EOF) {
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

    char command[MAX_CMD_SIZE] = {};
    int ip                     = 0;

    Label labels[MAX_LABEL_COUNT] = {};
    labelsInitialize(labels);

    #define CMD_(name, cmdCode, ...)                                 \
        if (customStrcmp(command, #name) == 0) {                     \
            ip = setArg(inCommands, commands, labels, ip, name);     \
            continue;                                                \
        }

    while (1) {
        if (fscanf(inCommands, "%s", command) == EOF) {
            break;
        }

        #include "cmd_generator.h"

        if (strchr(command, ':') != NULL) {
            setLabel(labels, command, ip);
            continue;
        }

        customPrint(red, bold, bgDefault, "Syntax Error: %s\n", command);
        customWarning(NULL != NULL, (void) 1);
    }

    #undef CMD_

    commands[ip] = END_OF_COMMANDS;

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

            if (LABELS[labelIndex].initAddress == -1) {
                customPrint  (red, bold, bgDefault, "Error: label isn't initialized\n");
                customWarning(NULL != NULL, (void) 1);
            }
        }
    }
}

static void putLabelAddress(Label LABELS[], int *commands, int *ip, FILE *inCommands) {
    char labelName[MAX_LABEL_NAME_SIZE] = {};

    fscanf(inCommands, "%s", labelName);
    int labelAddress  = findLabelAddress(LABELS, labelName, *ip);

    commands[(*ip)++] = labelAddress;
}

static int isRegister(const char *string) {
    if ((strcmp(string, "AX")) == 0 ||
        (strcmp(string, "BX")) == 0 ||
        (strcmp(string, "CX")) == 0 ||
        (strcmp(string, "DX")) == 0 ||
        (strcmp(string, "EX")) == 0 ||
        (strcmp(string, "FX")) == 0) {
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
            return getArgSD(commands, registerName, commandType, arg, ip);
        }

    if (sscanf(inputLine, "%d", &arg) == 1) {
        return getArgD(commands, commandType, arg, ip);
    }

    if (sscanf(inputLine, "%s", registerName) == 1) {
        return getArgS(commands, registerName, commandType, ip);
    }

    return ip;
}

static int setArg(FILE *inCommands, int *commands, Label *labels, int ip, int type) {
    if (type == PUSH || type == POP) {
        return setArgPushPop(inCommands, commands, ip, type);
    }

    else if ((type >= JMP && type <= JNE) || type == CALL) {
        commands[ip++] = type;
        putLabelAddress(labels, commands, &ip, inCommands);

        return ip;
    }

    else {
        commands[ip++] = type;

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

    // TODO DOUBLE LABEL INITIALIZE ERROR!

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

    commands[ip++] = (commandType | IMMED) | REG;
    commands[ip++] = argument;
    commands[ip++] = registerName[0] - 'A' + 1;

    return ip;
}

static int getArgD(int *commands, int commandType, int arg, int ip) {
    commands[ip++] = commandType | IMMED;
    commands[ip++] = arg;

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
    commands[ip++] = registerName[0] - 'A' + 1;

    return ip;
}

static int setArgPushPop(FILE *inCommands, int *commands, int ip, int type) {
    int commandIp      = ip;
    char inputLine[30] = {}; // TODO MAGIC

    fscanf(inCommands, "%[^\n]\n", inputLine);

    if (strchr(inputLine, '[') != NULL) {
        ip = getArg(strchr(inputLine, '[') + 1, commands, ip);

        if ((commands[commandIp] & IMMED) && ((commands[commandIp] & MEM) == 0) && type == POP) {
            customPrint  (red, bold, bgDefault, "Error: constant in pop\n");
            customWarning(NULL != NULL, 1);
        }

        commands[commandIp] |= type;

        return ip;
    }

    ip = getArg(inputLine, commands, ip);
    commands[commandIp] |= type;

    return ip;
}

int main(int argc, char *argv[]) {
    int commands[MAX_CMDS_SIZE] = {};

    run(commands, argc, argv);

    return 0;
}