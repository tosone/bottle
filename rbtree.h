#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <murmurhash.h>

#pragma once

typedef enum rbtree_color_t { rbtree_color_black, rbtree_color_red } rbtree_color_t;

typedef struct rbtree_entry_t {
  struct rbtree_entry_t *parent;
  struct rbtree_entry_t *left;
  struct rbtree_entry_t *right;
  struct rbtree_entry_t *next;
  rbtree_color_t color;
  uint32_t key_hash;
  char *key;
  void *value;
  size_t value_length;
} rbtree_entry_t;

typedef struct rbtree_t {
  rbtree_entry_t *root;
  uint32_t count;
} rbtree_t;

rbtree_t *rbtree_create();
rbtree_entry_t *rbtree_insert(rbtree_t *rbtree, const char *key, const void *value, const int value_length);
rbtree_entry_t *rbtree_delete(rbtree_t *rbtree, const char *key);
void rbtree_free(rbtree_t *rbtree);
void rbtree_dump(rbtree_t *rbtree, char *filename);
