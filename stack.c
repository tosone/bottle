#include <stack.h>

xstack_t *stack_create() {
  xstack_t *stack = (xstack_t *)malloc(sizeof(xstack_t));
  stack->count = 0;
  stack->head = NULL;
  return stack;
}

void stack_free_helper(stack_entry_t *entry) {
  if (entry->next != NULL) {
    stack_free_helper(entry->next);
  }
  free(entry->data);
  free(entry);
}

void stack_free(xstack_t *stack) {
  if (stack->head != NULL) {
    stack_free_helper(stack->head);
  }
  free(stack);
}

size_t stack_size(xstack_t *stack) {
  if (stack == NULL) {
    return 0;
  }
  return stack->count;
}

void stack_push(xstack_t *stack, void *data, size_t data_length) {
  stack_entry_t *entry = (stack_entry_t *)malloc(sizeof(stack_entry_t));
  entry->data = (void *)malloc(data_length);
  memcpy(entry->data, data, data_length);
  stack->count++;
  if (stack->head == NULL) {
    entry->next = NULL;
    stack->head = entry;
  } else {
    entry->next = stack->head;
    stack->head = entry;
  }
}

stack_entry_t *stack_pop(xstack_t *stack) {
  if (stack == NULL || stack->head == NULL) {
    return NULL;
  }
  if (stack->head->next == NULL) {
    stack_entry_t *entry = stack->head;
    stack->head = NULL;
    return entry;
  } else {
    stack_entry_t *entry = stack->head;
    stack->head = entry->next;
    entry->next = NULL;
    return entry;
  }
}

void stack_dump_helper(stack_entry_t *entry, FILE *stream, size_t index) {
  if (entry != NULL) {
    fprintf(stream, "node%zu [shape=record,label=\"{{%s}}\"];\n", index, (char *)entry->data);
  }
  if (entry->next != NULL) {
    size_t new_index = index + 1;
    fprintf(stream, "node%zu -> node%zu;\n", index, new_index);
    stack_dump_helper(entry->next, stream, new_index);
  }
}

void stack_dump(xstack_t *stack, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "graph {\n");
  if (stack->head != NULL) {
    stack_dump_helper(stack->head, stream, 0);
  }
  fprintf(stream, "}\n");
  fclose(stream);
}
