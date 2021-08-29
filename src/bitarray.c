#include <bitarray.h>

bitarray_t *bitarray_create() {
  bitarray_t *bitarray = (bitarray_t *)malloc(sizeof(bitarray_t));
  bitarray->root = deque_create();
  bitarray->count = 0;
  return bitarray;
}

void bitarray_set(bitarray_t *bitarray, char *key, size_t index, uint8_t value) {}

uint8_t bitarray_get(bitarray_t *bitarray, char *key, size_t index) { return 0; }

size_t bitarray_count(bitarray_t *bitarray) { return 0; }

void bitarray_free() {}
