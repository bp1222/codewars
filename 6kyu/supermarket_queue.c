#include <stdlib.h>

int queue_time(const int *customers, int customers_length, int n) {
  int retval = 0;
  
  if (n == 0) {
    return retval;
  } else if (n == 1) {
    for (int i = 0; i < customers_length; i++) {
      retval += customers[i];
    }
  } else {
    int *tills = calloc(sizeof(int) * n, 1);
    
    for (int c = 0; c < customers_length; c++) {
      int shortest = 0;
      
      for (int t = 0; t < n; t++) {
        if (tills[t] < tills[shortest]) {
          shortest = t;
        }
      }
      
      tills[shortest] += customers[c];
    }
    
    for (int t = 0; t < n; t++) {
      if (retval < tills[t]) {
        retval = tills[t];
      }
    }
  }
  
  return retval;
}
