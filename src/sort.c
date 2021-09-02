#include <sort.h>

sort_t *sort_create() {
  sort_t *sort = (sort_t *)malloc(sizeof(sort_t));
  sort->count = 0;
  sort->data = NULL;
  return sort;
}

void sort_free(sort_t *sort) {
  free(sort->data);
  free(sort);
}

void sort_add(sort_t *sort, int64_t num) {
  sort->count++;
  if (sort->data == NULL) {
    sort->data = (int64_t *)malloc(sizeof(int64_t) * sort->count);
  } else {
    sort->data = (int64_t *)realloc(sort->data, sizeof(int64_t) * sort->count);
  }
  sort->data[sort->count - 1] = num;
}

void sort_remove(sort_t *sort, int64_t num) {
  int64_t *new_data = NULL;
  size_t new_count = 0;

  bool found = false;
  for (size_t i = 0; i < sort->count; i++) {
    if (found == false && sort->data[i] == num) {
      found = true;
    } else {
      new_count++;
      if (new_data == NULL) {
        new_data = (int64_t *)malloc(new_count * sizeof(int64_t));
      } else {
        new_data = (int64_t *)realloc(new_data, new_count * sizeof(int64_t));
      }
      new_data[new_count - 1] = sort->data[i];
    }
  }
  free(sort->data);
  sort->data = new_data;
  sort->count = new_count;
}

sort_t *sort_duplicate(sort_t *sort) {
  sort_t *new_sort = (sort_t *)malloc(sizeof(sort_t));
  new_sort->count = sort->count;
  new_sort->data = (int64_t *)malloc(sizeof(int64_t) * sort->count);
  memcpy(new_sort->data, sort->data, sizeof(int64_t) * sort->count);
  return new_sort;
}

void sort_print(sort_t *sort) {
  bool first = true;
  for (size_t i = 0; i < sort->count; i++) {
    if (first) {
      first = false;
      printf("%" PRId64, sort->data[i]);
    } else {
      printf(" %" PRId64, sort->data[i]);
    }
  }
  printf("\n");
}

int cmpfunc(const void *a, const void *b) {
  int64_t arg1 = *(const int64_t *)a;
  int64_t arg2 = *(const int64_t *)b;
  if (arg1 < arg2) {
    return 1;
  } else if (arg1 > arg2) {
    return -1;
  } else {
    return 0;
  }
}

sort_t *sort_quick(sort_t *sort) {
  sort_t *new_sort = sort_duplicate(sort);
  qsort(new_sort->data, new_sort->count, sizeof(int64_t), cmpfunc);
  return new_sort;
}

void sort_test(sort_t *sort) {
  for (int64_t i = 0; i < 10; i++) {
    sort_add(sort, i);
  }

  int64_t target1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  CHECK_BUF_EQ(sort->data, target1, sort->count * sizeof(int64_t));

  sort_add(sort, 100);
  int64_t target2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100};
  CHECK_BUF_EQ(sort->data, target2, sort->count * sizeof(int64_t));
  CHECK_EQ(sort->count, 11);

  sort_remove(sort, 100);
  CHECK_BUF_EQ(sort->data, target1, sort->count * sizeof(int64_t));
  CHECK_EQ(sort->count, 10);

  sort_t *new_sort_quick = sort_quick(sort);
  int64_t target_quick[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  CHECK_BUF_EQ(new_sort_quick->data, target_quick, sort->count * sizeof(int64_t));
}
