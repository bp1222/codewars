#include <stdlib.h>
#include <stdbool.h>

int srt(const void *a, const void *b) {
  int arg1 = *(const int*)a;
  int arg2 = *(const int*)b;
  if (arg1 < arg2) return -1;
  if (arg1 > arg2) return 1;
  return 0;
}

unsigned long long next_smaller_number(unsigned long long n){
  int digits[128] = {0};
  int counter = 0, pivot = -1;

  // Store number into array
  while (n) {
    // Get the next right most
    digits[counter++] = n % 10;
    n /= 10;
  }

  // Find our pivot number
  for (int i = 1; i < 128; i++) {
    if (digits[i-1] < digits[i]) {
      pivot = i;
      break;
    }
  }

  // There was no valid pivot
  if (pivot == -1) {
    return pivot;
  }

  // Find previous highest number less than pivot
  int highest = -1, highest_pos = -1;
  for (int i = 0; i < pivot; i++) {
    if (digits[i] < digits[pivot] && highest < digits[i]) {
      highest = digits[i];
      highest_pos = i;
    }
  }

  // Swap pivot with highst previous
  int temp = digits[pivot];
  digits[pivot] = highest;
  digits[highest_pos] = temp;

  // Sort the remainder values high to low
  qsort(digits, pivot, sizeof(int), srt);

  if (digits[counter-1] == 0) {
    return -1;
  }

  // Rebuild n from digits
  n = 0;
  for (int i = counter - 1; i >= 0; i--) {
    n += digits[i];
    if (i > 0) {
      n *= 10;
    }
  }

  return n;
}
