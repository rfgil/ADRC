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

  //Avalia filho 0
  if ( getChild0(treeRoot) != NULL ){
    strcpy(str, prefix);
    GenerateCode(getChild0(treeRoot), strcat(str, "0"));
  }

  //Avalia filho 1
  if ( getChild1(treeRoot) != NULL ){
    strcpy(str, prefix);
    GenerateCode(getChild1(treeRoot), strcat(str, "1"));
  }

  //Verefica se o node é uma folha
  if (getChild0(treeRoot) == NULL && getChild1(treeRoot) == NULL) {
    //Imprime o elemento e o prefixo
    printf("%c - %s\n", *((char *) getItem(treeRoot)), prefix);
  }
}

void Decode(Node * treeRoot, char * InString, char * OutString){
  Node * aux;
  int pos = 0;

  aux = treeRoot;
  while(pos < strlen(InString)){
      //Avança na arvore em função do "bit" que está a ser avaliado
      if ( InString[pos] == '0') {
          aux = getChild0(aux);
      } else {
          aux = getChild1(aux);
      }

      pos += 1;
      //Confirma se o node da árvore a que se chegou é ou não uma folha
      if ( getChild0(aux) == NULL && getChild1(aux) == NULL ){
          //Caso seja, imprime o simbolo correspondente na outstring
          OutString[strlen(OutString)+1] = '\0';
          OutString[strlen(OutString)] = *((char *) getItem(aux));

          //E recomeça do inicio da árvore
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

  //Criar árvore
  while (fscanf(fp, "%c ", &symbol) == 1){
    //Lê um simbolo do ficheiro
    Node * new_leaf;
    char * item;

    item = malloc(sizeof(char));
    *item = symbol;

    //Cria um node de uma tree com o simbolo (Esse node é uma folha)
    new_leaf = createNode(item, NULL, NULL);

    if (treeRoot == NULL){
      // Caso só exista um simbolo no ficheiro a árvore será apenas um node com o simbolo
      // O programa não irá funcionar correctamente, mas não teria interesse codificar um conjunto com apenas um simbolo
      treeRoot = new_leaf;

    } else {
      //Cria um novo node na arvore cujos filhos são o node com o filho criado e a àrvore já existente
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
