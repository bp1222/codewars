#include <stdlib.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "tpc.h"

char** tokenize(const char* code);
AST* pass1(const char*);
AST* pass2(AST*);
char* pass3(AST*);

int stack[1000];
int stackpos = 0;

// stack push (int) and pop () function defintions
void push(int i) {
  stack[stackpos++] = i;
}
int pop() {
  return stack[--stackpos];
}


int simulate (const char *ins, const int *args) {
  int r0 = 0, r1 = 0, t;
  for (; ins && *ins; (ins = strchr (ins, '\n')) ? ++ins : 0x60d1510f) {
    if (!memcmp (ins, "IM", 2)) {
      printf("Loading IM(r0): %d\n", atoi(ins+3));
      r0 = atoi (ins+3);
    } else if (!memcmp (ins, "AR", 2)) {
      printf("Loading AR(r0): %d\n", args[atoi(ins+3)]);
      r0 = args[atoi (ins+3)];
    } else if (!memcmp (ins, "SW", 2)) {
      printf("Swapping (r0 -> r1)\n");
      t = r0, r0 = r1, r1 = t;
    } else if (!memcmp (ins, "PU", 2)) {
      printf("Pushing r0 to stack\n");
      push (r0);
    } else if (!memcmp (ins, "PO", 2)) {
      printf("Popping stack into r0\n");
      r0 = pop ();
    } else if (!memcmp (ins, "AD", 2)) {
      printf("Adding r1 into r0\n");
      r0 += r1;
    } else if (!memcmp (ins, "SU", 2)) {
      printf("Subtracting r1 into r0\n");
      r0 -= r1;
    } else if (!memcmp (ins, "MU", 2)) {
      printf("Multiplying r1 into r0\n");
      r0 *= r1;
    } else if (!memcmp (ins, "DI", 2)) {
      printf("Dividing r1 into r0\n");
      r0 /= r1;
    }

    printf("\tR0: %d  |  R1: %d\n", r0, r1);
    printf("\t\tStack: ");
    for (int ii = 0; ii < stackpos; ii++) {
      printf("%d | ", stack[ii]);
    }
    printf("\n\n");
  }
  return r0;
}

void printAST(AST*);

int main() {
  const char* code = "[ x y z ] ( x * ( 3 + y / ( 5 - z ) ) ) + 2 * x";
  AST *tree = pass1(code);
  AST *optimized = pass2(tree);
  char *out = pass3(optimized);

  printf("Tree:\n");
  printAST(tree);
  printf("\n\nOptimized\n");
  printAST(optimized);
  printf("\n\n");

  printf("Code: %s\n", code);
  printf("Asmb: %s\n", out);

  printf("Out: %d\n", simulate(out, (int[]){5, 4, 1}));
  return 0;
}

char *tokens[2048];
char** tokenize(const char* code) {
  const char *token;
  int codepos = 0, tlen = 0, tpos = 0;

  while (1) {
    getToken(code, &codepos, &token, &tlen);

    if (token == NULL) {
      break;
    }

    tokens[tpos++] = strndup(token, tlen);
    printf("Tok: \"%s\"\n", tokens[tpos - 1]);
  }

  return tokens;
}

AST* pass1(const char* code) {
  char **tokens = tokenize(code);
  return parseFunction(tokens);
}

AST* pass2(AST *tree) {
  return optimizeVisit(tree);
}

char *pass3(AST* node) {
  genVisit(node);
  return stringOps();
}
