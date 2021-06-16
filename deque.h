#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct deque_entry_t {
  struct deque_entry_t *prev;
  struct deque_entry_t *next;
  void *data;
  size_t data_length;
} deque_entry_t;

typedef struct deque_t {
  deque_entry_t *head;
  deque_entry_t *tail;
  size_t count;
} deque_t;

deque_t *deque_create();
void deque_free(deque_t *deque);
size_t deque_size(deque_t *deque);
void deque_push_back(deque_t *deque, void *data, size_t data_length);
deque_entry_t *deque_pop_back(deque_t *deque);
void deque_push_front(deque_t *deque, void *data, size_t data_length);
deque_entry_t *deque_pop_front(deque_t *deque);
void deque_dump(deque_t *deque, char *filename);
