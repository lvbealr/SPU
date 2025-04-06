#ifndef LABELS_H_
#define LABELS_H_

#include <cstdlib>

const size_t MAX_LABEL_NAME_SIZE = 32;
const int    MAX_LABEL_COUNT     = 8;
const int    RESIZE_CONSTANT     = 2;

struct Label {
  char labelName[MAX_LABEL_NAME_SIZE];
  int  initAddress;
  int *jmpAddress;
  int  jmpUsed;
  int  jmpCount;
};

enum LabelErrors {
  EXCEEDED_MAX_LABEL_COUNT = -1,
  DOUBLE_INITIALIZE        =  0,
  NO_LABEL_ERROR           =  1
};

// FUNCTION PROTOTYPES //
int labelsInitialize      (Label LABELS[]                          );
int findLabelAddress      (Label LABELS[], char labelName[], int ip);
int labelsDestruct        (Label LABELS[]                          );
int initializeLabelAddress(Label LABELS[], char labelName[], int ip);
// FUNCTION PROTOTYPES //

#endif // LABELS_H_