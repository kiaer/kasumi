#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "cipher/kasumi.h"
#include "misc.h"


void tableGenerator32(uint32_t * text){
    //int mMax=10;
    int mMax = 33554432, lMax = 236;
    int m, t, i, r;
    uint16_t * temp;
    uint16_t key[8], ep[2],sp[2];
    FILE * write_ptr;
    write_ptr = fopen("SpEp.bin", "wb");
    for(m = 0; m < mMax ; m++){
        srand(m);
        r = rand();
        temp = keyGen(r);
        for (i = 0; i < 2; i++){
             sp[i] = temp[i];
               }
        fwrite(sp,sizeof(sp),1,write_ptr);
        /* printf("\n ep-> 0x "); */
        /* for (i = 0; i < 2; i++) */
        /*      printf(" %04x ", sp[i]); */
        for (i = 0; i < 8; i++){
            key[i] = temp[i %2 ];
        }

        /* /\* printf("\n 0x "); *\/ */
        /* /\* for (i = 0; i < 8; i++) *\/ */
        /* /\*     printf(" %04x ", key[i]); *\/ */
        for (t = 0; t < lMax; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            for (i = 0; i < 8; i++){
                key[i] = temp[i % 2];
            }
            // for(i=0;i<2;i++)
            //    printf(" %04x ",key[i]);

            //printf("\n");
        }

        for (i = 0; i < 2; i++){
            ep[i] = key[i];
        }

        // printf("\n ep-> 0x ");
        // for (i = 0; i < 2; i++)
        //     printf(" %04x ", ep[i]);

        fwrite(ep,sizeof(ep),1,write_ptr);
    }
    fclose(write_ptr);

    /* int i; */
    /* uint16_t * key2 = keyGen(); */
    /* printf("\n 0x"); */
    /* for (i = 0; i < 4; i++) */
    /*     printf("%04x", key2[i]); */

}

int main(){
    /* uint16_t key[4] = { */
    /*     0x9900, 0xAABB, 0xCCDD, 0xEEFF */
    /* }; */
    /* int amountOfKeys=5; */
    /* uint16_t buffer[amountOfKeys*4]; */
    /* FILE *ptr; */


    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };


    tableGenerator32(text);
    /* ptr = fopen("test32.bin","rb");  // r for read, b for binary */

    /* fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer */
    /* int i,j,cnt,cnt1; */
    /* printf("\n Read \n"); */
    /* for(j = 0; j<amountOfKeys ; j++){ */
    /*     printf(" 0x "); */
    /*     cnt=j*2; */
    /*      for(i = 0; i<2; i++){ */
    /*          cnt1=cnt+i; */
    /*          printf(" %04x ", buffer[cnt1]); */
    /*      } // prints a series of bytes} */
    /*     printf("\n"); */
    /* } */
    return 0;

}
