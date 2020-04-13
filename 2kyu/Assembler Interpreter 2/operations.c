#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "token.h"

#define ISOP(o) (strlen((o)) == t->len && strncmp((o), t->t, t->len) == 0)

op *newOp(enum op_code code, int children) {
  op *o = (op*)calloc(1, sizeof(op) + (sizeof(op_arg) * children));
  o->code = code;
  o->num = children;
  return o;
}

bool fillOpArgFromToken(op_arg *a, token t) {
  if (isdigit(*t.t)) {
    a->type = num;
    for (int j = 0; j < t.len; j++) {
      a->v.num = a->v.num * 10 + (t.t[j] - '0');
    }
    return true;
  } else if(*t.t == '-' && isdigit(*(t.t+1))) {
    a->type = num;
    for (int j = 1; j < t.len; j++) {
      a->v.num = a->v.num * 10 + (t.t[j] - '0');
    }
    a->v.num *= -1;
    return true;
  } else if (isalpha(*t.t)) {
    a->type = reg;
    a->v.reg = t.t[0] - 'a';
    return true;
  } else if (*t.t == '\'') {
    a->type = str;
    char *end = strstr(t.t + 1, "'");
    if (end == NULL) {
      printf("Invalid string argument\n");
      exit(0);
    }
    a->v.str = strndup(t.t + 1, end - t.t - 1);
    return true;
  }
  return false;
}

void getOpArgs(op *o, int args, token tokens[MAX_TOKENS], int *tokenptr) {
  for (int i = 0; i < args; i++) {
    if (!fillOpArgFromToken(&o->children[i], tokens[++(*tokenptr)])) {
      printf("Unable to parse arg\n");
      exit(1);
    }
  }
}

#define LABEL_OP(lblop) {                                                     \
  tokenptr++;                                                                 \
  op *_l = newOp((lblop), 1);                                                 \
  _l->children[0].type = str;                                                 \
  _l->children[0].v.str = strndup(tokens[tokenptr].t, tokens[tokenptr].len);  \
  op_stack[stackptr++] = _l;                                                  \
}

#define GEN_OP(genop, size) {                   \
  op *_g = newOp((genop), (size));              \
  if ((size) > 0) {                             \
    getOpArgs(_g, (size), tokens, &tokenptr);   \
  }                                             \
  op_stack[stackptr++] = _g;                    \
}

void buildOpStack(token tokens[MAX_TOKENS], op *op_stack[OPSTACK_SIZE]) {
  token *t;
  int tokenptr = 0;
  int stackptr = 0;

  while ((t = &tokens[tokenptr])->t != NULL) {
    if (ISOP("msg")) {
      int tmpt = tokenptr + 1;
      int argc = 0;

      while (tokens[tmpt++].len > 0) {
        argc++;
        if (*tokens[tmpt].t == ';') {
          break;
        }
      }

      op *msgop = newOp(msg, argc);
      getOpArgs(msgop, argc, tokens, &tokenptr);
      op_stack[stackptr++] = msgop;
    } else if (t->t[t->len - 1] == ':') {
      op *lbo = newOp(lbl, 1);
      lbo->children[0].type = str;
      lbo->children[0].v.str = strndup(t->t, t->len - 1);
      op_stack[stackptr++] = lbo;
    } else if (ISOP("mov")) {
      GEN_OP(mov, 2)
    } else if (ISOP("inc")) {
      GEN_OP(inc, 1)
    } else if (ISOP("dec")) {
      GEN_OP(dec, 1)
    } else if (ISOP("add")) {
      GEN_OP(add, 2)
    } else if (ISOP("sub")) {
      GEN_OP(sub, 2)
    } else if (ISOP("mul")) {
      GEN_OP(mul, 2)
    } else if (ISOP("div")) {
      GEN_OP(div, 2)
    } else if (ISOP("jmp")) {
      LABEL_OP(jmp);
    } else if (ISOP("cmp")) {
      GEN_OP(cmp, 2)
    } else if (ISOP("jne")) {
      LABEL_OP(jne);
    } else if (ISOP("je")) {
      LABEL_OP(je);
    } else if (ISOP("jge")) {
      LABEL_OP(jge);
    } else if (ISOP("jg")) {
      LABEL_OP(jg);
    } else if (ISOP("jle")) {
      LABEL_OP(jle);
    } else if (ISOP("jl")) {
      LABEL_OP(jl);
    } else if (ISOP("call")) {
      LABEL_OP(call);
    } else if (ISOP("ret")) {
      GEN_OP(ret, 0)
    } else if (ISOP("end")) {
      GEN_OP(end, 0)
    } else if (ISOP(";")) {
    }

    tokenptr++;
  }
}
