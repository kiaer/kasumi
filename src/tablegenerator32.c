#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <openssl/md5.h>
#include "cipher/kasumi.c"

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

void tableGenerator(uint32_t * text){
    //int mMax=10;
    int mMax=33554432, lMax=236;
    int m, t, i;
    uint16_t *temp;
    uint16_t key[8], ep[2];
    FILE *write_ptr;
    write_ptr = fopen("test32.bin","wb");
    for(m = 0; m <mMax ; m++){
        temp = keyGen(m);
        /* for (i = 0; i < 4; i++){ */
        /*     sp[i] = temp[i];
               }*/

        temp = keyGen(*temp);
        for (i = 0; i < 8; i++){
            key[i] = temp[i %2 ];
        }

        /* /\* printf("\n 0x "); *\/ */
        /* /\* for (i = 0; i < 8; i++) *\/ */
        /* /\*     printf(" %04x ", key[i]); *\/ */
        for (t = 0; t < lMax; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            temp=keyGen(*temp);
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

/* int main(){ */
/*     /\* uint16_t key[4] = { *\/ */
/*     /\*     0x9900, 0xAABB, 0xCCDD, 0xEEFF *\/ */
/*     /\* }; *\/ */
/*     /\* int amountOfKeys=5; *\/ */
/*     /\* uint16_t buffer[amountOfKeys*4]; *\/ */
/*     /\* FILE *ptr; *\/ */


/*     uint32_t text[2] = { */
/*         0xFEDCBA09, 0x87654321 */
/*     }; */


/*     tableGenerator(text); */
/*     /\* ptr = fopen("test32.bin","rb");  // r for read, b for binary *\/ */

/*     /\* fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer *\/ */
/*     /\* int i,j,cnt,cnt1; *\/ */
/*     /\* printf("\n Read \n"); *\/ */
/*     /\* for(j = 0; j<amountOfKeys ; j++){ *\/ */
/*     /\*     printf(" 0x "); *\/ */
/*     /\*     cnt=j*2; *\/ */
/*     /\*      for(i = 0; i<2; i++){ *\/ */
/*     /\*          cnt1=cnt+i; *\/ */
/*     /\*          printf(" %04x ", buffer[cnt1]); *\/ */
/*     /\*      } // prints a series of bytes} *\/ */
/*     /\*     printf("\n"); *\/ */
/*     /\* } *\/ */
/*     return 0; */

/* } */
