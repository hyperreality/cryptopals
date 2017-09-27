#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/hex.c"
#include "../libs/xor.c"

int main() {
  char *str_ascii = "Burning 'em, if you ain't quick and nimble\nI go crazy "
                     "when I hear a cymbal", *xored_hex = NULL, *key = "ICE",
       *expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a2"
                   "6226324272765272a282b2f20430a652e2c652a3124333a653e2b202763"
                   "0c692b20283165286326302e27282f";
  unsigned char *xored;

  size_t str_len = strlen(str_ascii), key_len = strlen(key);

  int res = xor((unsigned char *)str_ascii, str_len, (unsigned char *)key, key_len, &xored);
  if (res == -1)
      return -1;
  ASCIIToHex((char *)xored, &xored_hex);

  printf("%s\n%s\n", str_ascii, xored_hex);

  if (strcmp(xored_hex, expected))
    printf("Oh no, not the expected result\n");

  free(xored);
  free(xored_hex);

  return 0;
}
