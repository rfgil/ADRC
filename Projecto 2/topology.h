#ifndef TOPOLOGY_H_INCLUDED
#define TOPOLOGY_H_INCLUDED

#include "Estruturas/avl_tree.h"
#include "Estruturas/list.h"

#define TRUE 1
#define FALSE 0

typedef struct domain {
  int id;
  int position;

  int n_clients, n_peers, n_providers;

  List * clients;
  List * providers;
  List * peers;
} Domain;

AVLTree * LoadTopology(FILE * network_file, int * idPosition);
void freeTopology(AVLTree * tree);

int checkConnectivity(AVLTree * tree);

#endif
