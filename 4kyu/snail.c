#include <stdlib.h>
#include <stdbool.h>

enum snake_dir {
 N = 0,
 S,
 E,
 W
};

int *snail(size_t *outsz, const int **mx, size_t rows, size_t cols) {
  int *result = calloc(1, sizeof(int) * rows * cols);
  int *rp = result;
  size_t min_col = 0, max_col = cols - 1;
  size_t min_row = 0, max_row = rows - 1;
  size_t c_col = 0, c_row = 0;
  size_t runs = 0, max_runs = rows * cols;
  bool running = true;
  
  enum snake_dir d = E;
  
  if (rows == 0 || cols == 0) {
    *outsz = 0;
    return result;
  }
 
  while (running) {
    *(rp++) = mx[c_row][c_col];
    
    switch (d) {
      case E:
        if (c_col == max_col) {
          d = S;
          c_row++;
          min_row++;
          break;
        }
        c_col++;
        break;
        
      case S:
        if (c_row == max_row) {
          d = W;
          c_col--;
          max_col--;
          break;
        }
        c_row++;
        break;
        
      case W:
        if (c_col == min_col) {
          d = N;
          c_row--;
          max_row--;
          break;
        }
        c_col--;
        break;
        
      case N:
        if (c_row == min_row) {
          d = E;
          c_col++;
          min_col++;
          break;
        }
        c_row--;
        break;
    }
    
    if (min_row == max_row && min_col == max_col) {
      *(rp++) = mx[c_row][c_col];
      running = false;
    } else if (++runs == max_runs) {
      running = false;
    }
  }
  
  *outsz = rows * cols;
  return result;
}
