#include <trie.h>

trie_t *trie_create() {
  trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
  trie->count = 0;
  trie->root = (trie_entry_t *)malloc(sizeof(trie_entry_t));
  trie->root->c = '\0';
  trie->root->end = false;
  trie->root->next = deque_create();
  return trie;
}

void trie_free_helper(deque_t *children) {
  deque_entry_t *child = children->head;
  for (size_t i = 0; i < children->count; i++) {
    trie_entry_t *inner = *(trie_entry_t **)child->data;
    trie_free_helper(inner->next);
  }
  deque_free(children);
}

void trie_free(trie_t *trie) {
  trie_free_helper(trie->root->next);
  free(trie->root);
  free(trie);
}

trie_entry_t *trie_add_helper(deque_t *children, char c) {
  deque_entry_t *child = children->head;
  for (size_t i = 0; i < children->count; i++) {
    trie_entry_t *inner = *(trie_entry_t **)child->data;
    if (inner->c == c) {
      return inner;
    }
    child = child->next;
  }
  trie_entry_t *entry = (trie_entry_t *)malloc(sizeof(trie_entry_t));
  entry->next = NULL;
  entry->str = NULL;
  return entry;
}

void trie_add(trie_t *trie, char *str) {
  size_t length = strlen(str);

  if (length == 0) {
    return;
  }

  trie_entry_t *parent = trie->root;
  for (size_t i = 0; i < length; i++) {
    trie_entry_t *entry = trie_add_helper(parent->next, str[i]);
    entry->c = str[i];
    if (entry->next == NULL) {
      entry->next = deque_create();
      deque_push_back(parent->next, &entry, sizeof(trie_entry_t *));
    }
    parent = entry;
  }
  if (parent->str == NULL) {
    parent->str = (char *)malloc(strlen(str) + 1);
    memcpy(parent->str, str, strlen(str) + 1);
  }
  parent->end = true;
}

bool trie_delete(trie_t *trie, char *str) { return false; }

void trie_print_helper(deque_t *children) {
  deque_entry_t *child = children->head;
  for (size_t i = 0; i < children->count; i++) {
    trie_entry_t *inner = *(trie_entry_t **)child->data;
    trie_print_helper(inner->next);
    if (inner->end) {
      printf("%s ", inner->str);
    }
    child = child->next;
  }
}

void trie_print(trie_t *trie) {
  trie_print_helper(trie->root->next);
  printf("\n");
}

void trie_dump_helper(FILE *stream, deque_t *children, char c, size_t index) {
  index++;
  deque_entry_t *child = children->head;
  for (size_t i = 0; i < children->count; i++) {
    trie_entry_t *inner = *(trie_entry_t **)child->data;
    if (c != 0) {
      fprintf(stream, "\tnode%zu%c -> node%zu%c\n", index - 1, c, index, inner->c);
    }
    trie_dump_helper(stream, inner->next, inner->c, index);
    if (inner->end) {
      fprintf(stream, "\tnode%zu%c[shape=record,label=\"{{%s}}\"];\n", index, inner->c, inner->str);
    } else {
      fprintf(stream, "\tnode%zu%c[shape=record,label=\"{{%c}}\"];\n", index, inner->c, inner->c);
    }
    child = child->next;
  }
}

void trie_dump(trie_t *trie, char *filename) {
  FILE *stream = fopen(filename, "w+");
  fprintf(stream, "digraph {\n");
  trie_dump_helper(stream, trie->root->next, 0, 0);
  fprintf(stream, "}\n");
  fclose(stream);
}

void trie_test(trie_t *trie) {
  char str1[] = "test1";
  printf("> trie add %s\n", str1);
  trie_add(trie, str1);
  char str2[] = "test2";
  printf("> trie add %s\n", str2);
  trie_add(trie, str2);
  char str3[] = "test3";
  printf("> trie add %s\n", str3);
  trie_add(trie, str3);
  char str4[] = "test";
  printf("> trie add %s\n", str4);
  trie_add(trie, str4);
  printf("> trie print\n");
  trie_print(trie);

  printf("> trie dump\n");
  trie_dump(trie, "trie.dot");
}
