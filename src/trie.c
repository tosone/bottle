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

void trie_free_helper(deque_t *entry) {
  deque_entry_t *deque_entry = entry->head;
  for (size_t i = 0; i < entry->count; i++) {
    trie_entry_t *inner = *(trie_entry_t **)deque_entry->data;
    trie_free_helper(inner->next);
  }
  deque_free(entry);
}

void trie_free(trie_t *trie) {
  trie_free_helper(trie->root->next);
  free(trie->root);
  free(trie);
}

void trie_add(trie_t *trie, char *str) {
  size_t length = strlen(str);

  if (length == 0) {
    return;
  }

  trie_entry_t *parent = trie->root;
  for (size_t i = 0; i < length; i++) {
    trie_entry_t *entry = (trie_entry_t *)malloc(sizeof(trie_entry_t));
    entry->c = str[i];
    entry->next = deque_create();
    deque_push_back(parent->next, &entry, sizeof(trie_entry_t *));
    parent = entry;
  }
  parent->end = true;
}

bool trie_delete(trie_t *trie, char *str) { return false; }

void trie_print_helper(deque_t *entry, char c) {
  deque_entry_t *deque_entry = entry->head;
  for (size_t i = 0; i < entry->count; i++) {
    if (c != 0) {
      printf("%c", c);
    }
    trie_entry_t *inner = *(trie_entry_t **)deque_entry->data;
    trie_print_helper(inner->next, inner->c);
    if (inner->end) {
      printf("%c ", inner->c);
    }
    deque_entry = deque_entry->next;
  }
}

void trie_print(trie_t *trie) {
  trie_print_helper(trie->root->next, 0);
  printf("\n");
}

void trie_dump(trie_t *trie) {}
void trie_test(trie_t *trie) {
  trie_add(trie, "test");
  trie_add(trie, "test1");
  trie_print(trie);
}
