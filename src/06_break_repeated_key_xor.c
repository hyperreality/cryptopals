#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/b64.c"
#include "../libs/blocks.c"
#include "../libs/file.c"
#include "../libs/stats.c"
#include "../libs/xor.c"

int main() {
  char *inp = NULL, *decoded = NULL;
  unsigned char *xor_key, *xored = NULL, **blocks = NULL;

  int file_len = readFile("../data/6.txt", &inp);
  if (file_len == -1) {
      printf("Failed to open file, exiting...");
      exit(1);
  }

  size_t decoded_len = base64Decode(inp, file_len, &decoded);

  size_t key_len = findXORKeyLen((unsigned char *)decoded, decoded_len, 2, 40);
  printf("Key length: %zu\n", key_len);

  xor_key = malloc(key_len);
  if (!xor_key)
    return -1;

  size_t block_len = blockTranspose((unsigned char *)decoded, decoded_len, key_len, &blocks);
  printf("Transposed into %zu blocks of %zu bytes.\n", key_len, block_len);

  printf("Key: ");
  size_t i;
  for (i = 0; i < key_len; i++) {
    xor_key[i] = findSingleByteXOR(blocks[i], key_len, frequencyScore);
    printf("%c", xor_key[i]);
  }
  xor_key[i] = 0;
  printf("\n\n");

  int res = xor((unsigned char *)decoded, decoded_len, xor_key, key_len, &xored);
  if (res != -1)
    printf("%s\n", xored);

  free(blocks);
  free(inp);
  free(xored);
  free(xor_key);

  return 0;
}
