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
bloom_t *bloom;
dag_t *dag;
bittree_t *bittree;

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
  if (bloom != NULL) {
    bloom_free(bloom);
  }
  if (dag != NULL) {
    dag_free(dag);
  }
  printf("\nclear all, bye\n");
}

bottle_error_t cli_bloom(commands_t commands, int commands_length) {
  if (bloom == NULL) {
    bloom = bloom_create();
  }

  if (strequal(commands[1], COMMAND_BLOOM_PUSH)) {
    command_length_check(!=, 3);
    char *data = commands[2];
    bloom_push(bloom, (void *)data, strlen(data) + 1);
  } else if (strequal(commands[1], COMMAND_BLOOM_CHECK)) {
    command_length_check(!=, 3);
    char *data = commands[2];
    if (bloom_check(bloom, (void *)data, strlen(data) + 1)) {
      printf("key is exist\n");
    } else {
      printf("key is not exist\n");
    }
  } else {
    return error_invalid_command;
  }
  return bottle_ok;
}

bottle_error_t cli_bittree(commands_t commands, int commands_length) {
  if (bittree == NULL) {
    bittree = bittree_create();
  }
  if (strequal(commands[1], COMMAND_BITTREE_TEST)) {
    command_length_check(!=, 2);
    bittree_test(bittree);
  }
  return bottle_ok;
}

bottle_error_t cli_trie(commands_t commands, int commands_length) { return bottle_ok; }

bottle_error_t cli_dag(commands_t commands, int commands_length) {
  if (dag == NULL) {
    dag = dag_create();
  }

  if (strequal(commands[1], COMMAND_DAG_VERTEX)) {
    if (strequal(commands[2], COMMAND_DAG_VERTEX_ADD)) {
      command_length_check(!=, 5);
      char *key = commands[3];
      void *value = commands[4];
      dag_entry_add(dag, key, value, strlen(value) + 1);
    } else if (strequal(commands[2], COMMAND_DAG_VERTEX_GET)) {
      command_length_check(!=, 4);
      char *key = commands[3];
      dag_vertex_t *vertex = dag_entry_get(dag, key);
      if (vertex == NULL) {
        printf("key not found\n");
      } else {
        printf("%s\n", (char *)vertex->value);
      }
    } else if (strequal(commands[2], COMMAND_DAG_VERTEX_DEL)) {
      command_length_check(!=, 4);
      char *key = commands[3];
      bool success = dag_entry_del(dag, key);
      if (!success) {
        printf("key not found\n");
      } else {
        printf("key has been deleted\n");
      }
    } else {
      return error_invalid_command;
    }
  } else if (strequal(commands[1], COMMAND_DAG_EDGE)) {
    if (strequal(commands[2], COMMAND_DAG_EDGE_ADD)) {
      command_length_check(!=, 5);
      char *key_from = commands[3];
      void *key_to = commands[4];
      dag_edge_add(dag, key_from, key_to);
    } else if (strequal(commands[2], COMMAND_DAG_EDGE_DEL)) {
      command_length_check(!=, 5);
      char *key_from = commands[3];
      void *key_to = commands[4];
      dag_edge_delete(dag, key_from, key_to);
    } else {
      return error_invalid_command;
    }
  } else if (strequal(commands[1], COMMAND_DAG_PRINT)) {
    dag_print(dag);
  } else if (strequal(commands[1], COMMAND_DAG_TEST)) {
    dag_test(dag);
  } else if (strequal(commands[1], COMMAND_DAG_DUMP)) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    dag_dump(dag, filename);
  } else {
    return error_invalid_command;
  }
  return bottle_ok;
}

bottle_error_t cli_stack(commands_t commands, int commands_length) {
  if (stack == NULL) {
    stack = stack_create();
  }

  if (strequal(commands[1], COMMAND_STACK_PUSH)) {
    command_length_check(!=, 3);
    char *data = commands[2];
    stack_push(stack, (void *)data, strlen(data) + 1);
  } else if (strequal(commands[1], COMMAND_STACK_POP)) {
    command_length_check(!=, 2);
    stack_entry_t *entry = stack_pop(stack);
    if (entry != NULL) {
      printf("%s\n", (char *)entry->data);
      free(entry->data);
      free(entry);
    } else {
      printf("stack is null\n");
    }
  } else if (strequal(commands[1], COMMAND_STACK_DUMP)) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    stack_dump(stack, filename);
  } else {
    return error_invalid_command;
  }
  return bottle_ok;
}

