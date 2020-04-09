#include <stdlib.h>
#include <stdio.h>
#include "tpc.h"

AST* optimizeVisit(AST *node) {
  AST *lhs = node->a;
  AST *rhs = node->b;

  if (IS_BINOP(lhs)) {
    lhs = optimizeVisit(lhs);
  }

  if (IS_BINOP(rhs)) {
    rhs = optimizeVisit(rhs);
  }

  if (lhs->op == imm && rhs->op == imm) {
    switch (node->op) {
      case plus:
        return Imm(lhs->n + rhs->n);
      case min:
        return Imm(lhs->n - rhs->n);
      case mul:
        return Imm(lhs->n * rhs->n);
      case diva:
        return Imm(lhs->n / rhs->n);
      default:
        printf("Two oimm on bad node\n");
        exit(0);
    }
  }

  if (IS_BINOP(node)) {
    return Bin(node->op, lhs, rhs);
  } else if (node->op == arg) {
    return Arg(node->n);
  } else if (node->op == imm) {
    return Imm(node->n);
  }

  return NULL;
}
