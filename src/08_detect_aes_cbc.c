#include <stdio.h>
#include <stdlib.h>

#include "../libs/blocks.c"
#include "../libs/file.c"
#include "../libs/hex.c"

#define LINE_LENGTH 322

int main() {
  char *str_ascii = NULL, line[LINE_LENGTH], *best_line = NULL, best_key;
  unsigned char **blocks = NULL;

  int str_len = 0, best_score = 0, current_score = 0, best_line_no = 1;

  FILE *file = fopen("../data/8.txt", "r");
  if (!file) {
    printf("Failed to open");
    return -1;
  }

  best_line = malloc(LINE_LENGTH);
  if (best_line == NULL)
    return -1;

  for (int line_no = 1; fgets(line, LINE_LENGTH, file); line_no++) {
    str_len = hexToASCII(line, &str_ascii);

    current_score = 0;
    int num_blocks = chunk((unsigned char *)str_ascii, str_len, 16, &blocks);

    for (int i = 0; i < num_blocks; i++) {
      for (int j = i + 1; j < num_blocks; j++) {
        if (!memcmp(blocks[i], blocks[j], 16)) {
          current_score++;
        }
      }
    }

    if (current_score > best_score) {
      best_score = current_score;

      strncpy(best_line, line, LINE_LENGTH);
      best_line_no = line_no;
      /* printf("Line %d: %s\n", best_line_no, best_line); */
    }
  }

  printf("Line %d: %s\n", best_line_no, best_line);

  fclose(file);

  free(str_ascii);
  free(best_line);
  free(blocks);

  return 0;
}