bottle_error_t cli_deque(commands_t commands, int commands_length) {
  if (deque == NULL) {
    deque = deque_create();
  }
  if (strequal(commands[1], COMMAND_DEQUE_PUSH)) {
    command_length_check(!=, 4);
    if (strequal(commands[2], COMMAND_DEQUE_BACK)) {
      char *data = commands[3];
      deque_push_back(deque, (void *)data, strlen(data) + 1);
    } else if (strequal(commands[2], COMMAND_DEQUE_FRONT)) {
      char *data = commands[3];
      deque_push_front(deque, (void *)data, strlen(data) + 1);
    } else {
      return error_invalid_command;
    }
  } else if (strequal(commands[1], COMMAND_DEQUE_POP)) {
    command_length_check(!=, 3);
    if (strequal(commands[2], COMMAND_DEQUE_BACK)) {
      deque_entry_t *entry = deque_pop_back(deque);
      if (entry != NULL) {
        printf("%s\n", (char *)entry->data);
        free(entry->data);
        free(entry);
      } else {
        printf("deque is null\n");
      }
    } else if (strequal(commands[2], COMMAND_DEQUE_FRONT)) {
      deque_entry_t *entry = deque_pop_front(deque);
      if (entry != NULL) {
        printf("%s\n", (char *)entry->data);
        free(entry->data);
        free(entry);
      } else {
        printf("deque is null\n");
      }
    } else {
      return error_invalid_command;
    }
  } else if (strequal(commands[1], COMMAND_DEQUE_DUMP)) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    deque_dump(deque, filename);
  } else if (strequal(commands[1], COMMAND_DEQUE_TEST)) {
    command_length_check(!=, 2);
    deque_test(deque);
  } else {
    return error_invalid_command;
  }
  return bottle_ok;
}

bottle_error_t cli_rbtree(commands_t commands, int commands_length) {
  if (rbtree == NULL) {
    rbtree = rbtree_create();
  }
  command_length_check(<, 3);
  if (strequal(commands[1], COMMAND_RBTREE_SET)) {
    command_length_check(!=, 4);
    char *key = commands[2];
    void *value = commands[3];
    if (rbtree_insert(rbtree, key, value, strlen(value) + 1) == NULL) {
      return error_invalid_command;
    }
  } else if (strequal(commands[1], COMMAND_RBTREE_DUMP)) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    rbtree_dump(rbtree, filename);
  } else if (strequal(commands[1], COMMAND_RBTREE_DEL)) {
    command_length_check(!=, 3);
    rbtree_entry_t *entry = rbtree_delete(rbtree, commands[2]);
    free(entry->key);
    free(entry->value);
    free(entry);
  } else {
    return error_invalid_command;
  }
  return bottle_ok;
}

bottle_error_t cli_sklist(commands_t commands, int commands_length) {
  if (hmap == NULL) {
    hmap = hashmap_create();
  }
  command_length_check(<, 3);
  if (strequal(commands[1], COMMAND_SKLIST_SET)) {
    command_length_check(!=, 4);
    double score = strtod(commands[2], NULL);
    sds ele = sdsnew(commands[3]);
    slInsert(sklist, score, ele);
  } else if (strequal(commands[1], COMMAND_SKLIST_GET)) {
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
  } else if (strequal(commands[1], COMMAND_SKLIST_DEL)) {
    command_length_check(!=, 4);
    double score = strtod(commands[2], NULL);
    sds ele = sdsnew(commands[3]);
    int ret = slDelete(sklist, score, ele, NULL);
    sdsfree(ele);
    if (ret == 0) {
      printf("cannot find this element");
    }
  }
  return bottle_ok;
}

