#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <command.h>

const int MAX_LINE = 256;

int main(int argc, char const *argv[]) {
  char buf[MAX_LINE] = {0};
  while (true) {
    printf("> ");
    if (fgets(buf, MAX_LINE, stdin) == NULL) return EXIT_FAILURE;
    size_t len = strlen(buf);
    while (len && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
      len--;
      buf[len] = '\0';
    }
    int commands_length;
    commands_t commands = commands_parse(buf, &commands_length);

    printf("%d, %s\n", commands_length, commands[0]);
    commands_free(commands, commands_length);
  }
  return 0;
}
