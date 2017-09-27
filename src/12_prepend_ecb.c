#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/random.c"

const unsigned char *key;
char *secret;
size_t secret_len;

/*
 * Encrypt some data prepended to a secret in ECB mode
 */
Bytes appendEncryptECB(char *data, size_t data_len) {
  Bytes out = {NULL, 0};
  size_t inp_len = data_len + secret_len;
  char *inp = malloc(inp_len * sizeof(char));
  if (!inp) {
    printf("Error: appendEncryptECB could not allocate memory.\n");
    return out;
  }
  memcpy(inp, data, data_len);
  memcpy(inp + data_len, secret, secret_len);

  out = aesEncryptECB(inp, inp_len, key, 16);
  free(inp);

  return out;
}

/*
 * Take some amount of A's followed by a given suffix for a total of `len`
 * chars, and use them as data to appendEncrypt
 */
Bytes probeEncrypt(size_t len, char *suffix, size_t suffix_len) {
  Bytes out = {NULL, 0};
  char *inp = malloc(len * sizeof(char));
  if (!inp) {
    printf("Error: probeEncrypt could not allocate memory.\n");
    return out;
  }
  memset(inp, 'A', len - suffix_len);
  memcpy(inp + len - suffix_len, suffix, suffix_len);
  /* printf("%s\n", inp); */

  out = appendEncryptECB(inp, len);
  free(inp);

  return out;
}

int main() {
  srand(time(NULL));
  key = randomBytes(16);

  Bytes zero_enc, enc, saved = {NULL,0};
  int block_size;
  size_t result_len;
  char *base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkga"
                 "GFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdX"
                 "N0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/"
                 "IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
  secret_len = base64Decode(base64, strlen(base64), &secret);

  zero_enc = appendEncryptECB("", 0); // Encryption of just the secret
  printf("Ciphertext length: %zu\n", zero_enc.len);

  // Encrypt with A's until output length changes
  int i;
  for (i = 1; i < 32; i++) {
    enc = probeEncrypt(i, 0, 0);

    block_size = enc.len - zero_enc.len;
    if (block_size > 0)
      break;
  }
  result_len = zero_enc.len - i;

  printf("Block size: %d\n", block_size);
  printf("Secret length: %zu\n", result_len);

  // Detect AES mode
  char ones[64] = {1};
  enc = appendEncryptECB(ones, 64);
  char *detected_mode = aes_oracle(enc.data, block_size);
  printf("%s mode\n\n", detected_mode);

  char result[result_len];

  for (size_t i = 0; i < result_len; i++) {
    size_t sz = nextBlock(i, block_size);

    // Save encryption with next letter of secret
    saved = probeEncrypt(sz - i - 1, 0, 0);

    // Try ASCII chars until we match that encryption
    for (unsigned char ch = 1; ch <= 128; ch++) {
      result[i] = ch;
      result[i + 1] = 0;
      enc = probeEncrypt(sz, result, i + 1);
      if (!memcmp(enc.data, saved.data, sz)) {
        /* printf("%s\n", result); */
        break;
      }
    }
  }
  printf("%s", result);
  if (strcmp(result, secret))
    printf("\nFail\n");

  free(zero_enc.data);
  free(enc.data);
  free(saved.data);

  return 0;
}
