#include <stdio.h>

#pragma once

typedef struct hashmap_entry_t {
  char *key;
  void *value;
  size_t value_length;
  struct hashmap_entry_t *next;
} hashmap_entry_t;

typedef struct {
  int cap;
  int len;
  hashmap_entry_t **entries;
} hashmap_t;

hashmap_t *hashmap_create();
hashmap_entry_t *hashmap_pair(const char *key, const void *value, const int value_length);
hashmap_t *hashmap_set(hashmap_t *hashmap, const char *key, const void *value, const int value_length);
void *hashmap_get(hashmap_t *hashmap, const char *key, size_t *value_length);
void hashmap_del(hashmap_t *hashmap, const char *key);
void hashmap_print(hashmap_t *hashmap);
void hashmap_free(hashmap_t *hashmap);
void hashmap_iterate(hashmap_t *hashmap, void (*f)(hashmap_entry_t *entry));
