#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int srt(const void *a, const void *b) {
	int arg1 = *(const int*)a;
	int arg2 = *(const int*)b;
	if (arg1 < arg2) return 1;
	if (arg1 > arg2) return -1;
	return 0;
}

unsigned long long next_bigger_number(unsigned long long n){
	int digits[128] = {0};
	int counter = 0, pivot = -1;

	// Store number into array
	while (n) {
		// Get the next right most
		digits[counter++] = n % 10;
		n /= 10;
	}

	// Find our pre-pivot number
	for (int i = 0; i < 127; i++) {
		if (digits[i] > digits[i+1]) {
			pivot = i + 1;
			break;
		}
	}

	// There was no valid pivot
	if (pivot == -1) {
		return pivot;
	}

	// Find the next lowest number larger than our pivot
	int lowest = INT_MAX, lowest_pos = -1;
	for (int i = 0; i < pivot; i++) {
		if (digits[i] > digits[pivot] && digits[i] < lowest) {
			lowest = digits[i];
			lowest_pos = i;
		}
	}

	// Swap post-pivot with highst
	int temp = digits[pivot];
	digits[pivot] = lowest;
	digits[lowest_pos] = temp;

	// Sort the remainder values low to high
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
