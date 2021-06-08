#include <avl.h>

int max(int a, int b) {
  return (a > b) ? a : b;
}

int height(avl_entry_t *entry) {
  return entry == NULL ? 0 : entry->height;
}

avl_entry_t *avl_create_node(int key) {
  avl_entry_t *node = (avl_entry_t *)malloc(sizeof(avl_entry_t));
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  return (node);
}

avl_entry_t *avl_rotate_right(avl_entry_t *y) {
  avl_entry_t *x = y->left;
  avl_entry_t *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

avl_entry_t *avl_rotate_left(avl_entry_t *x) {
  avl_entry_t *y = x->right;
  avl_entry_t *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}

int avl_balance(avl_entry_t *entry) {
  return entry == NULL ? 0 : height(entry->left) - height(entry->right);
}

avl_entry_t *avl_set(avl_entry_t *node, int key) {
  if (node == NULL) {
    return avl_create_node(key);
  }

  if (key < node->key) {
    node->left = avl_set(node->left, key);
  } else if (key > node->key) {
    node->right = avl_set(node->right, key);
  } else { // 等于 key 不做任何事
    return node;
  }

  node->height = 1 + max(height(node->left), height(node->right));

  int balance = avl_balance(node);

  // If this node becomes unbalanced, then there are 4 cases

  // Left Left Case
  if (balance > 1 && key < node->left->key)
    return avl_rotate_right(node);

  // Right Right Case
  if (balance < -1 && key > node->right->key)
    return avl_rotate_left(node);

  // Left Right Case
  if (balance > 1 && key > node->left->key) {
    node->left = avl_rotate_left(node->left);
    return avl_rotate_right(node);
  }

  // Right Left Case
  if (balance < -1 && key < node->right->key) {
    node->right = avl_rotate_right(node->right);
    return avl_rotate_left(node);
  }

  /* return the (unchanged) node pointer */
  return node;
}

bool avl_get(avl_entry_t *root, int key) {
  if (root != NULL) {
    if (root->key == key) {
      return true;
    }
    if (avl_get(root->left, key)) {
      return true;
    }
    if (avl_get(root->right, key)) {
      return true;
    }
  }
  return false;
}

void avl_pre_order_helper(avl_entry_t *root) {
  if (root != NULL) {
    printf("%d ", root->key);
    avl_pre_order_helper(root->left);
    avl_pre_order_helper(root->right);
  }
}

void avl_pre_order(avl_entry_t *entry) {
  avl_pre_order_helper(entry);
  printf("\n");
}

void avl_in_order_helper(avl_entry_t *root) {
  if (root != NULL) {
    avl_in_order_helper(root->left);
    printf("%d ", root->key);
    avl_in_order_helper(root->right);
  }
}

void avl_in_order(avl_entry_t *entry) {
  avl_in_order_helper(entry);
  printf("\n");
}

void avl_post_order_helper(avl_entry_t *root) {
  if (root != NULL) {
    avl_post_order_helper(root->left);
    avl_post_order_helper(root->right);
    printf("%d ", root->key);
  }
}

void avl_post_order(avl_entry_t *entry) {
  avl_post_order_helper(entry);
  printf("\n");
}

void avl_dump_helper(avl_entry_t *entry, FILE *stream) {
  if (entry != NULL) {
    fprintf(stream, "%d [shape=box];\n", entry->key);
    if (entry->left != NULL) {
      fprintf(stream, "%d -> %d;\n", entry->key, entry->left->key);
    }
    if (entry->right != NULL) {
      fprintf(stream, "%d -> %d;\n", entry->key, entry->right->key);
    }
    avl_post_order_helper(entry->left);
    avl_post_order_helper(entry->right);
  }
}

void avl_dump(avl_entry_t *entry, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  avl_dump_helper(entry, stream);
  fprintf(stream, "}\n");
  fclose(stream);
}

avl_entry_t *avl_create(avl_entry_t *node, int key) {
  return NULL;
}

void avl_free(avl_entry_t *entry) {
  if (entry != NULL) {
    avl_free(entry->left);
    avl_free(entry->right);
    free(entry);
  }
}
