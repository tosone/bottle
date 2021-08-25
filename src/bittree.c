#include <bittree.h>

bittree_t *bittree_create() {
  bittree_t *tree = (bittree_t *)malloc(sizeof(bittree_t));
  tree->size = 0;
  tree->bitarray = NULL;
  return tree;
}

int getSum(bittree_t *tree, int index) {
  index = index + 1;
  int sum = 0;
  while (index > 0) {
    sum += tree->bitarray[index];
    index = index - (index & -index);
  }
  return sum;
}

int64_t bittree_range_sum(bittree_t *tree, size_t i, size_t j) {
  int64_t sum = 0;
  for (; j > i; j -= LSB(j)) {
    sum += tree->bitarray[j - 1];
  }
  for (; i > j; i -= LSB(i)) {
    sum -= tree->bitarray[i - 1];
  }
  return sum;
}

int64_t bittree_get(bittree_t *tree, size_t i) { return bittree_range_sum(tree, i, i + 1); }

int64_t bittree_pre_sum(bittree_t *tree, size_t num) {
  int64_t ans = 0;
  for (size_t i = 1; i < num; i++) {
    if (i + LSB(i) == num) {
      ans += tree->bitarray[i - 1];
    }
  }
  return ans;
}

void bittree_add(bittree_t *tree, int64_t num) {
  tree->size++;
  if (tree->bitarray == NULL) {
    tree->bitarray = (int64_t *)malloc(tree->size * sizeof(int64_t));
  } else {
    tree->bitarray = (int64_t *)realloc(tree->bitarray, tree->size * sizeof(int64_t));
  }
  size_t index = tree->size - 1;
  tree->bitarray[index] = bittree_pre_sum(tree, index + 1) + num;
}

void bittree_delete(bittree_t *tree, int64_t index) {}

void bittree_print(bittree_t *tree) {
  bool first = true;
  for (size_t i = 0; i < tree->size; i++) {
    if (first) {
      printf("%" PRId64, tree->bitarray[i]);
      first = false;
    } else {
      printf(" %" PRId64, tree->bitarray[i]);
    }
  }
  printf("\n");
}

void bittree_dump(bittree_t *tree, char *filename) {
  if (tree == NULL) {
    printf("bittree is null");
  }
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  for (size_t i = 0; i < tree->size; i++) {
    fprintf(stream, "\tnode%zu -> node%zu\n", i + 1, i + 1 + LSB(i + 1));
  }
  for (size_t i = 0; i < tree->size; i++) {
    fprintf(stream, "\tnode%zu[shape=record,label=\"{{%" PRId64 "}}\"];\n", i + 1, bittree_get(tree, i));
  }
  fprintf(stream, "}\n");
  fclose(stream);
}

void bittree_test(bittree_t *tree) {
  int64_t num1 = 1;
  printf("> bittree add %" PRId64 "\n", num1);
  bittree_add(tree, num1);
  int64_t num2 = 2;
  printf("> bittree add %" PRId64 "\n", num2);
  bittree_add(tree, num2);
  int64_t num3 = 3;
  printf("> bittree add %" PRId64 "\n", num3);
  bittree_add(tree, num3);
  int64_t num4 = 4;
  printf("> bittree add %" PRId64 "\n", num4);
  bittree_add(tree, num4);
  printf("> bittree print\n");
  bittree_print(tree);
  assert(bittree_get(tree, 0) == 1);
  assert(bittree_get(tree, 1) == 2);
  assert(bittree_get(tree, 2) == 3);
  assert(bittree_get(tree, 3) == 4);
  bittree_dump(tree, "bittree.dot");
}
