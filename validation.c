#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "validation.h"

#define parse_words "  \t\n"
#define parse_delivery " ,\t\n["
#define parse_commands " ,\t\n:"

/* check if a given code is valid in assembly */
int validation(FILE *filePtr, LIST *names){
  int isValid = 1, lineNumber = 0;
  int neededOperands;
  char line[MAX_LINE_LENGTH];
  char *token = NULL;
  int numOfValidMacros = 0;

  /* for every line */
  while (fgets(line, MAX_LINE_LENGTH, filePtr)){
    char lineCopy[MAX_LINE_LENGTH];
    int wordNumber = 0;
    int notQualified = 1;
    lineNumber++;
    strcpy(lineCopy, line);
    token = strtok(lineCopy, parse_words); 

    /* the limit of 200 lines is for the makinng of the code */
    /* for every word */
    while( token != NULL && lineNumber <= 200 ){
      if( !wordNumber == 0 )
        token = strtok(NULL, parse_words);
      
      wordNumber++;
      /* Label Declaration */
      if( wordNumber == 1 && token[strlen(token)-1] == ':' ){
        if(!isValidLableDec(names, token)){
          printf("in line %d: %s is invalid label\n",lineNumber,token);
          isValid = 0;
        }
        token = strtok(NULL, parse_words);
        wordNumber++;
      }
      
      /* macro */
      if( notQualified && token != NULL){
        trimTrailing(token);
        
        if( wordNumber == 1 && !strcmp(token,"macro")){
          notQualified = 0;
          token = strtok(NULL, parse_words);
          wordNumber++;
          if(!isValidMacro(names, token)|| isCurNumOfWords( line, 2) == 0){
            isValid = 0;
            printf("in line %d: invalid macro\n",lineNumber);
          }
          else{
            numOfValidMacros++;
          }
        }
      }
      if(notQualified && tfindNode(names,token) != NULL){
        NODE *nodeFound = findNode(names,token);
        if(nodeFound->mac != 0 && countWords(line) == 1){
          notQualified = 0;
        }
      }
      /* endm */
      if( notQualified && !strcmp(token,"endm")){
        notQualified = 0;
        if(isCurNumOfWords(line,1)  && numOfValidMacros != 0)
          numOfValidMacros--;
        else{
          printf("in line %d: invalid endm\n",lineNumber);
          isValid = 0;
        }
      }
      
      /* extern */
      if( notQualified && !strcmp(token,".extern")){
        notQualified = 0;
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(!isCurNumOfWords(line, 2) || !isValidExternDec(names, token)){
          printf("in line %d: invalid extern\n",lineNumber);
          isValid = 0;
        }
      }

      /* entry */
      if( notQualified && token != NULL && !strcmp(token,".entry")){
        notQualified = 0;
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(!isCurNumOfWords( line, 2) || !isValidEntrynDec(names, token))
        {
          printf("in line %d: invalid entry\n",lineNumber);  
          isValid = 0;
        }
      }
      
      /* .data */
      if( notQualified && token != NULL){
        if(!strcmp(token,".data") ){
          notQualified = 0;
          if(wordNumber == 1 || wordNumber == 2){
            int wordsInLine = countWords(line);
            
            if(wordNumber == 2)
              wordsInLine--; /* for comparing it later to count commas */
            token = strtok(NULL, "\n ,");
            while( token != NULL )
            {
              trimTrailing(token);
              if(!isNumber(token) ){
                printf("line %d: is not a number!\n", lineNumber);
                isValid = 0;
              } 
              token = strtok(NULL, "\n ,");
            }
            
            if(!isValidCommas(wordsInLine-2,line)){
              printf("line %d: invalid commas!\n", lineNumber);
              isValid = 0;
            }
          }
        }
      }
    
    /* .string */ /* come here */
    if( notQualified && !strcmp(token,".string")){
        int i = 0;
        int j[MAX_LINE_LENGTH];
        int countQM = 0;
        notQualified = 0;
        
        /* token is now .string so we advance it */
        token = strtok(NULL, "\n");
        token = cutWhiteChars(token);

        if( token[0] == '\0' ){
          printf("empty string in line: %d\n", lineNumber);
          isValid = 0;
        }

        /* converting the chars to ASCII so we wont have the 
            problem that the first " ends the string */
        for(i = 0; i < strlen(token); i++)
          j[i] = token[i];

        /* check the ASCII value of the chars in the array */
        for(i = 0; i < strlen(token); i++){
          if(j[i] == 34){
            countQM++; /* count the quotation marks */
            if(i != 0 && i != strlen(token) - 1 ){
              printf("invalid string in line:%d\n", lineNumber);
              isValid = 0;  
            }
          }
        }
      
        if( isValid && countQM != 2 ){ 
          printf("invalid string in line:%d\n", lineNumber);
          isValid = 0;  
        }
        
        if(isValid && token != NULL ){ 
          token = strtok(NULL, "\t \n");
          if( token != NULL ){
            printf("invalid string in line:%d\n", lineNumber);
            isValid = 0;  
          }
        }
      }
      /* empty or comment */
      if(notQualified && token != NULL && skip(line))
        notQualified = 0;

      /* command */
      if( notQualified && token != NULL){
        if(isACommand(token)){           
          notQualified = 0;
          if(!isValidCommand(line,names)){
            printf("invalid operands in line %d\n",lineNumber);
            isValid = 0;
          }
        }
      }
      /* if the line is unrecognizable */
      if(notQualified){
        notQualified = 0;
        printf("unrecognizable line in line %d\n",lineNumber);
        isValid = 0;
      }
    }
  }
  return isValid;
}

