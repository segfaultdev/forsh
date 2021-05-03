#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <forsh.h>

static int is_num(char *str) {
  while (*str) {
    if (*str < '0' || *str > '9') return 0;
    str++;
  }

  return 1;
}

void forsh_parse(char *token) {
  if (!strcmp(token, "dup")) {
    forsh_value_t value = forsh_value_pop();
    forsh_value_push(value);
    forsh_value_push(value);
  } else if (!strcmp(token, "swap")) {
    forsh_value_t value_1 = forsh_value_pop();
    forsh_value_t value_2 = forsh_value_pop();
    forsh_value_push(value_1);
    forsh_value_push(value_2);
  } else if (!strcmp(token, "dump")) {
    forsh_value_dump();
  } else if (!strcmp(token, "pop")) {
    forsh_value_pop();
  } else if (!strcmp(token, "[")) {
    forsh_stop_push(forsh_value_pos);
  } else if (!strcmp(token, "]")) {
    forsh_stop_pop();
  } else if (!strcmp(token, "out")) {
    forsh_value_t last_pipe = forsh_value_pop();

    if (last_pipe.type == TYPE_PIPE) {
      dup2(STDOUT_FILENO, last_pipe.data.pipe[0]);
    } else {
      printf("[Error: Invalid type!]\n");
    }
  }



  else if (!strcmp(token, "+")) {
    forsh_value_t value_1 = forsh_value_pop();
    forsh_value_t value_2 = forsh_value_pop();

    if (value_1.type != value_2.type) {
      printf("[Error: Cannot add different types!]\n");
      forsh_value_push(value_1);
    }

    if (value_1.type == TYPE_NUM) {
      value_1.data.num += value_2.data.num;
      forsh_value_push(value_1);
    } else if (value_1.type == TYPE_STR) {
      value_1.data.str = realloc(value_1.data.str, strlen(value_1.data.str) + strlen(value_2.data.str) + 1);
      strcat(value_1.data.str, value_2.data.str);
      forsh_value_push(value_1);
      free(value_2.data.str);
    } else {
      printf("[Error: Invalid type!]\n");
    }
  } else if (!strcmp(token, ".")) {
    forsh_value_t value = forsh_value_pop();

    if (value.type == TYPE_NUM) {
      printf("%d\n", value.data.num);
    } else if (value.type == TYPE_STR) {
      printf("%s\n", value.data.str);
    } else {
      printf("[Error: Invalid type!]\n");
    }
  } else if (*token == '!') {
    char *argv[128];
    int argc = 0;

    forsh_value_t last_pipe;
    last_pipe.type = TYPE_NULL;

    argv[argc++] = token + 1;

    while (forsh_value_pos) {
      forsh_value_t value = forsh_value_pop();

      if (value.type == TYPE_NUM) {
        char *buffer = calloc(32, 1);
        sprintf(buffer, "%d", value.data.num);
        argv[argc++] = buffer;
      } else if (value.type == TYPE_STR) {
        argv[argc++] = value.data.str;
      } else if (value.type == TYPE_PIPE) {
        last_pipe = value;
      }
    }

    argv[argc] = NULL;

    forsh_value_t value;
    value.type = TYPE_PIPE;

    if (pipe(value.data.pipe) < 0) {
      printf("[Error: Cannot initialize pipe!]\n");
      return;
    }

    forsh_value_push(value);

    if (!fork()) {
      if (last_pipe.type != TYPE_NULL) {
        dup2(last_pipe.data.pipe[0], STDIN_FILENO);
      }

      close(value.data.pipe[0]);
      dup2(value.data.pipe[1], STDOUT_FILENO);

      execvp(argv[0], argv);
    }

    while (wait(NULL) > 0);

    close(value.data.pipe[1]);

    for (int i = 1; i < argc; i++)
      free(argv[i]);
  } else if (is_num(token)) {
    forsh_value_t value;
    value.type = TYPE_NUM;
    value.data.num = strtol(token, NULL, 0);
    forsh_value_push(value);
  } else {
    forsh_value_t value;
    value.type = TYPE_STR;
    value.data.str = malloc(strlen(token) + 1);
    strcpy(value.data.str, token);
    forsh_value_push(value);
  }
}
