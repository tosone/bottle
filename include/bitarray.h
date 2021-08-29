#include <stdio.h>
#include <stdlib.h>

#include <deque.h>

typedef struct bitarray_entry_t {
  char *key;
  void *data;
  size_t data_length;
} bitarray_entry_t;

typedef struct bitarray_t {
  deque_t *root;
  size_t count;
} bitarray_t;

bitarray_t *bitarray_create();
void bitarray_set(bitarray_t *bitarray, char *key, size_t index, uint8_t value);
uint8_t bitarray_get(bitarray_t *bitarray, char *key, size_t index);
size_t bitarray_count(bitarray_t *bitarray);
void bitarray_free();
