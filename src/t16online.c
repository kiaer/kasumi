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


int inTable(uint16_t key,uint16_t * ciphertext, uint32_t * text){
    uint16_t  buffer[4096], *temp2, keys[8], ep;
    uint16_t endpoint;
    uint16_t cipher[4];
    int cntr = 0,i,j;
    FILE *ptr;
    //printf("key -> %x\n",key);
    ptr = fopen("table16bitMD5.bin","rb");  // r for read, b for binary */
    //miss=0;
    for(;;){
        size_t n=fread(buffer,sizeof(buffer),1,ptr);
        //endpoint = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
        //printf("1\n");

        for(i=0;i<4095;i++){

            endpoint = buffer[i];
            //printf(" %x numb %i\n",endpoint,i);
            /* if(i==96) */
            /*     printf("%x %x\n",endpoint,key); */
            if(endpoint==key){
                // printf("--------------- hit %x\n",endpoint);
                temp2 = keyGen(cntr);
                for(i=0; i < 8; i++){
                    keys[i]=temp2[i];
                }
                for (j = 0; j < 2 ; j++){
                    keyschedule(keys);
                    temp2 = kasumi_enc(text);
                    cipher[0] = temp2[0];
                    cipher[1] = temp2[1];
                    cipher[2] = temp2[2];
                    cipher[3] = temp2[3];
                    ep = keys[0];
                    //printf("%x\n",temp2[0]);
                    // printf("ciphertext %08x %08x \n",ciphertext[0],ciphertext[1]);
                    //printf("cipher --- %08x %08x %i \n",*cipher,cipher[0],j);
                    if(cipher[0]==ciphertext[0]&&cipher[1]==ciphertext[1]&&
                       cipher[2]==ciphertext[2]&&cipher[3]==ciphertext[3]){
                        printf("Key found %i steps into chain \n", j);
                        printf("Key is the following: %04x \nciphertext: %04x %04x %04x %04x\n",ep,cipher[0],cipher[1],cipher[2],cipher[3]);
                        // printf("miss /> %i\n",miss);
                        fclose(ptr);
                        return 1;
                    }
                    for(i=0; i < 8; i++){
                        keys[i]=reduction(j,temp2);
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
            return -1;}
    }

}



int onlinePhase(uint16_t * ciphertext, uint32_t * text){
    printf("her");
    int t,i,j;
    uint16_t *temp, temp2[8];
    uint16_t ep[69];
    //uint16_t *key;
    //inTable(ciphertext[0],ciphertext,text);
    //key=kk;
    ep[0] = reduction(68, ciphertext);
    temp = ciphertext;
    printf("her");
    for (t = 67; t >= 0; t--){
        for(j = t; j < 69; j++){
            if(j == 68){
                ep[68-t] =  reduction(j, temp);
            } else{
                for(i = 0; i < 8; i++){
                    temp2[i] = reduction(j, temp);
                }
                keyschedule(temp2);
                temp = kasumi_enc(text);
                for(i = 0; i < 8; i++){
                    temp2[i] = temp[i];
                }
            }
        }
        /* keyschedule(key); */
        /* temp = kasumi_enc(text); */
        /* key = reduction(t,temp); */
        //printf("1st %x\n",temp[0]);

    /*     /\* ep[t] = key[0]; *\/ */

    /*     //printf("--> %x %i\n",ep[t],t); */

    /*     /\* printf("%x\n",temp[0]); *\/ */
    /*     /\* printf("--> %x %i\n",ep[t],t); *\/ */
    /*     /\* printf("//////////////////////\n"); *\/ */
    }
    /* printf("asd\n"); */
    /* for(t = 0; t < 69; t++){ */
    /*     printf("ep ---> %x ", ep[t]); */
    /* } */
    for(t=0;t<69;t++){
        //printf("%x \n",ep[t]);
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

int main(){
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
    while(j < 1000){
        //srand(time(NULL));
        //printf("%i\n",r);
        //temp = keyGen(j);
        //printf("%x\n",temp[2]);
        temp = randomme();
        key=temp;
        printf("/////////////////////////////////// %i \n",j);
        for (i=0;i<8;i++)
            key[i]=temp[i%1];
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
        printf("key  %04x \n",key[0]);
        cntr=cntr+onlinePhase(ciphertext, text);
        j++;

    }
    printf("%i\n",cntr);
    printf("%.2f\n",(float)((float)cntr/(float)j *100));
    return 0;
}
