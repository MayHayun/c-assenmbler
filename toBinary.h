#include "symbolTable.h"

int IC = 100; /*holds the address of the word in the CPU*/
int DC = 0; /*holds the num of commands that are in the data image*/

/*an objest that holds the 20 bytes array for each word in line and a pointer to the next (implementation of linked list)*/
typedef struct WORD
{
    int word[20];
    struct WORD *next;
}WORD;

WORD *firstPass(FILE *filePointer, symbolLink *headOfTable);
void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile);
void toBinaryGuidance(char line[], WORD *headOfFile);
void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable);
WORD *charToBinary(char ch);
void addWord(WORD *head, WORD *link);
int extractRegister(char * param);
int *decToBinary(int num);
void zeroMe(int word[]);
void deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable, WORD *headOfFile);
int allZero(WORD *link);
void changeWord(WORD *headOfFile, symbolLink *symbolFound, FILE *entFile, FILE *extFile);
void secondPass(FILE *filePointer, WORD *headOfFile, symbolLink *headOfTable, char *fileName);
