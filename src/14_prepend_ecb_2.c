#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/blocks.c"
#include "../libs/random.c"

const unsigned char *key;
char *secret;
size_t secret_len;
size_t prefix_len;
unsigned char *prefix;

/*
 * Encrypt some data appended to a random prefix and prepended to a secret in
 * ECB mode
 */
Bytes appendEncryptECB2(char *data, size_t data_len) {
  Bytes out = {NULL, 0};

  size_t inp_len = prefix_len + data_len + secret_len;

  char *inp = malloc(inp_len * sizeof(char));
  if (!inp) {
    printf("Error: appendEncryptECB2 could not allocate memory.\n");
    return out;
  }
  memcpy(inp, prefix, prefix_len);
  memcpy(inp + prefix_len, data, data_len);
  memcpy(inp + prefix_len + data_len, secret, secret_len);

  out = aesEncryptECB(inp, inp_len, key, 16);
  free(inp);

  return out;
}

/*
 * Take some amount of A's followed by a given suffix for a total of `len`
 * chars, and use them as data to appendEncrypt.
 *
 * Handle a prefix by padding out its block then discarding its ciphertext
 */
Bytes probeEncrypt2(size_t len, char *suffix, size_t suffix_len,
                    size_t padding_len) {
  Bytes out = {NULL, 0};

  char *inp = malloc(len + padding_len * sizeof(char));
  if (!inp) {
    printf("Error: probeEncrypt2 could not allocate memory.\n");
    return out;
  }
  memset(inp, 'A', len + padding_len - suffix_len);
  memcpy(inp + len + padding_len - suffix_len, suffix, suffix_len);

  out = appendEncryptECB2(inp, len + padding_len);
  free(inp);

  // Prepare ciphertext without prefix block
  size_t new_len = out.len - 16;
  unsigned char *new = malloc(new_len);
  memcpy(new, out.data + 16, new_len);
  free(out.data);

  return (Bytes){new, new_len};
}

int main() {
  srand(time(NULL));
  key = randomBytes(16);

  Bytes zero_enc, enc, saved = {NULL, 0};
  int block_size;
  size_t result_len;
  char *base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkga"
                 "GFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdX"
                 "N0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/"
                 "IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

  prefix_len = randomIntInclusive(2, 12);
  prefix = randomBytes(prefix_len);

  secret_len = base64Decode(base64, strlen(base64), &secret);

  // Determine length of the prefix by seeing which two encrypted blocks are
  // equal first
  int i;
  size_t padding_len;
  unsigned char **blocks = NULL;
  for (i = 32; i < 64; i++) {
    enc = probeEncrypt2(i, 0, 0, 0);
    int num_blocks = chunk(enc.data, enc.len, 16, &blocks);

    for (int j = 0; j < num_blocks - 1; j++) {
      if (!memcmp(blocks[j], blocks[j + 1], 16)) {
        goto breakloop;
      }
    }
  }
breakloop:
  padding_len = i % 16;
  free(blocks);
  printf("Prefix length: %zu\n", 16 - padding_len);

  zero_enc =
      probeEncrypt2(0, 0, 0, padding_len); // Encryption of just the secret
  printf("Ciphertext length: %zu\n", zero_enc.len);

  // Encrypt with A's until output length changes
  for (i = 0; i < 32; i++) {
    enc = probeEncrypt2(i, 0, 0, padding_len);

    block_size = enc.len - zero_enc.len;
    if (block_size > 0)
      break;
  }
  printf("Block size: %d\n", block_size);

  result_len = zero_enc.len - i;

  printf("Secret length: %zu\n", result_len);

  // Detect AES mode
  char ones[64] = {1};
  enc = appendEncryptECB2(ones, 64);
  char *detected_mode = aes_oracle(enc.data, block_size);
  printf("%s mode\n\n", detected_mode);

  char result[result_len];

  for (size_t i = 0; i < result_len; i++) {
    size_t sz = nextBlock(i, block_size);

    // Save encryption with next letter of secret
    saved = probeEncrypt2(sz - i - 1, 0, 0, padding_len);

    // Try ASCII chars until we match that encryption
    for (unsigned char ch = 1; ch <= 128; ch++) {
      result[i] = ch;
      result[i + 1] = 0;
      enc = probeEncrypt2(sz, result, i + 1, padding_len);
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
