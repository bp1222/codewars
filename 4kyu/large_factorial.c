#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20000

char *factorial(int n) {
  if (n < 0) {
    return "";
  }

  int result[MAX_SIZE] = {0};
  int result_size, carry, product;
  int i, j, pos = 0;

  result[0] = 1;
  result_size = 1;

  for (i = 2; i <= n; i++) {
    carry = 0;

    for (j = 0; j < result_size; j++) {
      product = (result[j] * i) + carry;
      result[j] = product % 10;
      carry = product / 10;
    }

    while (carry) {
      result[result_size++] = carry % 10;
      carry /= 10;
    }
  }

  char *retval = calloc(1, result_size + 1);
  for (i = result_size - 1; i >= 0; i--) {
    retval[pos++] = result[i] + '0';
  }
  retval[result_size] = '\0';

  return retval;
}
