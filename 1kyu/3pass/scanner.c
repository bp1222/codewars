#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tpc.h"

#define GET_DIG_ALP(f)    \
  if (f(s[*spos])) {      \
    int _pos = *spos;      \
    int _len = 1;          \
    ++*spos;              \
    while (f(s[*spos])) {  \
      _len++; ++*spos;    \
    }                      \
    *token = s + _pos;    \
    *tlen = _len;          \
    return;                \
  }


void getToken(const char *s, int *spos, const char **token, int *tlen) {
  while (isspace(s[*spos])) {
    ++*spos;
  }

  GET_DIG_ALP(isdigit)
  GET_DIG_ALP(isalpha)

  switch (s[*spos]) {
    case '(':
    case ')':
    case '[':
    case ']':
    case '+':
    case '-':
    case '*':
    case '/':
      *token = s + *spos;
      *tlen = 1;
      ++*spos;
      return;
  }

  if (*spos == strlen(s)) {
    *token = NULL;
    *tlen = -1;
    return;
  }

  printf("Unknown Grammar\n");
  exit(0);
}
