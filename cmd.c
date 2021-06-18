#include <cmd.h>

#define ANSI_CODE_RESET "\033[00m"
#define ANSI_CODE_BOLD "\033[1m"
#define ANSI_CODE_DARK "\033[2m"
#define ANSI_CODE_UNDERLINE "\033[4m"
#define ANSI_CODE_BLINK "\033[5m"
#define ANSI_CODE_REVERSE "\033[7m"
#define ANSI_CODE_CONCEALED "\033[8m"
#define ANSI_CODE_GRAY "\033[30m"
#define ANSI_CODE_GREY "\033[30m"
#define ANSI_CODE_RED "\033[31m"
#define ANSI_CODE_GREEN "\033[32m"
#define ANSI_CODE_YELLOW "\033[33m"
#define ANSI_CODE_BLUE "\033[34m"
#define ANSI_CODE_MAGENTA "\033[35m"
#define ANSI_CODE_CYAN "\033[36m"
#define ANSI_CODE_WHITE "\033[37m"

hashmap_t *hmap;
LRU *lru;
avl_entry_t *avl;
skiplist *sklist;
rbtree_t *rbtree;
deque_t *deque;
xstack_t *stack;

void clear() {
  if (hmap != NULL) {
    hashmap_free(hmap);
  }
  if (lru != NULL) {
    LRU_free(lru);
  }
  if (avl != NULL) {
    avl_free(avl);
  }
  if (sklist != NULL) {
    slFree(sklist);
  }
  if (rbtree != NULL) {
    rbtree_free(rbtree);
  }
  if (stack != NULL) {
    stack_free(stack);
  }
  printf("\nclear all, bye\n");
}

bool command_stack(commands_t commands, int commands_length) {
  if (stack == NULL) {
    stack = stack_create();
  }

  if (strncasecmp(commands[1], COMMAND_STACK_PUSH, strlen(COMMAND_STACK_PUSH)) == 0) {
    command_length_check(!=, 3);
    char *data = commands[2];
    stack_push(stack, (void *)data, strlen(data) + 1);
  } else if (strncasecmp(commands[1], COMMAND_STACK_POP, strlen(COMMAND_STACK_POP)) == 0) {
    command_length_check(!=, 2);
    stack_entry_t *entry = stack_pop(stack);
    if (entry != NULL) {
      printf("%s\n", (char *)entry->data);
      free(entry->data);
      free(entry);
    } else {
      printf("stack is null\n");
    }
  } else if (strncasecmp(commands[1], COMMAND_STACK_DUMP, strlen(COMMAND_STACK_DUMP)) == 0) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    stack_dump(stack, filename);
  } else {
    return MAP_COMMANDS_ERROR;
  }
  return MAP_COMMANDS_OK;
}

bool command_deque(commands_t commands, int commands_length) {
  if (deque == NULL) {
    deque = deque_create();
  }
  command_length_check(<, 3);
  if (strncasecmp(commands[1], COMMAND_DEQUE_PUSH, strlen(COMMAND_DEQUE_PUSH)) == 0 && strncasecmp(commands[2], COMMAND_DEQUE_BACK, strlen(COMMAND_DEQUE_BACK)) == 0) {
    char *data = commands[3];
    deque_push_back(deque, (void *)data, strlen(data) + 1);
  } else if (strncasecmp(commands[1], COMMAND_DEQUE_POP, strlen(COMMAND_DEQUE_POP)) == 0 && strncasecmp(commands[2], COMMAND_DEQUE_BACK, strlen(COMMAND_DEQUE_BACK)) == 0) {
    deque_entry_t *entry = deque_pop_back(deque);
    if (entry != NULL) {
      printf("%s\n", (char *)entry->data);
      free(entry->data);
      free(entry);
    } else {
      printf("deque is null\n");
    }
  } else if (strncasecmp(commands[1], COMMAND_DEQUE_PUSH, strlen(COMMAND_DEQUE_PUSH)) == 0 && strncasecmp(commands[2], COMMAND_DEQUE_FRONT, strlen(COMMAND_DEQUE_FRONT)) == 0) {
    char *data = commands[3];
    deque_push_front(deque, (void *)data, strlen(data) + 1);
  } else if (strncasecmp(commands[1], COMMAND_DEQUE_POP, strlen(COMMAND_DEQUE_POP)) == 0 && strncasecmp(commands[2], COMMAND_DEQUE_FRONT, strlen(COMMAND_DEQUE_FRONT)) == 0) {
    deque_entry_t *entry = deque_pop_front(deque);
    if (entry != NULL) {
      printf("%s\n", (char *)entry->data);
      free(entry->data);
      free(entry);
    } else {
      printf("deque is null\n");
    }
  } else if (strncasecmp(commands[1], COMMAND_DEQUE_DUMP, strlen(COMMAND_DEQUE_DUMP)) == 0) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    deque_dump(deque, filename);
  } else {
    return MAP_COMMANDS_ERROR;
  }
  return MAP_COMMANDS_OK;
}

