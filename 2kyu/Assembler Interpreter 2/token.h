#ifndef __TOKEN_H__
#define __TOKEN_H__
#define div divide

#define MAX_TOKENS 4096
#define OPSTACK_SIZE 2048
#define LABELS_SIZE  1024

typedef struct _token {
  int len;
  char *t;
} token;

typedef struct _op_arg {
  enum arg_type {str, num, reg} type;
  union {
    char *str;
    int num;
    int reg;
  } v;
} op_arg;

typedef struct _op {
  enum op_code {mov, inc, dec, add, sub, mul, div, lbl, jmp, cmp, jne, je, jge, jg, jle, jl, call, ret, msg, end, cmt} code;
  int num;
  op_arg children[0];
} op;

typedef struct label_ptr {
  op *op;
  int instruction;
} label_ptr;

#define consume_space(p) while ((isspace(*(p)) && *(p) != '\n') || *(p) == ',') { (p)++; }

void tokenize(const char *program, token tokens[MAX_TOKENS]);
void buildOpStack(token tokens[MAX_TOKENS], op *op_stack[OPSTACK_SIZE]);

#endif