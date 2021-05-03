#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <forsh.h>

void forsh_hello(void) {
  printf("FORsh, the FORTH shell, made by segfaultdev.\n\n");
}

int main(int argc, const char *argv[]) {
  forsh_hello();

  int result = 0;

  for (;;) {
    char *tokens[128];
    int token_cnt = 0, token_len = 0;

    tokens[token_cnt] = calloc(1, 1);

    char chr = '\0';

    printf("[%d]: ", result);

    while (scanf("%c", &chr)) {
      if (chr == '\n') {
        break;
      } else if (chr == ' ') {
        if (token_len) {
          tokens[++token_cnt] = calloc(1, 1);
          token_len = 0;
        }
      } else {
        tokens[token_cnt] = realloc(tokens[token_cnt], token_len + 2);
        tokens[token_cnt][token_len++] = chr;
        tokens[token_cnt][token_len] = '\0';
      }
    }

    if (token_len) token_cnt++;

    for (int i = 0; i < token_cnt; i++) {
      forsh_parse(tokens[i]);
      free(tokens[i]);
    }

    fflush(stdout);
  }

  return 0;
}
