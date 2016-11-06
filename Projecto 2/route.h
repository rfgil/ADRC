#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include <stdio.h>

#include "Estruturas/avl_tree.h"

enum ROUTE_TYPE {
  Itself, //Usado para definir o tipo de rota do destino para ele próprio
  Client,
  Peer,
  Provider,
  None
};

void CommercialRoute (AVLTree * tree, int destination);

char * routeString(enum ROUTE_TYPE route);
void CommercialRouteHops(AVLTree * tree, int destination);

#endif
