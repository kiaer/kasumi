#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include "cipher/kasumi.c"

uint16_t * keyGen(uint32_t * m){
    int i;
    static uint16_t data[8];

    for (i = 0; i < 8; i++){
        if(i%2==0)
            data[i] = m[0]>>16;
        else
            data[i] = m[0]<<0;
    }
    for (i = 0; i < 8; i++)
             printf(" %04x ", data[i]);

    return data;
}

void onlinePhase(uint32_t * text){
    int t, i,arrToInt,cntr=0;
    uint16_t *temp;
    uint32_t keys[236];
    uint16_t key[8];
    FILE *ptr;
            temp = keyGen(text);
        /* for (i = 0; i < 4; i++){ */
        /*     sp[i] = temp[i];
               }*/
        for (i = 0; i < 8; i++){
            key[i] = temp[i];
        }

        /* printf("\n 0x "); */
        /* for (i = 0; i < 8; i++) */
        /*     printf(" %04x ", key[i]); */
        for (t = 0; t < 236; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            for (i = 0; i < 8; i++){
                key[i] = temp[i % 2];
            }
            arrToInt=0;
            for(i=cntr;i<=cntr+1;i++)
                arrToInt =(arrToInt<<16) | key[i%4];
            keys[t] = arrToInt;

            /* printf("\n 0x "); */
            /* for (i = 0; i < 8; i++) */
            /*     printf(" %04x ", key[i]); */
        }



        for (i = 0; i < 236; i++){
            printf("\n 0x ");

            printf(" %08x ", keys[i]);

        }
        uint32_t buffer[2000000];

         ptr = fopen("test32.bin","rb");  // r for read, b for binary */
         fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer */


         int j;
         printf("\n Read \n");

         for(j = 0; j<236 ; j++){

             for (i=0;i<2000000;i++){
                 if(buffer[i]==keys[j])
                     printf("Huzzah %08x",buffer[j]);
             }
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


    onlinePhase(text);

    return 0;


}
