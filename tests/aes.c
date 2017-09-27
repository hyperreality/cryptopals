#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/file.c"

int main() {
  int res = 0;
  char *text = "Lorem Ipsum is simply dummy text of the printing and "
               "typesetting industry. Lorem Ipsum has been the industry's "
               "standard dummy text ever since the 1500s, when an unknown "
               "printer took a galley of type and scrambled it to make a type "
               "specimen book. It has survived not only five centuries, but "
               "also the leap into electronic typesetting, remaining "
               "essentially unchanged. It was popularised in the 1960s with "
               "the release of Letraset sheets containing Lorem Ipsum "
               "passages, and more recently with desktop publishing software "
               "like Aldus PageMaker including versions of Lorem Ipsum.";
  size_t text_len = strlen(text);

  char *key = "YELLOW SUBMARINE";
  unsigned char iv[16] = {0};

  Bytes enc =
      aesEncryptECB(text, text_len, (const unsigned char *)key, strlen(key));
  Bytes dec =
      aesDecryptECB(enc.data, enc.len, (const unsigned char *)key, strlen(key));
  if (!memcmp(text, dec.data, text_len)) {
    printf("Pass for ECB encryption\n");
  } else {
    printf("Fail, ECB decoded text was:\n %s\n", dec.data);
    res = -1;
  }

  enc = aesEncryptCBC(text, text_len, (const unsigned char *)key, strlen(key),
                      iv);
  dec = aesDecryptCBC(enc.data, enc.len, (const unsigned char *)key,
                      strlen(key), iv);
  if (!memcmp(text, dec.data, text_len)) {
    printf("Pass for CBC encryption\n");
  } else {
    printf("Fail, CBC decoded text was:\n %s\n", dec.data);
    res = -1;
  }

  free(enc.data);
  free(dec.data);

  return res;
}
