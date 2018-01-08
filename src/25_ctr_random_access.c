#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/file.c"
#include "../libs/random.c"

Bytes edit(Bytes ciphertext, const unsigned char *key, int offset,
           char *newtext, size_t newtext_len) {
  Bytes out = {NULL, 0};

  Bytes plain = aesCTR(ciphertext.data, ciphertext.len, key, AES_BLOCK_SIZE, 0);
  Bytes prefix = {plain.data, offset};
  Bytes suffix = {plain.data + offset + newtext_len,
                  plain.len - offset - newtext_len};

  char *buf = malloc(ciphertext.len * sizeof(char));
  if (!buf) {
    printf("Error: edit could not allocate memory.\n");
    goto cleanup;
  }
  memcpy(buf, prefix.data, prefix.len);
  memcpy(buf + prefix.len, newtext, newtext_len);
  memcpy(buf + prefix.len + newtext_len, suffix.data, suffix.len);

  out = aesCTR((const unsigned char *)buf, ciphertext.len, key, AES_BLOCK_SIZE,
               0);
  if (!out.data)
    printf("Error: edit could not allocate memory.\n");
  goto cleanup;

cleanup:
  free(plain.data);
  free(buf);

  return out;
}

int main() {
  srand(time(NULL));
  const unsigned char *randKey = randomBytes(AES_BLOCK_SIZE);
  const uint64_t nonce = 0;

  char *inp = NULL, *decoded = NULL;

  int file_len = readFile("../data/7.txt", &inp);
  if (file_len == -1) {
    printf("Failed to open file, exiting...");
    exit(1);
  }

  int decoded_len = base64Decode(inp, file_len, &decoded);

  Bytes out = aesDecryptECB((unsigned char *)decoded, decoded_len,
                            (const unsigned char *)"YELLOW SUBMARINE", 16);
  if (!out.data)
    return -1;

  Bytes ctrd = aesCTR(out.data, out.len, randKey, 16, nonce);
  if (!ctrd.data)
    return -1;

  char *zeroes = malloc(ctrd.len * sizeof(char));
  if (!zeroes) {
    printf("Error: could not allocate memory.\n");
    return -1;
  }
  memset(zeroes, 0, ctrd.len);

  // Recover original keystream
  Bytes keystream = edit(ctrd, randKey, 0, zeroes, ctrd.len);

  // Then XOR it with the ciphered text
  unsigned char *recovered;
  int res = xor(keystream.data, keystream.len, ctrd.data, ctrd.len, &recovered);
  if (res == -1)
    return -1;

  printf("%s", recovered);

  free(decoded);
  free(out.data);
  free(ctrd.data);
  free(keystream.data);

  return 0;
}
