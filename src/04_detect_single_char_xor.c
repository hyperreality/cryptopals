#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/hex.c"
#include "../libs/stats.c"
#include "../libs/xor.c"

#define LINE_LENGTH 61

int main() {
  char *str_ascii = NULL, line[LINE_LENGTH], *best_line = NULL, best_key;
  unsigned char *xored_line = NULL;

  int str1_len = 0, best_score = 0, current_score = 0, best_line_no = 1;

  FILE *file = fopen("../data/4.txt", "r");
  if (!file) {
    printf("Failed to open");
    return -1;
  }

  best_line = malloc(LINE_LENGTH);
  if (best_line == NULL)
    return -1;

  for (int line_no = 1; fgets(line, LINE_LENGTH, file); line_no++) {
    str1_len = hexToASCII(line, &str_ascii);

    best_key = findSingleByteXOR((unsigned char *)str_ascii, str1_len, frequencyScore);
    unsigned char key_arr[] = {best_key};
    xor((unsigned char *)str_ascii, str1_len, key_arr, 1, &xored_line);

    current_score = frequencyScore(xored_line, str1_len);
    if (current_score > best_score) {
      best_score = current_score;

      strncpy(best_line, (char *)xored_line, LINE_LENGTH);
      best_line_no = line_no;
      /* printf("Line %d: %s\n", best_line_no, best_line); */
    }
  }

  printf("Line %d: %s\n", best_line_no, best_line);

  fclose(file);

  free(str_ascii);
  free(best_line);
  free(xored_line);

  return 0;
}
