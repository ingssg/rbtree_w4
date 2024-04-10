#include "bst.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  node *root = NULL;

  root = insertNode(root, 30);
  insertNode(root, 50);
  insertNode(root, 40);
  insertNode(root, 45);
  insertNode(root, 15);
  inorder(root);
  deleteNode(root, 15);
  deleteNode(root, 40);
  deleteNode(root, 50);
  printf("\n");
  inorder(root);
}
