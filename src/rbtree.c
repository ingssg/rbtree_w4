#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

void left_rotate(rbtree *t, node_t *x) {  
  node_t *y = x->right;
  x->right = y->left;
  if(y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil) {
    t->root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
} 
  
void right_rotate(rbtree *t, node_t *y) {
  node_t *x = y->left;
  y->left = x->right;
  if(x->right != t->nil) {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if(y->parent == t->nil) {
    t->root = x;
  }
  else if(y == y->parent->left) {
    y->parent->left = x;
  }
  else {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

void insert_fixup(rbtree* t, node_t *z){
  node_t *y;
  while(z->parent->color == RBTREE_RED) {
    if(z->parent == z->parent->parent->left) {  // 왼쪽 일자 검-빨-빨
      y = z->parent->parent->right;
      if(y->color == RBTREE_RED) {  // 삼촌 빨강
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->right) { // 검-빨-빨 인데 꺾여있는 경우 풀어 준다.
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else {  // 오른쪽 일자 검-빨-빨
          y = z->parent->parent->left;
          if(y->color == RBTREE_RED) {  // 삼촌 빨강
            z->parent->parent->color = RBTREE_RED;
            z->parent->color = RBTREE_BLACK;
            y->color = RBTREE_BLACK;
            z = z->parent->parent;
          }
          else {
            if(z == z->parent->left) { // 꺾여있으면 돌려서 풀어줘
              z = z->parent;
              right_rotate(t, z);
            }
            z->parent->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED;
            left_rotate(t, z->parent->parent);
          }
    }
  }

  
  t->root->color = RBTREE_BLACK;
}

void transplant(rbtree* t, node_t *p, node_t *c) { // p의 부모와 c를 연결
  if(p->parent == t->nil) {
    t->root = c;
  }
  else if(p == p->parent->left) {
    p->parent->left = c;
  }
  else {
    p->parent->right = c;
  }
  c->parent = p->parent;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil -> color = RBTREE_BLACK;
  p -> root = nil;
  p -> nil = nil;
  return p;
}

void preorder_arr(const rbtree *t, key_t **arr, node_t *p) {
  if(p == t->nil) return;
  preorder_arr(t, arr, p->left);
  **arr = p->key;
  (*arr) += 1;
  preorder_arr(t, arr, p->right);
}

void inorder_del(const rbtree *t, node_t *p) {
  if(p == t->nil) return;
  node_t *left = p->left, *right = p->right;
  inorder_del(t, left);
  inorder_del(t, right);
  free(p);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  inorder_del(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *x, *y;
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->left = new_node->right = t->nil;
  new_node->color = RBTREE_RED;

  x = t->root;
  y = t->nil;

  while(x != t->nil) {
    y = x;
    if(x->key <= key) {
      x = x->right;
    }
    else {
      x = x->left;
    }
  }
  if(y == t->nil) {
    t->root = new_node;
  }
  if(y->key <= key) {
    y->right = new_node;
  }
  else {
    y->left = new_node;
  }
  new_node->parent = y;

  insert_fixup(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *temp = t->root;
  while(temp != t->nil) {
    if(temp -> key < key) {
      temp = temp->right;
    }
    else if(temp -> key > key) {
      temp = temp->left;
    }
    else {
      return temp;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->left != t->nil) {
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->right != t->nil) {
    current = current->right;
  }
  return current;
}

void swap_color(node_t *a, node_t *b) {
  color_t temp;
  temp = a->color;
  a->color = b->color;
  b->color = temp;
}

void erase_fixup(rbtree *t, node_t *x) {
  node_t *sibling;
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left) {
      sibling = x->parent->right;
      if(sibling->color == RBTREE_RED) {
        swap_color(sibling, x->parent);
        left_rotate(t, x->parent);
        sibling = x->parent->right;
      }
      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if(sibling->right->color == RBTREE_BLACK) {
          swap_color(sibling, sibling->left);
          right_rotate(t, sibling);
          sibling = x->parent->right;
        }
        sibling->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      sibling = x->parent->left;
      if(sibling->color == RBTREE_RED) {
        swap_color(sibling, x->parent);
        right_rotate(t, x->parent);
        sibling = x->parent->left;
      }
      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if(sibling->left->color == RBTREE_BLACK) {
          swap_color(sibling, sibling->right);
          left_rotate(t, sibling);
          sibling = x->parent->left;
        }
        sibling->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) { // p: 삭제할 놈
  // TODO: implement erase
  node_t *x, *y; // x: 검사할 놈, y: 이식될 놈
  color_t original_color;
  y = p;
  original_color = y->color;
  if(p->left == t->nil) {     // 자식이 0개 or 1개 일 때, 조상과 자식 연결(부모는 삭제할거니까)
    x = p->right;
    transplant(t, p, p->right);
  }
  else if(p->right == t->nil) {
    x = p->left;
    transplant(t, p, p->left);
  }
  else {  // 자식이 2놈일 때
    y = p->right;
    while(y->left != t->nil) {
      y = y->left;
    }
    original_color = y->color;
    x = y->right;
    if(y->parent == p) {
      x->parent = y;  // x가 nil일 수도 있는데, nil인 경우 parent정보가 없어서 일단 담아줌
    }
    else {
      transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if(original_color == RBTREE_BLACK) erase_fixup(t, x);

  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  preorder_arr(t, &arr, t->root);
  return 0;
}
