#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../libs/aes.c"
#include "../libs/random.c"

Bytes randomPrefixSuffix(char *data, size_t data_len) {
  Bytes out = {NULL, 0};
  size_t prefix_len = randomIntInclusive(5, 10);
  size_t suffix_len = randomIntInclusive(5, 10);
  unsigned char *prefix = randomBytes(prefix_len);
  unsigned char *suffix = randomBytes(suffix_len);

  size_t total_len = prefix_len + data_len + suffix_len;
  unsigned char *str = malloc(total_len * sizeof(unsigned char));
  if (!str) {
    printf("Error: randomPrefixSuffix could not allocate memory.\n");
    free(prefix);
    free(suffix);
    return out;
  }
  memcpy(str, prefix, prefix_len);
  memcpy(str + prefix_len, data, data_len);
  memcpy(str + prefix_len + data_len, suffix, suffix_len);

  free(prefix);
  free(suffix);

  return (Bytes){str, total_len};
}

int main() {
  srand(time(NULL));

  Bytes data, enc;
  char *mode, *detected_mode;
  char ones[64] = {1};
  unsigned char iv[16] = {0};

  for (int i = 1; i <= 10000; i++) {
    data = randomPrefixSuffix(ones, 64);

    if (rand() % 2) {
      mode = "ECB";
      enc = aesEncryptECB((char *)data.data, data.len, randomBytes(16), 16);
    } else {
      mode = "CBC";
      enc = aesEncryptCBC((char *)data.data, data.len, randomBytes(16), 16, iv);
    }

    detected_mode = aes_oracle(enc.data, 16);
    /* printf("%d: Was %s, detected %s\n", i, mode, detected_mode); */
    if (mode != detected_mode) {
      printf("Fail");
      free(data.data);
      free(enc.data);
      return -1;
    }
  }
  printf("All ciphertexts correctly detected\n");

  free(data.data);
  free(enc.data);

  return 0;
}
