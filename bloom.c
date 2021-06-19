#include <bloom.h>

uint64_t entries = 1e6;
double_t error = 0.01;

// inpired by https://git.sr.ht/~sircmpwn/bloom/tree/master/item/bloom.c and
// http://en.wikipedia.org/wiki/Bloom_filter

// Optimal number of bits is:
//   bits = (entries * ln(error)) / ln(2)^2
// Optimal number of hash functions is:
//   hashes = bpe * ln(2)

bloom_t *bloom_create() {
  bloom_t *bloom = (bloom_t *)malloc(sizeof(bloom_t));
  double_t denom = pow(M_LN2, 2);
  double_t bpe = -(log(error) / denom);
  int bits = (int)((double_t)entries * bpe);
  bloom->hashes = (int)ceil(M_LN2 * bpe);

  if (bits % 8 != 0) {
    bloom->bytes = (bits / 8) + 1;
  } else {
    bloom->bytes = bits / 8;
  }
  bloom->bf = (uint8_t *)calloc(bloom->bytes, sizeof(uint8_t));

  return bloom;
}

void bloom_push(bloom_t *bloom, void *data, size_t data_length) {
  for (int i = 0; i < bloom->hashes; i++) {
    uint32_t hash = murmurhash(data, data_length, i);
    hash %= bloom->bytes * 8;
    bloom->bf[hash / 8] |= 1 << hash % 8;
  }
}

bool bloom_check(bloom_t *bloom, void *data, size_t data_length) {
  for (int i = 0; i < bloom->hashes; i++) {
    uint32_t hash = murmurhash(data, data_length, i);
    hash %= bloom->bytes * 8;
    if (!(bloom->bf[hash / 8] & 1 << hash % 8)) {
      return false;
    }
  }
  return true;
}

void bloom_free(bloom_t *bloom) {
  if (bloom != NULL) {
    free(bloom->bf);
    free(bloom);
  }
}
