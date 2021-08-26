#include <hll.h>

uint8_t hll_get_zeros(uint64_t value) {
  uint8_t i;
  uint64_t mask = 1;
  for (i = 0; i < HLL_VALUE_BITS; i++) {
    if ((mask & value) != 0)
      break;
    mask <<= 1;
  }

  return i;
}

hll_t *hll_init(void) {
  uint32_t i;
  hll_t *h;
  h = (hll_t *)calloc(1, sizeof(hll_t));
  if (!h) {
    return HLL_ERROR;
  }

  h->PE[0] = 1;
  for (i = 1; i < 64; i++) {
    h->PE[i] = 1.0 / (1ULL << i);
  }
  return h;
}

uint8_t hll_add(hll_t *h, char *data, size_t datalen) {
  uint64_t hash, value;
  uint16_t index;
  uint8_t zero;

  hash = murmurhash64((void *)data, datalen);
  index = (uint16_t)HLL_GET_INDEX(hash);
  value = HLL_GET_VALUE(hash);
  zero = hll_get_zeros(value);
  h->registers[index] = MAX(zero, h->registers[index]);
  return HLL_OK;
}

uint64_t hll_count(hll_t *h) {
  double m = HLL_REGISTERS;
  double alpha = 0.7213 / (1 + 1.079 / m);
  double E = 0;
  int32_t ez = 0; /* Number of registers equal to 0. */
  int32_t i;

  for (i = 0; i < HLL_REGISTERS; i++) {
    uint8_t reg = h->registers[i];
    if (reg == 0) {
      ez++;
      E += 1;
    } else {
      E += h->PE[reg];
    }
  }

  E = (1 / E) * alpha * m * m;

  if (E < m * 2.5 && ez != 0) {
    E = m * log(m / ez);
  } else if (m == 16384 && E < 72000) {
    double bias = 5.9119 * 1.0e-18 * (E * E * E * E) - 1.4253 * 1.0e-12 * (E * E * E) + 1.2940 * 1.0e-7 * (E * E) - 5.2921 * 1.0e-3 * E + 83.3216;
    E -= E * (bias / 100);
  }

  return (uint64_t)E * 2;
}

void hll_print(hll_t *h) {
  for (uint32_t i = 0; i < HLL_REGISTERS; i++) {
    if (i % 64 == 0) {
      if (i != 0) {
        printf("\n");
      }
      printf("%5d: ", i);
    }
    printf("%-3d", h->registers[i]);
  }
  printf("\n");
}

void hll_test(hll_t *h) {
  size_t test_size = 1000;
  printf("> hll add ....\n");
  for (size_t i = 0; i < test_size; i++) {
    char test[9];
    sprintf(test, "%8zu", i);
    hll_add(h, test, 9);
  }
  printf("> hll print\n");
  hll_print(h);
  printf("> hll count\n");
  printf("%" PRId64 "\n", hll_count(h));
}
