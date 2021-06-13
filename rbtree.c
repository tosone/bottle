#include <rbtree.h>

rbtree_t *rbtree_create() {
  rbtree_t *rbtree = (rbtree_t *)malloc(sizeof(rbtree_t));
  rbtree->root = NULL;
  return rbtree;
}

void rbtree_rotate_left(rbtree_t *rbtree, rbtree_entry_t *node) {
  rbtree_entry_t *right = node->right;
  node->right = right->left;
  if (right->left != NULL)
    right->left->parent = node;

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
  if (left->right != NULL)
    left->right->parent = node;

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

  /* While not at the root and need fixing... */
  while (node != rbtree->root && node->parent->color == rbtree_color_red) {
    /* If our parent is left child of our grandparent... */
    if (node->parent == node->parent->parent->left) {
      uncle = node->parent->parent->right;

      /* If our uncle is red... */
      if (uncle != NULL && uncle->color == rbtree_color_red) {
        /* Paint the parent and the uncle black... */
        node->parent->color = rbtree_color_black;
        uncle->color = rbtree_color_black;

        /* And the grandparent red... */
        node->parent->parent->color = rbtree_color_red;

        /* And continue fixing the grandparent */
        node = node->parent->parent;
      } else { /* Our uncle is black... */
        /* Are we the right child? */
        if (node == node->parent->right) {
          node = node->parent;
          rbtree_rotate_left(rbtree, node);
        }
        /* Now we're the left child, repaint and rotate... */
        node->parent->color = rbtree_color_black;
        node->parent->parent->color = rbtree_color_red;
        rbtree_rotate_right(rbtree, node->parent->parent);
      }
    } else {
      uncle = node->parent->parent->left;

      /* If our uncle is red... */
      if (uncle != NULL && uncle->color == rbtree_color_red) {
        /* Paint the parent and the uncle black... */
        node->parent->color = rbtree_color_black;
        uncle->color = rbtree_color_black;

        /* And the grandparent red... */
        node->parent->parent->color = rbtree_color_red;

        /* And continue fixing the grandparent */
        node = node->parent->parent;
      } else { /* Our uncle is black... */
        /* Are we the right child? */
        if (node == node->parent->left) {
          node = node->parent;
          rbtree_rotate_right(rbtree, node);
        }
        /* Now we're the right child, repaint and rotate... */
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
  entry->key_hash = murmurhash(key, strlen(key), 0);
  if (rbtree->root == NULL) {
    entry->color = rbtree_color_black;
    rbtree->root = entry;
    return entry;
  }
  rbtree_entry_t *node = rbtree->root;
  rbtree_entry_t *parent = NULL;
  while (node != NULL) {
    parent = node;
    if (entry->key_hash > node->key_hash) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  entry->parent = parent;
  if (entry->key_hash > parent->key_hash)
    parent->left = entry;
  else {
    parent->right = entry;
  }

  rbtree_insert_fixup(rbtree, entry);

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

// void rbtree_delete_fixup(rbtree_t *rbtree, rbtree_entry_t *child, rbtree_entry_t *child_parent) {
//   rbtree_entry_t *sibling;
//   int go_up = 1;

//   /* determine sibling to the node that is one-black short */
//   if (child_parent->right == child)
//     sibling = child_parent->left;
//   else
//     sibling = child_parent->right;

//   while (go_up) {
//     if (child_parent == NULL) {
//       /* removed parent==black from root, every path, so ok */
//       return;
//     }

//     if (sibling->color == rbtree_color_red) { /* rotate to get a black sibling */
//       child_parent->color = rbtree_color_red;
//       sibling->color = rbtree_color_black;
//       if (child_parent->right == child)
//         rbtree_rotate_right(rbtree, child_parent);
//       else
//         rbtree_rotate_left(rbtree, child_parent);
//       /* new sibling after rotation */
//       if (child_parent->right == child)
//         sibling = child_parent->left;
//       else
//         sibling = child_parent->right;
//     }

//     if (child_parent->color == rbtree_color_black && sibling->color == rbtree_color_black &&
//         sibling->left->color == rbtree_color_black &&
//         sibling->right->color == rbtree_color_black) { /* fixup local with recolor of sibling */
//       if (sibling != NULL)
//         sibling->color = rbtree_color_red;

//       child = child_parent;
//       child_parent = child_parent->parent;
//       /* prepare to go up, new sibling */
//       if (child_parent->right == child)
//         sibling = child_parent->left;
//       else
//         sibling = child_parent->right;
//     } else
//       go_up = 0;
//   }

//   if (child_parent->color == rbtree_color_red && sibling->color == rbtree_color_black &&
//       sibling->left->color == rbtree_color_black && sibling->right->color == rbtree_color_black) {
//     /* move red to sibling to rebalance */
//     if (sibling != NULL)
//       sibling->color = rbtree_color_red;
//     child_parent->color = rbtree_color_black;
//     return;
//   }
//   // log_assert(sibling != NULL);

//   /* get a new sibling, by rotating at sibling. See which child
//      of sibling is red */
//   if (child_parent->right == child && sibling->color == rbtree_color_black &&
//       sibling->right->color == rbtree_color_red && sibling->left->color == rbtree_color_black) {
//     sibling->color = rbtree_color_red;
//     sibling->right->color = rbtree_color_black;
//     rbtree_rotate_left(rbtree, sibling);
//     /* new sibling after rotation */
//     if (child_parent->right == child)
//       sibling = child_parent->left;
//     else
//       sibling = child_parent->right;
//   } else if (child_parent->left == child && sibling->color == rbtree_color_black &&
//              sibling->left->color == rbtree_color_red && sibling->right->color == rbtree_color_black) {
//     sibling->color = rbtree_color_red;
//     sibling->left->color = rbtree_color_black;
//     rbtree_rotate_right(rbtree, sibling);
//     /* new sibling after rotation */
//     if (child_parent->right == child)
//       sibling = child_parent->left;
//     else
//       sibling = child_parent->right;
//   }

//   /* now we have a black sibling with a red child. rotate and exchange colors. */
//   sibling->color = child_parent->color;
//   child_parent->color = rbtree_color_black;
//   if (child_parent->right == child) {
//     // log_assert(sibling->left->color == rbtree_color_red);
//     sibling->left->color = rbtree_color_black;
//     rbtree_rotate_right(rbtree, child_parent);
//   } else {
//     // log_assert(sibling->right->color == rbtree_color_red);
//     sibling->right->color = rbtree_color_black;
//     rbtree_rotate_left(rbtree, child_parent);
//   }
// }

// void change_child_ptr(rbtree_entry_t *child, rbtree_entry_t *old, rbtree_entry_t *new) {
//   if (child == NULL)
//     return;
//   // log_assert(child->parent == old || child->parent == new);
//   if (child->parent == old)
//     child->parent = new;
// }

// int rbtree_find_less_equal(rbtree_t *rbtree, const char *key, rbtree_entry_t **result) {
//   int r;
//   rbtree_entry_t *node;

//   // log_assert(result);

//   /* We start at root... */
//   node = rbtree->root;

//   *result = NULL;
//   // fptr_ok(fptr_whitelist_rbtree_cmp(rbtree->cmp));

//   uint32_t hash = murmurhash(key, strlen(key), 0);

//   /* While there are children... */
//   while (node != NULL) {
//     r = rbtree->cmp(key, node->key);
//     if (r == 0) {
//       /* Exact match */
//       *result = node;
//       return 1;
//     }
//     if (r < 0) {
//       node = node->left;
//     } else {
//       /* Temporary match */
//       *result = node;
//       node = node->right;
//     }
//   }
//   return 0;
// }

// rbtree_entry_t *rbtree_search(rbtree_t *rbtree, const char *key) {
//   rbtree_entry_t *node;

//   if (rbtree_find_less_equal(rbtree, key, &node)) {
//     return node;
//   } else {
//     return NULL;
//   }
// }

// rbtree_entry_t *rbtree_delete(rbtree_t *rbtree, const char *key) {
//   rbtree_entry_t *to_delete;
//   rbtree_entry_t *child;
//   if ((to_delete = rbtree_search(rbtree, key)) == 0)
//     return 0;
//   rbtree->count--;

//   /* make sure we have at most one non-leaf child */
//   if (to_delete->left != NULL && to_delete->right != NULL) {
//     /* swap with smallest from right subtree (or largest from left) */
//     rbtree_entry_t *smright = to_delete->right;
//     while (smright->left != NULL)
//       smright = smright->left;
//     /* swap the smright and to_delete elements in the tree,
//      * but the rbnode_t is first part of user data struct
//      * so cannot just swap the keys and data pointers. Instead
//      * readjust the pointers left,right,parent */

//     /* swap colors - colors are tied to the position in the tree */
//     swap_int8(&to_delete->color, &smright->color);

//     /* swap child pointers in parents of smright/to_delete */
//     change_parent_ptr(rbtree, to_delete->parent, to_delete, smright);
//     if (to_delete->right != smright)
//       change_parent_ptr(rbtree, smright->parent, smright, to_delete);

//     /* swap parent pointers in children of smright/to_delete */
//     change_child_ptr(smright->left, smright, to_delete);
//     change_child_ptr(smright->left, smright, to_delete);
//     change_child_ptr(smright->right, smright, to_delete);
//     change_child_ptr(smright->right, smright, to_delete);
//     change_child_ptr(to_delete->left, to_delete, smright);
//     if (to_delete->right != smright)
//       change_child_ptr(to_delete->right, to_delete, smright);
//     if (to_delete->right == smright) {
//       /* set up so after swap they work */
//       to_delete->right = to_delete;
//       smright->parent = smright;
//     }

//     /* swap pointers in to_delete/smright nodes */
//     swap_np(&to_delete->parent, &smright->parent);
//     swap_np(&to_delete->left, &smright->left);
//     swap_np(&to_delete->right, &smright->right);

//     /* now delete to_delete (which is at the location where the smright previously was) */
//   }
//   // log_assert(to_delete->left == NULL || to_delete->right == NULL);

//   if (to_delete->left != NULL)
//     child = to_delete->left;
//   else
//     child = to_delete->right;

//   /* unlink to_delete from the tree, replace to_delete with child */
//   change_parent_ptr(rbtree, to_delete->parent, to_delete, child);
//   change_child_ptr(child, to_delete, to_delete->parent);

//   if (to_delete->color == rbtree_color_red) {
//     /* if node is red then the child (black) can be swapped in */
//   } else if (child->color == rbtree_color_red) {
//     /* change child to BLACK, removing a RED node is no problem */
//     if (child != NULL)
//       child->color = rbtree_color_black;
//   } else
//     rbtree_delete_fixup(rbtree, child, to_delete->parent);

//   /* unlink completely */
//   to_delete->parent = NULL;
//   to_delete->left = NULL;
//   to_delete->right = NULL;
//   to_delete->color = rbtree_color_black;
//   return to_delete;
// }
