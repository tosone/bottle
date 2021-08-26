#include <murmurhash.h>

uint32_t murmurhash32(const void *data, size_t len) {
  if (data == NULL || len == 0) {
    return 0;
  }

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  const int nblocks = len / 4;
  const uint32_t *blocks = (const uint32_t *)(data);
  const uint8_t *tail = (const uint8_t *)(data + (nblocks * 4));

  uint32_t h = 0;

  int i;
  uint32_t k;
  for (i = 0; i < nblocks; i++) {
    k = blocks[i];

    k *= c1;
    k = (k << 15) | (k >> (32 - 15));
    k *= c2;

    h ^= k;
    h = (h << 13) | (h >> (32 - 13));
    h = (h * 5) + 0xe6546b64;
  }

  k = 0;
  switch (len & 3) {
  case 3:
    k ^= tail[2] << 16;
  case 2:
    k ^= tail[1] << 8;
  case 1:
    k ^= tail[0];
    k *= c1;
    k = (k << 15) | (k >> (32 - 15));
    k *= c2;
    h ^= k;
  };

  h ^= len;

  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

uint64_t murmurhash64(const void *key, size_t len) {
  const uint64_t m = 0xc6a4a7935bd1e995;
  const int r = 47;
  uint64_t h = MURMURHASH_SEED ^ (len * m);
  const uint8_t *data = (const uint8_t *)key;
  const uint8_t *end = data + (len - (len & 7));

  while (data != end) {
    uint64_t k;
    k = *((uint64_t *)data);
    k *= m;
    k ^= k >> r;
    k *= m;
    h ^= k;
    h *= m;
    data += 8;
  }

  switch (len & 7) {
  case 7:
    h ^= (uint64_t)data[6] << 48; /* fall-thru */
  case 6:
    h ^= (uint64_t)data[5] << 40; /* fall-thru */
  case 5:
    h ^= (uint64_t)data[4] << 32; /* fall-thru */
  case 4:
    h ^= (uint64_t)data[3] << 24; /* fall-thru */
  case 3:
    h ^= (uint64_t)data[2] << 16; /* fall-thru */
  case 2:
    h ^= (uint64_t)data[1] << 8; /* fall-thru */
  case 1:
    h ^= (uint64_t)data[0];
    h *= m; /* fall-thru */
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;
  return h;
}
