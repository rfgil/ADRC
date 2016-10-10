#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Estruturas/tree.h"

#define MAX_STR_LENGHT 128

struct codeInfo {
  char symbol;
  char * prefix;
};

void printCodeInfo(void * item){
  printf("%c - %s\n", ((struct codeInfo *)item)->symbol, ((struct codeInfo *) item)->prefix);
}

void freeCodeInfo(void * item){
  free(((struct codeInfo *)item)->prefix);
  free(item);
}

void GenerateCode(Node * treeRoot, char * prefix){
  char str[MAX_STR_LENGHT];

  if ( getChild0(treeRoot) != NULL ){
    strcpy(str, prefix);
    GenerateCode(getChild0(treeRoot), strcat(str, "0"));
  }

  if ( getChild1(treeRoot) != NULL ){
    strcpy(str, prefix);
    GenerateCode(getChild1(treeRoot), strcat(str, "1"));
  }

  if (getChild0(treeRoot) == NULL && getChild1(treeRoot) == NULL) {
    //Imprime o elemento
    printf("%c - %s\n", *((char *) getItem(treeRoot)), prefix);
  }
}

void Decode(Node * treeRoot, char * InString, char * OutString){
  Node * aux;
  int pos = 0;

  aux = treeRoot;
  while(pos < strlen(InString)){
      if ( InString[pos] == '0') {
          aux = getChild0(aux);
      } else {
          aux = getChild1(aux);
      }

      pos += 1;
      if ( getChild0(aux) == NULL && getChild1(aux) == NULL ){
          OutString[strlen(OutString)+1] = '\0';
          OutString[strlen(OutString)] = *((char *) getItem(aux));
          //strncat(OutString, (char *) getItem(aux), 1);
          aux = treeRoot;
      }

  }
}

int main(){
  char buffer[MAX_STR_LENGHT];
  char OutString[MAX_STR_LENGHT];

  char symbol;
  FILE * fp;

  Node * treeRoot;

  fp = fopen("symbols", "r");
  treeRoot = NULL;
  while (fscanf(fp, "%c ", &symbol) == 1){
    //Criar árvore
    Node * new_leaf;
    char * item;

    item = malloc(sizeof(char));
    *item = symbol;

    new_leaf = createNode(item, NULL, NULL); //Leaf Node

    if (treeRoot == NULL){
      treeRoot = new_leaf;
    } else {
      treeRoot = createNode(EMPTY_NODE, treeRoot, new_leaf);
    }
  }

  fclose(fp);

  GenerateCode(treeRoot, "");

  printf("Introduza uma string de bits para descodificar!\n(Enter para sair)\n");
  while(fgets(buffer, MAX_STR_LENGHT, stdin)[0] != '\n'){
    buffer[strlen(buffer) - 1] = '\0'; //Remove o \n que o fgets coloca no final das strings
    OutString[0] = '\0'; //Limpa o conteúdo de Outrstring

    Decode(treeRoot, buffer, OutString);
    printf("%s\n", OutString);
  }

  freeTree(treeRoot, free);

  return 0;
}
