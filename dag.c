#include <dag.h>

dag_t *dag_create() {
  dag_t *dag = (dag_t *)malloc(sizeof(dag_t));
  dag->vertexes = NULL;
  dag->vertex_count = 0;
  dag->edge_count = 0;

  dag->vertexes_new = deque_create();
  return dag;
}

void dag_free(dag_t *dag) {
  while (true) {
    deque_entry_t *entry = deque_pop_back(dag->vertexes_new);
    if (entry == NULL) {
      break;
    }
    dag_vertex_t *vertex = *(dag_vertex_t **)entry->data;
    deque_free(vertex->children_new);
    deque_free(vertex->parents_new);
    free(vertex->key);
    free(vertex->value);
    free(vertex);
  }
  deque_free(dag->vertexes_new);
  free(dag);
}

bool dag_entry_isexist(dag_t *dag, char *key) {
  bool exist = false;
  for (int i = 0; i < dag->vertex_count; i++) {
    if (strlen(dag->vertexes[i].key) == strlen(key) && strncasecmp(dag->vertexes[i].key, key, strlen(key)) == 0) {
      return true;
    }
  }
  return exist;
}

dag_vertex_t *dag_entry_add(dag_t *dag, char *key, void *value, size_t value_length) {
  dag_vertex_t *entry = (dag_vertex_t *)malloc(sizeof(dag_vertex_t));
  entry->key = (char *)malloc(strlen(key) + 1);
  entry->value = (void *)malloc(value_length);
  entry->value_length = value_length;
  entry->children_length = 0;
  entry->parents_length = 0;
  entry->children = NULL;
  entry->parents = NULL;
  entry->children_new = deque_create();
  entry->parents_new = deque_create();

  bzero(entry->key, strlen(key) + 1);
  strcpy(entry->key, key);
  memcpy(entry->value, value, value_length);

  dag->vertex_count++;

  deque_push_back(dag->vertexes_new, &entry, sizeof(dag_vertex_t *));

  return entry;
}

bool dag_entry_get_callback(deque_t *deque, void *data, size_t data_length) {
  dag_vertex_t *entry = *(dag_vertex_t **)data;
  char *target = (char *)deque->user_data;
  if (strcmp(entry->key, target) == 0) {
    return true;
  }
  return false;
}

dag_vertex_t *dag_entry_get(dag_t *dag, char *key) {
  dag->vertexes_new->user_data = (void *)key;
  dag->vertexes_new->user_data_length = strlen(key);
  deque_entry_t *result = deque_entry_get(dag->vertexes_new, dag_entry_get_callback);
  dag_vertex_t *entry = NULL;
  if (result != NULL) {
    entry = *(dag_vertex_t **)result->data;
  }
  return entry;
}

bool dag_entry_del(dag_t *dag, char *key) {
  dag_vertex_t *vertex = dag_entry_get(dag, key);
  if (vertex == NULL) {
    printf("key not found\n");
    return false;
  }

  while (true) {
    deque_entry_t *children = deque_pop_back(vertex->children_new);
    if (children == NULL) {
      break;
    }
    dag_vertex_t *children_vertex = *(dag_vertex_t **)children->data;
    children_vertex->parents_new->user_data = (void *)key;
    children_vertex->parents_new->user_data_length = strlen(key);
    if (!deque_entry_delete(children_vertex->parents_new, dag_entry_get_callback)) {
      printf("children edge not found\n");
    }
    children_vertex->parents_length--;
  }

  while (true) {
    deque_entry_t *parents = deque_pop_back(vertex->parents_new);
    if (parents == NULL) {
      break;
    }
    dag_vertex_t *parents_vertex = *(dag_vertex_t **)parents->data;
    parents_vertex->children_new->user_data = (void *)key;
    parents_vertex->children_new->user_data_length = strlen(key);
    if (!deque_entry_delete(parents_vertex->children_new, dag_entry_get_callback)) {
      printf("parents edge not found\n");
    }
    parents_vertex->children_length--;
  }
  dag->vertexes_new->user_data = (void *)key;
  dag->vertexes_new->user_data_length = strlen(key);
  if (!deque_entry_delete(dag->vertexes_new, dag_entry_get_callback)) {
    printf("vertex not found\n");
  }

  free(vertex->key);
  free(vertex->value);
  deque_free(vertex->children_new);
  deque_free(vertex->parents_new);

  return true;
}

