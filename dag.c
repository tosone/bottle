#include <dag.h>

dag_t *dag_create() {
  dag_t *dag = (dag_t *)malloc(sizeof(dag_t));
  dag->vertexes = NULL;
  dag->vertex_count = 0;
  dag->edge_count = 0;
  return dag;
}

void dag_free(dag_t *dag) {
  for (int i = 0; i < dag->vertex_count; i++) {
    free(dag->vertexes[i].children);
    free(dag->vertexes[i].parents);
    free(dag->vertexes[i].key);
    free(dag->vertexes[i].value);
  }
  free(dag->vertexes);
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

  bzero(entry->key, strlen(key) + 1);
  strcpy(entry->key, key);
  memcpy(entry->value, value, value_length);

  dag->vertex_count++;
  if (dag->vertexes == NULL) {
    dag->vertexes = (dag_vertex_t *)malloc(sizeof(dag_vertex_t));
  } else {
    dag->vertexes = (dag_vertex_t *)realloc(dag->vertexes, sizeof(dag_vertex_t) * dag->vertex_count);
  }
  memcpy(dag->vertexes + dag->vertex_count - 1, entry, sizeof(dag_vertex_t));
  free(entry);
  return dag->vertexes + dag->vertex_count - 1;
}

dag_vertex_t *dag_entry_get(dag_t *dag, char *key) {
  dag_vertex_t *entry = NULL;
  for (int i = 0; i < dag->vertex_count; i++) {
    if (strlen(dag->vertexes[i].key) == strlen(key) && strncasecmp(dag->vertexes[i].key, key, strlen(key)) == 0) {
      entry = dag->vertexes + i;
      break;
    }
  }
  return entry;
}

bool dag_entry_del(dag_t *dag, char *key) {
  if (!dag_entry_isexist(dag, key)) {
    return false;
  }
  uint64_t new_count = 0;
  dag_vertex_t *vertexes = NULL;
  for (int i = 0; i < dag->vertex_count; i++) {
    if (strlen(dag->vertexes[i].key) == strlen(key) && strncasecmp(dag->vertexes[i].key, key, strlen(key)) == 0) {
      continue;
    } else {
      new_count++;
      if (vertexes == NULL) {
        vertexes = (dag_vertex_t *)malloc(sizeof(dag_vertex_t));
      } else {
        vertexes = (dag_vertex_t *)realloc(vertexes, sizeof(dag_vertex_t) * dag->vertex_count);
      }
      memcpy(vertexes + new_count - 1, dag->vertexes + i, sizeof(dag_vertex_t));
    }
  }
  free(dag->vertexes);
  dag->vertexes = vertexes;
  dag->vertex_count = new_count;
  return true;
}

bool dag_edge_isexist(dag_vertex_t *from, dag_vertex_t *to) {
  bool from_edge = false;
  bool to_edge = false;
  for (int i = 0; i < from->children_length; i++) {
    if (from->children[i] == to) {
      from_edge = true;
    }
  }
  for (int i = 0; i < to->parents_length; i++) {
    if (to->parents[i] == from) {
      to_edge = true;
    }
  }
  if (from_edge && to_edge) {
    return true;
  }
  return false;
}

bool dag_edge_add(dag_t *dag, char *from, char *to) {
  if (!dag_entry_isexist(dag, from) || !dag_entry_isexist(dag, to)) {
    printf("key not found\n");
    return false;
  }
  dag_vertex_t *from_entry = dag_entry_get(dag, from);
  dag_vertex_t *to_entry = dag_entry_get(dag, to);
  if (dag_edge_isexist(from_entry, to_entry)) {
    printf("edge already exist\n");
    return false;
  }

  dag->edge_count++;

  from_entry->children_length++;
  if (from_entry->children == NULL) {
    from_entry->children = (dag_vertex_t **)malloc(sizeof(dag_vertex_t *) * from_entry->children_length);
    from_entry->children[from_entry->children_length - 1] = to_entry;
  } else {
    from_entry->children = (dag_vertex_t **)realloc(from_entry->children, sizeof(dag_vertex_t *) * from_entry->children_length);
    from_entry->children[from_entry->children_length - 1] = to_entry;
  }
  to_entry->parents_length++;
  if (to_entry->parents == NULL) {
    to_entry->parents = (dag_vertex_t **)malloc(sizeof(dag_vertex_t *) * to_entry->parents_length);
    to_entry->parents[to_entry->parents_length - 1] = from_entry;
  } else {
    to_entry->parents = (dag_vertex_t **)realloc(to_entry->parents, sizeof(dag_vertex_t *) * to_entry->parents_length);
    to_entry->parents[to_entry->parents_length - 1] = from_entry;
  }
  return true;
}

