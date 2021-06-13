#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

#include <avl.h>
#include <cmddefine.h>
#include <command.h>
#include <hashmap.h>
#include <lru.h>
#include <rbtree.h>
#include <skiplist.h>

#include <pi.h>

#define ERR_COMMAND "invalid command"
#define ERR_COMMAND_NOT_FOUND "command not found"
#define ERR_INTERNAL "internal error"

#define MAP_COMMANDS_OK true
#define MAP_COMMANDS_ERROR false

#define command_length_check(x, y) \
  if (commands_length x y) {       \
    printf("%s\n", ERR_COMMAND);   \
    return MAP_COMMANDS_ERROR;     \
  }

void clear();

bool command_hash(commands_t commands, int commands_length);
bool command_base64(commands_t commands, int commands_length);
bool command_help(commands_t commands, int commands_length);
bool command_pi(commands_t commands, int commands_length);

bool command_hmap(commands_t commands, int commands_length);
bool command_lru(commands_t commands, int commands_length);
bool command_avl(commands_t commands, int commands_length);
bool command_sklist(commands_t commands, int commands_length);
bool command_rbtree(commands_t commands, int commands_length);
