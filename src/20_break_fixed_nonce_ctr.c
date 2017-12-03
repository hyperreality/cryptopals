#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"
#include "../libs/b64.c"
#include "../libs/file.c"
#include "../libs/random.c"
#include "../libs/stats.c"

#define LINE_LENGTH 64
#define NUM_LINES 40

int main() {
  char line[LINE_LENGTH], *decoded = NULL;
  Bytes ciphers[NUM_LINES];
  int longest = 0;

  const unsigned char *randKey = randomBytes(16);
  const uint64_t nonce = 0;

  FILE *file = fopen("../data/19.txt", "r");
  if (!file) {
    printf("Failed to open");
    return -1;
  }
  for (int line_no = 1; fgets(line, LINE_LENGTH, file); line_no++) {
    int decoded_len = base64Decode(line, strlen(line), &decoded);
    if (decoded_len > longest)
        longest = decoded_len;

    ciphers[line_no - 1] =
        aesCTR((unsigned char *)decoded, decoded_len, randKey, 16, nonce);
  }

  int **ascii_chars = malloc(sizeof *ascii_chars * longest);
  if (ascii_chars) {
      for (int i = 0; i < longest; i++)
          ascii_chars[i] = malloc(sizeof *ascii_chars[i] * 256);
  }

  // Rank each ASCII char at each position
  for (int i = 0; i < longest; i++) {
    for (int chr = 0; chr < 256; chr++) {
      for (int sample = 0; sample < NUM_LINES; sample++) {
        if (ciphers[sample].len >= (unsigned int)i) {
          unsigned char result = chr ^ ciphers[sample].data[i];
          ascii_chars[i][chr] += frequencyScore(&result, 1);
        }
      }
    }
  }

  // Build the key
  unsigned char key[longest];
  for (int i = 0; i < longest; i++) {
    unsigned char best = 0;
    int best_score = 0;

    for (int chr = 0; chr < 256; chr++) {
      if (ascii_chars[i][chr] > best_score) {
        best = chr;
        best_score = ascii_chars[i][chr];
      }
    }

    key[i] = best;
  }

  for (size_t line = 0; line < NUM_LINES; line++) {
      for (size_t i = 0; i < ciphers[line].len; i++)
        printf("%c", ciphers[line].data[i] ^ key[i]);
      
      printf("\n");
  }

  fclose(file);
  free(decoded);
  free(ascii_chars);

  return 0;
}
