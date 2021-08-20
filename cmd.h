#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include <error.h>

#include <avl.h>
#include <bloom.h>
#include <cmddefine.h>
#include <command.h>
#include <dag.h>
#include <deque.h>
#include <hashmap.h>
#include <lru.h>
#include <rbtree.h>
#include <skiplist.h>
#include <stack.h>

#include <pi.h>

// #define ERR_COMMAND "invalid command"
// #define ERR_COMMAND_NOT_FOUND "command not found"
// #define ERR_INTERNAL "internal error"

// #define MAP_COMMANDS_OK true
// #define MAP_COMMANDS_ERROR false

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
