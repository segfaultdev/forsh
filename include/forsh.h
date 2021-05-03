#ifndef __FORSH_H__
#define __FORSH_H__

#define TYPE_NUM   0
#define TYPE_STR   1
#define TYPE_PIPE  2
#define TYPE_NULL -1

typedef struct forsh_value_t forsh_value_t;

struct forsh_value_t {
  int type;

  union {
    int num;
    char *str;
    int pipe[2];
  } data;
};

extern forsh_value_t *forsh_value_stk;
extern int forsh_value_pos, forsh_value_len;

extern int *forsh_thread_stk;
extern int forsh_thread_pos, forsh_thread_len;

extern int *forsh_stop_stk;
extern int forsh_stop_pos, forsh_stop_len;

void forsh_value_push(forsh_value_t value);
forsh_value_t forsh_value_pop(void);
void forsh_value_dump(void);

void forsh_thread_push(int value);
int forsh_thread_pop(void);
void forsh_thread_dump(void);

void forsh_stop_push(int value);
int forsh_stop_pop(void);
void forsh_stop_dump(void);

void forsh_parse(char *token);

#endif
