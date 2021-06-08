#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmd.h>
#include <cmddefine.h>
#include <command.h>

#define VERSION "v1.0.0"

#define COMMANDS_CHECK(x)                                                                                              \
  if (x) {                                                                                                             \
    commands_free(commands, commands_length);                                                                          \
    continue;                                                                                                          \
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
      if (strncasecmp(commands[0], COMMAND_HELP, strlen(COMMAND_HELP)) == 0) {
        COMMANDS_CHECK(!command_help(commands, commands_length));
      } else if (strncasecmp(commands[0], COMMAND_EXIT, strlen(COMMAND_EXIT)) == 0) {
        commands_free(commands, commands_length);
        return EXIT_SUCCESS;
      } else if (strncasecmp(commands[0], COMMAND_VERSION, strlen(COMMAND_VERSION)) == 0) {
        printf("%s\n", VERSION);
      } else if (strncasecmp(commands[0], COMMAND_PI, strlen(COMMAND_PI)) == 0) {
        COMMANDS_CHECK(!command_pi(commands, commands_length));
      } else if (strncasecmp(commands[0], COMMAND_HMAP, strlen(COMMAND_HMAP)) == 0) {
        COMMANDS_CHECK(!command_hmap(commands, commands_length));
      } else if (strncasecmp(commands[0], COMMAND_LRU, strlen(COMMAND_LRU)) == 0) {
        COMMANDS_CHECK(!command_lru(commands, commands_length));
      } else if (strncasecmp(commands[0], COMMAND_AVL, strlen(COMMAND_AVL)) == 0) {
        COMMANDS_CHECK(!command_avl(commands, commands_length));
      } else if (strncasecmp(commands[0], COMMAND_SKLIST, strlen(COMMAND_SKLIST)) == 0) {
        COMMANDS_CHECK(!command_sklist(commands, commands_length));
      } else {
        printf("%s\n", ERR_COMMAND_NOT_FOUND);
      }
    }
    commands_free(commands, commands_length);
  }
  return EXIT_SUCCESS;
}
