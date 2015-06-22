#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "cipher/kasumi.h"
#include "misc.h"


uint16_t * reduce(uint32_t m){
    static uint16_t data[8];
    data[0]=m>>16;
    data[1]=m;
    return data;
}

const int step = 32;

int inTable(uint32_t key, uint16_t * ciphertext, uint32_t * text){
    // uint16_t *temp, *temp2, keys[8], ep;
    uint32_t endpoint,tp;
    //uint32_t *buffer;
    //uint32_t buffer[32768];
    uint32_t buffer[262144];
    //uint32_t buffer[264087];
    uint16_t cipher[4];
    long cntr = 0,i,j,y;
    FILE *ptr;
    size_t n;
    //printf("key -> %x\n",key);
    //buffer = (uint32_t *) malloc (sizeof(uint32_t) * 33554432);
    ptr = fopen("table32bits.bin","rb");  // r for read, b for binary */
    //miss=0;
    //32768

    for(;;){
        //n=fread(buffer, sizeof(buffer)*33554432, 1, ptr);
        n=fread(buffer, sizeof(buffer), 1, ptr);

        //for(y=0;y<33554432;y++){
        //for(y=0;y<264087;y++){
        for(y=0;y<262144;y++){
            tp = buffer[y];
            endpoint = tp<<16|tp>>16;

            /* if(endpoint==key){ */
            /*     /\* printf("--------------- hit %x\n",buffer[i]); *\/ */
            /*     temp2 = keyGen(cntr); */
            /*     for(i=0; i < 8; i++){ */
            /*         keys[i]=temp2[i]; */
            /*     } */
            /*     for (j = 0; j < 5 ; j++){ */
            /*         //printf("%i\n",j); */
            /*         keyschedule(keys); */
            /*         temp2 = kasumi_enc(text); */
            /*         cipher[0] = temp2[0]; */
            /*         cipher[1] = temp2[1]; */
            /*         cipher[2] = temp2[2]; */
            /*         cipher[3] = temp2[3]; */
            /*         ep = keys[0]; */
            /*         if(cipher[0]==ciphertext[0]&&cipher[1]==ciphertext[1]&& */
            /*            cipher[2]==ciphertext[2]&&cipher[3]==ciphertext[3]){ */
            /*             printf("Key found %ld steps into chain \n", j); */
            /*             printf("Key is the following: %04x \nciphertext: %04x %04x %04x %04x\n",ep,cipher[0],cipher[1],cipher[2],cipher[3]); */

            /*             //free(buffer); */
            /*             fclose(ptr); */
            /*             return 1; */
            /*         } */
            /*         temp=temp2; */
            /*         tp = reduction32(j,temp); */
            /*         temp[0] = tp >> 16; */
            /*         temp[1] = tp; */
            /*         for(i=0; i < 8; i++){ */
            /*             keys[i]=temp[i%2]; */
            /*         } */
            /*     } */
            /* } */
        cntr = cntr+1;
        }
        if(n==0){
            fclose(ptr);
            //free(buffer);
            return -1;
        }

        }

}



int onlinePhase(uint16_t * ciphertext, uint32_t * text){

    int t,i,j;
    int chainLength=5;
    uint32_t tp;
    uint16_t *temp, temp2[4], key[8];
    uint32_t ep;
    ep = reduction32((chainLength-1), ciphertext);
    i = inTable(ep,ciphertext,text);
    if (i>0){return 1;}
    temp = ciphertext;

    for (t = (chainLength-2); t >= 0; t--){
        for(i=0;i<4;i++)
            temp2[i] = ciphertext[i];
        temp=temp2;
        for(j = t; j < chainLength; j++){

            if(j == (chainLength-1)){
                ep = reduction32(j, temp);
                clock_t start = clock(), diff;
                i = inTable(ep,ciphertext,text);
                diff = clock() - start;
                int msec = diff * 1000 / CLOCKS_PER_SEC;
                printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
                if (i>0){return 1;}
            } else{
                tp = reduction32(j,temp);
                temp[0] = tp >> 16;
                temp[1] = tp;
                for(i = 0; i < 8; i++){
                    key[i] = temp[i%2];
                }
                keyschedule(key);
                temp = kasumi_enc(text);

            }
        }
    }



    return 0;
}

int online(){
    int j = 0, cntr = 0,i;
    uint16_t *key, * temp;
    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    /* uint16_t key[8] = { */
    /*     0xCC9C,0xBB8A,0xCC9C,0xBB8A,0xCC9C,0xBB8A,0xCC9C,0xBB8A */
    /* } */
    /* uint32_t ciphertext[2] = { */
    /*     0x591361f4, 0xdd05ce2f */
    /* }; */
    uint16_t ciphertext[4];
    while(j < 100){
        //srand(time(NULL));
        //printf("%i\n",r);
        //temp = keyGen(j);
        //printf("%x\n",temp[2]);
        temp = randomme();
        key=temp;
        printf("/////////////////////////////////// %i \n",j);
        for (i=0;i<8;i++)
            key[i]=temp[i%2];
        // printf("%x %x %x %x \n",key[0],key[1],key[2],key[3]);
        /* temp=keyGen(j); */
        /* key = temp; */
        //key=reduction(j,temp);
        keyschedule(key);
        temp = kasumi_enc(text);
        ciphertext[0] = temp[0];
        ciphertext[1] = temp[1];
        ciphertext[2] = temp[2];
        ciphertext[3] = temp[3];
        printf("ciphertext %04x %04x %04x %04x \n",ciphertext[0],ciphertext[1],ciphertext[2],ciphertext[3]);
        printf("key  %04x %04x \n",key[0],key[1]);
        cntr=cntr+onlinePhase(ciphertext, text);
        j++;

    }
    printf("%i\n",cntr);
    printf("%.2f\n",(float)((float)cntr/(float)j *100));
    return 0;
}
