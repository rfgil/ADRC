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

  while( fscanf(network_file, "%d %d ", &node1, &node2) == 2 ){
    // Criar edge
    new_edge = malloc(sizeof(Edge));

    new_edge->node1 = node1;
    new_edge->node2 = node2;

    new_edge->fluxFoward = 0;
    new_edge->fluxBackward = 0;

    // Inserir edge na lista dos nodes correspondentens
    node_vector[node1] = insertList(node_vector[node1], new_edge);
    node_vector[node2] = insertList(node_vector[node2], new_edge);

    edgeList = insertList(edgeList, new_edge);
  }

  return edgeList;
}
