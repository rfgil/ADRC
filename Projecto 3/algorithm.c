#include <stdio.h>
#include <stdlib.h>

#include "Estruturas/fifo.h"

#include "topology.h"

#define GROUP_A 100 //Diferente dos valores defindos para TRUE E FALSE
#define GROUP_B 200 //Diferente dos valores defindos para TRUE E FALSE

/*
static void doNothing(void * item){
  return;
}
*/
static void setFlowToZero(List * edgeList){
  Edge * edge;
  while(edgeList != NULL){
    edge = edgeList->item;
    edge->flowBackward = 0;
    edge->flowFoward = 0;

    edgeList = edgeList->next;
  }
}

static int AugmentingPath_DFS(int currentNode, int destNode, int * visited, List ** node_vector){
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
      if (edgeAllowsAugmenting(currentNode, currentEdge) && AugmentingPath_DFS(neighbor, destNode, visited, node_vector)){
        // Se conseguiu chegar ao destino, marcar o fluxo do edge como preenchido
        insertFlow(currentNode, currentEdge);
        break;
      }
    }
    nodeEdges = nodeEdges->next;
  }

  // Devolve TRUE se o while foi terminado porque foi possivel chegar ao destino
  // Ou FALSE se se esgotaram os vizinhos do currentNode sem chegar ao destino
  return (nodeEdges != NULL);
}

int getDisjointPaths(int startNode, int destNode, List ** node_vector, List * edgeList){
  int visited[MAX_NODES], count = 0;
  List * auxList;

  // Iniciliza os valores do fluxo nos edges
  setFlowToZero(edgeList);

  // Calcula os caminhos disjuntos
  do{
    for(int i = 0; i < MAX_NODES; i++){
      visited[i] = FALSE;
    }
  } while( AugmentingPath_DFS(startNode, destNode, visited, node_vector) == TRUE );


  // Contar quantidade de fluxo que chega ao destino (ou seja o nº de caminhos disjuntos)
  auxList = node_vector[destNode];
  while (auxList != NULL) {
    // Confirma se há fluxo no edge
    if (hasFlow(auxList->item)){
      count += 1;
    }
    auxList = auxList ->next;
  }

  return count;
}

static void walkResidualGraph_DFS(int currentNode, List ** node_vector, int * visited){
  List * edgeList;
  int neighbor;

  visited[currentNode] = GROUP_A;

  edgeList = node_vector[currentNode];
  while(edgeList != NULL){
    neighbor = getNeighborNode(edgeList->item, currentNode);

    if (visited[neighbor] != GROUP_A && canCrossEdgeResidualGraph(currentNode, edgeList->item)){
      walkResidualGraph_DFS(neighbor, node_vector, visited);
    }
    edgeList = edgeList->next;
  }
}

static void findMinimumCut_DFS(int currentNode, List ** node_vector, int * visited, List ** result){
  List * edgeList;
  int neighbor;

  visited[currentNode] = GROUP_B;

  edgeList = node_vector[currentNode];
  while(edgeList != NULL){
    neighbor = getNeighborNode(edgeList->item, currentNode);

    if(visited[neighbor] == GROUP_A){
      //Encontrou-se um edge que faz parte do corte mínimo
      //printf("Remover edge %d -> %d\n", ((Edge *)edgeList->item)->node1, ((Edge *)edgeList->item)->node2);
      *result = insertList(*result, edgeList->item);

    } else if(visited[neighbor] != GROUP_B && canCrossEdgeResidualGraph(currentNode, edgeList->item)){
      findMinimumCut_DFS(neighbor, node_vector, visited, result);
    }

    edgeList = edgeList->next;
  }
}

List * findMinimumEdgesToDisconnect(int startNode, int destNode, List ** node_vector){
  List * removableEdges;
  int visited[MAX_NODES];
  // Esta função assume que os fluxo já estão correctamente assinalados em todos os edges

  for(int i = 0; i < MAX_NODES; i++){
    visited[i] = FALSE;
  }

  removableEdges = newList();

  walkResidualGraph_DFS(startNode, node_vector, visited);
  findMinimumCut_DFS(destNode, node_vector, visited, &removableEdges);

  return removableEdges;
}
