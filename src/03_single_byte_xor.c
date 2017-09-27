#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/hex.c"
#include "../libs/stats.c"
#include "../libs/xor.c"

int main() {
  char *
      str_hex =
         "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736",
     *str_ascii;
  unsigned char *xored = NULL;
  size_t str_len = hexToASCII(str_hex, &str_ascii);

  unsigned char res =
      findSingleByteXOR((unsigned char *)str_ascii, str_len, &frequencyScore);
  if (res > 0) {
    unsigned char key_arr[] = {res};
    xor((unsigned char *)str_ascii, str_len, key_arr, 1, &xored);

    printf("Key: %c\nMsg: %s\n", res, xored);
  }

  free(str_ascii);
  free(xored);

  return 0;
}
