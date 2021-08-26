#include <rbtree.h>

rbtree_t *rbtree_create() {
  rbtree_t *rbtree = (rbtree_t *)malloc(sizeof(rbtree_t));
  rbtree->root = NULL;
  return rbtree;
}

void rbtree_rotate_left(rbtree_t *rbtree, rbtree_entry_t *node) {
  rbtree_entry_t *right = node->right;
  node->right = right->left;
  if (right->left != NULL) {
    right->left->parent = node;
  }

  right->parent = node->parent;

  if (node->parent != NULL) {
    if (node == node->parent->left) {
      node->parent->left = right;
    } else {
      node->parent->right = right;
    }
  } else {
    rbtree->root = right;
  }
  right->left = node;
  node->parent = right;
}

void rbtree_rotate_right(rbtree_t *rbtree, rbtree_entry_t *node) {
  rbtree_entry_t *left = node->left;
  node->left = left->right;
  if (left->right != NULL) {
    left->right->parent = node;
  }

  left->parent = node->parent;

  if (node->parent != NULL) {
    if (node == node->parent->right) {
      node->parent->right = left;
    } else {
      node->parent->left = left;
    }
  } else {
    rbtree->root = left;
  }
  left->right = node;
  node->parent = left;
}

void rbtree_insert_fixup(rbtree_t *rbtree, rbtree_entry_t *node) {
  rbtree_entry_t *uncle;

  while (node != rbtree->root && node->parent->color == rbtree_color_red) {
    if (node->parent == node->parent->parent->left) {
      uncle = node->parent->parent->right;

      if (uncle != NULL && uncle->color == rbtree_color_red) {
        node->parent->color = rbtree_color_black;
        uncle->color = rbtree_color_black;

        node->parent->parent->color = rbtree_color_red;

        node = node->parent->parent;
      } else {
        if (node == node->parent->right) {
          node = node->parent;
          rbtree_rotate_left(rbtree, node);
        }
        node->parent->color = rbtree_color_black;
        node->parent->parent->color = rbtree_color_red;
        rbtree_rotate_right(rbtree, node->parent->parent);
      }
    } else {
      uncle = node->parent->parent->left;

      if (uncle != NULL && uncle->color == rbtree_color_red) {
        node->parent->color = rbtree_color_black;
        uncle->color = rbtree_color_black;

        node->parent->parent->color = rbtree_color_red;

        node = node->parent->parent;
      } else {
        if (node == node->parent->left) {
          node = node->parent;
          rbtree_rotate_right(rbtree, node);
        }
        node->parent->color = rbtree_color_black;
        node->parent->parent->color = rbtree_color_red;
        rbtree_rotate_left(rbtree, node->parent->parent);
      }
    }
  }
  rbtree->root->color = rbtree_color_black;
}

rbtree_entry_t *rbtree_insert(rbtree_t *rbtree, const char *key, const void *value, const int value_length) {
  rbtree_entry_t *entry = (rbtree_entry_t *)malloc(sizeof(rbtree_entry_t));
  entry->next = entry->right = entry->left = entry->parent = NULL;
  entry->key = (char *)malloc(strlen(key) + 1);
  strcpy(entry->key, key);
  entry->value = (void *)malloc(value_length);
  memcpy(entry->value, value, value_length);
  entry->value_length = value_length;
  entry->color = rbtree_color_red;
  entry->key_hash = murmurhash32(key, strlen(key));
  if (rbtree->root == NULL) {
    entry->color = rbtree_color_black;
    rbtree->root = entry;
    rbtree->count++;
    return entry;
  }
  rbtree_entry_t *node = rbtree->root;
  rbtree_entry_t *parent = NULL;
  while (node != NULL) {
    parent = node;
    if (entry->key_hash > node->key_hash) {
      node = node->left;
    } else if (entry->key_hash < node->key_hash) {
      node = node->right;
    } else {
      free(node->key);
      free(node->value);
      node->key = entry->key;
      node->value = entry->value;
      node->value_length = entry->value_length;
      return entry;
    }
  }
  entry->parent = parent;
  if (entry->key_hash > parent->key_hash) {
    parent->left = entry;
  } else {
    parent->right = entry;
  }

  rbtree_insert_fixup(rbtree, entry);

  rbtree->count++;

  return entry;
}

