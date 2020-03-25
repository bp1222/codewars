#include <string.h>

char *rot13(const char *src)
{
  size_t len = strlen(src);
  char *out = calloc(1, len);
  
  for (int p = 0; p < len; p++) {
    if (islower(src[p]))
      out[p] = ((src[p] + 13 - 'a') % 26) + 'a';
    else if (isupper(src[p]))
      out[p] = ((src[p] + 13 - 'A') % 26) + 'A';
    else
      out[p] = src[p];
  }
  
  return out;
}
