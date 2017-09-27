#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/file.c"

int main() {
  char *inp = NULL, *decoded = NULL;

  int file_len = readFile("../data/10.txt", &inp);
  if (file_len == -1) {
    printf("Failed to open file, exiting...");
    exit(1);
  }

  int decoded_len = base64Decode(inp, file_len, &decoded);

  char *key = "YELLOW SUBMARINE";
  unsigned char iv[16] = {0};
  Bytes out = aesDecryptCBC((unsigned char *)decoded, decoded_len,
                            (const unsigned char *)key, strlen(key), iv);
  if (out.data) {
    printf("%s", out.data);
    free(out.data);
  }
  if (decoded)
    free(decoded);

  return 0;
}
