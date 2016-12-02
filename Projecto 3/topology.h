#ifndef TOPOLOGY_H_INCLUDED
#define TOPOLOGY_H_INCLUDED

#include <stdio.h>

#include "Estruturas/avl_tree.h"
#include "Estruturas/list.h"

#define TRUE 1
#define FALSE 0

#define MAX_NODES 100

typedef struct edge {
  int node1;
  int node2;

  int fluxFoward;   // Fluxo do node1 para node2
  int fluxBackward; // Fluxo do node2 para node1
} Edge;

List * LoadTopology(FILE * network_file, List ** node_vector);

#endif
