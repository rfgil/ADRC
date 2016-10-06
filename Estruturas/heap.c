#include <stdlib.h>

#include "heap.h"

#define GREATER 1
#define SMALLER 2

struct heap {
  int (*compare) (void *, void *);
  int n_elements;               /* # elements in heap */
  int size;                     /* max size of the heap. */
  void ** heapdata;               /* An array of Items. */
};

Heap * newHeap(int size, int (*compare)(void *, void *)){
  Heap * newHeap;

  newHeap = malloc(sizeof(Heap));

  newHeap->compare = compare;
  newHeap->n_elements = 0;
  newHeap->size = size;
  newHeap->heapdata = malloc(size * sizeof(void *));

  return newHeap;
}

void freeHeap(Heap * heap, void (*freeItem)(void *)){
  int i;

  for (i = 0; i < heap->n_elements; i++){
    freeItem(heap->heapdata[i]);
  }

  free(heap);
}

void InsertInHeap(Heap * heap, void * item){
  int pos;

  if (heap->n_elements == heap->size){
    return; //O heap está cheio
  }

  //Define a posição do novo item como sendo o final do heap
  pos = heap->n_elements;

  //Procurar a posição correcta do item no heap (e faz as trocas necessárias para poder inserir nessa posição)
  while( (pos > 0) && heap->compare(heap->heapdata[(pos - 1) / 2], item) == GREATER ){
    heap->heapdata[(pos - 1) / 2] = heap->heapdata[pos];
    pos = (pos - 1) / 2;
  }

  heap->heapdata[pos] = item;
  heap->n_elements ++;
}

void * RemoveMaxFromHeap(Heap * heap){
  void * item;
  void * last;
  int pos, i;

  if (heap->n_elements > 0){
    item = heap->heapdata[0];

    last = heap->heapdata[heap->n_elements-1];
    heap->n_elements --;

    pos = 0;
    while( (i = 2 * pos + 1) < heap->n_elements) {
      //Escolhe o filho com a prioridade mais baixa
      if ( (i + 1 < heap->n_elements) && heap->compare(heap->heapdata[i], heap->heapdata[i + 1]) == GREATER ){
        //Escolhe o filho à direita caso este seja menor que o da esquerda
        i ++;
      }

      if ( heap->compare(last, heap->heapdata[i]) != GREATER ){
        //Os elementos estão na ordem correcta
        break;
      }

      heap->heapdata[pos] = heap->heapdata[i];
      pos = i;
    }

    heap->heapdata[pos] = last;
    return item;
  }

  return NULL;
}
