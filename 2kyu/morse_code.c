#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

struct centers {
  int num;
  double val;
};

struct freq {
  int len;
  int count;
  struct centers *best_center;
  struct freq *next;
};
struct freq_map {
  struct freq *head;
  int count;
};

enum bitstr_type {
  HIGH = 0,
  LOW,
};
struct bitstr {
  int len;
  enum bitstr_type type;
  struct freq* freq;
  struct bitstr *next;
};
struct bitstr_map {
  struct bitstr *head;
  int count;
};

void parseFrequency(struct freq_map *freqs, struct bitstr_map *bitstrs) {
  struct freq *f = NULL, *prev = NULL;

  for (struct bitstr *b = bitstrs->head; b != NULL; b = b->next) {
    if (freqs->head == NULL) {
      f = (struct freq*)calloc(1, sizeof(struct freq));
      f->len = b->len;
      f->count = 1;
      f->best_center = NULL;
      f->next = NULL;

      freqs->head = f;
      freqs->count = 1;
    } else {
      for (prev = freqs->head; ; prev = prev->next) {
        if (prev->len == b->len) {
          prev->count++;
          f = prev;
          goto freq_loop;
        }
        if (prev->next == NULL) {
          break;
        }
      }

      f = (struct freq*)calloc(1, sizeof(struct freq));
      f->len = b->len;
      f->count = 1;
      f->best_center = NULL;
      f->next = NULL;

      prev->next = f;
      freqs->count++;
    }

freq_loop:
    b->freq = f;
  }
}

void insertBitstr(struct bitstr_map *bitstrs, int len, enum bitstr_type type) {
  struct bitstr *new = (struct bitstr*)calloc(1, sizeof(struct bitstr));
  new->len = len;
  new->type = type;

  if (bitstrs->head == NULL) {
    bitstrs->head = new;
    bitstrs->head->next = NULL;

    bitstrs->count = 1;
  } else {
    struct bitstr *last;
    for (last = bitstrs->head; ; last = last->next) {
      if (last->next == NULL) {
        break;
      }
    }

    last->next = new;

    bitstrs->count++;
  }
}

void parseBitstr(struct bitstr_map *map, const char *trimmedBits) {
  char *b = (char*)trimmedBits;
  int len = 0;

  while (*b != '\0') {
    len = 0;
    switch (*b) {
      case '1':
        do {
          len++;
          b++;
        } while (*b == '1');
        insertBitstr(map, len, HIGH);
        break;
      case '0':
        do {
          len++;
          b++;
        } while (*b == '0');
        insertBitstr(map, len, LOW);
        break;
    }
  }
}

char* trimBits(const char *bits) {
  const char *start = bits;
  const char *end = bits + strlen(bits);
  char *ret;

  while (*start == '0') ++start;

  if (*start == '\0') {
    ret = (char*)calloc(1, sizeof(char) * 1);
    return ret;
  }

  while (start < end && *(end - 1) == '0') {
    --end;
    if (*(end - 1) == '1') {
      break;
    }
  };

  ret = (char*)calloc(1, sizeof(char) * (end - start) + 1);
  memcpy(ret, start, (end - start));
  return ret;
}

double minFreq(struct freq_map *map) {
  double min = DBL_MAX;
  for (struct freq *n = map->head; n != NULL; n = n->next) {
    if (min > n->len) {
      min = n->len;
    }
  }
  return min;
}


double maxFreq(struct freq_map *map) {
  double max = 0;
  for (struct freq *n = map->head; n != NULL; n = n->next) {
    if (max < n->len) {
      max = n->len;
    }
  }
  return max;
}

void determineMeans(struct freq_map *map, struct centers *c, int num_centers) {
  struct centers *prev_centers = (struct centers*)calloc(1, sizeof(struct centers) * num_centers);
  int max_iter = 10000;

  while (1) {
    memcpy(prev_centers, c, sizeof(struct centers) * num_centers);

    for (struct freq *f = map->head; f != NULL; f = f->next) {
      struct centers *best_center = f->best_center != NULL ? f->best_center : &c[0];
      double distance, best_distance = fabs(f->len - best_center->val);

      for (int i = 0; i < num_centers; i++) {
        distance = fabs(f->len - c[i].val);
        if (distance < best_distance) {
          best_distance = distance;
          best_center = &c[i];
        }
      }

      f->best_center = best_center;
    }

    for (int i = 0; i < num_centers; i++) {
      double sum = 0;
      int freqs = 0;
      for (struct freq *f = map->head; f != NULL; f = f->next) {
        if (f->best_center != &c[i]) {
          continue;
        }
        sum += f->len * f->count;
        freqs += f->count;
      }

      if (freqs) {
        c[i].val = sum / freqs;
      }
    };

    if (memcmp(prev_centers, c, sizeof(double) * num_centers) == 0) {
      break;
    }
    if (max_iter-- <= 0) {
      exit(-1);
    }
  };

  free(prev_centers);
}

char* getMorse(struct freq_map *freqs, struct bitstr_map *bitstrs, double thresh13, double thresh37) {
  char morse[4096] = "";
  double len;

  for (struct bitstr *b = bitstrs->head; b != NULL; b = b->next) {
    len = b->freq->len;
    switch (b->type) {
      case LOW:
        if (thresh13 <= len && len < thresh37) {
          strcat(morse, " ");
        } else if (thresh37 <= len) {
          strcat(morse, "   ");
        }
        break;
      case HIGH:
        if (len <= thresh13) {
          strcat(morse, ".");
        } else if (thresh13 < len) {
          strcat(morse, "-");
        }
        break;
    }
  }

  return strdup(morse);
}

char* decodeBitsAdvanced (const char *bits) {
  char *trimmedBits = trimBits(bits);

  struct bitstr_map bitstrs = {0};
  parseBitstr(&bitstrs, trimmedBits);

  if (bitstrs.count == 0 || (bitstrs.count == 1 && bitstrs.head->type == LOW)) {
    return strdup("");
  }

  struct freq_map freqs = {0};
  parseFrequency(&freqs, &bitstrs);

  struct centers c[3] = {0};
  if (freqs.count == 1 || freqs.count == 2) {
    int min = minFreq(&freqs);
    c[0] = (struct centers){0, min};
    c[1] = (struct centers){1, min * 3};
    c[2] = (struct centers){2, min * 7};
  } else {
    c[0] = (struct centers){0, 0.0};
    c[2] = (struct centers){2, maxFreq(&freqs)};
    c[1] = (struct centers){1, c[2].val / 2.0};
  }

  determineMeans(&freqs, c, 3);

  double thresh13 = (c[0].val + c[1].val) / 2.0;
  double thresh37 = (c[1].val + c[2].val) / 2.0;
  if (bitstrs.count > 5) {
    thresh13 *= 1.1;
    thresh37 *= 1.1;
  } else {
    thresh13 *= 0.9;
    thresh37 *= 0.9;
  }

  char *morse = getMorse(&freqs, &bitstrs, thresh13, thresh37);

  struct freq *f;
  for (struct freq *q = freqs.head; q != NULL; ) {
    f = q->next;
    free(q);
    q = f;
  }

  free(trimmedBits);

  return morse;
}

char *decodeMorse (const char *morseCode) {
  char *out = (char*)calloc(1, sizeof(char) * strlen(morseCode) + 1);
  char *msg = strdup(morseCode);
  char *tok = strtok(msg, " ");
  
  while (tok != NULL) {
    strcat(out, morse_code(tok));
    if ((tok = strtok(NULL, " ")) && *(tok-1) == ' ') {
      strcat(out, " ");
    }
  }
  
  free(msg);
  return out;
}
