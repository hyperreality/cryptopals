#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int mod_table[] = {0, 2, 1};

int base64Encode(const char *inp, size_t inp_len, char **out) {
  size_t out_len = 4 * ((inp_len + 2) / 3); // 3-byte blocks to 4-byte
  int padding = mod_table[inp_len % 3];

  *out = (char *)malloc(out_len + 1);
  if (out == NULL)
    return -1;

  for (size_t i = 0, j = 0; i < inp_len;) {
    int octet_a = i < inp_len ? inp[i++] : 0;
    int octet_b = i < inp_len ? inp[i++] : 0;
    int octet_c = i < inp_len ? inp[i++] : 0;

    int triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    (*out)[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
    (*out)[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
    (*out)[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
    (*out)[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
  }

  for (int i = 0; i < padding; i++)
    (*out)[out_len - 1 - i] = '=';

  return out_len;
}

int base64Decode(const char *src, size_t len, char **out) {
  unsigned char dtable[256], in[4], block[4], tmp;
  char *pos;
  size_t i, count, out_len;

  memset(dtable, 0x80, 256);
  for (i = 0; i < sizeof(base64_table); i++)
    dtable[base64_table[i]] = i;
  dtable['='] = 0;

  count = 0;
  for (i = 0; i < len; i++) {
    if (dtable[(int)src[i]] != 0x80)
      count++;
  }

  if (count % 4)
    return -1;

  out_len = count / 4 * 3;
  pos = *out = (char *)malloc(count);
  if (out == NULL)
    return -1;

  count = 0;
  for (i = 0; i < len; i++) {
    tmp = dtable[(int)src[i]];
    if (tmp == 0x80)
      continue;

    in[count] = src[i];
    block[count] = tmp;
    count++;
    if (count == 4) {
      *pos++ = (block[0] << 2) | (block[1] >> 4);
      *pos++ = (block[1] << 4) | (block[2] >> 2);
      *pos++ = (block[2] << 6) | block[3];
      count = 0;
    }
  }

  if (pos > *out) {
    if (in[2] == '=') {
      out_len -= 2;
      pos -= 2;
    } else if (in[3] == '=') {
      out_len--;
      pos--;
    }
  }

  return out_len;
}
