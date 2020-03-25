#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct interval {
    int first;
    int second;
};

struct interval_ex {
  int first;
  int second;
  bool count;
};

#define BETWEEN(p, v) ((p) <= (v).second && (v).first <= (p))
#define CONTAINS(p, v) (BETWEEN((p).first, (v)) || \
                       BETWEEN((p).second, (v)) || \
                       BETWEEN((v).first, (p)) || \
                       BETWEEN((v).second, (p)))

int sum_intervals(const struct interval *v, size_t n) {
  size_t retval = 0;
  size_t intervals = 0;
  int prev = -1;
  bool new = true;

  struct interval_ex *overlap = calloc(1, sizeof(struct interval_ex) * n);
  struct interval_ex pass;

  for (size_t i = 0; i < n; i++) {
    pass = *(struct interval_ex*)&v[i];
    pass.count = true;
    prev = -1;

    for (size_t j = 0; j < intervals; j++) {
      if (CONTAINS(pass, overlap[j])) {
        new = false;
        overlap[j].first = overlap[j].first < pass.first ? overlap[j].first : pass.first;
        overlap[j].second = overlap[j].second > pass.second ? overlap[j].second : pass.second;

        pass.first = overlap[j].first;
        pass.second = overlap[j].second;
        if (prev >= 0) {
          overlap[prev].count = false;
        }
        prev = j;
      }
    }

    if (new) {
      overlap[intervals].first = pass.first;
      overlap[intervals].second = pass.second;
      overlap[intervals].count = true;
      intervals++;
    }
    new = true;
  }

  for (size_t i = 0; i < intervals; i++) {
    if (overlap[i].count) {
      retval += overlap[i].second - overlap[i].first;
    }
  }

  return retval;
}
