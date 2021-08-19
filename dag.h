#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dag_vertex_t {
  char *key;
  void *value;
  size_t value_length;
  struct dag_vertex_t **children;
  uint64_t children_length;
  struct dag_vertex_t **parents;
  uint64_t parents_length;
} dag_vertex_t;

typedef struct dag_t {
  dag_vertex_t *vertexes;
  uint64_t vertex_count;
  uint64_t edge_count;
} dag_t;

dag_t *dag_create();
dag_vertex_t *dag_entry_add(dag_t *dag, char *key, void *value, size_t value_length);
dag_vertex_t *dag_entry_get(dag_t *dag, char *key);
bool dag_entry_del(dag_t *dag, char *key);
bool dag_edge_add(dag_t *dag, char *from, char *to);
bool dag_edge_delete(dag_t *dag, char *from, char *to);
dag_vertex_t *dag_delete_entry(dag_t *dag, char *key);
bool dag_delete_edge(dag_t *dag, char *from, char *to);
void dag_dump(dag_t *dag, char *filename);
void dag_free(dag_t *dag);
void dag_print(dag_t *dag);
void dag_debug(dag_t *dag);
