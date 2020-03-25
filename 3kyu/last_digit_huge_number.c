#include <stdlib.h>
#include <math.h>

int last_digit(const unsigned long int *arr, size_t arr_size) {
  // Corner Case
  if (arr_size == 0) {
    return 1;
  }
  if (arr_size == 1) {
    return arr[0] % 10;
  }

  unsigned long int tmp, base, exp;

  tmp = arr[arr_size - 1];
  for (int i = arr_size - 2; i >= 0; i--) {
    base = arr[i] < 20 ? arr[i] : arr[i] % 20 + 20;
    exp = tmp < 4 ? tmp : tmp % 4 + 4;
    tmp = round(pow(base, exp));
  }

  return tmp % 10;
}

