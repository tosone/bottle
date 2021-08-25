#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <murmurhash.h>

typedef struct bloom_t {
  int bytes;
  unsigned char *bf;
  int hashes;
} bloom_t;

bloom_t *bloom_create();
void bloom_push(bloom_t *bloom, void *data, size_t data_length);
bool bloom_check(bloom_t *bloom, void *data, size_t data_length);
void bloom_free(bloom_t *bloom);
