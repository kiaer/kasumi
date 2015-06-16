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
        loadBar(m, mMax, 10000, 50);
        temp = keyGen(m);
        //key = reduction(m,temp);
        key=temp;
        for (t = 0; t < lMax; t++){
            keyschedule(key);
            temp = kasumi_enc(text);
            tp = reduction32(t,temp);
            temp[0]=tp>>16;
            temp[1]=tp;
            for(i=0; i < 8; i++){
                key[i]=temp[i%2];
            }
        }

        for(i=0;i<2;i++)
            ep[i] = key[i];

        fwrite(ep,sizeof(ep),1,write_ptr);
    }

    fclose(write_ptr);

}
