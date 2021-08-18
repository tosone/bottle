#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dag_entry_t {
  char *key;
  void *value;
  size_t value_length;
  struct dag_entry_t **children;
  uint64_t children_length;
  struct dag_entry_t **parents;
  uint64_t parents_length;
} dag_entry_t;

typedef struct dag_t {
  dag_entry_t *entries;
  uint64_t count;
} dag_t;

dag_t *dag_create();
dag_entry_t *dag_entry_add(dag_t *dag, char *key, void *value, size_t value_length);
dag_entry_t *dag_entry_get(dag_t *dag, char *key);
bool dag_entry_del(dag_t *dag, char *key);
bool dag_edge_add(dag_t *dag, char *from, char *to);
dag_entry_t *dag_delete_entry(dag_t *dag, char *key);
bool dag_delete_edge(dag_t *dag, char *from, char *to);
void dag_dump(dag_t *dag);
void dag_free(dag_t *dag);
