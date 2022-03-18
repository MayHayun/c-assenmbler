#include <stdio.h>
#include <stdlib.h>

#include "toBinary.h"

#define COMMAND_MAX_LENGTH 31
#define MAX_LINE_LENGTH 81
#define MAX_COMMANDS_IN_MACRO 6
#define MAX_LABEL_LENGTH 31

typedef struct commandsStruct{
    char commandName[COMMAND_MAX_LENGTH];
    int opcode;
    int funct;
    int numOfParam; 
}commandsStruct;

void freeList1(WORD *head);
void freeList2(symbolLink *head);
struct MACRO *head = NULL;
int move_to_none_white(char line[], int i);
int skip(char line[]);
void preProcessing(FILE *fileName, char *nameOfFile);
int getWord(char *word, char line[], int i);
void trimTrailing(char * str);
int isNum(char *str);
int isNameOk(char line []);
int isARegister(char []);
int isACommand(char []);
int isANaturalNum(char);
int giveTheLastNoneWhiteIndex(char line[]);
int countWords(char line []);


int isLableDec(char *lable);
commandsStruct *findCommand(char * command);
int isANum(char *param);
char *cutWhiteChars(char *str);
