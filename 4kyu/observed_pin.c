#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char possible[10][6] = {
  /* 0 */ { '0', '8', 0 },
  /* 1 */ { '1', '2', '4', 0 },
  /* 2 */ { '2', '1', '3', '5', 0 },
  /* 3 */ { '3', '2', '6', 0 },
  /* 4 */ { '4', '5', '1', '7', 0 },
  /* 5 */ { '5', '4', '6', '2', '8', 0 },
  /* 6 */ { '6', '5', '3', '9', 0 },
  /* 7 */ { '7', '8', '4', 0 },
  /* 8 */ { '8', '7', '9', '5', '0', 0 },
  /* 9 */ { '9', '8', '6', 0 },
};

void do_merge(char** result, const char *obs, int obs_len, int cur_p, size_t *count, char *tmp) {
  const char *p = (const char*)possible[obs[cur_p] - '0'];
  int p_len = strlen(p);

  // Need to write us and loop
  if (cur_p < obs_len - 1) {
    for (int w = 0; w < p_len; w++) {
      tmp[cur_p] = p[w];
      do_merge(result, obs, obs_len, cur_p + 1, count, tmp);
    }
  } else {
    for (int w = 0; w < p_len; w++) {
      result[*count] = (char*)malloc(sizeof(char) * obs_len + 1);
      memset(result[*count], 0, sizeof(char) * obs_len + 1);
      sprintf(result[*count], "%s%c", tmp, p[w]);
      *count += 1;
    }
  }
}

//Function should return an array of c-strings with all possible PINs.
//It must allocate all required meomry, but there are no requirements
//as to how it should be done. It must be possible to free
//allocated memory with a call to free_pins.
//Upon return, count should contain the amount of returned PINs.
const char** get_pins(const char* observed, size_t* count) {
  int observed_len = strlen(observed);
  long num_pins = 1;
  *count = 0;

  for (int i = 0; i < observed_len; i++) {
    num_pins *= strlen(possible[observed[i] - '0']);
  }

  char **result = (char**)malloc(sizeof(char*) * (num_pins + 1));
  char tmp[observed_len];

  memset(result, NULL, sizeof(char*) * (num_pins + 1));
  memset(tmp, NULL, sizeof(char) * observed_len + 1);

  do_merge(result, observed, observed_len, 0, count, tmp);

  return (const char**)result;
}

//This function will be called once per every call to get_pins
//with a pointer value returned by such call.
void free_pins(const char ** pins) {
  for (char **pin = (char**)pins; *pin != NULL; pin++) {
    free(*pin);
  }
  free(pins);
}