bottle_error_t cli_avl(commands_t commands, int commands_length) {
  command_length_check(<, 3);
  if (strequal(commands[1], COMMAND_AVL_SET)) {
    command_length_check(!=, 3);
    int key = atoi(commands[2]);
    avl = avl_set(avl, key);
  } else if (strequal(commands[1], COMMAND_AVL_GET)) {
    command_length_check(!=, 3);
    int key = atoi(commands[2]);
    printf("%s\n", avl_get(avl, key) ? "true" : "false");
  } else if (strequal(commands[1], COMMAND_AVL_PRINT)) {
    command_length_check(!=, 3);
    if (strequal(commands[2], COMMAND_AVL_PRE)) {
      avl_pre_order(avl);
    } else if (strequal(commands[2], COMMAND_AVL_IN)) {
      avl_in_order(avl);
    } else if (strequal(commands[2], COMMAND_AVL_POST)) {
      avl_post_order(avl);
    }
  } else if (strequal(commands[1], COMMAND_AVL_DUMP)) {
    command_length_check(!=, 3);
    char *filename = commands[2];
    avl_dump(avl, filename);
  }
  return bottle_ok;
}

bottle_error_t cli_lru(commands_t commands, int commands_length) {
  if (lru == NULL) {
    lru = LRU_create();
  }
  command_length_check(<, 2);
  if (strequal(commands[1], COMMAND_LRU_LEN)) {
    command_length_check(!=, 2);
    printf("%d\n", lru->len);
  } else if (strequal(commands[1], COMMAND_LRU_SET)) {
    command_length_check(!=, 4);
    char *key = commands[2];
    void *value = commands[3];
    LRU_set(lru, key, value, strlen(value) + 1);
  } else if (strequal(commands[1], COMMAND_LRU_GET)) {
    command_length_check(!=, 3);
    char *key = commands[2];
    size_t value_length = 0;
    char *value = (char *)LRU_get(lru, key, &value_length);
    if (value == NULL) {
      printf("key not found\n");
    } else {
      printf("%s\n", value);
    }
  } else if (strequal(commands[1], COMMAND_LRU_CAP)) {
    command_length_check(<, 3);
    if (strequal(commands[2], COMMAND_LRU_GET)) {
      printf("%d\n", lru->cap);
    } else if (strequal(commands[2], COMMAND_LRU_SET)) {
      int cap = atoi(commands[3]);
      if (cap < lru->cap) {
        printf("please set more bigger cap\n");
      } else {
        lru->cap = cap;
      }
    }
  } else if (strequal(commands[1], COMMAND_LRU_PRINT)) {
    command_length_check(!=, 2);
    LRU_print(lru);
  }
  return bottle_ok;
}

bottle_error_t cli_hmap(commands_t commands, int commands_length) {
  if (sklist == NULL) {
    sklist = slCreate();
  }
  command_length_check(<, 2);
  if (strequal(commands[1], COMMAND_HMAP_CAP)) {
    command_length_check(!=, 2);
    printf("%d\n", hmap->cap);
  } else if (strequal(commands[1], COMMAND_HMAP_LEN)) {
    command_length_check(!=, 2);
    printf("%d\n", hmap->len);
  } else if (strequal(commands[1], COMMAND_HMAP_SET)) {
    command_length_check(!=, 4);
    char *key = commands[2];
    void *value = commands[3];
    hmap = hashmap_set(hmap, key, value, strlen(value) + 1);
  } else if (strequal(commands[1], COMMAND_HMAP_GET)) {
    command_length_check(!=, 3);
    char *key = commands[2];
    size_t value_length = 0;
    char *value = (char *)hashmap_get(hmap, key, &value_length);
    if (value == NULL) {
      printf("key not found\n");
    } else {
      printf("%s\n", value);
    }
  } else if (strequal(commands[1], COMMAND_HMAP_DEL)) {
    command_length_check(!=, 3);
    char *key = commands[2];
    hashmap_del(hmap, key);
  } else if (strequal(commands[1], COMMAND_HMAP_PRINT)) {
    command_length_check(!=, 2);
    hashmap_print(hmap);
  }
  return bottle_ok;
}

bottle_error_t cli_pi(commands_t commands, int commands_length) {
  int length = 100;
  if (commands_length == 2) {
    length = atoi(commands[1]);
  }
  if (length < 100) {
    length = 100;
  }
  pi(length);
  return bottle_ok;
}

bottle_error_t cli_help(commands_t commands, int commands_length) {
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

  return bottle_ok;
}
