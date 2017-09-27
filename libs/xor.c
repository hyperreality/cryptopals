#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int xor (const unsigned char *str1, const size_t str1_len,
     const unsigned char *str2, const size_t str2_len, unsigned char **out) {
  *out = malloc(str1_len + 1);
  if (out == NULL)
    return -1;

  size_t i = 0;
  for (; i < str1_len; i++) {
    (*out)[i] = str1[i] ^ str2[i % str2_len];
  }

  (*out)[i] = 0;

  return 0;
}

char findSingleByteXOR(const unsigned char *inp, const size_t inp_len,
                       int (*score_function)(const unsigned char *,
                                             const size_t)) {
  unsigned char *buffer;
  int current_best_key = 0, best_score = 0, current_score;

  for (unsigned char i = 0; i < 255; i++) {
    unsigned char key_arr[] = {i};

    xor(inp, inp_len, key_arr, 1, &buffer);

    current_score = score_function(buffer, inp_len);

    if (current_score > best_score) {
      current_best_key = i;
      best_score = current_score;
    }
  }

  return current_best_key;
}

int hammingDistance(const unsigned char *str1, const size_t str1_len,
                    const unsigned char *str2, const size_t str2_len) {
  if (str1_len != str2_len)
    return -1;

  int score = 0, current_xor = 0;

  for (size_t i = 0; i < str1_len; i++) {
    current_xor = str1[i] ^ str2[i];

    do {
      score += current_xor & 1;
    } while (current_xor >>= 1);
  }

  return score;
}

int findXORKeyLen(const unsigned char *str, const size_t str_len,
                  const size_t min_key_len, const size_t max_key_len) {
  size_t distance_len = 0, best_key_len = 0, max_distance,
         best_distance = INT_MAX;

  float distance = 0.0;

  for (size_t key_len = min_key_len; key_len <= max_key_len; key_len++) {
    distance = 0.0;

    max_distance = (str_len / key_len) - 1;

    // Compute hamming distance over each n and n+1 string
    for (distance_len = 0; distance_len < max_distance; distance_len++) {
      int hamming =
          hammingDistance(str + (key_len * distance_len), key_len,
                          str + (key_len * (distance_len + 1)), key_len);
      distance += (float)hamming / key_len;
    }

    // Normalize
    distance /= (float)max_distance;

    if (distance < best_distance) {
      best_distance = distance;
      best_key_len = key_len;
    }
  }

  return best_key_len;
}
