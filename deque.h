#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmddefine.h>

#pragma once

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

  void *user_data;
  size_t user_data_length;
} deque_t;

deque_t *deque_create();
deque_t *deque_duplicate(deque_t *deque);
void deque_free(deque_t *deque);
size_t deque_size(deque_t *deque);
void deque_push_back(deque_t *deque, void *data, size_t data_length);
deque_entry_t *deque_pop_back(deque_t *deque);
void deque_push_front(deque_t *deque, void *data, size_t data_length);
deque_entry_t *deque_pop_front(deque_t *deque);
void deque_dump(deque_t *deque, char *filename);
deque_entry_t *deque_entry_get(deque_t *deque, bool (*callback)(deque_t *deque, void *data, size_t data_length));
bool deque_entry_delete(deque_t *deque, bool (*callback)(deque_t *deque, void *data, size_t data_length));
void deque_test(deque_t *deque);
void deque_debug(deque_t *deque);
