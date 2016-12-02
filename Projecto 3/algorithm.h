#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "Estruturas/list.h"

int getDisjointPaths(int startNode, int destNode, List ** node_vector, List * edgeList);

List * findMinimumEdgesToDisconnect(int startNode, int destNode, List ** node_vector);

#endif
