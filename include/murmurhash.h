#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MURMURHASH_SEED 0xadc83b19ULL

uint32_t murmurhash32(const void *data, size_t len);
uint64_t murmurhash64(const void *data, size_t len);
