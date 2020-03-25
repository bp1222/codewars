#include <stdlib.h>
#include <string.h>

const char *morse[55] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-.-.-", "--..--", "..--..", ".----.", "-.-.--", "-..-.", "-.--.", "-.--.-", ".-...", "---...", "-.-.-.", "-...-", ".-.-.", "-....-", "..--.-", ".-..-.", "...-..-", ".--.-.", "...---..."};  
const char *ascii[55] = {"A",  "B",    "C",    "D",   "E", "F",    "G",   "H",    "I",  "J",    "K",   "L",    "M",  "N",  "O",   "P",    "Q",    "R",   "S",   "T", "U",   "V",    "W",   "X",    "Y",    "Z",    "0",     "1",     "2",     "3",     "4",     "5",     "6",     "7",     "8",     "9",     ".",      ",",      "?",      "'",      "!",      "/",     "(",     ")",      "&",     ":",      ";",      "=",     "+",     "-",      "_",      "\"",     "$",       "@",      "SOS"};

int find_word(char* word) {
  int word_len = strlen(word);
  int morse_len;
  int big_len;

  for (int i = 0; i < 55; i++) {
    morse_len = strlen(morse[i]);
    big_len = word_len > morse_len ? word_len : morse_len;
    if (strncmp(morse[i], word, big_len) == 0) {
      return i;
    }
  }

  return 0;
}


char *decode_morse(const char* morse_code) {
  int start = 0, end = strlen(morse_code), wp = 0;
  char *retval = (char*)calloc(1, sizeof(char) * 1024);
  char word[10] = {};
  char *p;

  // Trim leading space
  p = (char*)morse_code;
  while (*p == ' ') { p++; start++; }

  // Trim ending space
  p = (char*)morse_code + end - 1;
  while (*p == ' ') { p--; end--; }

  // Loop looking for the next code
  for (int i = start; i < end; i++) {
    char cur = *(morse_code + i);

    if (cur == ' ') {
      // Flush Word
      if (wp > 0) {
        strcat(retval, ascii[find_word(word)]);
        wp = 0;
        memset(word, 0, 10);
      }

      // See if we're in a space mode
      if (*(morse_code + i + 1) == ' ' && *(morse_code + i + 2) == ' ') {
        i += 2;
        strcat(retval, " ");
      }
    } else {
      word[wp++] = cur;
    }
  }
  strcat(retval, ascii[find_word(word)]);

  return retval;
}
