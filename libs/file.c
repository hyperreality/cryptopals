#include <stdio.h>
#include <stdlib.h>

int readFile(const char *filename, char **result) {
  int file_len = 0;

  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Can't find %s\n", filename);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  file_len = ftell(file);
  rewind(file);

  *result = malloc(file_len + 1);
  if (!(*result)) {
    printf("Couldn't allocate %d bytes of memory for the file.\n",
           file_len);
    return -1;
  }

  if (1 != fread((*result), file_len, 1, file)) {
    printf("Couldn't read from the file.\n");
    return -1;
  }

  fclose(file);

  return file_len;
}
