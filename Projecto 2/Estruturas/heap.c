#include <stdlib.h>

#include "heap.h"

#define TRUE 1
#define FALSE 0

struct heap {
  int (*compare) (void *, void *);
  int n_elements;               // elements in heap
  int size;                     //max size of the heap

  int (*getId)(void *);			    //Função para obter id dos items no heap
  int * idTrack;                //keeps track of the position in the o heap of a give id

  void ** heapdata;             //An array of Items
};

Heap * newHeap(int size, int (*compare)(void *, void *), int (*getId)(void *)){
  Heap * newHeap;

  newHeap = malloc(sizeof(Heap));

  newHeap->compare = compare;
  newHeap->n_elements = 0;
  newHeap->size = size;
  newHeap->heapdata = malloc(size * sizeof(void *));
  newHeap->getId = getId;
  newHeap->idTrack = malloc(size * sizeof(int));
  for (int i = 0; i < size; i++){
    newHeap->idTrack[i] = -1;
  }

  return newHeap;
}

void freeHeap(Heap * heap, void (*freeItem)(void *)){
  int i;

  for (i = 0; i < heap->n_elements; i++){
    freeItem(heap->heapdata[i]);
  }

  free(heap->idTrack);
  free(heap->heapdata);
  free(heap);
}

void InsertInHeap(Heap * heap, void * item){
  void * aux;
  int pos, father;

  if (heap->n_elements == heap->size){
    return; //O heap está cheio
  }

  //Define a posição do novo item como sendo o final do heap
  pos = heap->n_elements;
  heap->n_elements ++;

  heap->heapdata[pos] = item;
  heap->idTrack[heap->getId(item)] = pos;

  // FixUp
  //Procurar a posição correcta do item no heap (e faz as trocas necessárias para poder inserir nessa posição)
  while( (pos > 0) && heap->compare(heap->heapdata[(pos - 1) / 2], heap->heapdata[pos]) == GREATER ){
    father = (pos - 1) / 2;

    // Troca os elementos
    aux = heap->heapdata[father];
    heap->heapdata[father] = heap->heapdata[pos];
    heap->heapdata[pos] = aux;

    // Actualiza o idTrack;
    heap->idTrack[heap->getId(heap->heapdata[pos])] = pos;
    heap->idTrack[heap->getId(heap->heapdata[father])] = father;

    // Faz a mesma verificação na posição para a qual se trocou
    pos = father;
  }
}

static void FixDown(Heap * heap, int pos){
  int i;
  void * aux;

  while( (i = 2 * pos + 1) < heap->n_elements) {
    //Escolhe o filho com a prioridade mais baixa
    if ( (i + 1 < heap->n_elements) && heap->compare(heap->heapdata[i], heap->heapdata[i + 1]) == GREATER ){
      //Escolhe o filho à direita caso este seja menor que o da esquerda
      i ++;
    }

    // Verifica a condição do acervo
    if ( heap->compare(heap->heapdata[pos], heap->heapdata[i]) != GREATER ){
      //Os elementos estão na ordem correcta
      break;
    }

    // Troca os elementos
    aux = heap->heapdata[i];
    heap->heapdata[i] = heap->heapdata[pos];
    heap->heapdata[pos] = aux;

    // Actualiza o idTrack;
    heap->idTrack[heap->getId(heap->heapdata[i])] = i;
    heap->idTrack[heap->getId(heap->heapdata[pos])] = pos;

    // Faz a mesma verificação na posição para a qual se trocou
    pos = i;
  }
}

void * RemoveMinFromHeap(Heap * heap){
  void * item;

  if (heap->n_elements > 0){
    item = heap->heapdata[0];

    // Decrementa a quantidade de elementos no acervo
    heap->n_elements --;

    //Coloca o ultimo elemento na posição 0
    heap->heapdata[0] = heap->heapdata[heap->n_elements];

    // Actualiza o idTrack
    heap->idTrack[heap->getId(heap->heapdata[heap->n_elements])] = 0;

    // Ordena os elementos para garantir a condição de acervo
    FixDown(heap, 0);

    return item;
  }

  return NULL;
}

int DecreaseKey(Heap * heap, int id){
  if ( heap->idTrack[id] != -1) {
    FixDown(heap, heap->idTrack[id]);
    return TRUE;
  }
  return FALSE;
}

int getHeapNElements(Heap * heap){
  return heap->n_elements;
}
