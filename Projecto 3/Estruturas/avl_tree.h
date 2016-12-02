#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

typedef struct tree AVLTree;

#define SMALLER 1
#define GREATER 2
#define EQUAL 3

AVLTree * newAvlTree(int (*compare)(void *, void *), int (*compareIdItem)(void *, void *));
void insertAvlTree(AVLTree * tree, void * item);
void * findAvlTree(AVLTree * tree, void * identifier);

int countElementsAvlTree(AVLTree * tree);

void * getAvlTreeRootNode(AVLTree * tree);
void * getAvlTreeLeftChildNode(void * node);
void * getAvlTreeRightChildNode(void * node);
void * getAvlTreeNodeItem(void * node);

void freeAvlTree(AVLTree * tree, void (*freeItem)(void *));

#endif
