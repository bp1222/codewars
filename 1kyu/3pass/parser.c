#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tpc.h"

arg_list *al;
const char* ast_op_code_str[6] = {"Imm", "Arg", "add", "sub", "mul", "div"};

static char **parse_tokens;
static int    tpos;

enum token_type {T_LB, T_RB, T_LP, T_RP, T_NUM, T_VAR, T_PLUS, T_MIN, T_MUL, T_DIV};

// Token Helpers
#define TOK(c)        (parse_tokens[tpos] != NULL && *parse_tokens[tpos] == (c))
#define IS_LB         TOK('[')
#define IS_RB         TOK(']')
#define IS_LP         TOK('(')
#define IS_RP         TOK(')')
#define IS_PLUS       TOK('+')
#define IS_MIN        TOK('-')
#define IS_MUL        TOK('*')
#define IS_DIV        TOK('/')
#define IS_NUM        (parse_tokens[tpos] != NULL && isdigit(*parse_tokens[tpos]))
#define IS_VAR        (parse_tokens[tpos] != NULL && isalpha(*parse_tokens[tpos]))

#define CONSUME(tok)                  \
  if (type == T_##tok && IS_##tok) {  \
    ++tpos;                            \
    return;                            \
  }

void consumeToken(enum token_type type) {
  CONSUME(LB)
  CONSUME(RB)
  CONSUME(LP)
  CONSUME(RP)
  CONSUME(PLUS)
  CONSUME(MIN)
  CONSUME(MUL)
  CONSUME(DIV)
  CONSUME(NUM)
  CONSUME(VAR)

  printf("Consuming bad token\n");
  exit(0);
}

AST* getArg(char s) {
  for (arg_list *l = al; l != NULL; l = l->next) {
    if (l->v == s) {
      return l->a;
    }
  }
  return NULL;
}

// factor     ::= number
//              | variable
//              | '(' expression ')'
AST* parseFactor() {
  AST* ret = NULL;

  if (IS_NUM) {
    ret = Imm(*parse_tokens[tpos] - '0');
    consumeToken(T_NUM);
  } else if (IS_VAR) {
    ret = getArg(*parse_tokens[tpos]);
    if (ret == NULL) {
      printf("Error finding variable\n");
      exit(0);
    }
    consumeToken(T_VAR);
  } else if (IS_LP) {
    consumeToken(T_LP);
    ret = parseExpression();
    consumeToken(T_RP);
  }

  return ret;
}

// term       ::= factor
//              | term '*' factor
//              | term '/' factor
AST* parseTerm() {
  AST *node = parseFactor();
  AST *factor;

  while (IS_MUL || IS_DIV) {
    if (IS_MUL){
      consumeToken(T_MUL);
      factor = parseFactor();
      node = Bin(mul, node, factor);
    } else if (IS_DIV) {
      consumeToken(T_DIV);
      factor = parseFactor();
      node = Bin(diva, node, factor);
    }
  }

  return node;
}

// expression ::= term
//              | expression '+' term
//              | expression '-' term
AST* parseExpression() {
  AST *node = parseTerm();
  AST *term;

  while (IS_PLUS || IS_MIN) {
    if (IS_PLUS) {
      consumeToken(T_PLUS);
      term = parseTerm();
      node = Bin(plus, node, term);
    } else if (IS_MIN) {
      consumeToken(T_MIN);
      term = parseTerm();
      node = Bin(min, node, term);
    }
  }

  return node;
}

// arg-list   ::= /* nothing */
//              | variable arg-list
void parseArgs() {
  arg_list *cur = NULL;
  arg_list *new = NULL;
  int num = 0;

  while (IS_VAR) {
    new = (arg_list*)calloc(1, sizeof(arg_list));
    new->n = num++;
    new->v = *parse_tokens[tpos];
    new->a = Arg(new->n);
    new->next = NULL;

    if (al == NULL) {
      al = new;
    }

    if (cur == NULL) {
      cur = new;
    } else {
      cur->next = new;
      cur = new;
    }
    consumeToken(T_VAR);
  }
}

// function   ::= '[' arg-list ']' expression
AST* parseFunction(char **t) {
  AST* expr = NULL;

  parse_tokens = t;
  tpos = 0;

  if (IS_LB) {
    consumeToken(T_LB);
    parseArgs();
    consumeToken(T_RB);
    expr = parseExpression();
  }

  return expr;
}

void printAST(AST *ast) {
  static int level = -1;
  level++;
  switch(ast->op) {
    case plus:
    case min:
    case mul:
    case diva:
      printf("%*sBin (%s, \n", level, "", ast_op_code_str[ast->op]);
      printAST(ast->a);
      printAST(ast->b);
      printf("%*s)\n", level, "");
      break;
    case imm:
      printf("%*sImm (%d)\n", level, "", ast->n);
      break;
    case arg:
      printf("%*sArg (%d)\n", level, "", ast->n);
      break;
  }
  level--;
}
