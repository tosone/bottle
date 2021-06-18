#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack_entry_t {
  struct stack_entry_t *next;
  void *data;
  size_t data_length;
} stack_entry_t;

typedef struct stack_t {
  stack_entry_t *head;
  size_t count;
} xstack_t;

xstack_t *stack_create();
void stack_free(xstack_t *stack);
size_t stack_size(xstack_t *stack);
void stack_push(xstack_t *stack, void *data, size_t data_length);
stack_entry_t *stack_pop(xstack_t *stack);
void stack_dump(xstack_t *stack, char *filename);
