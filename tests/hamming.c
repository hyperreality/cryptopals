#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/file.c"
#include "../libs/xor.c"

int main() {
    int res = 0;
    unsigned char str1[] = "this is a test";
    unsigned char str2[] = "wokka wokka!!!";
    size_t len = strlen((char *)str1);

    int ham = hammingDistance(str1, len, str2, len);
    if (ham == 37) {
        printf("Pass Hamming test");
    } else {
        printf("Failed Hamming test, distance was %d", ham);
        res = -1;
    }

    return res;
}
