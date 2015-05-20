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

    FILE *ptr;
    ptr = fopen("test32.bin","rb");  // r for read, b for binary */
    for(;;){
        size_t n=fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer */

        endpoint = buffer[1]<<24 | buffer[0]<<16 | buffer[3]<<8 | buffer[2];

        //printf(" %x ",endpoint);
        if(endpoint==text[0])
            printf("Huzzah %08x",buffer[0]);
        if(n==0){break;}
    }

}

void onlinePhase(uint32_t * ciphertext){
    int t, i,arrToInt,cntr=0;
    uint16_t *temp;
    uint16_t key[8];

    inTable(ciphertext);
    temp = reduction(ciphertext);

    for (i = 0; i < 8; i++){
        key[i] = temp[i];
        printf("%4x ",temp[i]);
    }

    for (t = 0; t < 236; t++){
        keyschedule(key);
        temp = kasumi_enc(ciphertext);
        for (i = 0; i < 8; i++){
            key[i] = temp[i % 2];
        }
        arrToInt=0;
        for(i=cntr;i<=cntr+1;i++)
            arrToInt =(arrToInt<<16) | key[i%4];
        }


}

int main(){

    uint32_t ciphertext[2] = {
        0x51489622, 0x6caa4f20
    };


    onlinePhase(ciphertext);

    return 0;


}
