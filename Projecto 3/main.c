#include <stdio.h>
#include <stdlib.h>

#include "Estruturas/avl_tree.h"

#include "topology.h"
#include "algorithm.h"

int main(int argc, char *argv[]){
  FILE * network_file;

  List * node_vector[MAX_NODES];
  List * edgeList, * aux;

  Edge * edge;
  int selection = 0;

  int origin, destination;


  // Verifica se foi introduzido como argumento o ficheiro com a topologia do grafo
  if (argc < 2 || (network_file = fopen(argv[1], "r")) == NULL) {
    printf("A network topology file was not provided!\n");
    return 1;
  }

  edgeList = LoadTopology(network_file, node_vector);
  fclose(network_file);

  while (selection != 6) {
    printf("Selecione uma opção:\n");
    printf("[1] Find disjoint paths\n");
    printf("[6] Quit\n");
    scanf("%d",&selection);

    // Confirma que foi selecionado um dominio de destino válido

    switch (selection) {
      case 1:
        printf("What is the origin? ");
        scanf("%d", &origin);

        printf("What is the destination? ");
        scanf("%d", &destination);

        getDisjointPaths(origin, destination, node_vector);

        aux = edgeList;
        while(aux != NULL){
          edge = aux->item;
          printf("Edge %d ->  %d | Foward: %d | Backward: %d\n", edge->node1, edge->node2, edge->fluxFoward, edge->fluxBackward);

          aux = aux->next;
        }

      case 6:
      default:
        selection = 6; //Quit
        break;
    }
    printf("---------------\n");
  }

  freeList(edgeList, free);

  return 0;
}