bool dag_edge_callback(deque_t *deque, void *data, size_t data_length) {
  dag_vertex_t *entry = *(dag_vertex_t **)data;
  char *target = (char *)deque->user_data;
  if (strcmp(entry->key, target) == 0) {
    return true;
  }
  return false;
}

bool dag_edge_add(dag_t *dag, char *from, char *to) {
  dag_vertex_t *from_entry = dag_entry_get(dag, from);
  dag_vertex_t *to_entry = dag_entry_get(dag, to);
  if (from_entry == NULL || to_entry == NULL) {
    printf("key not found\n");
    return false;
  }
  dag->edge_count++;
  from_entry->children_length++;
  deque_push_back(from_entry->children_new, &to_entry, sizeof(dag_vertex_t *));
  to_entry->parents_length++;
  deque_push_back(to_entry->parents_new, &from_entry, sizeof(dag_vertex_t *));
  return true;
}

bool dag_edge_delete(dag_t *dag, char *from, char *to) {
  dag_vertex_t *from_vertex = dag_entry_get(dag, from);
  dag_vertex_t *to_vertex = dag_entry_get(dag, to);
  if (from_vertex == NULL || to_vertex == NULL) {
    printf("key not found\n");
    return false;
  }

  from_vertex->children_new->user_data = (void *)to_vertex->key;
  from_vertex->children_new->user_data_length = strlen(to_vertex->key);
  bool from_edge_delete_result = deque_entry_delete(from_vertex->children_new, dag_edge_callback);

  to_vertex->parents_new->user_data = (void *)from_vertex->key;
  to_vertex->parents_new->user_data_length = strlen(from_vertex->key);
  bool to_edge_delete_result = deque_entry_delete(to_vertex->parents_new, dag_edge_callback);

  if (!from_edge_delete_result || !to_edge_delete_result) {
    printf("edge not found\n");
  }
  return true;
}

dag_t *dag_duplicate(dag_t *dag) {
  dag_t *new_dag = (dag_t *)malloc(sizeof(dag_t));
  new_dag->vertexes = NULL;
  new_dag->vertex_count = 0;
  new_dag->edge_count = 0;

  new_dag->vertexes_new = deque_create();

  deque_entry_t *entry = dag->vertexes_new->head;
  while (entry != NULL) {
    dag_vertex_t *vertex = *(dag_vertex_t **)entry->data;
    dag_entry_add(new_dag, vertex->key, vertex->value, vertex->value_length);
    entry = entry->next;
  }

  entry = dag->vertexes_new->head;
  while (entry != NULL) {
    dag_vertex_t *vertex = *(dag_vertex_t **)entry->data;
    deque_entry_t *children_entry = vertex->children_new->head;
    while (children_entry != NULL) {
      dag_vertex_t *children_vertex = *(dag_vertex_t **)children_entry->data;
      dag_edge_add(new_dag, vertex->key, children_vertex->key);
      children_entry = children_entry->next;
    }
    entry = entry->next;
  }
  return new_dag;
}

dag_vertex_t *dag_get_zero_indegree(dag_t *dag) {
  if (dag == NULL) {
    return NULL;
  }
  deque_entry_t *entry = dag->vertexes_new->head;
  while (entry != NULL) {
    dag_vertex_t *vertex = *(dag_vertex_t **)entry->data;
    if (vertex->parents_length == 0) {
      return vertex;
    }
    entry = entry->next;
  }
  return NULL;
}

