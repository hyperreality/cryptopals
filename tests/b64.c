#include <stdio.h>
#include <stdlib.h>

#include "../libs/b64.c"

int main() {
  int res = 0;
  char *text = "Lorem Ipsum is simply dummy text of the printing and "
               "typesetting industry. Lorem Ipsum has been the industry's "
               "standard dummy text ever since the 1500s, when an unknown "
               "printer took a galley of type and scrambled it to make a type "
               "specimen book. It has survived not only five centuries, but "
               "also the leap into electronic typesetting, remaining "
               "essentially unchanged. It was popularised in the 1960s with "
               "the release of Letraset sheets containing Lorem Ipsum "
               "passages, and more recently with desktop publishing software "
               "like Aldus PageMaker including versions of Lorem Ipsum.";
  size_t text_len = strlen(text);

  char *base64 = NULL, *result = NULL;
  int base64_len = base64Encode(text, text_len, &base64);

  base64Decode(base64, base64_len, &result);

  if (!strcmp(text, result)) {
    printf("Pass for base64\n");
  } else {
    printf("Fail, base64 decoded text was:\n %s\n", base64);
    res = -1;
  }

  return res;
}
