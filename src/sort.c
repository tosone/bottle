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

void sort_test(sort_t *sort) {
  sort_add(sort, 1);
  sort_add(sort, -1);
  sort_add(sort, 10);
  sort_add(sort, 8);
  sort_add(sort, 0);
  sort_remove(sort, 0);
  sort_quick(sort);
}

int cmpfunc(const void *a, const void *b) { return (*(int64_t *)b > *(int64_t *)a) ? 1 : 0; }

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

void sort_quick(sort_t *sort) {
  sort_t *new_sort = sort_duplicate(sort);
  qsort(new_sort->data, new_sort->count, sizeof(int64_t), cmpfunc);
  sort_print(new_sort);
  free(new_sort);
}
