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

const long big_T_64=35467640;
const long big_M_64=957180466911;

int inTable64(uint64_t key, uint16_t * ciphertext, uint32_t * text, long chain){
    uint16_t *temp, *temp2, keys[8], ep,cipher[4];
    uint64_t endpoint,tp;
    long mMax = 524288,cntr = 0,i,j,y;
    uint32_t buffer[mMax];
    FILE *ptr;
    size_t n;

    ptr = fopen("table64bit.bin","rb");  // r for read, b for binary */

    for(;;){
        n=fread(buffer, sizeof(buffer), 1, ptr);

        for(y=0;y<mMax;y++){
            tp = buffer[y];
            endpoint = tp<<32|tp>>32;

            if(endpoint==key){
                temp2 = keyGen(cntr);
                for(i=0; i < 8; i++){
                    keys[i]=temp2[i];
                }
                for (j = 0; j < chain ; j++){
                    keyschedule(keys);
                    temp2 = kasumi_enc(text);
                    cipher[0] = temp2[0];
                    cipher[1] = temp2[1];
                    cipher[2] = temp2[2];
                    cipher[3] = temp2[3];
                    ep = keys[0];
                    if(cipher[0]==ciphertext[0]&&cipher[1]==ciphertext[1]&&
                       cipher[2]==ciphertext[2]&&cipher[3]==ciphertext[3]){
                        printf("Key found %ld steps into chain \n", (j+1));
                        printf("Key is the following: %04x \nciphertext: %04x %04x %04x %04x\n",ep,cipher[0],cipher[1],cipher[2],cipher[3]);
                        fclose(ptr);
                        return 1;
                    }
                    temp=temp2;
                    tp = reduction64(j,temp);
                    temp[0] = tp >> 48;
                    temp[1] = tp >> 32;
                    temp[2] = tp >> 16;
                    temp[3] = tp;
                    for(i=0; i < 8; i++){
                        keys[i]=temp[i%4];
                    }
                }
            }
        cntr = cntr+1;
        }
        if(n==0){
            fclose(ptr);
            return -1;
        }
    }
}



int onlinePhase64(uint16_t * ciphertext, uint32_t * text){
    long chainLength=big_T_64, t,i,j;
    long cnt=chainLength;
    uint64_t tp,ep;
    uint16_t *temp, temp2[4], key[8];
    ep = reduction64((chainLength-1), ciphertext);
    i = inTable64(ep,ciphertext,text,cnt);
    if (i>0){return 1;}
    temp = ciphertext;
    for (t = (chainLength-2); t >= 0; t--){
        for(i=0;i<4;i++)
            temp2[i] = ciphertext[i];
        temp=temp2;
        cnt=chainLength;
        for(j = t; j < chainLength; j++){
            if(j == (chainLength-1)){
                ep = reduction32(j, temp);
                i = inTable64(ep,ciphertext,text,cnt);
                if (i>0){return 1;}
            } else{
                tp = reduction64(j,temp);
                temp[0] = tp >> 48;
                temp[1] = tp >> 32;
                temp[2] = tp >> 16;
                temp[3] = tp;
                for(i = 0; i < 8; i++){
                    key[i] = temp[i%2];
                }
                keyschedule(key);
                temp = kasumi_enc(text);
                cnt=cnt-1;
            }
        }
    }
    return 0;
}

int online64(){
    int j = 0, cntr = 0,i;
    uint16_t *key, * temp;
    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };

    uint16_t ciphertext[4];
    while(j < 1){
        temp = randomme();
        key=temp;
        printf("/////////////////////////////////// %i \n",j);
        for (i=0;i<8;i++)
            key[i]=temp[i%4];
        keyschedule(key);
        temp = kasumi_enc(text);
        ciphertext[0] = temp[0];
        ciphertext[1] = temp[1];
        ciphertext[2] = temp[2];
        ciphertext[3] = temp[3];
        printf("ciphertext %04x %04x %04x %04x \n",ciphertext[0],ciphertext[1],ciphertext[2],ciphertext[3]);
        printf("key  %04x %04x \n",key[0],key[1]);
        cntr=cntr+onlinePhase64(ciphertext, text);
        j++;

    }
    printf("%i\n",cntr);
    printf("%.2f\n",(float)((float)cntr/(float)j *100));
    return 0;
}
