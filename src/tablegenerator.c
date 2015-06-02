#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include "cipher/kasumi.h"
#include "misc.h"

/* uint16_t * keyGen(){ */
/*     int i; */
/*     int byte_count = 8; */
/*     static uint16_t data[8]; */
/*     FILE *fp; */
/*     fp = fopen("/dev/urandom", "r"); */
/*     fread(&data, 1, byte_count, fp); */
/*     fclose(fp); */
/*     for (i = 4; i < 8; i++){ */
/*         data[i] = data[(i + 4) % 8]; */
/*     } */
/*     return data; */
/* } */

void tableGenerator(uint32_t * text){

    int m, t, i;
    uint16_t *temp;
    uint16_t key[8], ep[4];
    FILE *write_ptr;
    write_ptr = fopen("test.bin","wb");
     for(m = 0; m < 5; m++){
        temp = keyGen(m);
        /* for (i = 0; i < 4; i++){ */

        /*     sp[i] = temp[i]; */
        /* } */
        for (i = 0; i < 8; i++){
            key[i] = temp[i];
        }
        //printf("\n 0x ");
        /* for (i = 0; i < 4; i++) */
        /*     printf(" %04x ", sp[i]); */
        for (t = 0; t < 10; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            for (i = 0; i < 8; i++){
                key[i] = temp[i % 4];
            }
        }

        for (i = 0; i < 4; i++){
            ep[i] = key[i];
        }
        printf("\n 0x ");
        for (i = 0; i < 4; i++)
            printf(" %04x ", ep[i]);

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
/*     int amountOfKeys=5; */
/*     uint16_t buffer[amountOfKeys*4]; */
/*     FILE *ptr; */


/*     uint32_t text[2] = { */
/*         0xFEDCBA09, 0x87654321 */
/*     }; */


/*     tableGenerator(text); */
/*     ptr = fopen("test.bin","rb");  // r for read, b for binary */

/*     fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer */
/*     int i,j,cnt,cnt1; */
/*     printf("\n Read \n"); */
/*     for(j = 0; j<amountOfKeys ; j++){ */
/*         printf(" 0x "); */
/*         cnt=j*4; */
/*          for(i = 0; i<4; i++){ */
/*              cnt1=cnt+i; */
/*              printf(" %04x ", buffer[cnt1]); */
/*          } // prints a series of bytes} */
/*         printf("\n"); */
/*     } */
/*     return 0; */
/* } */
