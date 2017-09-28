#include <stdio.h>
#include <stdlib.h>

#include "../libs/aes.c"

int main() {
    char *valid = "ICE ICE BABY\x04\x04\x04\x04";
    char *invalid = "ICE ICE BABY\x05\x05\x05\x05";
    char *invalid2 = "ICE ICE BABY\x01\x02\x03\x04";

    Bytes stripped = validateAndStrip((unsigned char *)valid, 16);
    if (stripped.data) {
        printf("%s\n", stripped.data);
    } else {
        printf("Invalid padding\n");
    }

    stripped = validateAndStrip((unsigned char *)invalid, 16);
    if (stripped.data) {
        printf("%s\n", stripped.data);
    } else {
        printf("Invalid padding\n");
    }

    stripped = validateAndStrip((unsigned char *)invalid2, 16);
    if (stripped.data) {
        printf("%s\n", stripped.data);
    } else {
        printf("Invalid padding\n");
    }

    free(stripped.data);

    return 0;
}


