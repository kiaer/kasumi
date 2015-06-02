#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <openssl/md5.h>

#include "tablegenerator.h"
#include "tablegenerator32.h"
#include "t32online.h"

int
main (int argc, char **argv)
{
    int c, err = 0;
    uint32_t text[2] = {
        0xFEDCBA09, 0x87654321
    };
    static char usage[] = "usage: %s TODO\n";


    while ((c = getopt (argc, argv, "tobkh")) != -1)
        switch (c)
        {
        case 'h':
            fprintf(stderr, usage, argv[0]);
            break;
        case 't':
            printf("Generating 32-bit rainbow table...\n");
            tableGenerator32(text); 
            break;
        case 'o':
            printf("Online phase performed on 32-bit table... \n");
            online();
            break;
        case 'b':
            printf("Generating 64-bit rainbow table...\n");
            tableGenerator(text);
            break;
        case 'k':
            printf("Online phase performed on 64-bit table... \n NOT FOUND");
            break;
        case '?':
            err = 1;
            break;
        }

    if (err) {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    return 0;
}
