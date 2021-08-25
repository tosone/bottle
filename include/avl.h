#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once

typedef struct avl_entry_t {
  int key;
  int height;
  struct avl_entry_t *left;
  struct avl_entry_t *right;
} avl_entry_t;

avl_entry_t *avl_create(avl_entry_t *node, int key);
avl_entry_t *avl_set(avl_entry_t *node, int key);
bool avl_get(avl_entry_t *root, int key);
void avl_pre_order(avl_entry_t *entry);
void avl_in_order(avl_entry_t *entry);
void avl_post_order(avl_entry_t *entry);
void avl_dump(avl_entry_t *entry, char *filename);
void avl_free(avl_entry_t *entry);
