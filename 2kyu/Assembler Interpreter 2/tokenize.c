#include <stdlib.h>
#include <ctype.h>
#include "token.h"

token getToken(char **p) {
  token t = {0};

  consume_space(*p);

  if (isdigit(**p) || (**p == '-' && isdigit(*(*p + 1)))) {
    t.t = (*p)++;
    t.len++;
    while (isdigit(**p)) { t.len++; ++*p; }
    return t;
  }

  if (isalpha(**p)) {
    t.t = (*p)++;
    t.len++;
    while (isalpha(**p) || **p == '_' || isdigit(**p)) { t.len++; ++*p; }
    if (**p == ':') { t.len++; ++*p; }
    return t;
  }

  switch (**p) {
    case '\'':
      t.t = (*p)++;
      t.len++;
      while (**p != '\'') { t.len++; ++*p; }
      t.len++; ++*p;
      return t;
    case '\n':
      t.t = (*p)++;
      t.len = 0;
      return t;
    case ';':
      t.t = (*p)++;
      t.len++;
      while (**p != '\n') { t.len++; ++*p; }
      return t;
  }

  return (token){0};
}

void tokenize(const char *program, token tokens[MAX_TOKENS]) {
  char *p = (char *)program;
  int tok = 0;

  while (*p != '\0') {
    tokens[tok++] = getToken(&p);
  }
  tokens[tok].t = NULL;
  tokens[tok].len = -1;
}