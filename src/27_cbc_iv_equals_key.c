#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../libs/aes.c"
#include "../libs/random.c"

unsigned char *key;
unsigned char *iv;

int high_ascii(unsigned char *inp, size_t inp_len) {
  for (size_t i = 0; i < inp_len; i++)
    if (inp[i] & 0x80)
      return 1;
  return 0;
}

Bytes validate(Bytes in) {
  Bytes dec = aesDecryptCBC(in.data, in.len, key, 16, iv);
  if (!dec.data) {
    return (Bytes){NULL, 0};
  }
  if (high_ascii(dec.data, dec.len)) {
    return dec;
  }

  free(dec.data);
  return (Bytes){NULL, 0};
}

int main() {
  srand(time(NULL));
  key = (unsigned char *)"ITSNOTCOMINGHOME";
  iv = key;

  Bytes enc, dec;

  while (1) {
    printf(".");

    unsigned char *random_data = randomBytes(16);
    unsigned char attack_blocks[48];
    Bytes payload = {attack_blocks, sizeof attack_blocks};
    memcpy(attack_blocks, random_data, AES_BLOCK_SIZE);
    memset(attack_blocks + AES_BLOCK_SIZE, 0, AES_BLOCK_SIZE);
    memcpy(attack_blocks + AES_BLOCK_SIZE + AES_BLOCK_SIZE, random_data, AES_BLOCK_SIZE);

    Bytes valid = validate(payload);
    if (valid.data == NULL)
      continue;

    for (size_t i = 0; i < AES_BLOCK_SIZE; i++)
      valid.data[i] ^= valid.data[i + AES_BLOCK_SIZE * 2];

    printf("\n%.16s\n", valid.data);
    free(valid.data);

    break;
  }

  return 0;
}