/* this function is the first pass that parses the
words and filter the special words words */

/*check if a given command line is valid */
int isValidCommand(char line [], LIST *names){
  char command[MAX_LABEL_LENGTH] = NULL;
  char firstOp[MAX_LABEL_LENGTH] = NULL;
  char secondOp[MAX_LABEL_LENGTH] = NULL;
  char lineCopy[MAX_LINE_LENGTH];
  char *token = NULL;
  int rightCommas = countWords(line) -2, i = 0;

  strcpy(lineCopy, line);
  token = strtok(lineCopy, parse_words); 
  if(token[strlen(token)-1] == ':'){
    token = strtok(NULL, parse_words);
    rightCommas--;
  }
  if(rightCommas < 0){
    rightCommas = 0;
  }
  if(!isValidCommas(rightCommas,line) || rightCommas > 1){
    return 0;
  }
  while(token != NULL){
    if(i == 0)
      strcpy(command,token);
    if(i == 1)
      strcpy(firstOp,token);
    if(i == 2)
      strcpy(secondOp,token);
    if(i == 3)
      return 0;
    i++;
    token = strtok(NULL, " ,\t\n");
  }
  return validOperands(command,firstOp,secondOp,names);
}

/* func to decide which delivery it is
   the given string is after move to none white!*/
int whichDelivery(char myStr[], LIST *names){
  char tempLine[MAX_LABEL_LENGTH];
  char tempLabel[MAX_LABEL_LENGTH];
  char tempReg[MAX_LABEL_LENGTH];
  char* token;
  
  if( myStr == NULL ){
    return 0;
  }
  
  /* Delivery 0 */
  if(isNum(myStr) && isNumber(myStr + 1)){
    return 0;
  }
  /* Delivery 1 */
  if(findNode(names,myStr) != NULL && findNode(names,myStr)->mac == 0){
    return 1;
  }
  /* Delivery 3 */
  if( isARegister(myStr) != -1 ){
    return 3;
  }
  /* Delivery 2 */
  strcpy(tempLine, myStr);
  token = strtok(tempLine, parse_delivery);
  strcpy( tempLabel, token );
 
  token = strtok(NULL, parse_delivery);
  if(token == NULL)
    return -1;
  strcpy( tempReg, token );
  if( strlen(tempReg) == 4 ){
      if( tempReg[0] == 'r' && tempReg[1] == '1' && tempReg[3] == ']' ){
        if( tempReg[2] >= '0' && tempReg[2] <= '5' ){
          if( findNode(names,tempLabel) != NULL && findNode(names,tempLabel)->mac == 0 ){
              return 2;
            }
          }
      } 
  }
  else
    return -1;
}

