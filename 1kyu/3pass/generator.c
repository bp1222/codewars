#include <stdlib.h>
#include <stdio.h>
#include "tpc.h"

static gen_ops *op_stack = NULL;

void emit(enum gen_op_code code, int n) {
  static gen_ops *head = NULL;

  gen_ops *o = (gen_ops*)calloc(1, sizeof(gen_ops));
  o->code = code;
  o->n = n;

  if (op_stack == NULL) {
    op_stack = o;
  }

  if (head == NULL) {
    head = o;
  } else {
    head->next = o;
    head = o;
  }
}

void visitBinary(AST *node) {
  AST *lhs = node->a;
  AST *rhs = node->b;

  genVisit(lhs);
  emit(PU, 0);
  genVisit(rhs);
  emit(SW, 0);
  emit(PO, 0);
  switch (node->op) {
    case plus:
      emit(AD, 0);
      break;
    case min:
      emit(SU, 0);
      break;
    case mul:
      emit(MU, 0);
      break;
    case diva:
      emit(DI, 0);
      break;
    default:
      printf("Unable to visit non binary node\n");
      exit(0);
  }
}

void genVisit(AST* node) {
  switch (node->op) {
    case imm:
      emit(IM, node->n);
      break;
    case arg:
      emit(AR, node->n);
      break;
    case plus:
    case min:
    case mul:
    case diva:
      visitBinary(node);
      break;
    default:
      printf("Unable to visit node\n");
      exit(0);
  }
}

char *stringOps() {
  char *ret = (char*)calloc(8192, sizeof(char));
  int len = 0;

  for (gen_ops *o = op_stack; o != NULL; o = o->next) {
    switch (o->code) {
      case IM:
        len = sprintf(ret, "%sIM %d\n", ret, o->n);
        break;
      case AR:
        len = sprintf(ret, "%sAR %d\n", ret, o->n);
        break;
      case SW:
        len = sprintf(ret, "%sSW\n", ret);
        break;
      case PU:
        len = sprintf(ret, "%sPU\n", ret);
        break;
      case PO:
        len = sprintf(ret, "%sPO\n", ret);
        break;
      case AD:
        len = sprintf(ret, "%sAD\n", ret);
        break;
      case SU:
        len = sprintf(ret, "%sSU\n", ret);
        break;
      case MU:
        len = sprintf(ret, "%sMU\n", ret);
        break;
      case DI:
        len = sprintf(ret, "%sDI\n", ret);
        break;
    }
  }
  return ret;
}
