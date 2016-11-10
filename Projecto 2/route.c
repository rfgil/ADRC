#include <stdlib.h>
#include <limits.h>

#include "route.h"

#include "Estruturas/fifo.h"


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
    case None:
      return "None";
      break;
    default:
      return "-";
      break;
  }
}

static void doNothing(void * item){
  return;
}

// -----------------------------------------------------------------------------
// Comercial Routes SEM o numero de hops

static void addListToFifo_CommercialRoute(List * list, Fifo * fifo, int * visited){
  //Insere no fifo os dominios da lista
  List * list_aux = list;

  while ( list_aux != NULL ){
    InsertFifo(fifo, list_aux->item);
    list_aux = list_aux->next;
  }
}

void CommercialRoute (AVLTree * tree, Domain * destination, int isTopologyConnected, enum ROUTE_TYPE * route_vector){
  Domain * domain;
  int * visited;

  Fifo * fifo_clients, * fifo_peers, * fifo_providers;

  // Inicialização dos vectores
  visited = malloc(countElementsAvlTree(tree)*sizeof(int));
  //route_vector = malloc(countElementsAvlTree(tree)*sizeof(enum ROUTE_TYPE));
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    visited[i] = FALSE;
    route_vector[i] = isTopologyConnected == TRUE ? Provider : None; // Se for garantido que é possivel aceder a todos os dominios, o pior dos caminhos é via fornecedor
  }

  // Inicializa as listas do tipo FIFO
  fifo_clients = newFifo();
  fifo_peers = newFifo();
  fifo_providers = newFifo();

  // Define onde começa o algoritmo (a partir do destino)
  domain = destination;


  route_vector[domain->position] = Itself; // Define como é possivel chegar ao destino ( via ele próprio )

  do {
    visited[domain->position] = TRUE;

    if (route_vector[domain->position] == Client || route_vector[domain->position] == Itself){
      //Quando a rota para o dominio é um cliente é possivel aceder a todos os dominios a ele ligados (fornecedores, clientes e pares)
      addListToFifo_CommercialRoute(domain->providers, fifo_clients, visited);
      addListToFifo_CommercialRoute(domain->peers, fifo_peers, visited);
    }

    if (isTopologyConnected == FALSE){
      // Quando a rota para o dominio é um fornecedor ou um par, apenas é possivel aceder aos clientes desse dominio
      // (Se for garantido que é possivel aceder a todos os dominios, não é necessário explorar os dominios cujo caminho será via fornecedor)
      addListToFifo_CommercialRoute(domain->clients, fifo_providers, visited);
    }

    // Seleciona o proximo dominio a visitar com o melhor percurso (cliente > par > fornecedor)
    do {
      // É necessário confirmar se o dominio já foi visitado depois de o remover do fifo
      // Em caso afirmativo, deve passar-se ao dominio seguinte
      if ( (domain = RemoveFifo(fifo_clients)) != NULL) {
        route_vector[domain->position] = visited[domain->position] == TRUE ? route_vector[domain->position] : Client;

      } else if ( (domain = RemoveFifo(fifo_peers)) != NULL){
        route_vector[domain->position] = visited[domain->position] == TRUE ? route_vector[domain->position] : Peer;

      } else if ( (domain = RemoveFifo(fifo_providers)) != NULL ){
        route_vector[domain->position] = visited[domain->position] == TRUE ? route_vector[domain->position] : Provider;
      }
    } while(domain != NULL && visited[domain->position] == TRUE);

  } while(domain != NULL);

  // Liberta a memória utilizada
  free(visited);

  freeFifo(fifo_clients, doNothing);
  freeFifo(fifo_peers, doNothing);
  freeFifo(fifo_providers, doNothing);
}

// -----------------------------------------------------------------------------
// Comercial Routes COM o numero de hops

static void addListToFifo_CommercialRouteHops(List * list, Fifo * fifo, Domain * source, enum ROUTE_TYPE route, enum ROUTE_TYPE * route_vector, int * n_hops, int * visited){
  //Insere no fifo os dominios da lista de origem que ainda não foram marcados como visitados
  List * list_aux = list;
  Domain * neighbor_domain;

  while ( list_aux != NULL ){
    neighbor_domain = (Domain*)list_aux->item;

    if (route_vector[neighbor_domain->position] == route
        && visited[neighbor_domain->position] == FALSE){

          visited[neighbor_domain->position] = TRUE;
          n_hops[neighbor_domain->position] = n_hops[source->position] + 1;
          InsertFifo(fifo, neighbor_domain);
    }
    // Avança na lista de vizinhos
    list_aux = list_aux->next;
  }
}

void CommercialRouteHops(AVLTree * tree, Domain * destination, int isTopologyConnected, enum ROUTE_TYPE * route_vector, int * n_hops){
  int * visited;
  Fifo * fifo;
  Domain * domain;

  // Inicializar os vectores
  CommercialRoute(tree, destination, isTopologyConnected, route_vector);
  visited = malloc(countElementsAvlTree(tree)*sizeof(int));
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    n_hops[i] = 0;
    visited[i] = FALSE;
  }

  fifo = newFifo();

  domain = destination;
  visited[domain->position] = TRUE;
  do {
    addListToFifo_CommercialRouteHops(domain->clients, fifo, domain, Provider, route_vector, n_hops, visited);
    if (route_vector[domain->position] == Client || route_vector[domain->position] == Itself){
      addListToFifo_CommercialRouteHops(domain->peers, fifo, domain, Peer, route_vector, n_hops, visited);
      addListToFifo_CommercialRouteHops(domain->providers, fifo, domain, Client, route_vector, n_hops, visited);
    }

    //printf("%d\tHops: %d\n", domain->id, n_hops[domain->position]);
    domain = RemoveFifo(fifo);

  } while(domain != NULL);

  free(visited);
  freeFifo(fifo, doNothing);
}


// -----------------------------------------------------------------------------
// Numero de hops sem restrições

static void addListToFifo_NoRestrictionHops(List * list, Fifo * fifo, Domain * source, int * n_hops, int * visited){
  //Insere no fifo os dominios da lista de origem que ainda não foram marcados como visitados
  List * list_aux = list;
  Domain * neighbor_domain;

  while ( list_aux != NULL ){
    neighbor_domain = (Domain*)list_aux->item;

    if (visited[neighbor_domain->position] == FALSE){

          visited[neighbor_domain->position] = TRUE;
          n_hops[neighbor_domain->position] = n_hops[source->position] + 1;
          InsertFifo(fifo, neighbor_domain);
    }
    // Avança na lista de vizinhos
    list_aux = list_aux->next;
  }
}

void NoRestrictionHops(AVLTree * tree, Domain * destination, int * n_hops){
  int * visited;
  Fifo * fifo;
  Domain * domain;

  // Inicializar os vector
  visited = malloc(countElementsAvlTree(tree)*sizeof(int));
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    n_hops[i] = 0;
    visited[i] = FALSE;
  }

  fifo = newFifo();

  domain = destination;
  visited[domain->position] = TRUE;
  do {
    addListToFifo_NoRestrictionHops(domain->clients, fifo, domain, n_hops, visited);
    addListToFifo_NoRestrictionHops(domain->peers, fifo, domain, n_hops, visited);
    addListToFifo_NoRestrictionHops(domain->providers, fifo, domain, n_hops, visited);

    domain = RemoveFifo(fifo);
  } while(domain != NULL);

  free(visited);
  freeFifo(fifo, doNothing);
}