/* check if a given string is a valid name */
int isNameOk(char line []){
  int i = 0;
  if(!isalpha(line[i]) || isARegister(line) != -1 || isACommand(line) || strlen(line) > MAX_LABEL_LENGTH){
    return 0;
  }
  while(line[i] != '\0'){
    if(!isalpha(line[i]) && !isANaturalNum(line[i]))
      return 0;
    i++;
  }
  return 1;
}

/* need to solve the option of two identical macros */
/* check if a given string is a valid macro */
int isValidMacro( LIST *names, char token[])
{
  if(findNode(names, token) != NULL && findNode(names, token)->mac == 1 )
  {
      findNode(names, token)->mac = 2;
      return 1;
  }
  return 0;
}

int isValidExternDec( LIST *names, char token[]){
  if(findNode(names, token) != NULL && findNode(names, token)->ext != 0)
    return 1;
  return 0;
}

int isValidEntryDec( LIST *names, char token[])
{
  if( findNode(names, token) != NULL && findNode(names, token)->ent != 0 )
    return 1;
  return 0;
}

int isValidLableDec( LIST *names, char token[])
{
    token[strlen(token)-1] = '\0';
    if(findNode(names, token) != NULL && findNode(names, token)->labDec){
      findNode(names, token)->labDec = 0;
      return 1;
    }
  return 0;
}

int validOperands(char com [], char firstOp [], char secondOp [], LIST *names)
{
  int op1 = -1;
  int op2 = -1;
  int res1 = 0;
  int res2 = 0;

  firstOp = cutWhileChars(firstOp);
  if(firstOp[0] != '\0')
    op1 = whichDelivery(firstOp,names); 
  secondOp = cutWhileChars(secondOp);
  if(secondOp[0] != '\0')
    op2 = whichDelivery(secondOp,names);
  /* op1 = 0,1,2,3 */ 
  if(!strcmp(com,"mov")||!strcmp(com,"cmp")||!strcmp(com,"add")||!strcmp(com,"sub"))
    if(op1 != -1)
      res1 = 1;
  
  if(!strcmp(com,"prn") && op1 != -1 && secondOp == NULL)
    return 1;
  
  /* op1 = 1,2,3  (they have only 1 operand) */
  if(!strcmp(com,"clr")||!strcmp(com,"not")||!strcmp(com,"inc")|| !strcmp(com,"dec"))
    if(op1 != -1 && op1 != 0 && secondOp == NULL)
      return 1;
  if(!strcmp(com,"red") && op1 != -1 && op1 != 0 && !strcmp(secondOp,"-1"))
    return 1;
  
  /* op1 = 1,2 */
  if(!strcmp(com,"lea") && (op1 == 1 || op1 == 2))
    res1 = 1;
  
  if(!strcmp(com,"jmp") || !strcmp(com,"bne") || !strcmp(com,"jsr"))
    if((op1 == 1 || op1 == 2) && secondOp == NULL)
      return 1;
    
  /* no operands */
  if(!strcmp(com,"rts") || !strcmp(com,"stop"))
    if(firstOp == NULL && secondOp == NULL)
      return 1;
  
  /* op2 = 0,1,2,3*/
  if(!strcmp(com,"cmp") && op2 != -1)
    res2 = 1;
  
  /* op2 = 1,2,3 */
  if(!strcmp(com,"mov") || !strcmp(com,"add") || !strcmp(com,"sub") ||!strcmp(com,"lea"))
    if(op2 != 0 && op2 != -1)
      res2 = 1;
    

  if(res1 && res2)
    return 1;
  
  return 0;
}
