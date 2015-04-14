#include <stdio.h>

#include "kasumi.c"

int main() {

    clock_t start, end;
    double cpu_time_used;

    uint16_t key[8] = {
        0x9900, 0xAABB, 0xCCDD, 0xEEFF, 0x1122, 0x3344, 0x5566, 0x7788
    };

    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    int n;
    start = clock();
    for (n = 0; n <= 10000000; n++){
        keyschedule(key);
        kasumi_enc(text);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used: %f \n", cpu_time_used);

    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KLi1[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KLi2[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KOi1[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KOi2[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KOi3[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KIi1[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KIi2[i]); */
    /* printf("\n"); */
    /* for (i = 0; i < 8; i++) */
    /*     printf("%04x ", KIi3[i]); */
    /* printf("\n"); */

    /* printf("0x"); */
    /* for (i = 0; i < 2; i++) */
    /*     printf("%02x", text[i]); */
    /* printf("\n"); */
    //kasumi_dec(text);
    return 0;
}
