#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include <stdlib.h>

#include "cipher/kasumi.h"
#include "misc.h"

uint16_t * reduce(uint32_t m){
    static uint16_t data[8];
    data[0]=m>>16;
    data[1]=m;
    return data;
}


int inTable(uint32_t key, uint16_t * ciphertext, uint32_t * text){
    uint16_t *temp, *temp2, keys[8], ep;
    uint32_t endpoint, *buffer,tp;
    uint16_t cipher[4];
    int cntr = 0,i,j,y;
    FILE *ptr;
    //printf("key -> %x\n",key);
    buffer = (uint32_t *) malloc (sizeof(uint32_t) * 33554432);
    ptr = fopen("table32bit.bin","rb");  // r for read, b for binary */
    //miss=0;
    for(;;){
        size_t n=fread(buffer,sizeof(buffer)*33554432,1,ptr);
        //endpoint = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
        //printf("1\n");

        for(y=0;y<33554432;y++){
            tp = buffer[y];
            endpoint = tp<<16|tp>>16;
            if(endpoint==key){
                printf("--------------- hit %x\n",buffer[i]);
                temp2 = keyGen(cntr);
                for(i=0; i < 8; i++){
                    keys[i]=temp2[i];
                }
                for (j = 0; j < 236 ; j++){
                    //printf("%i\n",j);
                    keyschedule(keys);
                    temp2 = kasumi_enc(text);
                    cipher[0] = temp2[0];
                    cipher[1] = temp2[1];
                    cipher[2] = temp2[2];
                    cipher[3] = temp2[3];
                    ep = keys[0];
                    //printf("%i\n",j);
                    // printf("ciphertext %04x %04x %04x %04x\n",ciphertext[0],ciphertext[1],ciphertext[2],ciphertext[3]);
                    // printf("cipher --> %04x %04x %04x %04x\n",*cipher,cipher[1],cipher[2],cipher[3]);
                    if(cipher[0]==ciphertext[0]&&cipher[1]==ciphertext[1]&&
                       cipher[2]==ciphertext[2]&&cipher[3]==ciphertext[3]){
                        printf("Key found %i steps into chain \n", j);
                        printf("Key is the following: %04x \nciphertext: %04x %04x %04x %04x\n",ep,cipher[0],cipher[1],cipher[2],cipher[3]);
                        // printf("miss /> %i\n",miss);
                        free(buffer);
                        fclose(ptr);
                        return 1;
                    }
                    //printf("hey\n");
                    temp=temp2;
                    tp = reduction32(j,temp);
                    temp[0] = tp >> 16;
                    temp[1] = tp;
                    //printf("tp %x %x %x\n",tp ,temp[0],temp[1]);
                    for(i=0; i < 8; i++){
                        keys[i]=temp[i%2];
                    }
                }
            }
        cntr = cntr+1;
        }
        if(n==0){
            //printf("/////////////////>>>>>>>>>> %x %i\n",endpoint, cntr);
            //if(miss>0)
                //printf("miss > %i\n",miss);
            fclose(ptr);
            free(buffer);
            return -1;}
    }

}



int onlinePhase(uint16_t * ciphertext, uint32_t * text){
    int t,i,j;
    int chainLength=236;
    uint32_t tp;
    uint16_t *temp,temp2[4], key[8];
    uint32_t ep[chainLength];
    ep[0] = reduction32((chainLength-1), ciphertext);
    temp = ciphertext;
    //printf("keytsxets <> %x\n",ciphertext[0]);
    for (t = (chainLength-2); t >= 0; t--){
        for(i=0;i<4;i++)
            temp2[i] = ciphertext[i];
        temp=temp2;
        //if(t==0)
            //printf("cipher %x %x\n",ciphertext[0],temp[0]);
        for(j = t; j < chainLength; j++){

            if(j == (chainLength-1)){
                ep[chainLength-1-t] = reduction32(j, temp);
            } else{
                tp = reduction32(j,temp);
                temp[0] = tp >> 16;
                temp[1] = tp;
                for(i = 0; i < 8; i++){
                    key[i] = temp[i%2];
                }
                keyschedule(key);
                temp = kasumi_enc(text);
                //if(t==0)
                    //printf("%x %x\n",temp[0],temp[1]);
            }
        }
    }

    for(t=0;t<chainLength;t++){
        // printf("ep: %x %i\n", ep[t],t);
        i = inTable(ep[t],ciphertext,text);
        if (i>0){return 1;}
    }

    return 0;
}


uint16_t * randomme(){

    int byte_count = 4;
    static uint16_t data[8];
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&data, 1, byte_count, fp);
    fclose(fp);
    return data;
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
