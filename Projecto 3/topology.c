#include <stdlib.h>
#include <stdio.h>

#include "topology.h"

List * LoadTopology(FILE * network_file, List ** node_vector){
  int node1, node2;
  Edge * new_edge;

  List * edgeList;

  // Inicializa vector dos nodes
  // (cada posição corresponderá a uma lista com os edges que esse node consegue aceder directamente)
  for(int i = 0; i < MAX_NODES; i++){
    node_vector[i] = newList();
  }

  edgeList = newList();

  while( fscanf(network_file, "%d %d ", &node1, &node2) == 2 ){
    // Criar edge
    new_edge = malloc(sizeof(Edge));

    new_edge->node1 = node1;
    new_edge->node2 = node2;

    new_edge->flowFoward = 0;
    new_edge->flowBackward = 0;

    // Inserir edge na lista dos nodes correspondentens
    node_vector[node1] = insertList(node_vector[node1], new_edge);
    node_vector[node2] = insertList(node_vector[node2], new_edge);

    edgeList = insertList(edgeList, new_edge);
  }

  return edgeList;
}

int getNeighborNode(Edge * edge, int currentNode){
  return edge->node1 != currentNode ? edge->node1 : edge->node2;
}

int edgeAllowsAugmenting(int originNode, Edge * edge){
  // Verifica se é possivel introduzir fluxo em edge tendo em conta o node de origem
  if (originNode == edge->node1){
    // Verifica se NÃO HÁ fluxo de 1 para 2 ou se HÁ fluxo de 2 para 1
    return (!edge->flowFoward || edge->flowBackward);

  } else if (originNode == edge->node2) {
    // Verifica se NÃO HÁ fluxo de 2 para 1 ou se HÁ fluxo de 1 para 2
    return (!edge->flowBackward || edge->flowFoward);
  }
  return FALSE;
}

void insertFlow(int originNode, Edge * edge){
  if (originNode == edge->node1){
    // Garante que nunca é possivel ter fluxo para a frente e simultaneamente para trás
    // Ou seja flowFoward = TRUE e flowBackward = TRUE simultaneamente
    // O que equivale a não ter fluxo

    if (edge->flowBackward){
      edge->flowBackward = FALSE;
    } else {
      edge->flowFoward = TRUE;
    }

  } else {

    if (edge->flowFoward){
      edge->flowFoward = FALSE;
    } else {
      edge->flowBackward = TRUE;
    }

  }
}

int hasFlow(Edge * edge){
  return edge->flowFoward ^ edge->flowBackward;
}

int canCrossEdgeResidualGraph(int originNode, Edge * edge){
  if (originNode == edge->node1){
    return !edge->flowFoward;
  } else {
    return !edge->flowBackward;
  }
}

static void doNothing(void * item){
  return;
}

void freeTopology(List ** node_vector){
  for(int i = 0; i < MAX_NODES; i++){
    freeList(node_vector[i], doNothing);
  }

}
