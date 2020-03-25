#include <math.h>

long long findNb(long long m) {
  long long n = 0;
  while (m > 0) m -= pow(++n, 3);
  return m ? -1 : n;
}
