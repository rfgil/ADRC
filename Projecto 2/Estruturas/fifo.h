#ifndef FIFO_H_INCLUDED
#define FIFO_H_INCLUDED

typedef struct root Fifo;

Fifo * newFifo();

void InsertFifo(Fifo * fifo, void * item);
void * RemoveFifo(Fifo * fifo);

#endif
