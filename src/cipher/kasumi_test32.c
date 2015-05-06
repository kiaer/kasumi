#include <stdio.h>
#include <time.h>

#include "kasumi.c"

int main() {

    uint16_t key[8] = {
        0x83a8,  0x5008,  0x83a8, 0x5008,  0x83a8, 0x5008, 0x83a8,  0x5008
    };

    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    uint32_t ciphertext[2];
    uint16_t *temp;
        keyschedule(key);
        temp = kasumi_enc(text);
        ciphertext[0] = temp[0]<<16 | temp[1];
        ciphertext[1] = temp[2]<<16 | temp[3];

        printf(" %08x %08x ",ciphertext[0],ciphertext[1]);




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
