#pragma once

typedef char **commands_t;

commands_t commands_parse(char *cmd, int *len);
void commands_free(commands_t commands, int length);
