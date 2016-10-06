#include <stdlib.h>

#include "tree.h"

struct node {
  void * item;
  struct node * child0;
  struct node * child1;
};

Node * createNode(void * item, Node * child0, Node * child1){
  Node * new;
  new = malloc(sizeof(Node));

  new->child0 = child0;
  new->child1 = child1;
  new->item = item;

  return new;
}

void freeTree(Node * root, void (*freeItem)(void *)){
  if (root != NULL ) {
      freeTree(root->child0, freeItem);
      freeTree(root->child1, freeItem);

      freeItem(root->item);
      free(root);
  }
}

Node * getChild0(Node * root){
  return root == NULL ? NULL : root->child0;
}

Node * getChild1(Node * root){
  return root == NULL ? NULL : root->child1;
}

void * getItem(Node * root){
  return root == NULL ? NULL : root->item;
}
