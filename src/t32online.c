#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include "cipher/kasumi.c"
#include <stdlib.h>


uint16_t * reduction(uint32_t m){
    static uint16_t data[8];
    data[0]=m>>16;
    data[1]=m;

    return data;
}
uint16_t * keyGen(int m){
    int j,i, arrToInt,cntr=0;
    static uint16_t data[8];
    unsigned char c[MD5_DIGEST_LENGTH];
    MD5_CTX mdContext;
    MD5_Init (&mdContext);
    MD5_Update (&mdContext,&m, sizeof(m));
    MD5_Final (c,&mdContext);
    for (i = 0; i < 8; i++){
        arrToInt=0;
        for(j=cntr;j<=cntr+1;j++)
            arrToInt =(arrToInt<<8) | c[j%4];
        data[i] = arrToInt;
        cntr=cntr+2;
    }
    return data;
}

int inTable(uint32_t text){
    uint16_t buffer[10000];
    uint32_t endpoint[5000];
    int cntr = 0,i,k=0;
    FILE *ptr;
    ptr = fopen("test32.bin","rb");  // r for read, b for binary */
    for(;;){
        size_t n=fread(buffer,sizeof(buffer),1,ptr);
        //endpoint = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
        k=0;
        //printf("1\n");
        for(i=0;i<5000;i++){
            // printf(" %i ",k);
            endpoint[i] = buffer[k]<<16 | buffer[k+1];
            k=k+2;
        //printf(" %x ",endpoint);
            // printf("%i\n",i);
        if(endpoint[i]==text){
            fclose(ptr);
            return cntr;
        }
        cntr = cntr+1;
        }
        if(n==0){
            fclose(ptr);
            return -1;}
    }

}



int onlinePhase(uint32_t * ciphertext, uint32_t * text){
    int t, i,k;
    uint16_t *temp,*temp2;
    uint32_t ep;
    uint32_t cipher[2];
    uint16_t key[8],keys[8];

    int dobreak = 0;
    inTable(ciphertext[0]);
    temp = reduction(ciphertext[0]);
    //reduciton function
    for (i = 0; i < 8; i++){
        key[i] = temp[i%2];
    }


    for (t = 0; t < 2214 && dobreak==0; t++){
        keyschedule(key);
        temp = kasumi_enc(text);

        //reduction function
        for (i = 0; i < 8; i++){
            key[i] = temp[i % 2];
             }
        ep = key[0]<<16 | key[1];
        i=inTable(ep);
        if(i>=0){

            temp2 = keyGen(i);

            for (i = 0; i < 8; i++){
                keys[i] = temp2[i%2];
            }
            for (k = 0; k < 2214 && dobreak==0; k++){
                keyschedule(keys);
                temp2 = kasumi_enc(text);
                cipher[0] = temp2[0]<<16 | temp2[1];
                cipher[1] = temp2[2]<<16 | temp2[3];
                ep= keys[0] << 16 | keys[1];
                //printf("ciphertext %08x %08x \n",ciphertext[0],ciphertext[1]);
                //printf("cipher  %08x %08x \n",cipher[0],cipher[1]);
                if(cipher[0]==ciphertext[0]){
                    printf("Key found %i steps into chain \n", k);
                    printf("Key is the following: %04x \n",ep);
                    return 1;
                    dobreak=1;
                    break;
                    }
                for (i = 0; i < 8; i++){
                    keys[i] = temp2[i % 2];
                }
            }
        }
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
    int i,j=0,cntr=0;
    uint16_t key[8],*temp;
    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    /* uint32_t ciphertext[2] = { */
    /*     0x591361f4, 0xdd05ce2f */
    /* }; */
    uint32_t ciphertext[2];
    while(j<10){
        //srand(time(NULL));
        //printf("%i\n",r);
        //temp = keyGen(r);
        temp = randomme();
        printf("--> %04x \n",temp[0]);
        for(i=0;i<8;i++){
            *(key+i)=temp[i%2];
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
    printf("%i\n",(cntr/j *100));
    return 0;


}
