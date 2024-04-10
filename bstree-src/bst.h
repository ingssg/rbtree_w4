#ifndef _BSTTREE_H_
#define _BSTTREE_H_

#include <stddef.h>

typedef struct node {
  int key;
  struct node *left, *right; 
} node;

node *newNode(int key);
node *insertNode(node *root, int key);
void inorder(node *root);

node *deleteNode(node *root, int key);
int findMinValue(node *node);

#endif
