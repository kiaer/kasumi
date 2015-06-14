#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include <stdlib.h>

#include "cipher/kasumi.h"
#include "misc.h"


uint16_t * reduction(uint32_t m){
    static uint16_t data[8];
    data[0]=m>>16;
    data[1]=m;
    return data;
}


int inTable(uint32_t key,uint32_t * ciphertext, uint32_t * text){
    uint16_t  buffer[3000000], *temp2, keys[8];
    uint32_t endpoint,ep;
    uint32_t cipher[2];
    int cntr = 0,i,j,y,k;
    FILE *ptr;
    //printf("key -> %x\n",key);
    ptr = fopen("table32bit.bin","rb");  // r for read, b for binary */
    //miss=0;
    for(;;){
        size_t n=fread(buffer,sizeof(buffer),1,ptr);
        //endpoint = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
        //printf("1\n");
        k=0;
        for(i=0;i<500000;i++){
            // printf(" %i ",k);
            endpoint = buffer[k] << 16 | buffer [k+1] ;

            k=k+2;
            if(endpoint==key){
                // printf("--------------- hit %x\n",endpoint);
                temp2 = reduction(cntr);

                for (y = 0; y < 8; y++){
                    keys[y] = temp2[y % 2];
                }
                for (j = 0; j < 236 ; j++){
                    keyschedule(keys);
                    temp2 = kasumi_enc(text);
                    cipher[0] = temp2[0]<<16 | temp2[1];
                    cipher[1] = temp2[2]<<16 | temp2[3];
                    ep = keys[0] << 16 | keys[1];
                    // printf("ciphertext %08x %08x \n",ciphertext[0],ciphertext[1]);
                    //printf("cipher --- %08x %08x %i \n",*cipher,cipher[0],j);
                    if(cipher[0]==ciphertext[0]&&cipher[1]==ciphertext[1]){
                        printf("Key found %i steps into chain \n", j);
                        printf("Key is the following: %08x \n",ep);
                        // printf("miss /> %i\n",miss);
                        fclose(ptr);
                        return 1;
                        break;
                    }
                    for (y = 0; y < 8; y++){
                        keys[y] = temp2[y % 2];
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



int onlinePhase(uint32_t * ciphertext, uint32_t * text){
    int t, i;
    uint16_t * temp;
    uint32_t ep;
    uint16_t key[8];
    inTable(ciphertext[0],ciphertext,text);
    temp = reduction(ciphertext[0]);
    //reduciton function
    for (i = 0; i < 8; i++){
        key[i] = temp[i%2];
    }

    for (t = 0; t < 235; t++){
        keyschedule(key);
        temp = kasumi_enc(text);

//reduction function
        for (i = 0; i < 8; i++){
            key[i] = temp[i % 2];
             }
        ep = key[0]<<16 | key[1];
        //printf("%i %x\n",t,ep);
        i=inTable(ep,ciphertext,text);
        if (i>0)
            return 1;
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
    int i, j = 0, cntr = 0;
    uint16_t key[8], * temp;
    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    /* uint16_t key[8] = { */
    /*     0xCC9C,0xBB8A,0xCC9C,0xBB8A,0xCC9C,0xBB8A,0xCC9C,0xBB8A */
    /* } */
    /* uint32_t ciphertext[2] = { */
    /*     0x591361f4, 0xdd05ce2f */
    /* }; */
    uint32_t ciphertext[2];
    while(j < 10){
        //srand(time(NULL));
        //printf("%i\n",r);
        //temp = reduction(j);
        temp = randomme();
        printf("///////////////////////////////////\n");
        for(i=0;i<8;i++){
           key[i]=temp[i%2];
        }
        keyschedule(key);
        temp = kasumi_enc(text);
        ciphertext[0] = temp[0]<<16 | temp[1];
        ciphertext[1] = temp[2]<<16 | temp[3];
        printf("ciphertext %08x %08x \n",ciphertext[0],ciphertext[1]);
        printf("key  %04x %04x \n",key[0],key[1]);
        cntr=cntr+onlinePhase(ciphertext, text);
        j++;
    }
    printf("%i\n",cntr);
    printf("%.2f\n",(float)((float)cntr/(float)j *100));
    return 0;
}
