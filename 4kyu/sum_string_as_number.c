#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

char *strsumreal(const char *small, const char *large) {
  int sl = strlen(small), ll = strlen(large), carry = 0;
  int sc, lc, op, l = 0;
  char buf[2000] = {0};

  if (sl == 0 && ll == 0) {
    return strdup("0");
  }

  do {
    sc = sl > 0 ? small[--sl] - '0' : 0;
    lc = large[--ll] - '0';

    op = lc + sc + carry;
    buf[l++] = (op % 10) + '0';
    carry = op / 10;
  } while (ll > 0);

  if (carry) {
    buf[l++] = '1';
  }

  char *retval = calloc(1, l);
  for (int i = 1; i <= l; i++) {
    retval[i - 1] = buf[l - i];
  }
  retval[l] = '\0';

  return retval;
}
  
char *strsum(const char *a, const char *b) {
  while (*a == '0') { a++; }
  while (*b == '0') { b++; }
  return strlen(a) > strlen(b) ? strsumreal(b, a) : strsumreal(a, b);
}
