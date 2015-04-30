#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include "cipher/kasumi.c"

uint16_t * reduction(uint32_t * m){
    int i;

    static uint16_t data[8];

    for (i = 0; i < 8; i++){
        if(i%2==0)
            data[i] = m[0]>>16;
        else
            data[i] = m[0]<<0;
    }

    return data;
}

void inTable(uint32_t *text){
    uint8_t buffer[4];
    uint32_t endpoint;
    int i;
    FILE *ptr;
    ptr = fopen("test32.bin","rb");  // r for read, b for binary */
    for(;;){
        size_t n=fread(buffer,sizeof(buffer),1,ptr);

        endpoint = buffer[1]<<24 | buffer[0]<<16 | buffer[3]<<8 | buffer[2];

        //printf(" %x ",endpoint);
        if(endpoint==text[0]){
            printf("Huzzah : ");
            for(i=0;i<4;i++)
                printf("%02x",buffer[i]);
            printf("%04x",text[0]);
            printf("\n");
        }
        if(n==0){break;}
    }

}

void onlinePhase(uint32_t * ciphertext, uint32_t * text){
    int t, i,arrToInt;
    uint16_t *temp;
    uint32_t tempCipher[2];
    uint16_t key[8];

    //inTable(ciphertext);
    temp = reduction(ciphertext);

    for (i = 0; i < 8; i++){
        key[i] = temp[i];
    }

    for (t = 0; t < 236; t++){
        keyschedule(key);
        temp = kasumi_enc(text);
        for (i = 0; i < 8; i++){
            key[i] = temp[i % 2];
            printf("%04x",temp[i]);
        }
        printf("\n");
        arrToInt=0;
        for(i=0;i<4;i++)
            arrToInt =(arrToInt<<16) | temp[i];
        tempCipher[0]=arrToInt;
        for(i=0;i<2;i++)
            printf("cipher  %8x \n",tempCipher[i]);

        //inTable(tempCipher);
        /* printf("\n 0x "); */
        /* for (i = 0; i < 8; i++) */
            /*     printf(" %04x ", key[i]); */
        }





    /* int i; */
    /* uint16_t * key2 = keyGen(); */
    /* printf("\n 0x"); */
    /* for (i = 0; i < 4; i++) */
    /*     printf("%04x", key2[i]); */

}

int main(){
    /* uint16_t key[4] = { */
    /*     0x9900, 0xAABB, 0xCCDD, 0xEEFF */
    /* }; */
    /* int amountOfKeys=5; */
    /* FILE *ptr; */


    uint32_t text[2] = {
         0xFEDCBA09, 0x87654321
     };

    uint32_t ciphertext[2] = {
        0x51489622, 0x6caa4f20
    };


    onlinePhase(ciphertext, text);

    return 0;


}
