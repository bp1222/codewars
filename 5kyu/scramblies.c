#include <stdbool.h>

bool scramble(const char *str1, const char *str2)
{
  int available_chars[26] = {0};
  int str1_len = strlen(str1);
  int str2_len = strlen(str2);
  
  for (int i = 0; i < str1_len; i++) {
    available_chars[str1[i] - 'a']++;
  }
  
  for (int i = 0; i < str2_len; i++) {
    if (available_chars[str2[i] - 'a'] == 0) {
      return false;
    }
    available_chars[str2[i] - 'a']--;
  }

  return true;
}
