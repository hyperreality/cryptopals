#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char *randomBytes(size_t num_bytes) {
  unsigned char *bytes = malloc(num_bytes);
  if (!bytes) {
      printf("randomBytes failed to allocate memory\n");
      return NULL;
  }

  for (size_t i = 0; i < num_bytes; i++) {
    bytes[i] = rand();
  }

  return bytes;
}

int randomIntInclusive(int min, int max) {
  int range, result, cutoff;

  if (min >= max)
    return min;

  range = max - min + 1;
  cutoff = (RAND_MAX / range) * range;

  do { // Rejection method
    result = rand();
  } while (result >= cutoff);

  return result % range + min;
}