bool dag_edge_delete(dag_t *dag, char *from, char *to) {
  if (!dag_entry_isexist(dag, from) || !dag_entry_isexist(dag, to)) {
    printf("key not found\n");
    return false;
  }
  dag_vertex_t *from_vertex = dag_entry_get(dag, from);
  dag_vertex_t *to_vertex = dag_entry_get(dag, to);
  if (!dag_edge_isexist(from_vertex, to_vertex)) {
    printf("edge not found\n");
    return false;
  }

  dag_vertex_t **new_from_vertex_children = NULL;
  uint64_t new_from_vertex_childred_length = 0;
  for (uint64_t i = 0; i < from_vertex->children_length; i++) {
    if (from_vertex->children[i] == to_vertex) {
      continue;
    } else {
      new_from_vertex_childred_length++;
      if (new_from_vertex_children == NULL) {
        new_from_vertex_children = (dag_vertex_t **)malloc(sizeof(dag_vertex_t *) * new_from_vertex_childred_length);
      } else {
        new_from_vertex_children = (dag_vertex_t **)realloc(new_from_vertex_children, sizeof(dag_vertex_t *) * new_from_vertex_childred_length);
      }
      new_from_vertex_children[new_from_vertex_childred_length - 1] = from_vertex->children[i];
    }
  }
  free(from_vertex->children);
  from_vertex->children = new_from_vertex_children;
  from_vertex->children_length = new_from_vertex_childred_length;

  dag_vertex_t **new_to_vertex_parents = NULL;
  uint64_t new_to_vertex_parents_length = 0;
  for (uint64_t i = 0; i < to_vertex->parents_length; i++) {
    if (to_vertex->parents[i] == from_vertex) {
      continue;
    } else {
      new_to_vertex_parents_length++;
      if (new_to_vertex_parents == NULL) {
        new_to_vertex_parents = (dag_vertex_t **)malloc(sizeof(dag_vertex_t *) * new_to_vertex_parents_length);
      } else {
        new_to_vertex_parents = (dag_vertex_t **)realloc(new_to_vertex_parents, sizeof(dag_vertex_t *) * new_to_vertex_parents_length);
      }
      new_to_vertex_parents[new_to_vertex_parents_length - 1] = to_vertex->children[i];
    }
  }
  free(to_vertex->parents);
  to_vertex->parents = new_to_vertex_parents;
  to_vertex->parents_length = new_to_vertex_parents_length;

  return true;
}

dag_t *dag_duplicate(dag_t *dag) {
  dag_t *new_dag = (dag_t *)malloc(sizeof(dag_t));
  new_dag->vertex_count = 0;
  new_dag->edge_count = 0;
  new_dag->vertexes = NULL;
  for (int i = 0; i < dag->vertex_count; i++) {
    new_dag->vertex_count++;
    if (new_dag->vertexes == NULL) {
      new_dag->vertexes = (dag_vertex_t *)malloc(sizeof(dag_vertex_t) * new_dag->vertex_count);
    } else {
      new_dag->vertexes = (dag_vertex_t *)realloc(new_dag->vertexes, sizeof(dag_vertex_t) * new_dag->vertex_count);
    }

    new_dag->vertexes[new_dag->vertex_count - 1].children = NULL;
    new_dag->vertexes[new_dag->vertex_count - 1].children_length = 0;
    new_dag->vertexes[new_dag->vertex_count - 1].parents = NULL;
    new_dag->vertexes[new_dag->vertex_count - 1].parents_length = 0;
    new_dag->vertexes[new_dag->vertex_count - 1].key = (char *)malloc(strlen(dag->vertexes[i].key) + 1);
    bzero(new_dag->vertexes[new_dag->vertex_count - 1].key, strlen(dag->vertexes[i].key) + 1);
    strcpy(new_dag->vertexes[new_dag->vertex_count - 1].key, dag->vertexes[i].key);
    new_dag->vertexes[new_dag->vertex_count - 1].value = (void *)malloc(dag->vertexes[i].value_length);
    memcpy(new_dag->vertexes[new_dag->vertex_count - 1].value, dag->vertexes[i].value, dag->vertexes[i].value_length);
    new_dag->vertexes[new_dag->vertex_count - 1].value_length = dag->vertexes[i].value_length;
  }

  for (int i = 0; i < dag->vertex_count; i++) {
    dag_vertex_t *entry = dag_entry_get(new_dag, dag->vertexes[i].key);

    for (int j = 0; j < dag->vertexes[i].children_length; j++) {
      dag_vertex_t *to_entry = dag_entry_get(new_dag, dag->vertexes[i].children[j]->key);
      entry->children_length++;
      if (entry->children == NULL) {
        entry->children = (dag_vertex_t **)malloc(sizeof(dag_vertex_t *));
      } else {
        entry->children = (dag_vertex_t **)realloc(entry->children, sizeof(dag_vertex_t *) * entry->children_length);
      }
      entry->children[entry->children_length - 1] = to_entry;
    }

    for (int j = 0; j < dag->vertexes[i].parents_length; j++) {
      dag_vertex_t *from_entry = dag_entry_get(new_dag, dag->vertexes[i].parents[j]->key);
      entry->parents_length++;
      if (entry->parents == NULL) {
        entry->parents = (dag_vertex_t **)malloc(sizeof(dag_vertex_t *));
      } else {
        entry->parents = (dag_vertex_t **)realloc(entry->parents, sizeof(dag_vertex_t *) * entry->parents_length);
      }
      entry->parents[entry->parents_length - 1] = from_entry;
    }
  }
  return new_dag;
}

void dag_range(dag_t *dag) {
  dag_t *new_dag = dag_duplicate(dag);
  for (int i = 0; i < dag->vertex_count; i++) {
    printf("node: %s \n", dag->vertexes[i].key);
    for (int j = 0; j < dag->vertexes[i].children_length; j++) {
      printf("children: %s \n", dag->vertexes[i].children[j]->key);
    }
    for (int j = 0; j < dag->vertexes[i].parents_length; j++) {
      printf("parents: %s \n", dag->vertexes[i].parents[j]->key);
    }
  }
  for (int i = 0; i < new_dag->vertex_count; i++) {
    printf("node: %s \n", new_dag->vertexes[i].key);
    for (int j = 0; j < new_dag->vertexes[i].children_length; j++) {
      printf("children: %s \n", new_dag->vertexes[i].children[j]->key);
    }
    for (int j = 0; j < new_dag->vertexes[i].parents_length; j++) {
      printf("parents: %s \n", new_dag->vertexes[i].parents[j]->key);
    }
  }
  return;
}
