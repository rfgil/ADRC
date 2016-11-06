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

    if (fifo->first == last_element){
      fifo->first = NULL;
      fifo->last = NULL;
    } else {

      //Coloca o apontador last a apontar para o penultimo elemento
      fifo->last = last_element->ant;
      //Coloca o apontador next do novo ultimo elemento a apontar para null
      fifo->last->next = NULL;
    }

    item = last_element->item;
    free(last_element); // Liberta a memoria do elemento
    return item;
  }
  return NULL;
}


static void freeElements(Element * element, void (*freeItem)(void *)){
  if (element != NULL){
    freeElements(element->next, freeItem);
    freeItem(element->item);
    free(element);
  }
}

void freeFifo(Fifo * fifo, void (*freeItem)(void *)){
  freeElements(fifo->first, freeItem);
  free(fifo);
}