void rbtree_dump_helper(rbtree_entry_t *entry, FILE *stream) {
  if (entry != NULL) {
    if (entry->color == rbtree_color_black) {
      fprintf(stream, "node%u [shape=record,label=\"{{key: %s|val: %s}}\"];\n", entry->key_hash, entry->key, (char *)entry->value);
    } else {
      fprintf(stream, "node%u [color=red,fontcolor=red,shape=record,label=\"{{key: %s|val: %s}}\"];\n", entry->key_hash, entry->key, (char *)entry->value);
    }
    if (entry->left != NULL) {
      fprintf(stream, "node%u -> node%u;\n", entry->key_hash, entry->left->key_hash);
    }
    if (entry->right != NULL) {
      fprintf(stream, "node%u -> node%u;\n", entry->key_hash, entry->right->key_hash);
    }
    rbtree_dump_helper(entry->left, stream);
    rbtree_dump_helper(entry->right, stream);
  }
}

void rbtree_dump(rbtree_t *rbtree, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  rbtree_dump_helper(rbtree->root, stream);
  fprintf(stream, "}\n");
  fclose(stream);
}

void rbtree_delete_fixup(rbtree_t *rbtree, rbtree_entry_t *child, rbtree_entry_t *child_parent) {
  rbtree_entry_t *sibling;
  int go_up = 1;

  if (child_parent->right == child) {
    sibling = child_parent->left;
  } else {
    sibling = child_parent->right;
  }
  while (go_up) {
    if (child_parent == NULL) {
      return;
    }

    if (sibling->color == rbtree_color_red) {
      child_parent->color = rbtree_color_red;
      sibling->color = rbtree_color_black;
      if (child_parent->right == child) {
        rbtree_rotate_right(rbtree, child_parent);
      } else {
        rbtree_rotate_left(rbtree, child_parent);
      }
      if (child_parent->right == child) {
        sibling = child_parent->left;
      } else {
        sibling = child_parent->right;
      }
    }

    if (child_parent->color == rbtree_color_black && sibling->color == rbtree_color_black && sibling->left->color == rbtree_color_black && sibling->right->color == rbtree_color_black) {
      if (sibling != NULL) {
        sibling->color = rbtree_color_red;
      }

      child = child_parent;
      child_parent = child_parent->parent;
      if (child_parent->right == child) {
        sibling = child_parent->left;
      } else {
        sibling = child_parent->right;
      }
    } else
      go_up = 0;
  }

  if (child_parent->color == rbtree_color_red && sibling->color == rbtree_color_black && sibling->left->color == rbtree_color_black && sibling->right->color == rbtree_color_black) {
    if (sibling != NULL) {
      sibling->color = rbtree_color_red;
    }
    child_parent->color = rbtree_color_black;
    return;
  }

  if (child_parent->right == child && sibling->color == rbtree_color_black && sibling->right->color == rbtree_color_red && sibling->left->color == rbtree_color_black) {
    sibling->color = rbtree_color_red;
    sibling->right->color = rbtree_color_black;
    rbtree_rotate_left(rbtree, sibling);
    if (child_parent->right == child) {
      sibling = child_parent->left;
    } else {
      sibling = child_parent->right;
    }
  } else if (child_parent->left == child && sibling->color == rbtree_color_black && sibling->left->color == rbtree_color_red && sibling->right->color == rbtree_color_black) {
    sibling->color = rbtree_color_red;
    sibling->left->color = rbtree_color_black;
    rbtree_rotate_right(rbtree, sibling);
    if (child_parent->right == child) {
      sibling = child_parent->left;
    } else {
      sibling = child_parent->right;
    }
  }

  sibling->color = child_parent->color;
  child_parent->color = rbtree_color_black;
  if (child_parent->right == child) {
    sibling->left->color = rbtree_color_black;
    rbtree_rotate_right(rbtree, child_parent);
  } else {
    sibling->right->color = rbtree_color_black;
    rbtree_rotate_left(rbtree, child_parent);
  }
}

void change_child_ptr(rbtree_entry_t *child, rbtree_entry_t *old, rbtree_entry_t *new) {
  if (child == NULL) {
    return;
  }
  if (child->parent == old) {
    child->parent = new;
  }
}

