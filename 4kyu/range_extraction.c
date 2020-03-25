#include <stdlib.h>
#include <stdbool.h>

char *range_extraction(const int *args, size_t n) {
  char buf[2048] = {0};
  int tmp[2] = {0};
  int i = 0;
  bool first = true;

  while (i < n) {
    tmp[0] = args[i++];

    // Determine if we're in a run
    if ((tmp[0] + 1 == args[i]) && (args[i] + 1 == args[i + 1])) {
      tmp[1] = args[++i];
      while (tmp[1] + 1 == args[i + 1]) {
        tmp[1] = args[++i];
      }
      ++i;
      sprintf(buf, "%s%s%d-%d", buf, first ? "" : ",", tmp[0], tmp[1]);
    } else {
      sprintf(buf, "%s%s%d", buf, first ? "" : ",", tmp[0]);
    }

    first = false;
  }

  return strdup(buf);
}
