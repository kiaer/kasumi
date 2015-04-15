#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include "cipher/kasumi.c"

uint16_t * keyGen(){
    int i;
    int byte_count = 4;
    static uint16_t data[4];
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&data, 1, byte_count, fp);
    fclose(fp);
    for (i = 2; i < 4; i++){
        data[i] = data[(i + 2) % 4];
    }
    return data;
}

void onlinePhase(uint32_t * text){

    int m, t, i,cntr=0;
    uint16_t eps[20];
    uint16_t *temp;
    uint16_t key[4];
    for (t = 0; t < 10; t++){
        keyschedule(key);
        temp = kasumi_enc(text);
        for (i = 0; i < 4; i++){
            key[i] = temp[i % 2];

        }
        for (i = 0; i < 4; i++){
            printf(" %04x ", key[i]);
        }
        printf("\n");

        for (i = 0; i < 2; i++){
            eps[cntr] = key[i];
            cntr=cntr+1;
        }




    }
    printf("\n Check \n");

    for(i=0;i<20;i++){
        printf(" %04x ", eps[i]);
        if(i%2!=0){
        printf("\n");

        }
    }
}

int main(){
    /* uint16_t key[4] = { */
    /*     0x9900, 0xAABB, 0xCCDD, 0xEEFF */
    /* }; */
    int amountOfKeys=5;
    uint16_t buffer[amountOfKeys*2];
    FILE *ptr;


    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };


    onlinePhase(text);
    ptr = fopen("test.bin","rb");  // r for read, b for binary

    fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
    int i,j,cnt,cnt1;
    printf("\n Read \n");
    for(j = 0; j<amountOfKeys ; j++){
        printf(" 0x ");
        cnt=j*2;
         for(i = 0; i<2; i++){
             cnt1=cnt+i;
             printf(" %04x ", buffer[cnt1]);
         } // prints a series of bytes}
        printf("\n");
    }
    return 0;
}
