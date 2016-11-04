#include <stdio.h>
#include <stdlib.h>

#include "Estruturas/avl_tree.h"
#include "Estruturas/list.h"
#include "Estruturas/fifo.h"

#include "domains.h"

#define TRUE 1
#define FALSE 0

enum ROUTE_TYPE {
  Client,
  Peer,
  Provider,
  Itself //Usado para definir o tipo de rota do destino para ele próprio
};

char * routeString(enum ROUTE_TYPE route){
  switch (route) {
    case Client:
      return "Client";
      break;
    case Peer:
      return "Peer";
      break;
    case Provider:
      return "Provider";
      break;
    case Itself:
      return "Destination";
      break;
    default:
      return "-";
      break;
  }
}

void traverseList(List * list, Fifo * fifo, int * visited){
  //Insere no fifo os dominios da lista de origem que ainda não foram marcados como visitados
  List * list_aux = list;

  while ( list_aux != NULL ){
    if (visited[((Domain*)list_aux->item)->position] == FALSE){
      //Marca o dominio como visitado uma vez que já se encontra na lista para ser visitado
      visited[((Domain*)list_aux->item)->position] = TRUE;
      InsertFifo(fifo, list_aux->item);
    }
    list_aux = list_aux->next;
  }
}

void CommercialRoute (AVLTree * tree, int destination){
  Domain * domain;
  int * visited;
  int end = FALSE;

  Fifo * fifo_clients, * fifo_peers, * fifo_providers;
  enum ROUTE_TYPE route;

  // Obtem-se o dominio de destino
  domain = findAvlTree(tree, &destination);
  if (domain == NULL ){
    printf("The selected destination does not exist.\n");
    return;
  }

  // Inicialização do vector
  visited = malloc(countElementsAvlTree(tree)*sizeof(int));
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    visited[i] = FALSE;
  }
  // Marca o dominio de destino como visitado
  visited[domain->position] = TRUE;

  // Define como é possivel chegar ao destino ( via ele próprio )
  route = Itself;

  fifo_clients = newFifo();
  fifo_peers = newFifo();
  fifo_providers = newFifo();

  // DFS
  do {
    if (route == Provider || route == Peer){
      //Quando a rota para o dominio é um fornecedor ou um par, apenas é possivel aceder aos clientes desse dominio
      traverseList(domain->clients, fifo_providers, visited);

    } else {
      //Quando a rota para o dominio é um cliente é possivel aceder a todos os dominios a ele ligados (fornecedores, clientes e pares)
      traverseList(domain->providers, fifo_clients, visited);
      traverseList(domain->peers, fifo_peers, visited);
      traverseList(domain->clients, fifo_providers, visited);
    }

    //Output
    printf("%d\t%d (%s)\n", domain->id, route, routeString(route));

    // Seleciona o proximo dominio a visitar com o melhor percurso (cliente > par > fornecedor)
    if ( (domain = RemoveFifo(fifo_clients)) != NULL ) {
      route = Client;
    } else if ( (domain = RemoveFifo(fifo_peers)) != NULL  ){
      route = Peer;
    } else if ( (domain = RemoveFifo(fifo_providers)) != NULL  ){
      route = Provider;
    } else {
      end = TRUE;
    }
  } while(end == FALSE);

  free(visited);
  free(fifo_clients);
  free(fifo_peers);
  free(fifo_providers);
}

int main(int argc, char *argv[]){
  FILE * network_file;
  int selection = 0;
  int destination;
  AVLTree * tree;

  if (argc < 2 || (network_file = fopen(argv[1], "r")) == NULL) {
    printf("A network topology file was not provided!\n");
    return 1;
  }

  tree = LoadTopology(network_file);
  fclose(network_file);

  while (selection != 3) {
    printf("Selecione uma opção:\n");
    printf("[1] Type of comercial route\n");
    printf("[2] Type of comercial route and number of hops\n");
    printf("[3] Quit\n");

    scanf("%d",&selection);

    switch (selection) {
      case 1:
        printf("What is the destination? ");
        scanf("%d",&destination);
        CommercialRoute(tree, destination);
        break;
      case 2:
        printf("What is the destination? ");
        scanf("%d",&destination);
        break;
      case 3:
      default:
        selection = 3; //Quit
        break;
    }
    printf("---------------\n");
  }

  freeTopology(tree);

  return 0;
}
