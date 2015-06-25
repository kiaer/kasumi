#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "cipher/kasumi.h"
#include "misc.h"

const long big_T_64_G=35467640;
const long big_M_64_G=957180466911;
static inline void loadBar(long x, long n, int r, int w)
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

void tableGenerator64(uint32_t * text){
    long mMax = big_M_64_G, lMax = big_T_64_G;
    long m, t, i;
    uint16_t * temp;
    uint16_t * key, ep[4];
    uint64_t tp;
    FILE * write_ptr;
    write_ptr = fopen("table64bit.bin", "wb");
    for(m = 0; m < mMax ; m++){
        loadBar(m, mMax, 10000, 50);
        temp = keyGen(m);
        key=temp;
        for (t = 0; t < lMax; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            tp = reduction64(t,temp);
            temp[0] = tp >> 48;
            temp[1] = tp >> 32;
            temp[2] = tp >> 16;
            temp[3] = tp;
            for(i=0; i < 8; i++){
                key[i]=temp[i%4];
            }
        }

        for(i=0;i<4;i++)
            ep[i] = key[i];

        fwrite(ep,sizeof(ep),1,write_ptr);
    }

    fclose(write_ptr);

}
