#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <openssl/md5.h>

#include "cipher/kasumi.h"
#include "misc.h"

/* uint16_t * keyGen(int m){ */
/*     int j,i, arrToInt,cntr=0; */
/*     static uint16_t data[8]; */
/*     unsigned char c[MD5_DIGEST_LENGTH]; */
/*     MD5_CTX mdContext; */
/*     MD5_Init (&mdContext); */
/*     MD5_Update (&mdContext,&m, sizeof(m)); */
/*     MD5_Final (c,&mdContext); */
/*     for (i = 0; i < 8; i++){ */
/*         arrToInt=0; */
/*         for(j=cntr;j<=cntr+1;j++) */
/*             arrToInt =(arrToInt<<8) | c[j%4]; */
/*         data[i] = arrToInt; */
/*         cntr=cntr+2; */
/*     } */
/*     return data; */
/* } */


void tableGenerator32(uint32_t * text){
    //int mMax=33554432;
    int mMax = 33554432, lMax = 236;
    //int mMax = 40, lMax = 10,cntr=0;
    int m, t, i;
    uint16_t * temp;
    uint16_t * key, ep[2];
    uint32_t tp;
    FILE * write_ptr;
    write_ptr = fopen("table32bits.bin", "wb");
    for(m = 0; m < mMax ; m++){
        temp = keyGen(m);
        //key = reduction(m,temp);
        key=temp;
        if(m==0)
            printf("--> %x %x %i \n",key[0],key[1], m);
        //search(key[0]);

        /* if(m%1000==0) */
        /*     printf("%i %04x %04x\n",m,key[0],key[1]); */


        /* /\* for (i = 0; i < 8; i++) *\/ */
        /* /\*     printf(" %04x ", key[i]); *\/ */
        for (t = 0; t < lMax; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            // printf("%x\n",temp[0]);
            tp = reduction32(t,temp);
            //printf("%08x\n",tp);
            temp[0]=tp>>16;
            temp[1]=tp;
            for(i=0; i < 8; i++){
                key[i]=temp[i%2];
            }
            // printf("%x\n",key[0]);
            //  printf("//////////////////\n");
            //search(key[0]);
            if(m==0){
                printf("chain link %i ",t);
                for(i=0;i<2;i++)
                    printf("%04x",key[i]);
                printf("\n");
            }
            //cntr = cntr+1;
        }

        for(i=0;i<2;i++)
            ep[i] = key[i];

        fwrite(ep,sizeof(ep),1,write_ptr);
    }

    fclose(write_ptr);

}
