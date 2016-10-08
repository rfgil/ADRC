#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node
{
    int val;
    char *letra;
    struct node *next;
    struct node *previous;
    struct node *child0;
    struct node *child1;
}Node;



struct node* create_new_node(int val, char *key )
{
    struct node *aux = (struct node*)malloc(sizeof(struct node));

    if(NULL == aux){
        printf("Auxiliar node creation failed \n");
        return NULL;
    }
    aux->val = val;
    aux->letra = key;
    aux->next= NULL ;
    aux->previous = NULL;
    aux->child0 = NULL ;
    aux->child1 = NULL ;
    return aux;
}



struct node* create_empty_list()
{
    printf("creating list...");

    struct node *aux;
    struct node *aux1;

    aux = create_new_node(0, NULL );
    aux1 = create_new_node(0, NULL );

    aux->next = aux1;
    aux->previous = aux1;
    aux1->next = aux;
    aux1->previous = aux;
        printf("done\n ");

    return aux;
}


//insere ordenado do menor valor de val para o maior valor
struct node* insert_ordered_list(Node *list, Node *element){

    struct node *aux;

    aux = list->next;
    //verifica lista vazia
    if (aux->next == list){
        list->next = element;
        aux->previous = element;
        element->next = aux;
        element->previous = list;
    }else {
        //lista tem mais que 1 elemento
        //algoritmo de ordenação
        while(aux->val < element->val && aux->next != list){
            aux = aux->next;
        }
        //insere
        aux->previous->next = element;
        element->previous = aux->previous;
        aux->previous = element;
        element->next = aux;
    }
    return list;
}


void print_list(Node *list){
    struct node *ptr= list->next;

    printf("--Printing list-- \n");
    while(ptr->next != list)
    {
        printf("[%d] [%s]\n",ptr->val, ptr->letra);
        ptr = ptr->next;
    }
}


void HuffmanCode(char** symbols, int *freq, char** code){

    //ver disto..
    int tamanho = 6;
    printf("[%d]\n",tamanho);

    int i;
    struct node *lista = create_empty_list();
    struct node *aux, *temp, *novo;
    //insere na lista
    for(i=0;i<tamanho;i++){
        aux = create_new_node(freq[i], symbols[i] );
        insert_ordered_list(lista, aux);
    }

    print_list(lista);

    //faz a arvore através de Huffman

    while(lista->next->next != lista->previous){  //enquanto não houver so 1 elemento na lista
        aux=lista->next;
        temp=aux->next;
        novo = create_new_node((aux->val+temp->val), NULL);
        novo->child0 = aux;
        novo->child1 = temp;

        lista->next = temp->next;
        temp->next->previous = lista;
        aux->next=NULL;
        aux->previous=NULL;
        temp->next=NULL;
        temp->previous=NULL;

        insert_ordered_list(lista, novo); //insere o novo elemento ordenadamente

        print_list(lista);
    }
    //tendo a arvore feita, agora retiramos os codigos da arvore




    //prefix code functions


}




int main(void){
    int freq[] = {3,6,14,14,22,41};
    char* symbols[] = {"b","f","c","d","a","e" };
    char* code[] = {};

    HuffmanCode(symbols, freq, code);

    return 0;
}
