#include <stdlib.h>
#include <string.h>

#include <command.h>

commands_t commands_parse(char *cmd, int *len) {
  int index = 0;
  commands_t commands = NULL;
  char *token = strtok(cmd, " ");
  while (token != NULL) {
    if (index == 0) { // 虽然这个地方花费的时间会比较多，但是功能是解析人敲的命令，一般不会太长，所以还 OK
      commands = (char **)malloc(sizeof(char *) * (index + 1));
    } else {
      commands = (char **)realloc(commands, sizeof(char *) * (index + 1));
    }
    size_t size = strlen(token) + 1; // 将要声明字符串本身长度和 '\0' 的空间
    commands[index] = (char *)malloc(sizeof(char) * size);
    memcpy(commands[index], token, size); // 将当前得到的结果复制到新的内存空间中
    token = strtok(NULL, " ");
    index++;
  }
  *len = index;
  return commands;
}

void commands_free(commands_t commands, int length) {
  for (int i = 0; i < length; i++) {
    free(commands[i]); // 释放每个字符串的空间
  }
  free(commands); // 释放指针数组的空间
}
