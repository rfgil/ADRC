#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include <stdio.h>
#include "topology.h"
#include "Estruturas/avl_tree.h"

enum ROUTE_TYPE {
  Itself, //Usado para definir o tipo de rota do destino para ele próprio
  Client,
  Peer,
  Provider,
  None
};
char * routeString(enum ROUTE_TYPE route);

void CommercialRoute (AVLTree * tree, Domain * destination, int isTopologyConnected, enum ROUTE_TYPE * route_vector);
void CommercialRouteHops(AVLTree * tree, Domain * destination, int isTopologyConnected, enum ROUTE_TYPE * route_vector, int * n_hops);
void NoRestrictionHops(AVLTree * tree, Domain * destination, int * n_hops);
#endif
