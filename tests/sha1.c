#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/hash.c"

int main(int argc, char **argv) {
    FILE *fp;
    char buf[100];
    fp = popen("echo -n YELLOWSUBMARINEhello | sha1sum | cut -d' ' -f1", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(-1);
    }
    char *expected = fgets(buf, sizeof(buf)-1, fp);
    if (expected == NULL) {
        printf("Failed to get output\n");
        exit(-1);
    }
    printf("sha1sum: %s", expected);

    char *key_ascii = "YELLOWSUBMARINE";
    int key_len = strlen(key_ascii);
    char *data_ascii = "hello";
    int data_len = strlen(data_ascii);

    char *hash = mac_sha1(key_ascii, key_len, data_ascii, data_len);
    printf("our output: %s\n", hash);

    assert(!strncmp(hash, expected, 40));
}
