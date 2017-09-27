#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/aes.c"

int main() {
  char *str = "YELLOW SUBMARINE";
  printf("%s\n", pkcs7(str, strlen(str), 16).data);

  str = "YELLOW SUBMARIN";
  printf("%s\n", pkcs7(str, strlen(str), 16).data);

  str = "YELLOW SUBMARIN";
  printf("%s\n", pkcs7(str, strlen(str), 32).data);

  return 0;
}
