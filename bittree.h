#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LSB(i) ((i) & -(i))

typedef struct bittree_t {
  int64_t *bitarray; // binary index array
  size_t size;
} bittree_t;

bittree_t *bittree_create();
void bittree_add(bittree_t *tree, int64_t num);
void bittree_delete(bittree_t *tree, int64_t index);
void bittree_print(bittree_t *tree);
void bittree_dump(bittree_t *tree, char *filename);
void bittree_test(bittree_t *tree);
void bittree_free();
