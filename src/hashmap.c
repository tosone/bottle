#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashmap.h>
#include <murmurhash.h>

const int HMAP_MAX_LINK_LIST_DEPTH = 8;   // 分支链表的最大深度
const int HMAP_INITIALIZE_SIZE = 8;       // 初始化空间
const int HMAP_GROW_FACTOR = 2;           // 增长因子
const int HMAP_GROW_FACTOR_USABLE = 1024; // 增长因子最大的有效范围
const int HMAP_GROW_MAX = 1024;           // 增长的最大数值
const float HMAP_THRESHOLD = 0.75f;       // 负载因子

hashmap_t *hashmap_create() {
  hashmap_t *hashmap = (hashmap_t *)malloc(sizeof(hashmap_t));
  hashmap->cap = HMAP_INITIALIZE_SIZE;
  hashmap->len = 0;
  hashmap->entries = (hashmap_entry_t **)malloc(sizeof(hashmap_entry_t *) * hashmap->cap);
  for (int i = 0; i < hashmap->cap; i++) {
    hashmap->entries[i] = NULL;
  }
  return hashmap;
}

hashmap_entry_t *hashmap_pair(const char *key, const void *value, const int value_length) {
  hashmap_entry_t *entry = (hashmap_entry_t *)malloc(sizeof(hashmap_entry_t));
  entry->key = (char *)malloc(strlen(key) + 1);
  entry->value = (void *)malloc(value_length);
  entry->value_length = value_length;

  strcpy(entry->key, key);
  memcpy(entry->value, value, value_length);
  entry->next = NULL;

  return entry;
}

void hashmap_rehash_helper(hashmap_t *hashmap, hashmap_entry_t *entry) {
  if (entry == NULL) {
    return;
  }
  hashmap_set(hashmap, entry->key, entry->value, entry->value_length);
  if (entry->next != NULL) {
    hashmap_rehash_helper(hashmap, entry->next);
  }
}

hashmap_t *hashmap_rehash(hashmap_t *hashmap) {
  int new_cap = hashmap->cap * HMAP_GROW_FACTOR;
  if (hashmap->cap >= HMAP_GROW_FACTOR_USABLE) {
    new_cap = hashmap->cap + HMAP_GROW_MAX;
  }

  hashmap_t *new_hashmap = (hashmap_t *)malloc(sizeof(hashmap_t));
  new_hashmap->cap = new_cap;
  new_hashmap->len = 0;
  new_hashmap->entries = (hashmap_entry_t **)malloc(sizeof(hashmap_entry_t *) * new_cap);
  for (int i = 0; i < new_hashmap->cap; i++) {
    new_hashmap->entries[i] = NULL;
  }
  for (int i = 0; i < hashmap->cap; i++) {
    if (hashmap->entries[i] != NULL) {
      hashmap_rehash_helper(new_hashmap, hashmap->entries[i]);
    }
  }
  hashmap_free(hashmap);
  return new_hashmap;
}

hashmap_t *hashmap_set(hashmap_t *hashmap, const char *key, const void *value, const int value_length) {
  if (hashmap->len / (hashmap->cap * 1.0) >= HMAP_THRESHOLD) {
    hashmap = hashmap_rehash(hashmap);
  }
  uint32_t slot = murmurhash(key, strlen(key), 0);
  hashmap_entry_t *entry = hashmap->entries[slot % hashmap->cap];

  hashmap->len++;

  if (entry == NULL) {
    hashmap->entries[slot % hashmap->cap] = hashmap_pair(key, value, value_length);
    return hashmap;
  }

  hashmap_entry_t *prev;

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      free(entry->value);
      entry->value = (void *)malloc(value_length);
      entry->value_length = value_length;
      memcpy(entry->value, value, value_length);
      return hashmap;
    }

    prev = entry;
    entry = prev->next;
  }

  prev->next = hashmap_pair(key, value, value_length);
  return hashmap;
}

void *hashmap_get(hashmap_t *hashmap, const char *key, size_t *value_length) {
  uint32_t slot = murmurhash(key, strlen(key), 0);
  hashmap_entry_t *entry = hashmap->entries[slot % hashmap->cap];
  if (entry == NULL) {
    return NULL;
  }
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      if (value_length != NULL) {
        *value_length = entry->value_length;
      }
      return entry->value;
    }
    entry = entry->next;
  }
  return NULL;
}

void hashmap_del(hashmap_t *hashmap, const char *key) {
  uint32_t slot = murmurhash(key, strlen(key), 0);
  hashmap_entry_t *entry = hashmap->entries[slot % hashmap->cap];
  if (entry == NULL) {
    return;
  }
  hashmap_entry_t *prev = NULL;
  int index = 0;
  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      if (entry->next == NULL && index == 0) {
        hashmap->entries[slot % hashmap->cap] = NULL;
      }
      if (entry->next != NULL && index == 0) {
        hashmap->entries[slot % hashmap->cap] = entry->next;
      }
      if (entry->next == NULL && index != 0) {
        prev->next = NULL;
      }
      if (entry->next != NULL && index != 0) {
        prev->next = entry->next;
      }
      free(entry->key);
      free(entry->value);
      free(entry);
      return;
    }
    prev = entry;
    entry = prev->next;
    index++;
  }
}

void entry_free(hashmap_entry_t *entry) {
  if (entry->next != NULL) {
    entry_free(entry->next);
  }
  free(entry->key);
  free(entry->value);
  free(entry);
}

void hashmap_free(hashmap_t *hashmap) {
  for (int i = 0; i < hashmap->cap; i++) {
    if (hashmap->entries[i] != NULL) {
      entry_free(hashmap->entries[i]);
    }
  }
  free(hashmap->entries);
  free(hashmap);
}

void hashmap_print_entry_helper(hashmap_entry_t *entry, bool *first) {
  if (*first) {
    *first = false;
    printf("%s:%s", entry->key, (char *)entry->value);
  } else {
    printf(" %s:%s", entry->key, (char *)entry->value);
  }
  if (entry->next != NULL) {
    hashmap_print_entry_helper(entry->next, first);
  }
}

void hashmap_print_helper(hashmap_t *hashmap, bool first) {
  for (int i = 0; i < hashmap->cap; i++) {
    if (hashmap->entries[i] != NULL) {
      hashmap_print_entry_helper(hashmap->entries[i], &first);
    }
  }
}

void hashmap_print(hashmap_t *hashmap) {
  hashmap_print_helper(hashmap, true);
  printf("\n");
}

void hashmap_iterate_helper(hashmap_t *hashmap, void (*f)(hashmap_entry_t *entry), hashmap_entry_t *entry) {
  if (entry->next != NULL) {
    hashmap_iterate_helper(hashmap, f, entry);
  }
  if (f != NULL) {
    f(entry);
  }
}

void hashmap_iterate(hashmap_t *hashmap, void (*f)(hashmap_entry_t *entry)) {
  for (int i = 0; i < hashmap->cap; i++) {
    if (hashmap->entries[i] != NULL) {
      hashmap_iterate_helper(hashmap, f, hashmap->entries[i]);
    }
  }
}
