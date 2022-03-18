#ifndef _validation_H_
#define _validation_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "list.h"

#define parse " \t\n"
#define parse_words "  \t\n"
#define parse_delivery " ,\t\n["
#define parse_commands " ,\t\n:"


int validation(FILE *filePtr, LIST *names);
int isRight(char line [], LIST *names);
int whichDelivery(char myStr[], LIST *names);
int isNameOk(char line []);
int isValidMacro( LIST *names, char token[]);
int isValidExternDec( LIST *names, char token[]);
int isValidEntryDec( LIST *names, char token[]);
int isValidLable( LIST *names, char token[]);
int validOperands(char com [], char firstOp [], char secondOp [], LIST *names);

#endif
