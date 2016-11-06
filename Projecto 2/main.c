#include <stdio.h>
#include <stdlib.h>

#include "Estruturas/avl_tree.h"

#include "topology.h"
#include "route.h"

int getDestination(){
  int destination;
  printf("What is the destination? ");
  scanf("%d",&destination);
  return destination;
}

int main(int argc, char *argv[]){
  FILE * network_file;
  AVLTree * tree;
  int selection = 0;

  if (argc < 2 || (network_file = fopen(argv[1], "r")) == NULL) {
    printf("A network topology file was not provided!\n");
    return 1;
  }

  tree = LoadTopology(network_file);
  fclose(network_file);

  while (selection != 3) {
    printf("Selecione uma opção:\n");
    printf("[1] Type of comercial route\n");
    printf("[2] Type of comercial route and number of hops\n");
    printf("[3] Quit\n");

    scanf("%d",&selection);

    switch (selection) {
      case 1:
        CommercialRoute(tree, getDestination());
        break;
      case 2:
        CommercialRouteHops(tree, getDestination());
        break;

      case 3:
      default:
        selection = 3; //Quit
        break;
    }
    printf("---------------\n");
  }

  freeTopology(tree);

  return 0;
}
