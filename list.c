#include "list.h"

void insert(LIST *l,char input []){
  NODE *p = (NODE*)malloc(sizeof(NODE));
  char *value  = (char*)malloc(sizeof(char)*(strlen(input)+1));
  strcpy(value,input);
  p-> val = value;
  p-> mac = 0;
  p-> ext= 0; 
  p-> ent= 0;
  p-> labDec = 0;
  p-> next = l-> head;
  l-> head = p;
  return;
}

LIST* newList(){
  LIST *head = (LIST*)malloc(sizeof(LIST));
  return head;
}

NODE* findNode(LIST *l, char name[]){
  NODE *p = l->head;
  while(p != NULL){
    if(!strcmp(p->val,name))
      return p;
    p = p->next;
  }
  return NULL;
}
