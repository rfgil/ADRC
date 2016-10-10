#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef struct heap Heap;

#define GREATER 1
#define SMALLER 2

Heap * newHeap(int size, int (*compare)(void *, void *));
void freeHeap(Heap * heap, void (*freeItem)(void *));
void InsertInHeap(Heap * heap, void * item);
void * RemoveMaxFromHeap(Heap * heap);
int getHeapNElements(Heap * heap);

#endif // HEAP_H_INCLUDED
