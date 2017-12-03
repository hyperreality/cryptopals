#include <stdio.h>
#include <stdlib.h>

#include "../libs/hex.c"
#include "../libs/hash/sph_sha1.h"

char *hmac_sha1(const char *key, size_t key_len, const char *data,
                size_t data_len) {
  sph_sha1_context ctx;
  sph_sha1_init(&ctx);
  sph_sha1(&ctx, key, key_len);
  sph_sha1(&ctx, data, data_len);

  unsigned char hashbuf[20];
  sph_sha1_close(&ctx, hashbuf);

  char *hash_hex = NULL;
  int res = toHex(hashbuf, 20, &hash_hex);
  if (res == -1) {
    printf("Fatal error in toHex");
    exit(-1);
  }

  return hash_hex;
}
