#include "tpc.h"

AST* Imm (int n) {
  AST *ret = (AST*)calloc(1, sizeof(AST));
  ret->op = imm;
  ret->n = n;
  return ret;
}

AST* Arg (int n) {
  AST *ret = (AST*)calloc(1, sizeof(AST));
  ret->op = arg;
  ret->n = n;
  return ret;
}

AST* Bin (enum ast_op_code op, AST *a, AST *b) {
  AST *ret = (AST*)calloc(1, sizeof(AST));
  ret->op = op;
  ret->a = a;
  ret->b = b;
  return ret;
}

