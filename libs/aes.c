#include <endian.h>
#include <openssl/aes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xor.c"

#define htole64(x) (x)

typedef struct {
  unsigned char *data;
  size_t len;
} Bytes;

int nextBlock(int length, size_t block_size) {
  return (length + block_size) & ~(block_size - 1);
}

Bytes pkcs7(const char *data, const size_t data_len, size_t block_size) {
  Bytes out = {NULL, 0};
  unsigned char *res;

  size_t out_len = nextBlock(data_len, block_size);

  res = malloc(out_len * sizeof(unsigned char));
  if (!res) {
    printf("Error: pkcs7 could not allocate memory.\n");
    return out;
  }

  unsigned char pad_len = out_len - data_len;

  memcpy(res, data, data_len);
  for (size_t i = data_len; i < out_len; i++) {
    res[i] = pad_len;
  }

  return (Bytes){res, out_len};
}

Bytes stripPKCS7(unsigned char *data, size_t data_len) {
  Bytes out = {NULL, 0};
  unsigned char *res;

  size_t pad_len = data[data_len - 1];
  size_t out_len = data_len - pad_len;

  res = malloc(out_len * sizeof(unsigned char));
  if (!res) {
    printf("Error: stripPKCS7 could not allocate memory.\n");
    return out;
  }

  memcpy(res, data, out_len);

  return (Bytes){res, out_len};
}

int validPKCS7(unsigned char *data, size_t data_len) {
  unsigned char *res;

  size_t pad_len = data[data_len - 1];

  if (pad_len == 0 || pad_len > data_len)
    return 0;

  for (size_t i = 1; i < pad_len; i++) {
    if (pad_len != data[data_len - 1 - i])
      return 0;
  }

  return 1;
}

Bytes validateAndStrip(unsigned char *data, size_t data_len) {
  Bytes out = {NULL, 0};

  if (validPKCS7(data, data_len))
    out = stripPKCS7(data, data_len);

  return out;
}

Bytes aesEncryptECB(char *data, const size_t data_len, const unsigned char *key,
                    const size_t key_len) {
  Bytes out = {NULL, 0};
  unsigned char *res;

  if (!data) {
    printf("Error: aesEncryptECB data input is NULL.\n");
    return out;
  } else if (!key) {
    printf("Error: aesEncryptECB key input is NULL.\n");
    return out;
  }

  AES_KEY aesKey;
  AES_set_encrypt_key(key, key_len * 8, &aesKey);

  Bytes padded = pkcs7(data, data_len, AES_BLOCK_SIZE);
  if (!padded.data) {
    printf("Error: aesEncryptECB padding failed.\n");
    return out;
  }

  res = calloc(padded.len, sizeof(unsigned char));
  if (!res) {
    printf("Error: aesEncryptECB could not allocate memory.\n");
    return out;
  }

  for (size_t count = 0; count < padded.len; count += AES_BLOCK_SIZE) {
    AES_ecb_encrypt(padded.data + count, res + count, &aesKey, AES_ENCRYPT);
  }

  free(padded.data);

  return (Bytes){res, padded.len};
}

Bytes aesDecryptECB(const unsigned char *data, size_t data_len,
                    const unsigned char *key, size_t key_len) {
  Bytes out = {NULL, 0};

  if (!data) {
    printf("Error: aesDecryptECB data input is NULL.\n");
    return out;
  } else if (!key) {
    printf("Error: aesDecryptECB key input is NULL.\n");
    return out;
  }

  unsigned char *decrypted = calloc(data_len + 1, sizeof(unsigned char));
  if (!decrypted) {
    printf("Error: aesDecryptECB could not allocate memory.\n");
    return out;
  }

  AES_KEY aesKey;
  AES_set_decrypt_key(key, key_len * 8, &aesKey);

  for (size_t count = 0; count < data_len; count += AES_BLOCK_SIZE) {
    AES_ecb_encrypt(data + count, decrypted + count, &aesKey, AES_DECRYPT);
  }

  if (data_len != 0)
      out = stripPKCS7(decrypted, data_len);

  free(decrypted);

  return out;
}

