#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <openssl/md5.h>

#include "cipher/kasumi.h"
#include "misc.h"

static inline void loadBar(int x, int n, int r, int w)
{
    if ( x % (n/r +1) != 0 ) return;

    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;

    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );

    // Show the load bar.
    for (x=0; x<c; x++)
        printf("=");

    for (x=c; x<w; x++)
        printf(" ");

    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\n\033[F\033[J");
}

void tableGenerator40(uint32_t * text){
    float mMax = 2147483648, lMax = 942; //2^31 2^9.88
    int m, t, i;
    uint16_t * temp;
    uint16_t * key;
    uint8_t ep[5],temp2;
    uint64_t tp;
    FILE * write_ptr;
    write_ptr = fopen("table40bit.bin", "wb");
    for(m = 0; m < mMax ; m++){
        loadBar(m, mMax, 10000, 50);
        key = keyGen(m);
        for (t = 0; t < lMax; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            tp = reduction64(t,temp);
            temp[0]=tp>>24;
            temp[1]=tp>>8;
            temp2=tp;
            temp[3]=temp2<<16|temp2;
            for(i=0; i < 8; i++){
                key[i]=temp[i%3];
            }
        }
        ep[0] = key[0]<<8;
        ep[1] = key[0];
        ep[2] = key[1]<<8;
        ep[3] = key[1];
        ep[4] = key[2];
        fwrite(ep,sizeof(ep),1,write_ptr);
    }

    fclose(write_ptr);

}
