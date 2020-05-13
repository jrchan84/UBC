#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "element.h"
#include "list.h"
#include "tree.h"
#include "refcount.h"

struct tree {
  struct node* root;
};

struct node {
  struct element* e;
  struct node*    left;
  struct node*    right;
};

/**
 * Create a new, empty tree
 */

struct tree* tree_new() {
  struct tree* t = rc_malloc (sizeof (*t));
  t->root  = NULL;
  return t;
}

static void tree_delete_helper (struct node* n) {
  if (n != NULL) {
    tree_delete_helper (n->left);
    tree_delete_helper (n->right);
    rc_free_ref (element_get_value(n->e));
    rc_free_ref (n->e);
    rc_free_ref (n);
  }
}

/**
 * Delete tree and all of its nodes
 */
void tree_delete (struct tree* t) {
  tree_delete_helper (t->root);
  rc_free_ref (t);
}

static struct node* tree_insert_node_helper (struct node** np, struct element* e) {
  if (*np != NULL) {
    if (strcmp (element_get_value (e), element_get_value ((*np)->e)) <= 0)
      return tree_insert_node_helper (&(*np)->left, e);
    else
      return tree_insert_node_helper (&(*np)->right, e);
  } else {
    *np = rc_malloc (sizeof (**np));
    (*np)->e    = e;
    rc_keep_ref (e);
    rc_keep_ref (element_get_value(e));
    (*np)->left = (*np)->right = NULL;
    return *np;
  }
}

/**
 * Insert a new list element into the tree
 */
struct node* tree_insert_node (struct tree* t, struct element* e) {
  return tree_insert_node_helper (&t->root, e);
}

static void tree_ordered_suffix_print_helper (struct node* n) {
  if (n != NULL) {
    tree_ordered_suffix_print_helper (n->left);
    for (struct element* e = n->e; e != NULL; e = element_get_next (e))
      printf ("%s ", element_get_value (e));
    printf ("\n");
    tree_ordered_suffix_print_helper (n->right);
  }
}

/**
 * Perform depth-first traversal of tree, printing the values
 * in the linked list rooted at each node.
 */
void tree_ordered_suffix_print (struct tree* t) {
  tree_ordered_suffix_print_helper (t->root);
}
