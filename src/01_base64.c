#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/b64.c"
#include "../libs/hex.c"

int main() {
  char input[] = "49276d206b696c6c696e6720796f757220627261696e206c696b652061207"
                 "06f69736f6e6f7573206d757368726f6f6d",
       expected[] =
           "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t",
       *ascii = NULL;
  size_t str_len = hexToASCII(input, &ascii);

  char *base64 = NULL;
  base64Encode(ascii, str_len, &base64);

  printf("%s\n %s\n", ascii, base64);

  if (strcmp(base64, expected))
    printf("Oh no, not the expected result\n");

  free(ascii);
  free(base64);

  return 0;
}
