#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_LEN 200000

char *multiply(char *a, char *b) {
  int al = strlen(a), bl = strlen(b);
  int iresult[MAX_LEN] = {0};

  if (al == 0 || bl == 0) {
    return strdup("0");
  }

  int an = 0, bn = 0, sum = 0, ia = 0, ib = 0, carry = 0;

  for (int i = al - 1; i >= 0; i--) {
    carry = 0;
    an = a[i] - '0';
    ib = 0;

    for (int j = bl - 1; j >= 0; j--) {
      bn = b[j] - '0';
      sum = (an * bn) + iresult[ia + ib] + carry;
      carry = sum / 10;
      iresult[ia + ib] = sum % 10;
      ib++;
    }
    if (carry) {
      iresult[ia + ib] += carry;
    }

    ia++;
  }

  char *ret = malloc(al+bl+1);
  int spos = 0;
  bool trunc = true;
  for (int i = al+bl-1; i >= 0; i--) {
    if (trunc && iresult[i] == 0) {
      continue;
    } else {
      trunc = false;
    }
    ret[spos++] = iresult[i] + '0';
  }

  if (spos == 0) {
    return strdup("0");
  }

  ret[spos] = '\0';
  return ret;
}
