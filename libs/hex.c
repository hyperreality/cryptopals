#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hexToInt(char c) {
  int first = c / 16 - 3;
  int second = c % 16;
  int out = first * 10 + second;

  if (out > 9)
    out--;
  if (out > 29) // lowercase
    out -= 20;

  return out;
}

int hexToASCII(const char *hex, char **out) {
  size_t hex_len = strlen(hex);
  size_t out_len = hex_len / 2;

  *out = malloc(out_len + 1);
  if (out == NULL)
    return -1;

  char buf = 0;
  size_t i;
  for (i = 0; i < hex_len; i++) {
    if (i % 2 != 0) {
      int high = hexToInt(buf) * 16;
      int low = hexToInt(hex[i]);
      (*out)[i / 2] = high + low;
    } else {
      buf = hex[i];
    }
  }
  (*out)[i] = 0;

  return out_len;
}

char hexDigit(unsigned n) {
  if (n < 10) {
    return n + '0';
  } else {
    return (n - 10) + 'a';
  }
}

int ASCIIToHex(const char *ascii, char **out) {
  size_t inp_len = strlen((char *)ascii), out_len = inp_len * 2;

  *out = (char *)malloc(out_len + 1);
  if (out == NULL)
    return -1;

  size_t i;
  for (i = 0; i < inp_len; i++) {
    (*out)[i * 2] = hexDigit(ascii[i] / 0x10);
    (*out)[i * 2 + 1] = hexDigit(ascii[i] % 0x10);
  }

  (*out)[i * 2] = 0;

  return out_len;
}

int toHex(const unsigned char *data, size_t data_len, char **out) {
  size_t out_len = data_len * 2;

  *out = (char *)malloc(out_len + 1);
  if (out == NULL)
    return -1;

  size_t i;
  for (i = 0; i < data_len; i++) {
    (*out)[i * 2] = hexDigit(data[i] / 0x10);
    (*out)[i * 2 + 1] = hexDigit(data[i] % 0x10);
  }

  (*out)[i * 2] = 0;

  return out_len;
}
