#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef struct node Node;

#define EMPTY_NODE NULL

Node * createNode(void * item, Node * child0, Node * child1);
void freeTree(Node * root, void (*freeItem)(void *));

Node * getChild0(Node * root);
Node * getChild1(Node * root);
void * getItem(Node * root);

#endif // TREE_H_INCLUDED