void dag_debug(dag_t *dag) {
  if (dag->vertexes_new->head == NULL) {
    return;
  }
  printf("---------------------\n");
  deque_entry_t *entry = dag->vertexes_new->head;
  while (entry != NULL) {
    dag_vertex_t *vertex = *(dag_vertex_t **)entry->data;
    printf("node: %s %p\n", vertex->key, vertex);

    deque_entry_t *children_entry = vertex->children_new->head;
    while (true) {
      if (children_entry == NULL) {
        break;
      }
      dag_vertex_t *children_vertex = *(dag_vertex_t **)children_entry->data;
      printf("\tchildren: %s %p\n", children_vertex->key, children_vertex);
      children_entry = children_entry->next;
    }

    deque_entry_t *parents_entry = vertex->parents_new->head;
    while (true) {
      if (parents_entry == NULL) {
        break;
      }
      dag_vertex_t *parents_vertex = *(dag_vertex_t **)parents_entry->data;
      printf("\tparents: %s %p\n", parents_vertex->key, parents_vertex);
      parents_entry = parents_entry->next;
    }

    entry = entry->next;
  }
  printf("---------------------\n");
}

void dag_print(dag_t *dag) {
  if (dag->vertex_count == 0) {
    return;
  }
  dag_t *new_dag = dag_duplicate(dag);
  bool first = true;

  for (;;) {
    dag_vertex_t *vertex = dag_get_zero_indegree(new_dag);
    if (vertex == NULL) {
      break;
    }
    if (!first) {
      printf(" %s", vertex->key);
    } else {
      printf("%s", vertex->key);
      first = false;
    }
    if (vertex->children_length == 0) {
      break;
    }
    dag_entry_del(new_dag, vertex->key);
  }
  printf("\n");
  dag_free(new_dag);

  return;
}

void dag_dump(dag_t *dag, char *filename) {
  if (dag == NULL) {
    printf("dag is null");
  }
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  if (dag->vertex_count != 0) {
    deque_entry_t *entry = dag->vertexes_new->head;
    while (entry != NULL) {
      dag_vertex_t *vertex = *(dag_vertex_t **)entry->data;
      printf("node: %s %s\n", vertex->key, (char *)vertex->value);
      fprintf(stream, "node%s[shape=record,label=\"{{%s: %s}}\"];\n", vertex->key, vertex->key, (char *)vertex->value);
      entry = entry->next;
    }
  }
  fprintf(stream, "}\n");
  fclose(stream);
}

void dag_test(dag_t *dag) {
  char *key1 = "a";
  char *value1 = "1";
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_VERTEX, COMMAND_DAG_VERTEX_ADD, key1, value1);
  dag_entry_add(dag, key1, (void *)value1, strlen(value1) + 1);
  char *key2 = "b";
  char *value2 = "2";
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_VERTEX, COMMAND_DAG_VERTEX_ADD, key2, value2);
  dag_entry_add(dag, key2, (void *)value2, strlen(value1) + 1);
  char *key3 = "c";
  char *value3 = "3";
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_VERTEX, COMMAND_DAG_VERTEX_ADD, key3, value3);
  dag_entry_add(dag, key3, (void *)value3, strlen(value1) + 1);
  char *key4 = "d";
  char *value4 = "4";
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_VERTEX, COMMAND_DAG_VERTEX_ADD, key4, value4);
  dag_entry_add(dag, key4, (void *)value4, strlen(value1) + 1);

  // printf("> %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_VERTEX, COMMAND_DAG_VERTEX_GET, key1);
  // dag_vertex_t *result_key1 = dag_entry_get(dag, key1);
  // assert(result_key1 != NULL);
  // printf("%s\n", (char *)result_key1->value);

  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_EDGE, COMMAND_DAG_EDGE_ADD, key1, key2);
  dag_edge_add(dag, key1, key2);
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_EDGE, COMMAND_DAG_EDGE_ADD, key2, key3);
  dag_edge_add(dag, key2, key3);
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_EDGE, COMMAND_DAG_EDGE_ADD, key1, key4);
  dag_edge_add(dag, key1, key4);
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_EDGE, COMMAND_DAG_EDGE_ADD, key4, key3);
  dag_edge_add(dag, key4, key3);
  printf("> %s %s %s %s %s\n", COMMAND_DAG, COMMAND_DAG_EDGE, COMMAND_DAG_EDGE_ADD, key2, key4);
  dag_edge_add(dag, key2, key4);

  printf("> %s %s\n", COMMAND_DAG, COMMAND_DAG_PRINT);
  dag_print(dag);
  char *filename = "dag.dot";
  printf("> %s %s %s\n", COMMAND_DAG, COMMAND_DAG_DUMP, filename);
  dag_dump(dag, filename);
}
