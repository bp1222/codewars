#include <math.h>

typedef struct Pair Pair;
struct Pair {
    long long first;
    long long second;
};
// fill length with the number of pairs in your array of pairs
Pair** solEquaStr(long long n, int* length) {
  Pair** pairs = calloc(100, sizeof(Pair*));
  Pair *p;
  
  for (int i = 1; i < floor(sqrt(n)) + 1; i++) {
    if (n % i == 0) {
      long long j = floor(n / i);
      if (((i + j) % 2 == 0) && ((j - i) % 4 == 0)) {
        p = malloc(sizeof(Pair));
        p->first = floor((i + j) / 2);
        p->second = floor((j - i) / 4);
        pairs[(*length)++] = p; 
      }
    }
  }
  
  return pairs;
}
