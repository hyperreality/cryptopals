#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

#include "../libs/bytes.c"
#include "../libs/mt19937.c"
#include "../libs/random.c"

uint16_t randKey;

Bytes mt19937_encrypt(uint32_t key, const unsigned char *data,
                      size_t data_len) {
  Bytes out = {NULL, 0};
  unsigned char *encrypted = calloc(data_len + 1, sizeof(unsigned char));
  if (!encrypted) {
    printf("Error: could not allocate memory.\n");
    return out;
  }

  mt19937_state mt = mt19937_initialise(key);

  for (size_t i = 0; i < data_len; i++)
    encrypted[i] = data[i] ^ mt19937_extract(&mt);

  return (Bytes){encrypted, data_len};
}

Bytes prefixEncrypt(unsigned char *data, size_t data_len) {
  Bytes out = {NULL, 0};

  size_t prefix_len = randomIntInclusive(2, 12);
  unsigned char *prefix = randomBytes(prefix_len);

  size_t inp_len = prefix_len + data_len;
  unsigned char *inp = malloc(inp_len * sizeof(unsigned char));
  if (!inp) {
    printf("Error: could not allocate memory.\n");
    free(prefix);
    return out;
  }
  memcpy(inp, prefix, prefix_len);
  memcpy(inp + prefix_len, data, data_len);

  randKey = rand() % 0xffff;

  out = mt19937_encrypt(randKey, inp, inp_len);
  free(prefix);
  free(inp);
  return out;
}

int main() {
  srand(time(NULL));

  uint32_t key = 0xBADF00D;
  char *data = "AAAAAAAAAAAAAA";
  size_t data_len = strlen(data);
  Bytes encrypted = mt19937_encrypt(key, (unsigned char *)data, data_len);
  Bytes decrypted = mt19937_encrypt(key, encrypted.data, encrypted.len);
  assert(!memcmp(decrypted.data, (unsigned char *)data, data_len));

  encrypted = prefixEncrypt((unsigned char *)data, data_len);

  size_t plaintextStart = encrypted.len - data_len;

  uint16_t guessedKey = 0;
  for (; guessedKey < 0xffff; guessedKey++) {
    Bytes candidate =
        mt19937_encrypt(guessedKey, encrypted.data, encrypted.len);

    if (!memcmp(candidate.data + plaintextStart, data, data_len))
      break;
  }
  assert(guessedKey == randKey);

  printf("Key 0x%04" PRIx16 " successfully recovered\n", guessedKey);

  return 0;
}