bool command_rbtree(commands_t commands, int commands_length) {
  if (rbtree == NULL) {
    rbtree = rbtree_create();
  }
  command_length_check(<, 3);
  if (strncasecmp(commands[1], COMMAND_RBTREE_SET, strlen(COMMAND_RBTREE_SET)) == 0) {
    command_length_check(!=, 4);
    char *key = commands[2];
    void *value = commands[3];
    if (rbtree_insert(rbtree, key, value, strlen(value) + 1) == NULL) {
      return MAP_COMMANDS_ERROR;
    }
  } else if (strncasecmp(commands[1], COMMAND_RBTREE_DUMP, strlen(COMMAND_RBTREE_DUMP)) == 0) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    rbtree_dump(rbtree, filename);
  } else if (strncasecmp(commands[1], COMMAND_RBTREE_DEL, strlen(COMMAND_RBTREE_DEL)) == 0) {
    command_length_check(!=, 3);
    rbtree_entry_t *entry = rbtree_delete(rbtree, commands[2]);
    free(entry->key);
    free(entry->value);
    free(entry);
  } else {
    return MAP_COMMANDS_ERROR;
  }
  return MAP_COMMANDS_OK;
}

bool command_sklist(commands_t commands, int commands_length) {
  if (hmap == NULL) {
    hmap = hashmap_create();
  }
  command_length_check(<, 3);
  if (strncasecmp(commands[1], COMMAND_SKLIST_SET, strlen(COMMAND_SKLIST_SET)) == 0) {
    command_length_check(!=, 4);
    double score = strtod(commands[2], NULL);
    sds ele = sdsnew(commands[3]);
    slInsert(sklist, score, ele);
  } else if (strncasecmp(commands[1], COMMAND_SKLIST_GET, strlen(COMMAND_SKLIST_GET)) == 0) {
    command_length_check(!=, 4);
    double score = strtod(commands[2], NULL);
    sds ele = sdsnew(commands[3]);
    unsigned long rank = slGetRank(sklist, score, ele);
    sdsfree(ele);
    skiplistNode *node = sklist->header->level->forward;
    while (node != NULL) {
      printf("%s\n", node->ele);
      node = node->level->forward;
    }
    if (rank == 0) {
      printf("cannot find this element\n");
    } else {
      printf("%lu\n", rank);
    }
  } else if (strncasecmp(commands[1], COMMAND_SKLIST_DEL, strlen(COMMAND_SKLIST_DEL)) == 0) {
    command_length_check(!=, 4);
    double score = strtod(commands[2], NULL);
    sds ele = sdsnew(commands[3]);
    int ret = slDelete(sklist, score, ele, NULL);
    sdsfree(ele);
    if (ret == 0) {
      printf("cannot find this element");
    }
  }
  return MAP_COMMANDS_OK;
}

