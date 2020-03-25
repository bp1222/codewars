#include <inttypes.h>

void uint32_to_ip(uint32_t ip, char *output) {
  int oct[4];

  for (int i = 0; i < 4; i++) {
    oct[i] = ip & 0b11111111;
    ip = ip >> 8;
  }

  sprintf(output, "%d.%d.%d.%d", oct[3], oct[2], oct[1], oct[0]);
}
