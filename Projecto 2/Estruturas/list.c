#include <stdlib.h>

#include "list.h"

List * newList(){
  return NULL;
}

List * insertList(List * list, void * item){
  List * new;

  new = malloc(sizeof(List));
  new->item = item;
  new->next = list;

  return new;
}

void freeList(List * element, void (*freeItem)(void *)){
  if (element != NULL){
    freeList(element->next, freeItem);
    freeItem(element->item);
    free(element);
  }
}

List * removeFirstList(List * list, void ** domain){
  if (list != NULL){
    (*domain) =  list->item;
    return list->next;
  }
  return NULL;
}
