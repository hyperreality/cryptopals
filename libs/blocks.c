#include <stdio.h>
#include <stdlib.h>

int blockTranspose(const unsigned char *str, const int str_len, const int num_blocks,
                   unsigned char ***blocks) {
  int block_len;

  block_len = str_len / num_blocks;

  *blocks = malloc(num_blocks * sizeof(char *));
  if (!(*blocks)) {
    printf("Could not allocate %d blocks\n", num_blocks);
    exit(1);
  }

  for (int i = 0; i < num_blocks; i++) {
    (*blocks)[i] = malloc(block_len);
    if (!(*blocks)[i]) {
      printf("Could not allocate block %d\n", i);
      exit(1);
    }

    // Finally, tranpose
    for (int j = 0; j < block_len; j++) {
      (*blocks)[i][j] = str[i + j * num_blocks];
    }
  }

  return block_len;
}

int chunk(const unsigned char *str, const int str_len, const int block_len,
          unsigned char ***blocks) {
  int num_blocks = str_len / block_len;

  *blocks = malloc(num_blocks * sizeof(char *));
  if (!(*blocks)) {
    printf("Could not allocate %d blocks\n", num_blocks);
    exit(1);
  }

  for (int i = 0; i < num_blocks; i++) {
    (*blocks)[i] = malloc(block_len);
    if (!(*blocks)[i]) {
      printf("Could not allocate block %d\n", i);
      exit(1);
    }

    for (int j = 0; j < block_len; j++) {
      (*blocks)[i][j] = str[i * block_len + j];
    }
  }

  return num_blocks;
}
