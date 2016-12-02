#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Estruturas/avl_tree.h"

#include "topology.h"
#include "algorithm.h"

static void doNothing(void * item){
  return;
}

void printEdgesList(List * edgeList){
  while(edgeList != NULL){
    printf("%d -> %d\n", ((Edge *)edgeList->item)->node1, ((Edge *)edgeList->item)->node2);
    edgeList = edgeList->next;
  }
}

void complementaryCumulativeDistribution(List ** node_vector, List * edgeList){
  int nEdgesSeparation[MAX_NODES];

  for (int i = 0; i < MAX_NODES; i++){
    nEdgesSeparation[i] = 0;
  }

  for(int i = 0; i < MAX_NODES; i++){
    for(int j = i + 1; j < MAX_NODES; j++){
      nEdgesSeparation[getDisjointPaths(i, j, node_vector, edgeList)] ++;
    }
  }

  for (int i = 1; i < MAX_NODES; i++){
    printf("%2d - %d \n", i, nEdgesSeparation[i]);
  }
}

void edgeConnectivity(List ** node_vector, List * edgeList){
  int numDisjointPaths, edgeConnectivity = INT_MAX;
  List * removableEdges;

  removableEdges = newList();

  for(int i = 0; i < MAX_NODES; i++){
    for(int j = i + 1; j < MAX_NODES; j++){
      numDisjointPaths = getDisjointPaths(i, j, node_vector, edgeList);

      if ( numDisjointPaths != 0 && numDisjointPaths < edgeConnectivity){

        freeList(removableEdges, doNothing);
        removableEdges = findMinimumEdgesToDisconnect(i, j, node_vector);
        edgeConnectivity = numDisjointPaths;
      }
    }
  }

  printf("This network has a edge-connectivity of %d.\n", edgeConnectivity);
  printf("The following edges are an exemple of a cut that prevents the network from being connected:\n");
  printEdgesList(removableEdges);

  freeList(removableEdges, doNothing);
}

int main(int argc, char *argv[]){
  FILE * network_file;

  List * node_vector[MAX_NODES], * edgeList, * auxList;
  int source, destination, numDisjointPaths, selection = 0;

  // Verifica se foi introduzido como argumento o ficheiro com a topologia do grafo
  if (argc < 2 || (network_file = fopen(argv[1], "r")) == NULL) {
    printf("A network topology file was not provided!\n");
    return 1;
  }

  edgeList = LoadTopology(network_file, node_vector);
  fclose(network_file);

  while (selection != 4) {
    printf("Select an option:\n");
    printf("[1] Find minimum edges to disconnect two nodes.\n");
    printf("[2] Complementary cumulative distribution of the minimum number of edges that separates one node from another.\n");
    printf("[3] Compute the edge-connectivity of the network.\n");
    printf("[4] Quit\n");
    scanf("%d",&selection);

    // Confirma que foi selecionado um dominio de destino válido

    switch (selection) {
      case 1:
        printf("What is the source? ");
        scanf("%d", &source);
        printf("What is the destination? ");
        scanf("%d", &destination);
        if (source == destination){
          printf("Error: Origin is equal to the destination.\n");
          break;
        }

        numDisjointPaths = getDisjointPaths(source, destination, node_vector, edgeList);
        auxList = findMinimumEdgesToDisconnect(source, destination, node_vector);

        printf("There are %d disjoint paths from node %d to node %d.\n", numDisjointPaths, source, destination);
        printf("The removal of the following edges allow to separate source from destination:\n");
        printEdgesList(auxList);

        freeList(auxList, doNothing);
        break;

      case 2:
        complementaryCumulativeDistribution(node_vector, edgeList);
        break;

      case 3:
        edgeConnectivity(node_vector, edgeList);
        break;

      case 4:
      default:
        selection = 4; //Quit
        break;
    }
      printf("---------------\n");
  }

  freeList(edgeList, free);
  freeTopology(node_vector);

  return 0;
}
