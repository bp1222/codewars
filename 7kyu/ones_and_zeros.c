#include <stddef.h>

unsigned binary_array_to_numbers(const unsigned *bits, size_t count) {
  unsigned retval = 0;
  
  for (int i = 0; i < count; i++) {
    if (bits[i]) {
      retval += (1 << (count - i - 1));
    }
  }
  
  return retval;
}
