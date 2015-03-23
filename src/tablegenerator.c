#include <stdio.h>
#include <stdint.h>

#include "cipher/kasumi.c"

uint16_t * keyGen(){
    int i;
    int byte_count = 8;
    static uint16_t data[8];
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&data, 1, byte_count, fp);
    fclose(fp);
    for (i = 4; i < 8; i++){
        data[i] = data[(i + 4) % 8];
    }
    return data;
}

void tableGenerator(uint32_t * text){
    
    int m, t, i;
    uint16_t *temp;
    uint16_t key[8], ep[4], sp[4];
    
    for(m = 0; m < 1000; m++){
        temp = keyGen();
        for (i = 0; i < 4; i++){
            sp[i] = temp[i];
        }
        for (i = 0; i < 8; i++){
            key[i] = temp[i];
        }
        printf("\n 0x");
        for (i = 0; i < 4; i++)
            printf("%04x", sp[i]);
        for (t = 0; t < 10048576; t++){
            keyschedule(key);            
            temp = kasumi_enc(text);
            for (i = 0; i < 8; i++){
                key[i] = temp[i % 4];
            }
        }
        for (i = 0; i < 4; i++){
            ep[i] = key[i];
        }
        printf("\n 0x");
        for (i = 0; i < 4; i++)
            printf("%04x", ep[i]);
    }
    
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

    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    tableGenerator(text);
    return 0;
}
