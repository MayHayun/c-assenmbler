
#include "utils.h"

#define CUT "\t\n "
#define MAX_LINE_LENGTH 81
#define MAX_MACRO_COMMANDS 6


typedef struct MACRO
{
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int numOfCommands;
    struct MACRO *next; 
}MACRO;
