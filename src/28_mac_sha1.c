#include <stdio.h>
#include <string.h>

#include "../libs/hash.c"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Must supply KEY and INPUT as hex strings\n");
    exit(-1);
  }

  char *key_ascii;
  char *data_ascii;
  int key_len = hexToASCII(argv[1], &key_ascii);
  int data_len = hexToASCII(argv[2], &data_ascii);

  char *hash = mac_sha1(key_ascii, key_len, data_ascii, data_len);
  printf("%s\n", hash);
}
