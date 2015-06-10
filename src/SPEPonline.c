#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include "cipher/kasumi.h"


uint16_t * reduction(uint32_t m){
    static uint16_t data[8];
    data[0]=m>>16;
    data[1]=m;

    return data;
}


uint32_t inTable(uint32_t key, uint32_t * ciphertext, uint32_t * text){
    uint16_t buffer[100],keys[8],*temp2;
    uint32_t startpoint, endpoint, keytemp,cipher[2];
    int cntr = 0,i,k=0,j,y;
    FILE *ptr;
    ptr = fopen("SpEp.bin","rb");  // r for read, b for binary */
    for(;;){
        size_t n=fread(buffer,sizeof(buffer),1,ptr);
        //endpoint = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
        k=0;
        //printf("1\n");
        for(i=0;i<25;i++){
            // if (k%100 == 0 )
                //printf("%i \n",k);

            startpoint = buffer[k]<<16 | buffer[k+1];
            endpoint = buffer[k+2]<<16 | buffer[k+3];
            k=k;
            //printf("%i \n ",k);
            //printf("%x\n",endpoint);
            if(endpoint==key){
                //printf("%x, %x, %x, %i\n",key,endpoint,startpoint,k);

                temp2 = reduction(startpoint);
                //printf("%x, %x\n",temp2[0],temp2[1]);

                for (j = 0; j < 8; j++){
                    keys[j] = temp2[j%2];
                }
                //printf("%x, %x \n",keys[1],keys[2]);
                for (j = 0; j < 236; j++){
                    keyschedule(keys);
                    temp2 = kasumi_enc(text);

                    cipher[0] = temp2[0]<<16 | temp2[1];
                    cipher[1] = temp2[2]<<16 | temp2[3];
                    keytemp = keys[0] << 16 | keys[1];
                    //printf("ciphertext %08x %08x \n",ciphertext[0],ciphertext[1]);
                    //printf("cipher  %08x %08x \n",cipher[0],cipher[1]);
                    if(cipher[0]==ciphertext[0] && cipher[1]==ciphertext[1]){
                        //printf("%s\n",text);
                        printf("Key found %i steps into chain \n", j);
                        printf("Key is the following: %04x \n",keytemp);
                        fclose(ptr);
                        return 1;
                        break;
                    }if(cipher[1]==ciphertext[0] && cipher[0]==ciphertext[1]){
                        //printf("%s\n",text);
                        printf("Key found %i steps into chain \n", j);
                        printf("Key is the following: %04x \n",keytemp);
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
            fclose(ptr);
            return 0;}
    }

}



int onlinePhase(uint32_t * ciphertext, uint32_t * text){
    int t, i;
    uint16_t * temp;
    uint32_t ep;
    uint16_t key[8];


    inTable(ciphertext[0], ciphertext, text);
    temp = reduction(ciphertext[0]);
    //reduciton function
    for (i = 0; i < 8; i++){
        key[i] = temp[i%2];
    }


    for (t = 0; t < 236; t++){
        keyschedule(key);
        temp = kasumi_enc(text);

        //reduction function
        for (i = 0; i < 8; i++){
            key[i] = temp[i % 2];
             }
        ep = key[0]<<16 | key[1];
        inTable(ep, ciphertext,text);
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
    /* uint32_t ciphertext[2] = { */
    /*     0x591361f4, 0xdd05ce2f */
    /* }; */
    uint32_t ciphertext[2];
    while(j < 10){
        //srand(time(NULL));
        //printf("%i\n",r);
        //temp = keyGen(r);
        temp = randomme();
        for(i=0;i<8;i++){
            *(key+i)=temp[i%2];
        }
        keyschedule(key);
        temp = kasumi_enc(text);
        printf("-------------------------------------\n");
        ciphertext[0] = temp[0]<<16 | temp[1];
        ciphertext[1] = temp[2]<<16 | temp[3];
        printf("ciphertext %08x %08x \n",ciphertext[0],ciphertext[1]);
        printf("key  %04x %04x \n",key[0],key[1]);
        cntr=cntr+onlinePhase(ciphertext, text);
        j++;
    }
    printf("%i\n",cntr);
    printf("%.2f\n",(((float)cntr)/((float)j)));
    return 1;
}
