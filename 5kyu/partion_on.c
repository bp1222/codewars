#include <stdbool.h>
#include <stddef.h>
#include <malloc.h>

size_t partition_on(void *base, size_t nmemb, size_t size, bool (*predicate)(const void *)) {
  void *truthy = malloc(nmemb * size);
  void *falsy = malloc(nmemb * size);
  int truthy_num = 0, falsy_num = 0;


  for (int i = 0; i < nmemb; i++) {
    if (predicate(base + (i * size))) {
      memcpy(truthy + (truthy_num++ * size), base + (i * size), size);
    } else {
      memcpy(falsy + (falsy_num++ * size), base + (i * size), size);
    }
  }

  for (int i = 0; i < falsy_num; i++) {
    memcpy(base + (i * size), falsy + (i * size), size);
  }

  for (int i = falsy_num; i < falsy_num + truthy_num; i++) {
    memcpy(base + (i * size), truthy + ((i-falsy_num) * size), size);
  }

  return falsy_num;
}
