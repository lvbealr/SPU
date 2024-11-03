#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

#include "labels.h"
#include "customWarning.h"

static int isNum         (char  name[]);
static int labelIndex    (Label LABELS[], char name[]);
static int resizeLabelJmp(Label LABELS[], int labelInd);

int labelsInitialize(Label LABELS[]) {
  customWarning(LABELS != NULL, 1);

  for (size_t index = 0; index < MAX_LABEL_COUNT; index++) {
    LABELS[index].labelName[0]  = '\0';
    LABELS[index].initAddress   = -1;
    LABELS[index].jmpAddress    = (int *)calloc(1, sizeof(int));
    LABELS[index].jmpAddress[0] = -1;
    LABELS[index].jmpCount      = 1;
    LABELS[index].jmpUsed       = 0;
  }

  return NO_LABEL_ERROR;
}

int findLabelAddress(Label LABELS[], char name[], int ip) {
  customWarning(LABELS != NULL, 1);
  customWarning(name   != NULL, 1);

  char *ptrToCall = strchr(name, ':');

  if (ptrToCall != NULL) {
    (*ptrToCall)    = '\0';
  }

  if (isNum(name)) {
    int address = 0;
    sscanf(name, "%d", &address);

    return address;
  }

  int labelInd = labelIndex(LABELS, name);

  if (labelInd == EXCEEDED_MAX_LABEL_COUNT) {
    return EXCEEDED_MAX_LABEL_COUNT;
  }

  resizeLabelJmp(LABELS, labelInd);

  LABELS[labelInd].jmpAddress[LABELS[labelInd].jmpUsed] = ip;
  LABELS[labelInd].jmpUsed++;

  return LABELS[labelInd].initAddress;
}

int initializeLabelAddress(Label LABELS[], char name[], int ip) {
  customWarning(LABELS != NULL, 1);
  customWarning(name   != NULL, 1);

  char *ptrToCall = strchr(name, ':');
  (*ptrToCall)    = '\0';

  int labelInd = labelIndex(LABELS, name);

  if (labelInd == EXCEEDED_MAX_LABEL_COUNT) {
    return EXCEEDED_MAX_LABEL_COUNT;
  }

  if (LABELS[labelInd].initAddress != -1) {
    return DOUBLE_INITIALIZE;
  }

  LABELS[labelInd].initAddress = ip;
  printf("Set Label Index: %d [%s]\n", LABELS[labelInd].initAddress, name);
  return NO_LABEL_ERROR;
}

int labelsDestruct(Label LABELS[]) {
  customWarning(LABELS != NULL, 1);

  for (size_t index = 0; index < MAX_LABEL_COUNT; index++) {
    LABELS[index].labelName[0] = '\0';
    LABELS[index].initAddress  = -1;

    free(LABELS[index].jmpAddress);
    LABELS[index].jmpAddress = NULL;
  }

  return NO_LABEL_ERROR;
}

static int isNum(char name[]) {
  customWarning(name != NULL, 1);

  char *ptr = name;

  while (*ptr != '\0') {
    if (!isdigit(*ptr)) {
      break;
    }

    ptr++;
  }

  if (*ptr == '\0') {
    return 1;
  }

  return 0;
}

static int labelIndex(Label LABELS[], char name[]) {
  customWarning(LABELS != NULL, 1);
  customWarning(name   != NULL, 1);;

  for (int index = 0; index < MAX_LABEL_COUNT; index++) {
    if (LABELS[index].labelName[0] == '\0') {
      strcpy(LABELS[index].labelName, name);

      return index;
    }

    if (strcmp(LABELS[index].labelName, name) == 0) {
      return index;
    }
  }

  return EXCEEDED_MAX_LABEL_COUNT;
}

static int resizeLabelJmp(Label LABELS[], int labelIndex) {
  customWarning(LABELS != NULL, 1);

  if (LABELS[labelIndex].jmpCount == LABELS[labelIndex].jmpUsed) {
    LABELS[labelIndex].jmpCount *= RESIZE_CONSTANT;

    int newSize                   = LABELS[labelIndex].jmpCount * (int)sizeof(int);
    LABELS[labelIndex].jmpAddress = (int *)realloc(LABELS[labelIndex].jmpAddress, (size_t)newSize);
  }

  return NO_LABEL_ERROR;
}