#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../libs/mt19937.c"
#include "../libs/random.c"

#define MIN_DELAY 4
#define MAX_DELAY 100

void delayBetween(int min, int max) {
  int wait = randomIntInclusive(min, max);
  printf("Sleeping %d seconds...\n", wait);
  sleep(wait);
}

uint32_t outputForSeed(uint32_t seed) {
  mt19937_state mt = mt19937_initialise(seed);
  return mt19937_extract(&mt);
}

int main() {
  srand(time(NULL));

  delayBetween(MIN_DELAY, MAX_DELAY);
  int seed = (int)time(NULL);
  uint32_t first = outputForSeed(seed);
  printf("Seeded PRNG\n");

  delayBetween(MIN_DELAY, MAX_DELAY);
  int end = (int)time(NULL);

  uint32_t current;
  for (current = end; outputForSeed(current) != first; current--)
    if (current % 10 == 0)
      printf(".");

  printf("\nFound seed of %d", current);

  return 0;
}
