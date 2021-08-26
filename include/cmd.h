#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include <error.h>

#include <cmddefine.h>

#include <avl.h>
#include <bittree.h>
#include <bloom.h>
#include <command.h>
#include <dag.h>
#include <deque.h>
#include <hashmap.h>
#include <hll.h>
#include <lru.h>
#include <rbtree.h>
#include <skiplist.h>
#include <stack.h>
#include <trie.h>

#include <pi.h>

#define command_length_check(x, y)                 \
  if (commands_length x y) {                       \
    printf("%s\n", errstr(error_invalid_command)); \
    return error_invalid_command;                  \
  }

#define cli_function_arguments commands_t commands, int commands_length

#define strequal(x, y) strlen(x) == strlen(y) && strncasecmp(x, y, strlen(y)) == 0

void clear();

bottle_error_t cli_help(cli_function_arguments);
bottle_error_t cli_pi(cli_function_arguments);

bottle_error_t cli_hmap(cli_function_arguments);
bottle_error_t cli_lru(cli_function_arguments);
bottle_error_t cli_avl(cli_function_arguments);
bottle_error_t cli_sklist(cli_function_arguments);
bottle_error_t cli_rbtree(cli_function_arguments);
bottle_error_t cli_deque(cli_function_arguments);
bottle_error_t cli_stack(cli_function_arguments);
bottle_error_t cli_bloom(cli_function_arguments);
bottle_error_t cli_dag(cli_function_arguments);
bottle_error_t cli_bittree(cli_function_arguments);
bottle_error_t cli_trie(cli_function_arguments);
bottle_error_t cli_hll(cli_function_arguments);
