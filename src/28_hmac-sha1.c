#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../libs/hash.c"

int main() {
  char *key = "YELLOWSUBMARINE";
  char *data = "Some rise by sin, and some by virtue fall.";
  char *data2 = "Some rise by sin, and some by virtue fall!";

  char *hash1 = hmac_sha1(key, strlen(key), data, strlen(data));
  char *hash2 = hmac_sha1(key, strlen(key), data2, strlen(data2));

  printf("%s\n", hash1);
  printf("%s\n", hash2);
  assert(strcmp(hash1, hash2));
}
