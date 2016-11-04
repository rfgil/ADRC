#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Estruturas/heap.h"
#include "Estruturas/tree.h"

#define MAX_STR_LENGHT 128
#define HEAP_SIZE 30

typedef struct _symbol {
  char symbol;
  int freq;
} Symbol;

void GenerateCode(Node * treeRoot, char * prefix){
  char str[MAX_STR_LENGHT];

  //Avalia o filho 0
  if ( getChild0(treeRoot) != NULL ){
    strcpy(str, prefix);
    GenerateCode(getChild0(treeRoot), strcat(str, "0"));
  }

  //avalia o filho 1
  if ( getChild1(treeRoot) != NULL ){
    strcpy(str, prefix);
    GenerateCode(getChild1(treeRoot), strcat(str, "1"));
  }

  //Verefica se o node é uma folha
  if (getChild0(treeRoot) == NULL && getChild1(treeRoot) == NULL) {
    //Imprime o elemento, a sua frequencia e o prefixo
    Symbol * data;
    data = (Symbol*) getItem(treeRoot);
    printf("%c (freq: %d) - %s\n", data->symbol, data->freq, prefix);
  }
}

Node * HuffmanCode(Heap * heap){
  Node * tree_node1;
  Node * tree_node2;
  Node * tree_root;

  Symbol * data;

  while( getHeapNElements(heap) > 1){
    //Remove os dois nodes com menor frequencia
    tree_node1 = RemoveMaxFromHeap(heap);
    tree_node2 = RemoveMaxFromHeap(heap);

    //Cria um node cujos filhos serão os nodes que se acabaram de remover
    data = malloc(sizeof(Symbol));
    data->symbol = 0;
    data->freq = ( (Symbol*) getItem(tree_node1) )->freq +
                 ( (Symbol*) getItem(tree_node2) )->freq;

    tree_root = createNode(data, tree_node1, tree_node2);

    //Insere esse node no heap
    InsertInHeap(heap, tree_root);
  }

  //Quando o heap só tiver um elemento, esse elemento será a arvore que representa
  //o Huffman Code  para o conjunto de simbolos inicial.
  return RemoveMaxFromHeap(heap);
}

int compare_node(void * item1, void * item2){
  Symbol * data1, * data2;

  data1 = ((Symbol*) getItem( (Node *) item1) );
  data2 = ((Symbol*) getItem( (Node *) item2) );

  return data1->freq > data2->freq ? GREATER : SMALLER;
}

void freeTreeItem(void * item){
  freeTree((Node *) item, free);
}

int main(){
  FILE * fp;

  char symbol;
  int freq;

  Heap * heap;

  Node * tree_node;
  Symbol * data;

  fp = fopen("letras_freq", "r");

  heap = newHeap(HEAP_SIZE, compare_node);

  while( fscanf(fp, "%c %d ", &symbol, &freq) == 2 ){
    //Cria a estrutura com a informação sobre um simbolo
    data = malloc(sizeof(Symbol));

    data->symbol = symbol;
    data->freq = freq;

    //Insere esse simbolo num node de uma arvore (Esse node é uma folha)
    tree_node = createNode(data, NULL, NULL);

    //Insere o node da arvore no heap (que vai funcionar como lista de prioridade)
    InsertInHeap(heap, tree_node);
  }

  tree_node = HuffmanCode(heap);
  GenerateCode(tree_node, "");

  freeHeap(heap, freeTreeItem);
  freeTree(tree_node, free);

  fclose(fp);
  return 0;
}
