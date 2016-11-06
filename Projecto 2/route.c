#include <limits.h>

#include "route.h"
#include "topology.h"

#include "Estruturas/heap.c"
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

static void traverseList(List * list, Fifo * fifo, int * visited){
  //Insere no fifo os dominios da lista de origem que ainda não foram marcados como visitados
  List * list_aux = list;
  Domain * neighbor_domain;

  while ( list_aux != NULL ){
    neighbor_domain = (Domain*)list_aux->item;

    // Verifica se o dominio já foi visitado
    if (visited[neighbor_domain->position] == FALSE){

      // Marca o dominio como visitado uma vez que já se encontra na lista para ser visitado
      visited[neighbor_domain->position] = TRUE;

      // Insere o dominio na lista FIFO
      InsertFifo(fifo, list_aux->item);
    }

    // Avança na lista de vizinhos
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

  // Inicializa as listas do tipo FIFO
  fifo_clients = newFifo();
  fifo_peers = newFifo();
  fifo_providers = newFifo();

  // DFS
  do {
    //Quando a rota para o dominio é um fornecedor ou um par, apenas é possivel aceder aos clientes desse dominio
    traverseList(domain->clients, fifo_providers, visited);
    if (route == Client || route == Itself){
      //Quando a rota para o dominio é um cliente é possivel aceder a todos os dominios a ele ligados (fornecedores, clientes e pares)
      traverseList(domain->providers, fifo_clients, visited);
      traverseList(domain->peers, fifo_peers, visited);
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

  // Liberta a memória utilizada
  free(visited);

  freeFifo(fifo_clients, doNothing);
  freeFifo(fifo_peers, doNothing);
  freeFifo(fifo_providers, doNothing);
}

// -----------------------------------------------------------------------------
// Comercial Routes COM o numero de hops

struct path_info {
  Domain * domain;
  int n_hops;
  int visited;
  enum ROUTE_TYPE route;
};

static int comparePathInfo(void * a, void * b){
  // Compara dois path_info, para que possa ser estabelecido um prioridade no heap
  struct path_info * item1 = a;
  struct path_info * item2 = b;

  if (item1->route == item2->route){
    // Está a usar-se um heap de minimos e pretende-se que o mínimo esteja no topo
    return item1->n_hops > item2->n_hops ? GREATER : SMALLER;
  } else {
    return item1->route > item2->route ? GREATER : SMALLER;
  }
}

static int getPathInfoId(void * item){
  // Devolve o indice do dominio de um struct path_info
  return ((struct path_info *)item)->domain->position;
}

static void evaluateDomains(List * list, struct path_info * source, enum ROUTE_TYPE route, struct path_info ** path_vector, Heap * heap){
  List * list_aux = list;
  Domain * neighbor_domain;
  int neighbor_pos;

  while ( list_aux != NULL ){
    // Obtem informação do vizinho a ser avaliado
    neighbor_domain = (Domain*)list_aux->item;
    neighbor_pos = neighbor_domain->position;

    // Confirma se este vizinho do dominio já foi visitado
    if (path_vector[neighbor_pos]->visited == FALSE){

      // Averigua se a perspectiva do vizinho é melhorada a partir do dominio "source"
      if ( route <= path_vector[neighbor_pos]->route
        && path_vector[neighbor_pos]->n_hops > source->n_hops + 1 ){

          // Relaxa o dominio
          path_vector[neighbor_pos]->domain = neighbor_domain;
          path_vector[neighbor_pos]->n_hops = source->n_hops + 1;
          path_vector[neighbor_pos]->route = route;

          // Actualiza a posição do vizinho no heap
          if ( DecreaseKey(heap, neighbor_pos) == FALSE ){
            // Caso o vizinho não se encontre no heap é inserido
            InsertInHeap(heap, path_vector[neighbor_pos]);
          }
      }
    }

    // Avança-se na lista de vizinhos do dominio de origem
    list_aux = list_aux->next;
  }
}

void CommercialRouteHops(AVLTree * tree, int destination){
  Domain * domain;
  struct path_info * path;
  struct path_info ** path_vector;
  Heap * heap;

  // Obtem-se o dominio de destino
  domain = findAvlTree(tree, &destination);
  if (domain == NULL ){
    printf("The selected destination does not exist.\n");
    return;
  }

  // Inicialização do vector
  path_vector = malloc(countElementsAvlTree(tree)*sizeof(struct path_info *));
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    path_vector[i] = malloc(sizeof(struct path_info));

    path_vector[i]->domain = NULL;
    path_vector[i]->n_hops = INT_MAX;
    path_vector[i]->visited = FALSE;
    path_vector[i]->route = None;
  }

  // Relaxa o dominio onde o algoritmo de Dijkstra terá inicio (o destino)
  path_vector[domain->position]->domain = domain;
  path_vector[domain->position]->n_hops = 0;
  path_vector[domain->position]->visited = TRUE;
  path_vector[domain->position]->route = Itself;

  // Define o dominio onde começa o algoritmo
  path = path_vector[domain->position];

  // Cria o heap
  heap = newHeap(countElementsAvlTree(tree), comparePathInfo, getPathInfoId);

  do {
    // Marca o dominio a ser avaliado como visitado
    path->visited = TRUE;

    // Os clientes de qualquer dominio são visitados
    evaluateDomains(path->domain->clients, path, Provider, path_vector, heap);
    if (path->route == Client || path->route == Itself){
      // Caso o percurso até ao domain seja Client ou ele próprio, todos os seus vizinhos serão visitados (Fornecedores e pares)
      evaluateDomains(path->domain->peers, path, Peer, path_vector, heap);
      evaluateDomains(path->domain->providers, path, Client, path_vector, heap);
    }

    // Output
    printf("%d\t%d Hops (%s)\n", path->domain->id, path->n_hops, routeString(path->route));

    // Remove o minimo do heap: O proximo dominio a ser visitado
    path = RemoveMinFromHeap(heap);
  } while(path != NULL);


  // Liberta a memória
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    free(path_vector[i]);
  }
  free(path_vector);
  freeHeap(heap, doNothing);
}
