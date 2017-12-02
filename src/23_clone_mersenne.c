#include <inttypes.h>
#include <stdio.h>

#include "../libs/mt19937.c"

int main() {
  uint32_t state[mt19937_N];

  mt19937_state mt = mt19937_initialise(0xBADF00D);
  for (int i = 0; i < mt19937_N; i++) {
    state[i] = mt19937_extract(&mt);
    state[i] = mt19937_untemper(state[i]);
  }

  mt19937_state cloned = mt19937_initialise(0);
  memcpy(cloned.mt, state, sizeof state);

  for (int i = 0; i < mt19937_N; i++) {
    uint32_t x1 = mt19937_extract(&mt);
    uint32_t x2 = mt19937_extract(&cloned);
    if (x1 != x2) {
      printf("%d: %" PRIu32 " does not match %" PRIu32, i, x1, x2);
      return -1;
    }
  }
  printf(
      "Success: cloned generator produced same output as original generator\n");

  return 0;
}
