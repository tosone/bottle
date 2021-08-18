#include <dag.h>

dag_t *dag_create() {
  dag_t *dag = (dag_t *)malloc(sizeof(dag_t));
  dag->entries = NULL;
  return dag;
}

bool dag_entry_isexist(dag_t *dag, char *key) {
  bool exist = false;
  for (int i = 0; i < dag->count; i++) {
    if (strlen(dag->entries[i].key) == strlen(key) && strncasecmp(dag->entries[i].key, key, strlen(key)) == 0) {
      return true;
    }
  }
  return exist;
}

dag_entry_t *dag_entry_add(dag_t *dag, char *key, void *value, size_t value_length) {
  dag_entry_t *entry = (dag_entry_t *)malloc(sizeof(dag_entry_t));
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

  dag->count++;
  if (dag->entries == NULL) {
    dag->entries = (dag_entry_t *)malloc(sizeof(dag_entry_t));
  } else {
    dag->entries = (dag_entry_t *)realloc(dag->entries, sizeof(dag_entry_t) * dag->count);
  }
  memcpy(dag->entries + dag->count - 1, entry, sizeof(dag_entry_t));
  free(entry);
  return dag->entries + dag->count - 1;
}

dag_entry_t *dag_entry_get(dag_t *dag, char *key) {
  dag_entry_t *entry = NULL;
  for (int i = 0; i < dag->count; i++) {
    if (strlen(dag->entries[i].key) == strlen(key) && strncasecmp(dag->entries[i].key, key, strlen(key)) == 0) {
      entry = dag->entries + i;
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
  dag_entry_t *entries = NULL;
  for (int i = 0; i < dag->count; i++) {
    if (strlen(dag->entries[i].key) == strlen(key) && strncasecmp(dag->entries[i].key, key, strlen(key)) == 0) {
      continue;
    } else {
      new_count++;
      if (entries == NULL) {
        entries = (dag_entry_t *)malloc(sizeof(dag_entry_t));
      } else {
        entries = (dag_entry_t *)realloc(entries, sizeof(dag_entry_t) * dag->count);
      }
      memcpy(entries + new_count - 1, dag->entries + i, sizeof(dag_entry_t));
    }
  }
  free(dag->entries);
  dag->entries = entries;
  dag->count = new_count;
  return true;
}

bool dag_edge_isexist(dag_entry_t *from, dag_entry_t *to) {
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
  dag_entry_t *from_entry = dag_entry_get(dag, from);
  dag_entry_t *to_entry = dag_entry_get(dag, to);
  if (dag_edge_isexist(from_entry, to_entry)) {
    printf("edge already exist\n");
    return false;
  }
  from_entry->children_length++;
  if (from_entry->children == NULL) {
    from_entry->children = (dag_entry_t **)malloc(sizeof(dag_entry_t *) * from_entry->children_length);
    from_entry->children[from_entry->children_length - 1] = to_entry;
  } else {
    from_entry->children = (dag_entry_t **)realloc(from_entry->children, sizeof(dag_entry_t *) * from_entry->children_length);
    from_entry->children[from_entry->children_length - 1] = to_entry;
  }
  to_entry->parents_length++;
  if (to_entry->parents == NULL) {
    to_entry->parents = (dag_entry_t **)malloc(sizeof(dag_entry_t *) * to_entry->parents_length);
    to_entry->parents[to_entry->parents_length - 1] = from_entry;
  } else {
    to_entry->parents = (dag_entry_t **)realloc(to_entry->parents, sizeof(dag_entry_t *) * to_entry->parents_length);
    to_entry->parents[to_entry->parents_length - 1] = from_entry;
  }
  return true;
}

dag_t *dag_duplicate(dag_t *dag) {
  dag_t *new_dag = (dag_t *)malloc(sizeof(dag_t));
  new_dag->count = 0;
  new_dag->entries = NULL;
  for (int i = 0; i < dag->count; i++) {
    new_dag->count++;
    if (new_dag->entries == NULL) {
      new_dag->entries = (dag_entry_t *)malloc(sizeof(dag_entry_t) * new_dag->count);
    } else {
      new_dag->entries = (dag_entry_t *)realloc(new_dag->entries, sizeof(dag_entry_t) * new_dag->count);
    }
    dag_entry_t *entry = (dag_entry_t *)malloc(sizeof(dag_entry_t));

    entry->children = NULL;
    entry->children_length = 0;
    entry->parents = NULL;
    entry->parents_length = 0;
    entry->key = (char *)malloc(strlen(dag->entries[i].key) + 1);
    entry->value = (void *)malloc(dag->entries[i].value_length);
    entry->value_length = dag->entries[i].value_length;
    bzero(entry->key, strlen(dag->entries[i].key) + 1);
    strcpy(entry->key, dag->entries[i].key);
    memcpy(entry->value, dag->entries[i].value, dag->entries[i].value_length);
  }

  for (int i = 0; i < dag->count; i++) {
    dag_entry_t *entry = dag_entry_get(new_dag, dag->entries[i].key);

    for (int j = 0; j < dag->entries[i].children_length; j++) {
      dag_entry_t *to_entry = dag_entry_get(new_dag, dag->entries[i].children[j]->key);
      entry->children_length++;
      if (entry->children == NULL) {
        entry->children = (dag_entry_t **)malloc(sizeof(dag_entry_t *));
      } else {
        entry->children = (dag_entry_t **)realloc(entry->children, sizeof(dag_entry_t *) * entry->children_length);
      }
      entry->children[entry->children_length - 1] = to_entry;
    }

    for (int j = 0; j < dag->entries[i].parents_length; j++) {
      dag_entry_t *from_entry = dag_entry_get(new_dag, dag->entries[i].parents[j]->key);
      entry->parents_length++;
      if (entry->parents == NULL) {
        entry->parents = (dag_entry_t **)malloc(sizeof(dag_entry_t *));
      } else {
        entry->parents = (dag_entry_t **)realloc(entry->parents, sizeof(dag_entry_t *) * entry->parents_length);
      }
      entry->parents[entry->parents_length - 1] = from_entry;
    }
  }
  return new_dag;
}

void dag_range(dag_t *dag) { return; }
