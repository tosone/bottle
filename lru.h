#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>

#pragma once



typedef struct LRUEntry {
  char *key;
  void *value;
  size_t value_length;
  struct LRUEntry *prev;
  struct LRUEntry *next;
} LRUEntry;

typedef struct LRU {
  int len;
  int cap;
  hashmap_t *hmap;
  LRUEntry *head;
  LRUEntry *tail;
} LRU;

LRU *LRU_create();
void LRU_set(LRU *lru, char *key, void *value, size_t value_length);
void *LRU_get(LRU *lru, char *key, size_t *value_length);
void LRU_print(LRU *lru);
void LRU_free(LRU *lru);
