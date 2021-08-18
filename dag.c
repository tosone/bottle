#include <dag.h>

dag_t *dag_create() {
  dag_t *dag = (dag_t *)malloc(sizeof(dag_t));
  dag->entries = NULL;
  return dag;
}

bool dag_entry_isexist(dag_t *dag, char *key) {
  bool exist = false;
  for (int i = 0; i < dag->count; i++) {
    if (strncasecmp(dag->entries[i].key, key, strlen(key)) == 0) {
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
    if (strncasecmp(dag->entries[i].key, key, strlen(key)) == 0) {
      entry = dag->entries + i;
      break;
    }
    // printf("%s %s\n", dag->entries[i].key, (char *)dag->entries[i].value);
  }
  return entry;
}
