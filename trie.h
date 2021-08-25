#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <deque.h>

#define trie_max_length 100

typedef struct trie_entry_t {
  char c;
  bool end;
  deque_t *next;
} trie_entry_t;

typedef struct trie_t {
  trie_entry_t *root;
  size_t count;
} trie_t;

trie_t *trie_create();
void trie_free(trie_t *trie);
void trie_add(trie_t *trie, char str);
bool trie_delete(trie_t *trie, char str);
void trie_print(trie_t *trie_t);
void trie_dump(trie_t *trie_t);
void trie_test(trie_t *trie_t);
