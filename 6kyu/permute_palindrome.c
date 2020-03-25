#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

extern char* strdup(const char*);
#include <string.h>

bool permute_a_palindrome(const char *str_in) {
  // Edge test case
  if (str_in == NULL) {
    return true;
  }
  
  char *mine = strdup(str_in);
  char *next = NULL;
  int len = strlen(mine);
  bool can_have_middle = len % 2 == 1;
  bool has_seen_middle = false;
  bool retval = true;
  
  if (len == 0 || len == 1) {
    goto finished;
  }
  
  for (int i = 0; i <= len; i++) {
     if (mine[i] == '0') {
       continue;
     }
     
     if ((next = strchr(&mine[i+1], mine[i])) != NULL) {
       mine[i] = '0';
       *next = '0';
     } else if (can_have_middle) {
       if (has_seen_middle) {
         retval = false;
         goto finished;
       } else {
         has_seen_middle = true;
         mine[i] = '0';
       }
     } else {
       retval = false;
       goto finished;
     }
  }
  
finished:
  free(mine);
  return retval;
}
