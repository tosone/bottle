#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dag_entry_t {
  char *key;
  void *value;
  size_t value_length;
  struct dag_entry_t **nexts;
} dag_entry_t;

typedef struct dag_t {
  dag_entry_t *root;
  uint64_t count;
} dag_t;

dag_t *dag_create();
dag_entry_t *dag_add_entry(dag_t *dag, char *key, void *value, size_t value_length);
bool dag_add_edge(dag_t *dag, char *from, char *to);
dag_entry_t *dag_delete_entry(dag_t *dag, char *key);
bool dag_delete_edge(dag_t *dag, char *from, char *to);
void dag_dump(dag_t *dag);
void dag_free(dag_t *dag);
