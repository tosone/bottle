#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sort_t {
  int64_t *data;
  size_t count;
} sort_t;

sort_t *sort_create();
void sort_free(sort_t *sort);
void sort_add(sort_t *sort, int64_t num);
void sort_remove(sort_t *sort, int64_t num);
sort_t *sort_duplicate(sort_t *sort);
void sort_test(sort_t *sort);

void sort_quick(sort_t *sort);
