#define MEMSIZE 30000

void brainfuck(const char *code, const char *input, char *output) {
  // Memory Tape
  unsigned char memory[MEMSIZE] = {0};
  unsigned char *mptr = memory;
  
  while (*code != (char)0) {
    switch (*code) {
      case '>':
        ++mptr; break;
      case '<':
        --mptr; break;
      case '+':
        ++*mptr; break;
      case '-':
        --*mptr; break;
      case '.':
        *output++ = *mptr; break;
      case ',':
        *mptr = *input++; break;
      case '[':
        if (*mptr == (char)0) {
          int loops = 1;
          do {
            code++;
            switch (*code) {
              case '[':
                ++loops; break;
              case ']':
                --loops; break;
            }
          } while (loops > 0);
        }
        break;
      case ']':
        if (*mptr != (char)0) {
          int loops = 1;
          do {
            --code;
            switch (*code) {
              case '[':
                --loops; break;
              case ']':
                ++loops; break;
            }
          } while (loops > 0);
        }
        break;
    }
    ++code;
  }
  *output = '\0';
}
