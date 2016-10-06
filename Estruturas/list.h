#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

typedef struct _element ListElement;

ListElement * newList();
ListElement * insertInList(ListElement * list, void * item);
void freeList(ListElement * list, void (*freeItem)(void *));

void printList(ListElement * list, void (*printItem)(void *));

#endif // LISTA_H_INCLUDED