int rbtree_find_less_equal(rbtree_t *rbtree, const char *key, rbtree_entry_t **result) {
  rbtree_entry_t *node;

  node = rbtree->root;

  *result = NULL;

  uint32_t hash = murmurhash32(key, strlen(key));

  while (node != NULL) {
    if (hash == node->key_hash) {
      *result = node;
      return 1;
    }
    if (hash > node->key_hash) {
      node = node->left;
    } else {
      *result = node;
      node = node->right;
    }
  }
  return 0;
}

rbtree_entry_t *rbtree_search(rbtree_t *rbtree, const char *key) {
  rbtree_entry_t *node;

  if (rbtree_find_less_equal(rbtree, key, &node)) {
    return node;
  } else {
    return NULL;
  }
}

void swap_int8(uint8_t *x, uint8_t *y) {
  uint8_t t = *x;
  *x = *y;
  *y = t;
}

void swap_color(rbtree_color_t *x, rbtree_color_t *y) {
  rbtree_color_t t = *x;
  *x = *y;
  *y = t;
}

void swap_np(rbtree_entry_t **x, rbtree_entry_t **y) {
  rbtree_entry_t *t = *x;
  *x = *y;
  *y = t;
}

void change_parent_ptr(rbtree_t *rbtree, rbtree_entry_t *parent, rbtree_entry_t *old, rbtree_entry_t *new) {
  if (parent == NULL) {
    if (rbtree->root == old) {
      rbtree->root = new;
    }
    return;
  }
  if (parent->left == old) {
    parent->left = new;
  }
  if (parent->right == old) {
    parent->right = new;
  }
}

rbtree_entry_t *rbtree_delete(rbtree_t *rbtree, const char *key) {
  rbtree_entry_t *to_delete;
  rbtree_entry_t *child;
  if ((to_delete = rbtree_search(rbtree, key)) == 0) {
    return NULL;
  }
  rbtree->count--;

  if (to_delete->left != NULL && to_delete->right != NULL) {
    rbtree_entry_t *smright = to_delete->right;
    while (smright->left != NULL) {
      smright = smright->left;
    }

    swap_color(&to_delete->color, &smright->color);

    change_parent_ptr(rbtree, to_delete->parent, to_delete, smright);
    if (to_delete->right != smright) {
      change_parent_ptr(rbtree, smright->parent, smright, to_delete);
    }

    change_child_ptr(smright->left, smright, to_delete);
    change_child_ptr(smright->left, smright, to_delete);
    change_child_ptr(smright->right, smright, to_delete);
    change_child_ptr(smright->right, smright, to_delete);
    change_child_ptr(to_delete->left, to_delete, smright);
    if (to_delete->right != smright) {
      change_child_ptr(to_delete->right, to_delete, smright);
    }
    if (to_delete->right == smright) {
      to_delete->right = to_delete;
      smright->parent = smright;
    }

    swap_np(&to_delete->parent, &smright->parent);
    swap_np(&to_delete->left, &smright->left);
    swap_np(&to_delete->right, &smright->right);
  }

  if (to_delete->left != NULL) {
    child = to_delete->left;
  } else {
    child = to_delete->right;
  }
  change_parent_ptr(rbtree, to_delete->parent, to_delete, child);
  change_child_ptr(child, to_delete, to_delete->parent);
  if (to_delete->color == rbtree_color_red) {
  } else if (child != NULL && child->color == rbtree_color_red) {
    if (child != NULL) {
      child->color = rbtree_color_black;
    }
  } else if (to_delete->color == rbtree_color_black && to_delete->parent == NULL) {
    rbtree->root = NULL;
  } else {
    rbtree_delete_fixup(rbtree, child, to_delete->parent);
  }
  to_delete->parent = NULL;
  to_delete->left = NULL;
  to_delete->right = NULL;
  to_delete->color = rbtree_color_black;
  return to_delete;
}

void rbtree_entry_free(rbtree_entry_t *entry) {
  if (entry != NULL) {
    rbtree_entry_free(entry->left);
    rbtree_entry_free(entry->right);

    free(entry->key);
    free(entry->value);
    free(entry);
  }
}

void rbtree_free(rbtree_t *rbtree) {
  if (rbtree->root != NULL) {
    rbtree_entry_free(rbtree->root);
  }
}
