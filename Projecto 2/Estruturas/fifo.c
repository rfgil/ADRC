#include <stdlib.h>

#include "fifo.h"

typedef struct element {
  void * item;
  struct element * next;
  struct element * ant;
} Element;

struct root {
  Element * first;
  Element * last;
};

Fifo * newFifo(){
  Fifo * new;

  new = malloc(sizeof(Fifo));
  new->first = NULL;
  new->last = NULL;

  return new;
}

void InsertFifo(Fifo * fifo, void * item){
  Element * new_element;

  new_element = malloc(sizeof(Element));
  new_element->item = item;

  new_element->next = fifo->first;
  new_element->ant = NULL;


  // Verifica se é o primeiro elemento a ser inserido
  if (fifo->last == NULL){
    fifo->last = new_element;
  } else {
    fifo->first->ant = new_element;
  }

  fifo->first = new_element;
}

void * RemoveFifo(Fifo * fifo){
  //Condirma se o fifo não está vazio
  if (fifo->last != NULL){
    void * item;
    Element * last_element;

    last_element = fifo->last;
    //Coloca o apontador last a apontar para o penultimo elemento
    fifo->last = last_element->ant;
    last_element->next = NULL;

    item = last_element->item;
    free(last_element);
    return item;
  }
  return NULL;
}

/*
void freeElements(Element * element, void (*freeItem)(void *)){

}

void freeFifo(Fifo * fifo, void (*freeItem)(void *)){
  freeElements(fifo->first, freeItem);
  free(fifo);
}
*/
