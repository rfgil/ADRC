#include <stdlib.h>

#include "list.h"

struct _element {
  void * item;
  struct _element * next;
};

ListElement * newList(){
  return NULL;
}

ListElement * insertInList(ListElement * list, void * item){
  ListElement * new;

  new = malloc(sizeof(ListElement));
  new->item = item;
  new->next = list;

  return new;
}

void freeList(ListElement * list, void (*freeItem)(void *)){
  if (list != NULL){
    freeList(list->next, freeItem);

    freeItem(list->item);
    free(list);
  }
}

void printList(ListElement * list, void (*printItem)(void *)){
  ListElement * aux;
  aux = list;
  while (aux != NULL){
    printItem(aux->item);
    aux = aux->next;
  }
}
