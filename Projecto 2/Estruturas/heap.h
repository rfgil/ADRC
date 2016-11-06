#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef struct heap Heap;

#define SMALLER 1
#define GREATER 2
#define EQUAL 3

Heap * newHeap(int size, int (*compare)(void *, void *), int (*getId)(void *));
void freeHeap(Heap * heap, void (*freeItem)(void *));
void InsertInHeap(Heap * heap, void * item);
void * RemoveMinFromHeap(Heap * heap);
int getHeapNElements(Heap * heap);

int DecreaseKey(Heap * heap, int id);

#endif // HEAP_H_INCLUDED
