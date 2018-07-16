#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../libs/aes.c"
#include "../libs/random.c"

const unsigned char *key;

char *stripSpecialChars(char *inp, size_t inp_len) {
  char *out = malloc(inp_len);
  if (!out)
    return NULL;

  for (size_t i = 0; i < inp_len; i++)
    if (inp[i] == ';' || inp[i] == '=')
      out[i] = ' ';
    else
      out[i] = inp[i];

  return out;
}

Bytes stripEncryptCTR(char *data, size_t data_len) {
  Bytes out = {NULL, 0};

  char *prefix = "comment1=cooking%20MCs;userdata=";
  char *suffix = ";comment2=%20like%20a%20pound%20of%20bacon";
  size_t prefix_len = strlen(prefix);
  size_t suffix_len = strlen(suffix);

  size_t inp_len = data_len + prefix_len + suffix_len;
  char *inp = malloc(inp_len * sizeof(char));
  if (!inp) {
    printf("Error: stripEncryptCTR could not allocate memory.\n");
    return out;
  }
  memcpy(inp, prefix, prefix_len);
  memcpy(inp + prefix_len, data, data_len);
  memcpy(inp + prefix_len + data_len, suffix, suffix_len);

  char *stripped = stripSpecialChars(inp, inp_len);
  if (!stripped) {
    printf("Error: stripSpecialChars could not allocate memory.\n");
    free(inp);
    return out;
  }

  out = aesCTR((unsigned char *)stripped, inp_len, key, 16, 0);
  free(inp);
  free(stripped);

  return out;
}

int main() {
  srand(time(NULL));
  key = randomBytes(16);

  Bytes enc, dec;

  char *data = ":admin<true:";

  enc = stripEncryptCTR(data, 12);
  if (!enc.data)
      return -1;

  enc.data[32] ^= 0x01;
  enc.data[32 + 6] ^= 0x01;
  enc.data[32 + 11] ^= 0x01;

  dec = aesCTR(enc.data, enc.len, key, 16, 0);
  if (!dec.data) {
      free(enc.data);
      return -1;
  }

  char *target = ";admin=true;";
  if (contains(dec.data, dec.len, target)) {
    printf("Success: %s in decrypted data\n", target);
  } else {
    printf("%.*s did not include %s\n", (int)dec.len, dec.data, target);
  }

  free(enc.data);
  free(dec.data);

  return 0;
}
