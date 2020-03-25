#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *camel_case(const char *s)
{
  char *output = calloc(1, strlen(s));
  char *opos = output;
  bool caps_next = true;
  
  for (; *s; ++s) {
    if (*s == ' ') {
      caps_next = true;
    } else {
      if (caps_next) {
        *opos = toupper(*s);
      } else {
        *opos = *s;
      }
      opos++;
      caps_next = false;
    }
  }
  
  return output;
}
