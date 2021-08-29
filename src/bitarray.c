#include <bitarray.h>

uint8_t get_kth_bit(uint8_t n, uint8_t k) { return ((1 << k) & n) >> k; }
uint8_t set_kth_bit(uint8_t n, uint8_t k) { return ((1 << k) | n); }
uint8_t clear_kth_bit(uint8_t n, uint8_t k) { return ((1 << k) & n); }
uint8_t toggle_kth_bit(uint8_t n, uint8_t k) { return ((1 << k) ^ n); }

bitarray_t *bitarray_create() {
  bitarray_t *bitarray = (bitarray_t *)malloc(sizeof(bitarray_t));
  bitarray->root = deque_create();
  bitarray->count = 0;
  return bitarray;
}

bitarray_entry_t *bitarray_get_helper(bitarray_t *bitarray, char *key) {
  deque_entry_t *root = bitarray->root->head;
  while (root != NULL) {
    bitarray_entry_t *entry = *(bitarray_entry_t **)root->data;
    if (strcmp(key, entry->key) == 0) {
      return entry;
    }
    root = root->next;
  }
  bitarray_entry_t *entry = (bitarray_entry_t *)malloc(sizeof(bitarray_entry_t));
  entry->key = NULL;
  entry->data = NULL;
  entry->data_length = 0;
  return entry;
}

void bitarray_set(bitarray_t *bitarray, char *key, size_t index, uint8_t value) {
  bitarray_entry_t *entry = bitarray_get_helper(bitarray, key);
  if (entry->key == NULL) {
    entry->key = (char *)malloc(strlen(key) + 1);
    strcpy(entry->key, key);
    deque_push_back(bitarray->root, &entry, sizeof(bitarray_entry_t *));
    bitarray->count++;
  }
  size_t offset = index / 8;
  if (entry->data_length < offset) {
    if (entry->data == NULL) {
      entry->data = malloc(offset + 1);
    } else {
      entry->data = realloc(entry->data, offset + 1);
    }
    memset(entry->data + entry->data_length, 0, offset - entry->data_length + 1);
    entry->data_length = offset + 1;
    uint8_t val = *(uint8_t *)(entry->data + offset);
    if (value == 1) {
      *(uint8_t *)(entry->data + offset) = set_kth_bit(val, index % 8);
    }
  } else {
    uint8_t val = *(uint8_t *)(entry->data + offset);
    uint8_t nval = get_kth_bit(val, index % 8);
    if ((get_kth_bit(val, index % 8) == 1 && value == 1) || (nval == 0 && value == 1)) {
      *(uint8_t *)(entry->data + offset) = set_kth_bit(val, index % 8);
    }
  }
}

uint8_t bitarray_get(bitarray_t *bitarray, char *key, size_t index) {
  bitarray_entry_t *entry = bitarray_get_helper(bitarray, key);
  if (entry->key == NULL) {
    entry->key = (char *)malloc(strlen(key) + 1);
    strcpy(entry->key, key);
    deque_push_back(bitarray->root, &entry, sizeof(bitarray_entry_t *));
    bitarray->count++;
  }
  size_t offset = index / 8;
  if (entry->data_length < offset) {
    if (entry->data == NULL) {
      entry->data = malloc(offset + 1);
    } else {
      entry->data = realloc(entry->data, offset + 1);
    }
    memset(entry->data + entry->data_length, 0, offset - entry->data_length + 1);
    entry->data_length = offset + 1;
  } else {
    return get_kth_bit(*(uint8_t *)(entry->data + offset), index % 8);
  }
  return 0;
}

size_t bitarray_count(bitarray_t *bitarray) { return 0; }

void bitarray_free(bitarray_t *bitarray) {}

void bitarray_test(bitarray_t *bitarray) {
  char key1[] = "test1";
  printf("> bitarray set 9 1\n");
  bitarray_set(bitarray, key1, 9, 1);
  printf("> bitarray get 9 1\n");
  printf("%d\n", bitarray_get(bitarray, key1, 9));
}
