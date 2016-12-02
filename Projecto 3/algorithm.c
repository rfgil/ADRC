#include <stdio.h>
#include <stdlib.h>

#include "Estruturas/fifo.h"

#include "topology.h"


int getNeighborNode(Edge * edge, int currentNode){
  return edge->node1 != currentNode ? edge->node1 : edge->node2;
}

int checkFlux(int currentNode, Edge * edge){

  if (currentNode == edge->node1){
    // Verifica se NÃO HÁ fluxo de 1 para 2 ou se HÁ fluxo de 2 para 1
    return (!edge->fluxFoward || edge->fluxBackward);

  } else {
    // Verifica se NÃO HÁ fluxo de 2 para 1 ou se HÁ fluxo de 1 para 2
    return (!edge->fluxBackward || edge->fluxFoward);
  }
}

int checkFluxEntering(int currentNode, Edge * edge){
  if (currentNode == edge->node1){
    return edge->fluxBackward;
  } else {
    return edge->fluxFoward;
  }
}

void insertFlux(int currentNode, Edge * edge){
  if (currentNode == edge->node1){

    if (edge->fluxBackward){
      edge->fluxBackward = FALSE;
    } else {
      edge->fluxFoward = TRUE;
    }

  } else {

    if (edge->fluxFoward){
      edge->fluxFoward = FALSE;
    } else {
      edge->fluxBackward = TRUE;
    }

  }
}

int DFS(int currentNode, int destNode, int * visited, List ** node_vector){
  List * nodeEdges = node_vector[currentNode];
  int neighbor;
  Edge * currentEdge;

  // Marca o node a ser analisado como visitado
  visited[currentNode] = TRUE;

  // Verifica se o próprio nó é o destino
  if (currentNode == destNode){
    return TRUE;
  }

  while(nodeEdges != NULL){
    currentEdge = (Edge *) nodeEdges->item;
    neighbor = getNeighborNode(currentEdge, currentNode);

    // Verifica se este node já foi visitado
    if (visited[neighbor] == FALSE){

      //Verifica se este edge permite passagem de fluxo e se o seu vizinho consegue chegar ao destino
      if (checkFlux(currentNode, currentEdge) && DFS(neighbor, destNode, visited, node_vector)){
        // Se conseguiu chegar ao destino, marcar o fluxo do edge como preenchido
        insertFlux(currentNode, currentEdge);
        break;
      }
    }
    nodeEdges = nodeEdges->next;
  }

  // Verifica se o while foi terminado porque foi possivel chegar ao destino
  // Ou porque se esgotaram os vizinhos do currentNode sem chegar ao destino
  return (nodeEdges != NULL);
}


int hasFlux(Edge * edge){
  return edge->fluxFoward ^ edge->fluxBackward;
}


List * BFS(int currentNode, List ** node_vector){
  List * nodeEdges, * stack;
  Fifo * fifo;
  int * nextNode, neighbor, visited[MAX_NODES];

  // Inicializa as listas
  stack = newList();
  fifo = newFifo();

  // Inicializa o vector visited
  for(int i = 0; i < MAX_NODES; i++){
    visited[i] = FALSE;
  }
  visited[currentNode] = TRUE;

  // Insere o node onde se inicia a BFS no fifo e na stack
  nextNode = malloc(sizeof(int));
  *nextNode = currentNode;
  stack  = insertList(stack, nextNode);
  InsertFifo(fifo, nextNode);

  // Executa a BFS colocando na stack todos os nodes que é possivel alcançar sem passar pelos caminhos disjuntos
  while( (nextNode = RemoveFifo(fifo)) != NULL){
    currentNode = *nextNode;

    nodeEdges = node_vector[currentNode];

    // Avalia cada um dos edges acessiveis diretamente por esse node
    while(nodeEdges != NULL){
      neighbor = getNeighborNode(nodeEdges->item, currentNode);

      // Caso este edge não tenha fluxo (não faça parte do caminh disjunto)
      // e o node a que ele permita chegar ainda não tenha sido visitado
      if (!visited[neighbor] && !hasFlux(nodeEdges->item)){
        visited[neighbor] = TRUE;

        nextNode = malloc(sizeof(int));
        *nextNode = neighbor;

        // Insere o node na stack e no fifo
        stack  = insertList(stack, nextNode);
        InsertFifo(fifo, nextNode);
      }

      nodeEdges = nodeEdges->next;
    }
  }

  return stack;
}

void selectEdges(List * stack, List ** node_vector, int count){
  int * node;
  List * nodeEdges;
  Edge * edge;

  while( stack != NULL && count > 0){
    stack = removeFirstList(stack, (void **) &node);

    nodeEdges = node_vector[*node];
    while(nodeEdges != NULL){
      edge = nodeEdges->item;
      if (checkFluxEntering(*node, edge)){
        printf("Remover edge %d -> %d\n", edge->node1, edge->node2);
        count --;
      }
      nodeEdges = nodeEdges->next;
    }
    free(node);
  }
}


void getDisjointPaths(int startNode, int destNode, List ** node_vector){
  int visited[MAX_NODES], count = 0;

  List * auxList, * stackBFS;

  // Calcula os caminhos disjuntos
  do{
    for(int i = 0; i < MAX_NODES; i++){
      visited[i] = FALSE;
    }
  } while( DFS(startNode, destNode, visited, node_vector) == TRUE );


  // Contar quantidade de fluxo que chega ao destino (ou seja o nº de caminhos disjuntos)
  auxList = node_vector[destNode];
  while (auxList != NULL) {
    // Confirma se há fluxo no edge
    if (hasFlux(auxList->item)){
      count += 1;
    }
    auxList = auxList ->next;
  }

  printf("Existem %d caminho(s) disjuntos que permitem chegar ao node %d.\n", count, destNode);

  stackBFS = BFS(destNode, node_vector);
  selectEdges(stackBFS, node_vector, count);
}
