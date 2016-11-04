#include <stdlib.h>
#include <stdio.h>

#include "domains.h"

int compare(int a, int b){
  if (a > b){
    return GREATER;
  } else if (a < b) {
    return SMALLER;
  } else {
    return EQUAL;
  }
}

int compareDomains(void * item1, void * item2){
  int id1 = ((Domain *) item1)->id;
  int id2 = ((Domain *) item2)->id;

  return compare(id1, id2);
}

int compareId_Domain(void * item1, void * item2){
  int id1 = *((int *) item1);
  int id2 = ((Domain *) item2)->id;

  return compare(id1, id2);
}

Domain * createDomain(int id){
  Domain * new;

  new = malloc(sizeof(Domain));
  new->id = id;
  new->clients = newList();
  new->providers = newList();
  new->peers = newList();

  return new;
}

AVLTree * LoadTopology(FILE * network_file){
  AVLTree * tree;
  Domain * DomainTail, * DomainHead;
  int head, tail, type;

  tree = newAvlTree(compareDomains, compareId_Domain);
  while( fscanf(network_file, "%d %d %d ", &tail, &head, &type) == 3 ){
    //Procura na tree se os Domains que se pretendem já existem
    DomainTail = findAvlTree(tree, &tail);
    DomainHead = findAvlTree(tree, &head);

    //Cria novas estruturas para esses Domains (se não existirem)
    if (DomainTail == NULL){
      DomainTail = createDomain(tail);
      DomainTail->position = countElementsAvlTree(tree);
      insertAvlTree(tree, DomainTail);
    }
    if (DomainHead == NULL){
      DomainHead = createDomain(head);
      DomainHead->position = countElementsAvlTree(tree);
      insertAvlTree(tree, DomainHead);
    }

    //Adiciona a Head à lista correspondente (cliente, par ou fornecedor) da Tail
    switch (type) {
      case 1: //Provider: Tail é fornecedor da Head -> A Head tem que ir para a lista de clientes da Tail.
        DomainTail->clients = insertList(DomainTail->clients, DomainHead);
        break;

      case 2: //Peer: Tail é par da Head -> A Head tem que ir para a lista de pares da Tail.
        DomainTail->peers = insertList(DomainTail->peers, DomainHead);
        break;

      case 3: //Client: Tail é cliente da Head -> A Head tem que ir para a lista de fornecedores da Tail.
        DomainTail->providers = insertList(DomainTail->providers, DomainHead);
        break;

      default:
        printf("An unexpected value was found in network topology file.\n");
        break;
    }
  }

  return tree;
}

void doNothing(void * item){
  return;
}

void freeDomain(void * item){
  Domain * domain = item;
  freeList(domain->clients, doNothing);
  freeList(domain->peers, doNothing);
  freeList(domain->providers, doNothing);
  free(domain);
}

void freeTopology(AVLTree * tree){
  freeAvlTree(tree, freeDomain);
}