bool command_avl(commands_t commands, int commands_length) {
  command_length_check(<, 3);
  if (strncasecmp(commands[1], COMMAND_AVL_SET, strlen(COMMAND_AVL_SET)) == 0) {
    command_length_check(!=, 3);
    int key = atoi(commands[2]);
    avl = avl_set(avl, key);
  } else if (strncasecmp(commands[1], COMMAND_AVL_GET, strlen(COMMAND_AVL_GET)) == 0) {
    command_length_check(!=, 3);
    int key = atoi(commands[2]);
    printf("%s\n", avl_get(avl, key) ? "true" : "false");
  } else if (strncasecmp(commands[1], COMMAND_AVL_PRINT, strlen(COMMAND_AVL_PRINT)) == 0) {
    command_length_check(!=, 3);
    if (strncasecmp(commands[2], COMMAND_AVL_PRE, strlen(COMMAND_AVL_PRE)) == 0) {
      avl_pre_order(avl);
    } else if (strncasecmp(commands[2], COMMAND_AVL_IN, strlen(COMMAND_AVL_IN)) == 0) {
      avl_in_order(avl);
    } else if (strncasecmp(commands[2], COMMAND_AVL_POST, strlen(COMMAND_AVL_POST)) == 0) {
      avl_post_order(avl);
    }
  } else if (strncasecmp(commands[1], COMMAND_AVL_DUMP, strlen(COMMAND_AVL_DUMP)) == 0) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    avl_dump(avl, filename);
  }
  return MAP_COMMANDS_OK;
}

bool command_lru(commands_t commands, int commands_length) {
  if (lru == NULL) {
    lru = LRU_create();
  }
  command_length_check(<, 2);
  if (strncasecmp(commands[1], COMMAND_LRU_LEN, strlen(COMMAND_LRU_LEN)) == 0) {
    command_length_check(!=, 2);
    printf("%d\n", lru->len);
  } else if (strncasecmp(commands[1], COMMAND_LRU_SET, strlen(COMMAND_LRU_SET)) == 0) {
    command_length_check(!=, 4);
    char *key = commands[2];
    void *value = commands[3];
    LRU_set(lru, key, value, strlen(value) + 1);
  } else if (strncasecmp(commands[1], COMMAND_LRU_GET, strlen(COMMAND_LRU_GET)) == 0) {
    command_length_check(!=, 3);
    char *key = commands[2];
    size_t value_length = 0;
    char *value = (char *)LRU_get(lru, key, &value_length);
    if (value == NULL) {
      printf("key not found\n");
    } else {
      printf("%s\n", value);
    }
  } else if (strncasecmp(commands[1], COMMAND_LRU_CAP, strlen(COMMAND_LRU_CAP)) == 0) {
    command_length_check(<, 3);
    if (strncasecmp(commands[2], COMMAND_LRU_GET, strlen(COMMAND_LRU_GET)) == 0) {
      printf("%d\n", lru->cap);
    } else if (strncasecmp(commands[2], COMMAND_LRU_SET, strlen(COMMAND_LRU_SET)) == 0) {
      int cap = atoi(commands[3]);
      if (cap < lru->cap) {
        printf("please set more bigger cap\n");
      } else {
        lru->cap = cap;
      }
    }
  } else if (strncasecmp(commands[1], COMMAND_LRU_PRINT, strlen(COMMAND_LRU_PRINT)) == 0) {
    command_length_check(!=, 2);
    LRU_print(lru);
  }
  return MAP_COMMANDS_OK;
}

bool command_hmap(commands_t commands, int commands_length) {
  if (sklist == NULL) {
    sklist = slCreate();
  }
  command_length_check(<, 2);
  if (strncasecmp(commands[1], COMMAND_HMAP_CAP, strlen(COMMAND_HMAP_CAP)) == 0) {
    command_length_check(!=, 2);
    printf("%d\n", hmap->cap);
  } else if (strncasecmp(commands[1], COMMAND_HMAP_LEN, strlen(COMMAND_HMAP_LEN)) == 0) {
    command_length_check(!=, 2);
    printf("%d\n", hmap->len);
  } else if (strncasecmp(commands[1], COMMAND_HMAP_SET, strlen(COMMAND_HMAP_SET)) == 0) {
    command_length_check(!=, 4);
    char *key = commands[2];
    void *value = commands[3];
    hmap = hashmap_set(hmap, key, value, strlen(value) + 1);
  } else if (strncasecmp(commands[1], COMMAND_HMAP_GET, strlen(COMMAND_HMAP_GET)) == 0) {
    command_length_check(!=, 3);
    char *key = commands[2];
    size_t value_length = 0;
    char *value = (char *)hashmap_get(hmap, key, &value_length);
    if (value == NULL) {
      printf("key not found\n");
    } else {
      printf("%s\n", value);
    }
  } else if (strncasecmp(commands[1], COMMAND_HMAP_DEL, strlen(COMMAND_HMAP_DEL)) == 0) {
    command_length_check(!=, 3);
    char *key = commands[2];
    hashmap_del(hmap, key);
  } else if (strncasecmp(commands[1], COMMAND_HMAP_PRINT, strlen(COMMAND_HMAP_PRINT)) == 0) {
    command_length_check(!=, 2);
    hashmap_print(hmap);
  }
  return MAP_COMMANDS_OK;
}

