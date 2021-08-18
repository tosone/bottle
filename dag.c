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

dag_entry_t *dag_add_entry(dag_t *dag, char *key, void *value, size_t value_length) {
  dag_entry_t *entry = (dag_entry_t *)malloc(sizeof(dag_entry_t));
  entry->key = (char *)malloc(strlen(key) + 1);
  entry->value = (void *)malloc(value_length);
  entry->value_length = value_length;

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

dag_entry_t *dag_get_entry(dag_t *dag, char *key) {
  dag_entry_t *entry = NULL;
  for (int i = 0; i < dag->count; i++) {
    if (strlen(dag->entries[i].key) == strlen(key) && strncasecmp(dag->entries[i].key, key, strlen(key)) == 0) {
      entry = dag->entries + i;
      break;
    }
  }
  return entry;
}

bool dag_del_entry(dag_t *dag, char *key) {
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
