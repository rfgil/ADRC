#include <stdlib.h>
#include <stdio.h>

#include "topology.h"

static void doNothing(void * item){
  return;
}

static int compare(int a, int b){
  if (a > b){
    return GREATER;
  } else if (a < b) {
    return SMALLER;
  } else {
    return EQUAL;
  }
}

static int compareDomains(void * item1, void * item2){
  //Compara dois dominios
  int id1 = ((Domain *) item1)->id;
  int id2 = ((Domain *) item2)->id;

  return compare(id1, id2);
}

static int compareId_Domain(void * item1, void * item2){
  //Compara um id (de um possivel dominio) com um dominio
  // É necessário para confirmar se determinado id já existe na árvore antes de o inserir
  int id1 = *((int *) item1);
  int id2 = ((Domain *) item2)->id;

  return compare(id1, id2);
}

Domain * createDomain(int id){
  Domain * new;

  new = malloc(sizeof(Domain));
  new->id = id;

  new->n_clients  = 0;
  new->n_peers  = 0;
  new->n_providers  = 0;

  new->clients = newList();
  new->providers = newList();
  new->peers = newList();

  return new;
}

AVLTree * LoadTopology(FILE * network_file, int * idPosition){
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
      idPosition[DomainTail->position] = DomainTail->id;
      insertAvlTree(tree, DomainTail);
    }
    if (DomainHead == NULL){
      DomainHead = createDomain(head);
      DomainHead->position = countElementsAvlTree(tree);
      idPosition[DomainHead->position] = DomainHead->id;
      insertAvlTree(tree, DomainHead);
    }

    //Adiciona a Head à lista correspondente (cliente, par ou fornecedor) da Tail
    switch (type) {
      case 1: //Provider: Tail é fornecedor da Head -> A Head tem que ir para a lista de clientes da Tail.
        DomainTail->clients = insertList(DomainTail->clients, DomainHead);
        DomainTail->n_clients += 1;
        break;

      case 2: //Peer: Tail é par da Head -> A Head tem que ir para a lista de pares da Tail.
        DomainTail->peers = insertList(DomainTail->peers, DomainHead);
        DomainTail->n_peers += 1;
        break;

      case 3: //Client: Tail é cliente da Head -> A Head tem que ir para a lista de fornecedores da Tail.
        DomainTail->providers = insertList(DomainTail->providers, DomainHead);
        DomainTail->n_providers += 1;
        break;

      default:
        printf("An unexpected value was found in network topology file.\n");
        break;
    }
  }

  return tree;
}

static void getTopProviders(void * AvlTreeNode, List ** list, int * count){
  Domain * domain;

  if (AvlTreeNode == NULL){
    return;
  }

  // Percorre todos os elementos da AVL Tree
  getTopProviders(getAvlTreeLeftChildNode(AvlTreeNode), list, count); // Avalia o filho à esquerda
  getTopProviders(getAvlTreeRightChildNode(AvlTreeNode), list, count); // Avalia o filho à direita

  domain = getAvlTreeNodeItem(AvlTreeNode);
  if (domain->n_providers == 0){
    *list = insertList(*list, domain);
    *count += 1;
  }
}

int checkConnectivity(AVLTree * tree){
  List * top_providers, * aux1, * aux2;
  int n_top_providers, result;
  Domain * domain;

  top_providers = newList();
  n_top_providers = 0;

  getTopProviders(getAvlTreeRootNode(tree), &top_providers, &n_top_providers);

  // Verifica se todos os providers de topo são pares entre si
  aux1 = top_providers;
  result = TRUE;
  while(aux1 != NULL){
    domain = ((Domain *)aux1->item);
    aux1 = aux1->next; // Passa para o o dominio de topo seguinte

    // aux2 já é o elemento seguinte da lista de dominios de topo
    // (não faz sentido confirmar se o próprio dominio está na sua lista de peers)
    aux2 = aux1;

    // Verifica se todos os restantes dominios de topo estão na lista de pares do domino a avaliar
    // (Não é necessário verificar os elementos anteriores uma vez que a ligação é reciproca)
    while( aux2 != NULL ){
      if (findListItem(domain->peers, aux2->item) == FALSE){
        result = FALSE;
        aux1 = NULL; // Condição para sair do segundo while
        break;
      };
      aux2 = aux2->next;
    }
  }

  freeList(top_providers, doNothing);
  return result;
}

static void freeDomain(void * item){
  Domain * domain = item;
  freeList(domain->clients, doNothing);
  freeList(domain->peers, doNothing);
  freeList(domain->providers, doNothing);
  free(domain);
}

void freeTopology(AVLTree * tree){
  freeAvlTree(tree, freeDomain);
}
