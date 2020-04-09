#ifndef __TPC_H__
#define __TPC_H__

#include <stdlib.h>

#define IS_BINOP(n) (n)->op == plus || (n)->op == min || (n)->op == mul || (n)->op == diva

enum ast_op_code {imm, arg, plus, min, mul, diva};
enum gen_op_code {IM, AR, SW, PU, PO, AD, SU, MU, DI};
extern const char* ast_op_code_str[6];

typedef struct _AST AST;
struct _AST {
  enum ast_op_code op;
  AST *a, *b;
  int n;
};

typedef struct _arg_list arg_list;
struct _arg_list {
  int n;
  char v;
  AST *a;
  arg_list *next;
};

typedef struct _gen_ops gen_ops;
typedef struct _gen_ops {
  enum gen_op_code code;
  int n;
  gen_ops *next;
} gen_ops;

void genVisit(AST* node);

AST* Imm  (int);
AST* Arg  (int);
AST* Bin  (enum ast_op_code, AST*, AST*);

void getToken(const char *, int *, const char **, int *);

AST* optimizeVisit(AST *);

AST* parseFunction(char**);
AST* parseExpression();
char *stringOps();

extern arg_list *al;
#endif
