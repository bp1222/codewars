#include <stdbool.h>

bool isValidWalk(const char *walk) {
  int steps[2] = {0, 0};
  int len = strlen(walk);
  
  if (len != 10) return false;
  
  for (int i = 0; i < len; i++) {
    switch (walk[i]) {
      case 'n':
        steps[0]++; break;
      case 's':
        steps[0]--; break;
      case 'e':
        steps[1]++; break;
      case 'w':
        steps[1]--; break;
    }
  }
  
  return (steps[0] == 0) && (steps[1] == 0);
}
