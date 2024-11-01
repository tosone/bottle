#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Binary Search Tree

typedef struct bst_node_t {
  int value;
  struct bst_node_t *left;
  struct bst_node_t *right;
} bst_node_t;

typedef struct bst_t {
  bst_node_t *root;
} bst_t;

bst_t *bst_create();
bst_node_t *bst_create_node(int value);
bst_t *bst_insert_node(bst_t *tree, int value);
void bst_dump(bst_t *tree, char *filename);
void bst_test(bst_t *tree);
void bst_list_dump(bst_node_t *node, char *filename);
bst_node_t *to_double_list(bst_t *tree);
