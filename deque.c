#include <deque.h>

deque_t *deque_create() {
  deque_t *deque = (deque_t *)malloc(sizeof(deque_t));
  deque->head = NULL;
  deque->tail = NULL;
  deque->count = 0;
  return deque;
}

void deque_push_back(deque_t *deque, void *data, size_t data_length) {
  deque_entry_t *entry = (deque_entry_t *)malloc(sizeof(deque_entry_t));
  entry->data = (void *)malloc(data_length);
  memcpy(entry->data, data, data_length);
  entry->data_length = data_length;
  entry->next = NULL;
  entry->prev = NULL;
  deque->count++;
  if (deque->tail == NULL) {
    deque->head = entry;
    deque->tail = entry;
  } else {
    deque->tail->next = entry;
    entry->prev = deque->tail;
    deque->tail = entry;
  }
}

void deque_push_front(deque_t *deque, void *data, size_t data_length) {
  deque_entry_t *entry = (deque_entry_t *)malloc(sizeof(deque_entry_t));
  entry->data = (void *)malloc(data_length);
  memcpy(entry->data, data, data_length);
  entry->data_length = data_length;
  entry->next = NULL;
  entry->prev = NULL;
  deque->count++;
  if (deque->head == NULL) {
    deque->head = entry;
    deque->tail = entry;
  } else {
    deque->head->prev = entry;
    entry->next = deque->head;
    deque->head = entry;
  }
}

deque_entry_t *deque_pop_back(deque_t *deque) {
  if (deque->tail == NULL) {
    return NULL;
  }
  deque->count--;
  if (deque->tail == deque->head) {
    deque_entry_t *entry = deque->tail;
    deque->head = deque->tail = NULL;
    return entry;
  }
  deque_entry_t *entry = deque->tail;
  deque->tail = entry->prev;
  entry->prev = NULL;
  return entry;
}

deque_entry_t *deque_pop_front(deque_t *deque) {
  if (deque->head == NULL) {
    return NULL;
  }
  deque->count--;
  if (deque->tail == deque->head) {
    deque_entry_t *entry = deque->head;
    deque->head = deque->tail = NULL;
    return entry;
  }
  deque_entry_t *entry = deque->head;
  deque->head = entry->next;
  entry->next = NULL;
  return entry;
}

size_t deque_size(deque_t *deque) { return deque->count; }

void deque_dump_helper(deque_entry_t *entry, FILE *stream, size_t index) {
  if (entry != NULL) {
    fprintf(stream, "node%zu [shape=record,label=\"{{%s}}\"];\n", index, (char *)entry->data);
  }
  if (entry->next != NULL) {
    size_t new_index = index + 1;
    fprintf(stream, "node%zu -- node%zu;\n", index, new_index);
    deque_dump_helper(entry->next, stream, new_index);
  }
}

void deque_dump(deque_t *deque, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "graph {\n");
  deque_dump_helper(deque->head, stream, 0);
  fprintf(stream, "}\n");
  fclose(stream);
}

deque_entry_t *deque_entry_search(deque_t *deque, bool (*callback)(void *data, size_t data_length)) {
  deque_entry_t *result = NULL;
  if (deque == NULL || deque->head == NULL) {
    return result;
  }

  deque_entry_t *ans = deque->head;
  while (ans != NULL) {
    if (callback(ans->data, ans->data_length)) {
      return ans;
    }
    ans = ans->next;
  }
  return result;
}

bool deque_entry_delete(deque_t *deque, bool (*callback)(void *data, size_t data_length)) {
  deque_entry_t *result = deque_entry_search(deque, callback);
  if (result == NULL) {
    return false;
  }
  if (result->prev != NULL) {
    result->prev->next = result->next;
  }
  if (result->next != NULL) {
    result->next->prev = result->prev;
  }

  return true;
}

bool deque_test_callback(void *data, size_t data_length) {
  if (strcmp((char *)data, "key_search") == 0) {
    return true;
  }
  return false;
}

void deque_test(deque_t *deque) {
  char *key1 = "test001";
  printf("> %s %s %s %s\n", COMMAND_DEQUE, COMMAND_DEQUE_PUSH, COMMAND_DEQUE_BACK, key1);
  deque_push_back(deque, (void *)key1, strlen(key1) + 1);
  char *key2 = "test002";
  printf("> %s %s %s %s\n", COMMAND_DEQUE, COMMAND_DEQUE_PUSH, COMMAND_DEQUE_BACK, key2);
  deque_push_back(deque, (void *)key2, strlen(key2) + 1);
  char *key3 = "test003";
  printf("> %s %s %s %s\n", COMMAND_DEQUE, COMMAND_DEQUE_PUSH, COMMAND_DEQUE_BACK, key3);
  deque_push_front(deque, (void *)key3, strlen(key3) + 1);
  char *key4 = "test004";
  printf("> %s %s %s %s\n", COMMAND_DEQUE, COMMAND_DEQUE_PUSH, COMMAND_DEQUE_FRONT, key4);
  deque_push_front(deque, (void *)key4, strlen(key4) + 1);

  bool result = deque_entry_delete(deque, deque_test_callback);
  assert(result == false);

  char *key5 = "key_search";
  printf("> %s %s %s %s\n", COMMAND_DEQUE, COMMAND_DEQUE_PUSH, COMMAND_DEQUE_BACK, key5);
  deque_push_front(deque, (void *)key5, strlen(key5) + 1);

  result = deque_entry_delete(deque, deque_test_callback);
  assert(result == true);
}
