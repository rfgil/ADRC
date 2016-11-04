
#ifndef DOMAINS_H_INCLUDED
#define DOMAINS_H_INCLUDED

#include "Estruturas/avl_tree.h"
#include "Estruturas/list.h"

typedef struct domain {
  int id;
  int position;

  List * clients;
  List * providers;
  List * peers;
} Domain;


AVLTree * LoadTopology(FILE * network_file);

void freeTopology(AVLTree * tree);

#endif
