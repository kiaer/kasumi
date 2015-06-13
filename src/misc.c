#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <openssl/md5.h>
#include <stdlib.h>

#include "cipher/kasumi.h"


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

uint16_t reduction(int n, uint16_t * tempkey){
    uint16_t key;
    //printf("key -> %x",key[0]);
    key = tempkey[0]+n;
    //printf("%i\n",n);
    /* key[1]=ep>>32; */
    /* key[2]=ep>>16; */
    /* key[3]=ep; */
     /* if(t%8==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else if(t%7==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else if(t%6==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else if(t%5==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else if(t%4==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else if(t%3==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else if(t%2==0){ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* }else{ */
    /*     for (i = 0; i < 8; i++){ */
    /*         key[i] = tempkey[3]; */
    /*     } */
    /* } */
    return key;
}
