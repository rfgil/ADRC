#include <stdio.h>
#include <stdlib.h>

#include "Estruturas/avl_tree.h"

#include "topology.h"
#include "route.h"


Domain * getDestination(AVLTree * tree){
  int destination;
  Domain * domain;

  // Questiona o utilizador
  printf("What is the destination? ");
  scanf("%d", &destination);

  // Procura na árvore
  domain = findAvlTree(tree, &destination);
  if (domain == NULL ){
    printf("The selected destination does not exist.\n");
  }

  return domain;
}

void allToall(void (*func)(AVLTree *, Domain *, int, enum ROUTE_TYPE *, int *),
              AVLTree * tree, int * idPosition, int isTopologyConnected, enum ROUTE_TYPE * route_vector, int * n_hops){

  int * total_hops;
  FILE * output;

  // Inicializa o vector para armazenar a seguinte informação:
  // Quantos dominios conseguem chegar ao destino com i saltos? (sendo i o indice do vector)
  total_hops = malloc(countElementsAvlTree(tree)*sizeof(int));
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    total_hops[i] = 0;
  }

  // Loop para incluir todos os dominios existentes
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    func(tree, findAvlTree(tree, &idPosition[i]), isTopologyConnected, route_vector, n_hops);

    // Transformar o resultado obtido, na estatistica que se pretende
    for (int j = 0; j < countElementsAvlTree(tree); j++){
      total_hops[n_hops[j]] += 1;
    }
  }

  // Armazena a informação num ficheiro chamado 'output'
  output = fopen("output", "w");
  for (int i = 0; i < countElementsAvlTree(tree); i++){
    fprintf(output, "%d\t%d\n", i, total_hops[i]);
  }
  fclose(output);

  free(total_hops);
}


void CallNoRestrictionHops(AVLTree * tree, Domain * destination, int isTopologyConnected, enum ROUTE_TYPE * route_vector, int * n_hops){
  // Função para que os argumentos de NoRestrictionHops sejam iguais aos de CommercialRouteHops
  // para que se possa chamar as duas funções por referencia na mesma função
  NoRestrictionHops(tree, destination, n_hops);
}

int main(int argc, char *argv[]){
  FILE * network_file;
  AVLTree * tree;
  Domain * destination;

  int idPosition[50000];
  int selection, isTopologyConnected;

  enum ROUTE_TYPE * route_vector;
  int * n_hops;

  if (argc < 2 || (network_file = fopen(argv[1], "r")) == NULL) {
    printf("A network topology file was not provided!\n");
    return 1;
  }

  tree = LoadTopology(network_file, idPosition);
  fclose(network_file);

  isTopologyConnected = checkConnectivity(tree);

  route_vector = malloc(countElementsAvlTree(tree)*sizeof(enum ROUTE_TYPE));
  n_hops = malloc(countElementsAvlTree(tree)*sizeof(int));

  while (selection != 6) {
    printf("Selecione uma opção:\n");
    printf("[1] Type of comercial route\n");
    printf("[2] Type of comercial route and number of hops\n");
    printf("[3] Type of comercial route and number of hops (all to all)\n");
    printf("[4] Number of hops with no comercial restrictions\n");
    printf("[5] Number of hops with no comercial restrictions (all to all)\n");
    printf("[6] Quit\n");

    scanf("%d",&selection);

    // Confirma que foi selecionado um dominio de destino válido
    if( (selection == 1 || selection == 2 || selection == 4) && (destination = getDestination(tree)) == NULL){
      printf("---------------\n");
      continue;
    }

    switch (selection) {
      case 1: // Calcula qual a rota comercial de cada dominio
         CommercialRoute(tree, destination, isTopologyConnected, route_vector);

        // Imprime o resultado na consola
        for (int  i = 0; i < countElementsAvlTree(tree); i++){
          printf("ID: %8d\t%s\n", idPosition[i], routeString(route_vector[i]));
        }
        break;


      case 2: // Calcula qual a rota comercial de cada dominio e o numero de saltos minimo necessário para chegar ao destino
        CommercialRouteHops(tree, destination, isTopologyConnected, route_vector, n_hops);

        // Imprime o resultado na consola
        for (int  i = 0; i < countElementsAvlTree(tree); i++){
          printf("ID: %8d\t%11s\tHops: %d\n", idPosition[i], routeString(route_vector[i]), n_hops[i]);
        }
        break;


      case 3: // Igual a 2, mas repete o cálculo para todos os dominios
          allToall(CommercialRouteHops, tree, idPosition, isTopologyConnected, route_vector, n_hops);
          break;


      case 4:  // Calcula o numero de saltos minimo necessário para chegar ao destino sem restrições
        NoRestrictionHops(tree, destination, n_hops);

        for (int i = 0; i < countElementsAvlTree(tree); i++){
          printf("ID: %8d\tHops: %d\n", idPosition[i], n_hops[i]);
        }
        break;


      case 5: // Igual a 4, mas repete o cálculo para todos os dominios
        allToall(CallNoRestrictionHops, tree, idPosition, isTopologyConnected, route_vector, n_hops);
        break;

      case 6:
      default:
        selection = 6; //Quit
        break;
    }
    printf("---------------\n");
  }

  free(route_vector);
  free(n_hops);

  freeTopology(tree);

  return 0;
}
