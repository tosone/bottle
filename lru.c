#include <lru.h>

const int LRU_INITIALIZE_SIZE = 2; // 初始化空间

LRU *LRU_create() {
  LRU *lru = (LRU *)malloc(sizeof(LRU));
  lru->len = 0;
  lru->cap = LRU_INITIALIZE_SIZE;
  lru->head = (LRUEntry *)malloc(sizeof(LRUEntry));
  lru->tail = (LRUEntry *)malloc(sizeof(LRUEntry));
  lru->head->next = lru->tail;
  lru->tail->prev = lru->head;
  lru->hmap = hashmap_create();
  return lru;
}

void addToHead(LRU *lru, LRUEntry *entry) {
  entry->prev = lru->head;
  entry->next = lru->head->next;
  lru->head->next->prev = entry;
  lru->head->next = entry;
}

void removeNode(LRU *lru, LRUEntry *entry) {
  entry->prev->next = entry->next;
  entry->next->prev = entry->prev;
}

void moveToHead(LRU *lru, LRUEntry *entry) {
  removeNode(lru, entry);
  addToHead(lru, entry);
}

LRUEntry *removeTail(LRU *lru) {
  LRUEntry *node = lru->tail->prev;
  removeNode(lru, node);
  return node;
}

void *LRU_get(LRU *lru, char *key, size_t *value_length) {
  LRUEntry *entry = (LRUEntry *)hashmap_get(lru->hmap, key, NULL);
  if (entry == NULL) {
    return NULL;
  }
  if (value_length != NULL) {
    *value_length = entry->value_length;
  }
  moveToHead(lru, entry);
  return entry->value;
}

void LRU_set(LRU *lru, char *key, void *value, size_t value_length) {
  LRUEntry *entry = (LRUEntry *)hashmap_get(lru->hmap, key, NULL);
  if (entry == NULL) {
    lru->len++;
    LRUEntry *new_entry = (LRUEntry *)malloc(sizeof(LRUEntry));
    new_entry->key = (char *)malloc(strlen(key) + 1);
    memcpy(new_entry->key, key, strlen(key) + 1);
    new_entry->value = (void *)malloc(value_length);
    memcpy(new_entry->value, value, value_length);
    new_entry->value_length = value_length;
    addToHead(lru, new_entry);
    hashmap_set(lru->hmap, key, (void *)new_entry, sizeof(LRUEntry));
    if (lru->len > lru->cap) {
      LRUEntry *entry = removeTail(lru);
      char *temp_key = (char *)malloc(strlen(entry->key) + 1);
      memcpy(temp_key, entry->key, strlen(entry->key) + 1);
      free(entry->key);
      free(entry->value);
      hashmap_del(lru->hmap, temp_key);
      free(temp_key);
      lru->len = lru->cap;
    }
  } else {
    free(entry->value);
    entry->value = (void *)malloc(value_length);
    memcpy(entry->value, value, value_length);
    entry->value_length = value_length;
    moveToHead(lru, entry);
  }
}

void LRU_print(LRU *lru) {
  hashmap_print(lru->hmap);
}

void LRU_free(LRU *lru) {
  hashmap_free(lru->hmap);
  free(lru->head);
  free(lru->tail);
}
