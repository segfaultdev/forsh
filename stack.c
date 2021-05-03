#include <stdio.h>
#include <stdlib.h>

#include <forsh.h>

forsh_value_t *forsh_value_stk = NULL;
int forsh_value_pos = 0, forsh_value_len = 0;

int *forsh_thread_stk = NULL;
int forsh_thread_pos = 0, forsh_thread_len = 0;

int *forsh_stop_stk = NULL;
int forsh_stop_pos = 0, forsh_stop_len = 0;

void forsh_value_push(forsh_value_t value) {
  if (forsh_value_pos >= forsh_value_len) {
    forsh_value_stk = realloc(forsh_value_stk, (forsh_value_len + 16) * sizeof(forsh_value_t));
    forsh_value_len += 16;
  }

  forsh_value_stk[forsh_value_pos++] = value;
}

forsh_value_t forsh_value_pop(void) {
  if (forsh_value_pos) {
    return forsh_value_stk[--forsh_value_pos];
  } else {
    forsh_value_t value;
    value.type = TYPE_NULL;
    return value;
  }
}

void forsh_value_dump(void) {
  printf("[");

  for (int i = 0; i < forsh_value_pos; i++) {
    if (forsh_value_stk[i].type == TYPE_NUM)
      printf("%d", forsh_value_stk[i].data.num);
    else if (forsh_value_stk[i].type == TYPE_STR)
      printf("'%s'", forsh_value_stk[i].data.str);
    else if (forsh_value_stk[i].type == TYPE_PIPE)
      printf("(%d, %d)", forsh_value_stk[i].data.pipe[0], forsh_value_stk[i].data.pipe[1]);
    else continue;

    if (i < forsh_value_pos - 1)
      printf(", ");
  }

  printf("]\n");
}

void forsh_thread_push(int value) {
  if (forsh_thread_pos >= forsh_thread_len) {
    forsh_thread_stk = realloc(forsh_thread_stk, (forsh_thread_len + 16) * sizeof(int));
    forsh_thread_len += 16;
  }

  forsh_thread_stk[forsh_thread_pos++] = value;
}

int forsh_thread_pop(void) {
  if (forsh_thread_pos) {
    return forsh_thread_stk[--forsh_thread_pos];
  } else {
    return -1;
  }
}

void forsh_thread_dump(void) {
  printf("[");

  for (int i = 0; i < forsh_thread_pos; i++) {
    printf("%d", forsh_thread_stk[i]);

    if (i < forsh_thread_pos - 1)
      printf(", ");
  }

  printf("]\n");
}

void forsh_stop_push(int value) {
  if (forsh_stop_pos >= forsh_stop_len) {
    forsh_stop_stk = realloc(forsh_stop_stk, (forsh_stop_len + 16) * sizeof(int));
    forsh_stop_len += 16;
  }

  forsh_stop_stk[forsh_stop_pos++] = value;
}

int forsh_stop_pop(void) {
  if (forsh_stop_pos) {
    return forsh_stop_stk[--forsh_stop_pos];
  } else {
    return -1;
  }
}

void forsh_stop_dump(void) {
  printf("[");

  for (int i = 0; i < forsh_stop_pos; i++) {
    printf("%d", forsh_stop_stk[i]);

    if (i < forsh_stop_pos - 1)
      printf(", ");
  }

  printf("]\n");
}
