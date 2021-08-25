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

void trie_free(trie_t *trie) {}
void trie_add(trie_t *trie, char str) {}
bool trie_delete(trie_t *trie, char str) { return false; }
void trie_print(trie_t *trie_t) {}
void trie_dump(trie_t *trie_t) {}
void trie_test(trie_t *trie_t) {}