Bytes aesEncryptCBC(char *data, const size_t data_len, const unsigned char *key,
                    const size_t key_len, unsigned char *iv) {
  Bytes out = {NULL, 0};
  unsigned char *res = NULL, *xored = NULL;

  if (!data) {
    printf("Error: aesEncryptCBC data input is NULL.\n");
    return out;
  } else if (!key) {
    printf("Error: aesEncryptCBC key input is NULL.\n");
    return out;
  }

  AES_KEY aesKey;
  AES_set_encrypt_key(key, key_len * 8, &aesKey);

  Bytes padded = pkcs7(data, data_len, AES_BLOCK_SIZE);
  if (!padded.data) {
    printf("Error: aesEncryptCBC padding failed.\n");
    return out;
  }

  res = calloc(padded.len, sizeof(unsigned char));
  if (!res) {
    printf("Error: aesEncryptCBC could not allocate memory.\n");
    free(padded.data);
    return out;
  }

  for (size_t count = 0; count < padded.len; count += AES_BLOCK_SIZE) {
    xor(padded.data + count, AES_BLOCK_SIZE, iv, AES_BLOCK_SIZE, &xored);
    AES_ecb_encrypt(xored, res + count, &aesKey, AES_ENCRYPT);
    iv = res + count;
  }

  free(padded.data);
  free(xored);

  return (Bytes){res, padded.len};
}

Bytes aesDecryptCBC(unsigned char *data, size_t data_len,
                    const unsigned char *key, size_t key_len,
                    unsigned char *iv) {
  Bytes out = {NULL, 0};
  unsigned char *xored = NULL;

  if (!data) {
    printf("Error: aesDecryptCBC data input is NULL.\n");
    return out;
  } else if (!key) {
    printf("Error: aesDecryptCBC key input is NULL.\n");
    return out;
  }

  unsigned char *decrypted = calloc(data_len + 1, sizeof(unsigned char));
  if (!decrypted) {
    printf("Error: aesDecryptCBC could not allocate memory.\n");
    return out;
  }
  unsigned char *xorDecrypted = calloc(data_len + 1, sizeof(unsigned char));
  if (!xorDecrypted) {
    printf("Error: aesDecryptCBC could not allocate memory.\n");
    free(decrypted);
    return out;
  }

  AES_KEY aesKey;
  AES_set_decrypt_key(key, key_len * 8, &aesKey);

  for (size_t count = 0; count < data_len; count += AES_BLOCK_SIZE) {
    AES_ecb_encrypt((const unsigned char *)data + count, decrypted + count,
                    &aesKey, AES_DECRYPT);

    xor(decrypted + count, AES_BLOCK_SIZE, iv, AES_BLOCK_SIZE, &xored);
    memcpy(xorDecrypted + count, xored, AES_BLOCK_SIZE);

    iv = (unsigned char *)(data + count);
  }

  out = validateAndStrip(xorDecrypted, data_len);

  free(decrypted);
  free(xorDecrypted);
  free(xored);

  return out;
}

union aes_ctr_counter {
  unsigned char buf[AES_BLOCK_SIZE];

  struct nonce_counter {
    uint64_t nonce;
    uint64_t counter;
  } nc;
};

Bytes aesCTR(const unsigned char *data, size_t data_len,
             const unsigned char *key, size_t key_len, const uint64_t nonce) {
  Bytes out = {NULL, 0};
  unsigned char keystream[AES_BLOCK_SIZE], *xored = NULL;

  if (!data) {
    printf("Error: aesDecryptCTR data input is NULL.\n");
    return out;
  } else if (!key) {
    printf("Error: aesDecryptCTR key input is NULL.\n");
    return out;
  }

  unsigned char *decrypted = calloc(data_len + 1, sizeof(unsigned char));
  if (!decrypted) {
    printf("Error: aesDecryptCTR could not allocate memory.\n");
    return out;
  }

  AES_KEY aesKey;
  AES_set_encrypt_key(key, key_len * 8, &aesKey);

  union aes_ctr_counter ctr_counter = {.nc = {.counter = 0, .nonce = nonce}};

  for (size_t count = 0; count < data_len; count += AES_BLOCK_SIZE) {
    size_t block_len =
        data_len - count < AES_BLOCK_SIZE ? data_len - count : AES_BLOCK_SIZE;

    ctr_counter.nc.counter = htole64(count / AES_BLOCK_SIZE);
    AES_ecb_encrypt(ctr_counter.buf, keystream, &aesKey, AES_ENCRYPT);
    xor(keystream, block_len, data + count, block_len, &xored);

    memcpy(decrypted + count, xored, block_len);
  }
  free(xored);

  return (Bytes){decrypted, data_len};
}

char *aes_oracle(unsigned char *enc, size_t block_size) {
  if (!memcmp(enc + block_size, enc + block_size * 2, block_size))
    return "ECB";
  else
    return "CBC";
}
