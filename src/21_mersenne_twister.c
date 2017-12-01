#include <inttypes.h>
#include <stdio.h>

#include "../libs/mt19937.c"

int main() {
  uint32_t seed = 5489;
  uint32_t known_vals[] = {-795755684,  581869302,  -404620562, -708632711,
                           545404204,   -133711905, -372047867, 949333985,
                           -1579004998, 1323567403};

  mt19937_state mt = mt19937_initialise(seed);

  for (int i = 0; i < 10; i++) {
    uint32_t extracted = mt19937_extract(&mt);
    printf("%" PRIu32 " ", extracted);
    if (extracted != known_vals[i]) {
      printf("does not match known value of %" PRIu32, known_vals[i]);
      return -1;
    }
  }

  return 0;
}
