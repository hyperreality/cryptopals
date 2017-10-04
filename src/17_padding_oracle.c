#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/blocks.c"
#include "../libs/random.c"

#define LINE_LENGTH 81

unsigned char *key;
size_t key_len;
unsigned char iv[16] = {0};

int isValidPadding(unsigned char *enc, size_t enc_len) {
  Bytes dec = aesDecryptCBC((unsigned char *)enc, enc_len,
                            (const unsigned char *)key, key_len, iv);

  if (NULL == dec.data)
    return 0;

  free(dec.data);
  return 1;
}

Bytes padding_attack(unsigned char *enc, size_t enc_len, unsigned char *iv) {
  Bytes out = {NULL, 0};
  unsigned char **blocks = NULL;
  unsigned char *prev_block = iv;
  unsigned char attack_block[32] = {0};

  char *plaintext = malloc(1 + (enc_len * sizeof(char)));
  if (!plaintext) {
    printf("Error: attack could not allocate memory.\n");
    return out;
  }
  plaintext[enc_len] = 0;

  size_t num_blocks = chunk(enc, enc_len, 16, &blocks);

  for (size_t block = 0; block < num_blocks; block++) {
    // Arrange current block in position
    memcpy(attack_block + 16, blocks[block], 16);

    // For each byte position in the block
    for (size_t i = 0; i < 16; i++) {
      // Start from 0x01
      char pad_byte = i + 1;

      // If not last byte, we need to setup all j last bytes of the attack block
      for (size_t j = 0; j < i; j++)
        attack_block[15 - j] =
            prev_block[15 - j] ^ plaintext[(16 * block) + 15 - j] ^ pad_byte;

      // Try every possible padding byte
      for (int byte = 0; byte < 256; byte++) {
        attack_block[15 - i] = byte;

        if (isValidPadding(attack_block, 32)) {
          char decrypted = prev_block[15 - i] ^ byte ^ pad_byte;
          int index = (16 * block) + 15 - i;
          plaintext[index] = decrypted;
          break;
        }
      }
    }

    // This block is now the previous block, used for XORing
    memcpy(prev_block, blocks[block], 16);
  }

  out = stripPKCS7((unsigned char *)plaintext, enc_len);

  return out;
}

int main() {
  char *decoded = NULL, line[LINE_LENGTH];
  Bytes encrypted, decrypted;
  key = randomBytes(16);
  key_len = 16;

  FILE *file = fopen("../data/17.txt", "r");
  if (!file) {
    printf("Failed to open");
    return -1;
  }

  for (int line_no = 1; fgets(line, LINE_LENGTH, file); line_no++) {
    int decoded_len = base64Decode(line, strlen(line), &decoded);

    encrypted = aesEncryptCBC(decoded, decoded_len, (const unsigned char *)key,
                              key_len, iv);

    if (encrypted.data) {
      decrypted = padding_attack(encrypted.data, encrypted.len, iv);
      /* printf("%s\n", decoded); */
      printf("%s\n", decrypted.data);
      if (strncmp((char *)decrypted.data, decoded, decoded_len))
        printf("Fail\n");

      free(encrypted.data);
      free(decrypted.data);
    }
  }
  free(decoded);

  return 0;
}
