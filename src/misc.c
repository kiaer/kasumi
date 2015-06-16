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

uint16_t * keyGen32(int m){
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
    key = tempkey[0]+n;
    return key;
}

uint32_t reduction32(int n, uint16_t * tempkey){
    uint32_t key;
    key = (tempkey[0]+n)<<16 | (tempkey[1]+n);
    return key;
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

uint64_t reduction64(int n, uint16_t * tempkey){
    uint64_t key;
    uint32_t key1;
    uint32_t key2;
    key1 = (tempkey[0]+n) << 16 | (tempkey[1]+n);
    key2 = (tempkey[2]+n) << 16 | (tempkey[3]+n);
    key =  (uint64_t) key1 << 32 | key2;
    return key;
}