bool command_pi(commands_t commands, int commands_length) {
  int length = 100;
  if (commands_length == 2) {
    length = atoi(commands[1]);
  }
  if (length < 100) {
    length = 100;
  }
  pi(length);
  return MAP_COMMANDS_OK;
}

bool command_help(commands_t commands, int commands_length) {
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "help");
  printf(ANSI_CODE_YELLOW "\n\tprint help information" ANSI_CODE_RESET "\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "exit");
  printf(ANSI_CODE_YELLOW "\n\texit the application" ANSI_CODE_RESET "\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "version");
  printf(ANSI_CODE_YELLOW "\n\tprint version" ANSI_CODE_RESET "\n\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "hmap");
  printf(" <set> <key> <value>\n");
  printf(ANSI_CODE_YELLOW "\tadd new key and value to hashmap" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "hmap");
  printf(" <get/del> <key>\n");
  printf(ANSI_CODE_YELLOW "\tget/delete the specific key from hashmap" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "hmap");
  printf(" <cap/len>\n");
  printf(ANSI_CODE_YELLOW "\tget the cap/len from hashmap" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "hmap");
  printf(" <print>\n");
  printf(ANSI_CODE_YELLOW "\tprint all of keys in the hashmap" ANSI_CODE_RESET "\n\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "lru");
  printf(" <set> <key> <value>\n");
  printf(ANSI_CODE_YELLOW "\tadd new key and value to lru" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "lru");
  printf(" <get> <key>\n");
  printf(ANSI_CODE_YELLOW "\tget the specific key from lru" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "lru");
  printf(" <cap/len>\n");
  printf(ANSI_CODE_YELLOW "\tget the cap/len from lru" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "lru");
  printf(" <print>\n");
  printf(ANSI_CODE_YELLOW "\tprint all of keys in the lru" ANSI_CODE_RESET "\n\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "avl");
  printf(" <set> <key> <value>\n");
  printf(ANSI_CODE_YELLOW "\tadd new key and value to avl" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "avl");
  printf(" <get> <key>\n");
  printf(ANSI_CODE_YELLOW "\tget the specific key from avl" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "avl");
  printf(" <print>\n");
  printf(ANSI_CODE_YELLOW "\tprint all of keys in the avl" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "avl");
  printf(" <in/pre/post>\n");
  printf(ANSI_CODE_YELLOW "\ttraverse the avl with specific method" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "avl");
  printf(" <dump> <filename>\n");
  printf(ANSI_CODE_YELLOW "\tdump the avl to dot file" ANSI_CODE_RESET "\n\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "sklist");
  printf(" <set> <key> <value>\n");
  printf(ANSI_CODE_YELLOW "\tadd new key and value to sklist" ANSI_CODE_RESET "\n");
  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "sklist");
  printf(" <get/del> <key>\n");
  printf(ANSI_CODE_YELLOW "\tget/delete the specific key from sklist" ANSI_CODE_RESET "\n\n");

  printf(ANSI_CODE_GREEN "%s" ANSI_CODE_RESET, "pi");
  printf(" <length>\n");
  printf(ANSI_CODE_YELLOW "\tcalculate the pi to specific length" ANSI_CODE_RESET "\n\n");

  return MAP_COMMANDS_OK;
}
