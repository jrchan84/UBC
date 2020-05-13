#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "integernode.h"

struct IntegerNode_class IntegerNode_class_table = {
  &Node_class_table,
  IntegerNode_compareTo,
  IntegerNode_printNode,
  Node_insert,
  Node_print,
  IntegerNode_delete,
  IntegerNode_sum,
};

void IntegerNode_ctor(void* thisv, int* i) {
  struct IntegerNode* this = thisv;
  Node_ctor(this);
  this->i = i;
}

int IntegerNode_compareTo(void* thisv, void* nodev) {
  struct IntegerNode* this = thisv;
  struct IntegerNode* node = nodev;
  if (this->i == node->i) {   // not really needed with insert
    return 0;
  } else if (this->i > node->i) {
    return 1;
  } else {
    return -1;
  }
}

void IntegerNode_printNode(void* thisv) {
  struct IntegerNode* this = thisv;
  printf("%d\n", this->i);
}

void* new_IntegerNode(int* i) {
  struct IntegerNode* obj = malloc(sizeof(struct IntegerNode));
  obj->class = &IntegerNode_class_table;
  IntegerNode_ctor(obj,i);
  return obj;
}

void IntegerNode_delete(void* thisv) {
  struct IntegerNode* this = thisv;
  if (this != NULL) {
    IntegerNode_delete(this->left);
    IntegerNode_delete(this->right);
    free(this);
  }
}

int IntegerNode_sum(void* nodev) {
  struct IntegerNode* node = nodev;
  int sum = node->i;
  if (node->left != NULL) {
    sum += IntegerNode_sum(node->left);
  }
  if (node->right != NULL) {
    sum += IntegerNode_sum(node->right);
  }
  return sum;
}
