#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmd.h>
#include <cmddefine.h>
#include <command.h>
#include <error.h>

#define VERSION "v1.0.0"

#define COMMANDS_CHECK(x)           \
  bottle_error_t result = x;        \
  if (result != bottle_ok) {        \
    printf("%s\n", errstr(result)); \
  }

#define MAX_LINE 256

void handle_signals(int signo) {
  if (signo == SIGINT) {
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char const *argv[]) {
  atexit(clear);

  if (signal(SIGINT, handle_signals) == SIG_ERR) {
    printf("failed to register interrupts with kernel\n");
    return EXIT_FAILURE;
  }

  char buf[MAX_LINE] = {0};
  while (true) {
    printf("> ");
    if (fgets(buf, MAX_LINE, stdin) == NULL)
      return EXIT_FAILURE;
    size_t len = strlen(buf);
    while (len && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
      len--;
      buf[len] = '\0';
    }
    int commands_length;
    commands_t commands = commands_parse(buf, &commands_length);

    if (commands_length >= 1) {
      if (strequal(commands[0], COMMAND_HELP)) {
        COMMANDS_CHECK(cli_help(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_EXIT)) {
        commands_free(commands, commands_length);
        return EXIT_SUCCESS;
      } else if (strequal(commands[0], COMMAND_VERSION)) {
        printf("%s\n", VERSION);
      } else if (strequal(commands[0], COMMAND_PI)) {
        COMMANDS_CHECK(cli_pi(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_HMAP)) {
        COMMANDS_CHECK(cli_hmap(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_LRU)) {
        COMMANDS_CHECK(cli_lru(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_AVL)) {
        COMMANDS_CHECK(cli_avl(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_SKLIST)) {
        COMMANDS_CHECK(cli_sklist(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_RBTREE)) {
        COMMANDS_CHECK(cli_rbtree(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_DEQUE)) {
        COMMANDS_CHECK(cli_deque(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_STACK)) {
        COMMANDS_CHECK(cli_stack(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_BLOOM)) {
        COMMANDS_CHECK(cli_bloom(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_DAG)) {
        COMMANDS_CHECK(cli_dag(commands, commands_length));
      } else if (strequal(commands[0], COMMAND_BITTREE)) {
        COMMANDS_CHECK(cli_bittree(commands, commands_length));
      } else {
        printf("%s\n", errstr(error_invalid_command));
      }
    }
    commands_free(commands, commands_length);
  }
  return EXIT_SUCCESS;
}
