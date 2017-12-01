#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"
#include "../libs/b64.c"

int main() {
  char *inp = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/"
              "kXX0KSvoOLSFQ==",
       *decoded = NULL;

  int decoded_len = base64Decode(inp, strlen(inp), &decoded);

  uint64_t nonce = 0;
  Bytes out = aesCTR((unsigned char *)decoded, decoded_len,
                     (const unsigned char *)"YELLOW SUBMARINE", 16, nonce);
  if (out.data) {
    printf("%s", out.data);
    free(out.data);
  }
  free(decoded);

  return 0;
}
