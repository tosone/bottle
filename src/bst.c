#include "bst.h"

bst_t *bst_create() {
  bst_t *tree = (bst_t *)malloc(sizeof(bst_t));
  tree->root = NULL;
  return tree;
}

bst_node_t *bst_create_node(int value) {
  bst_node_t *node = (bst_node_t *)malloc(sizeof(bst_node_t));
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  return node;
}

bst_node_t *_bst_insert_node(bst_node_t *tree, int value) {
  if (tree == NULL) {
    return bst_create_node(value);
  }
  if (value < tree->value) {
    tree->left = _bst_insert_node(tree->left, value);
  } else {
    tree->right = _bst_insert_node(tree->right, value);
  }
  return tree;
}

bst_t *bst_insert_node(bst_t *tree, int value) {
  if (tree == NULL) {
    tree = bst_create();
  }
  tree->root = _bst_insert_node(tree->root, value);
  return tree;
}

void bst_dump_node_heler(bst_node_t *node, FILE *stream) {
  if (node != NULL) {
    fprintf(stream, "%d [shape=box];\n", node->value);
    if (node->left != NULL) {
      fprintf(stream, "%d -> %d;\n", node->value, node->left->value);
    }
    if (node->right != NULL) {
      fprintf(stream, "%d -> %d;\n", node->value, node->right->value);
    }
    bst_dump_node_heler(node->left, stream);
    bst_dump_node_heler(node->right, stream);
  }
}

void bst_dump_helper(bst_t *tree, FILE *stream) {
  if (tree != NULL) {
    bst_dump_node_heler(tree->root, stream);
  }
}

void bst_dump(bst_t *tree, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  bst_dump_helper(tree, stream);
  fprintf(stream, "}\n");
  fclose(stream);
}

void _to_double_list(bst_node_t *node, bst_node_t **pre, bst_node_t **head) {
  if (node == NULL) {
    return;
  }
  _to_double_list(node->left, pre, head);
  if (*pre != NULL) {
    (*pre)->right = node;
  } else {
    *head = node;
  }
  node->left = *pre;
  *pre = node;
  _to_double_list(node->right, pre, head);
}

bst_node_t *to_double_list(bst_t *tree) {
  if (tree == NULL)
    return NULL;
  bst_node_t *pre = NULL;
  bst_node_t *head = NULL;
  _to_double_list(tree->root, &pre, &head);
  head->left = pre;
  pre->right = head;

  return head;
}

void bst_list_dump_helper(bst_node_t *node, bst_node_t *end, FILE *stream) {
  if (node != NULL) {
    fprintf(stream, "%d [shape=box];\n", node->value);
    if (node->left != NULL) {
      fprintf(stream, "%d -> %d;\n", node->value, node->left->value);
    }
    if (node->right != NULL) {
      fprintf(stream, "%d -> %d;\n", node->right->value, node->value);
    }
    if (node->left == end) {
      return;
    }
    bst_list_dump_helper(node->left, end, stream);
  }
}

void bst_list_dump(bst_node_t *node, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  bst_list_dump_helper(node, node, stream);
  fprintf(stream, "}\n");
  fclose(stream);
}

void bst_test(bst_t *tree) {
  bst_insert_node(tree, 10);
  bst_insert_node(tree, 6);
  bst_insert_node(tree, 4);
  bst_insert_node(tree, 8);
  bst_insert_node(tree, 14);
  bst_insert_node(tree, 12);
  bst_insert_node(tree, 16);

  // bst_node_t *node = to_double_list(tree);
  // bst_list_dump(node, "test.dot");
}
