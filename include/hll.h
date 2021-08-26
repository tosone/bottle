#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <murmurhash.h>

#define HLL_REGISTERS (1 << 14)

#define HLL_INDEX_BITS 14
#define HLL_VALUE_BITS 50
#define HLL_INDEX_MASK 0xFFFC000000000000
#define HLL_VALUE_MASK 0x0003FFFFFFFFFFFF

#define HLL_GET_INDEX(x) ((x & HLL_INDEX_MASK) >> (64 - HLL_INDEX_BITS))
#define HLL_GET_VALUE(x) (x & HLL_VALUE_MASK)

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

typedef struct hll_t {
  uint8_t registers[HLL_REGISTERS];
  double PE[64];
} hll_t;

hll_t *hll_init(void);
void hll_add(hll_t *h, char *data, size_t datalen);
void hll_print(hll_t *h);
void hll_test(hll_t *h);
uint64_t hll_count(hll_t *h);
