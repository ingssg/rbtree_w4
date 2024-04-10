#include "bst.h"
#include <stdlib.h>
#include <stdio.h>

node *newNode(int key) {
  node *temp = (node *)malloc(sizeof(node));
  temp -> key = key;
  temp -> left = temp -> right = NULL;

  return temp;
}

node *insertNode(node *node, int key) {
  if(node == NULL) {
    node = newNode(key);
  }
  else {
    if(node -> key < key) {
      node -> right = insertNode(node -> right, key);
    }
    else if(node -> key > key) {
      node -> left = insertNode(node -> left, key);
    }
  }

  return node;
}

void inorder(node *root) {
  if(root != NULL) {
    inorder(root -> left);
    printf("%d ", root -> key);
    inorder(root -> right);
  }
}

int findMinValue(node *root) {
  node *current = root;
  while (current -> left != NULL) {
    current = current -> left;
  }
  return current -> key;
}

node *deleteNode(node *root, int key) {
  if(root -> key < key) {
    root -> right = deleteNode(root -> right, key);
  }
  else if(root -> key > key) {
    root -> left = deleteNode(root -> left, key);
  }
  else {
    if(root -> left == NULL) {
      node *tmp = root -> right;
      free(root);
      return tmp;
    }
    else if(root -> right == NULL) {
      node *tmp = root -> left;
      free(root);
      return tmp;
    }
  
    int minValue = findMinValue(root -> right);
    root -> key = minValue;
    root-> right = deleteNode(root->right, minValue);
  }

  return root;
}
