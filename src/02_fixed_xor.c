#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/hex.c"
#include "../libs/xor.c"

int main() {
  char *str1_hex = "1c0111001f010100061a024b53535009181c", *str1_ascii;
  int str1_len = hexToASCII(str1_hex, &str1_ascii);
  char *str2_hex = "686974207468652062756c6c277320657965", *str2_ascii,
       *expected = "746865206b696420646f6e277420706c6179";
  int str2_len = hexToASCII(str2_hex, &str2_ascii);

  unsigned char *xored = NULL;
  char *hex = NULL;

  int res = xor((unsigned char *)str1_ascii, str1_len,
                (unsigned char *)str2_ascii, str2_len, &xored);
  if (res > -1) {
    ASCIIToHex((char *)xored, &hex);

    printf("%s\n%s\n", xored, hex);

    if (strcmp(hex, expected))
      printf("Oh no, not the expected result\n");
  }

  free(str1_ascii);
  free(str2_ascii);
  free(xored);
  free(hex);

  return 0;
}
