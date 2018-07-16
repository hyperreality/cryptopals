#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
      unsigned char *data;
      size_t len;
} Bytes;

int contains(const unsigned char *data, size_t data_len, const char *expr) {
  size_t expr_len = strlen(expr);
  if (expr_len > data_len)
    return 0;

  for (size_t i = 0; i < data_len - expr_len; i++) {
    if (memcmp(data + i, expr, expr_len) == 0)
      return 1;
  }

  return 0;
}

