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

struct node{
    int cntr;
    uint16_t data;
    struct node *next;
}*head;


void append(uint16_t num )
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    temp->cntr=1;
    if (head== NULL)
    {
        head=temp;
        head->next=NULL;
    }
    else
    {
        temp->next=head;
        head=temp;
    }
}

void search(uint16_t num)
{
    struct node *n;
    int found = 0;
    n=head;
    while(n!=NULL)
    {
        if(n->data==num){
            n->cntr= n->cntr+1;
            found = 1;
            break;
        }
         n=n->next;
    }
    if(found == 0){
        append(num);
    }
}
int count()
{
    struct node *n;
    int c=0;
    n=head;
    while(n!=NULL)
    {
        n=n->next;
        c++;
    }
    return c;
}
void  display(struct node *r)
{
    r=head;
    if(r==NULL)
    {
        return;
    }
    while(r!=NULL)
    {
        printf("%d ",r->data);
        r=r->next;
    }
    printf("\n");
}
uint16_t * gg(uint32_t m){
    static uint16_t data[8];
    data[0]=m>>16;
    data[1]=m;
    return data;
}

void tableGenerator(uint32_t * text){
    //int mMax=33554432;
    int mMax = 33554432, lMax = 236;
    //int mMax = 40, lMax = 10,cntr=0;
    int m, t, i;
    uint16_t * temp;
    uint16_t * key, ep[2];
    uint32_t tp;
    FILE * write_ptr;
    struct node *n;
    n = (struct node*) malloc(sizeof(struct node));
    n->next=NULL;
    head=NULL;
    write_ptr = fopen("table32bit2.bin", "wb");
    for(m = 0; m < mMax ; m++){
        temp = keyGen(m);
        //key = reduction(m,temp);
        key=temp;
        if(m==96)
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
            if(m==96){
                printf("chain link %i ",t);
                for(i=0;i<2;i++)
                    printf("%04x",key[i]);
                printf("\n");
            }
            //cntr = cntr+1;
        }

        for(i=0;i<2;i++)
            ep[i] = key[i];


            //printf("count is -> %i\n",count());
        // for (i = 0; i < 2; i++)
        //     printf(" %04x ", ep[i]);
        fwrite(ep,sizeof(ep),1,write_ptr);
    }

    fclose(write_ptr);
    //printf("count is -> %i\n",count());
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


    tableGenerator(text);
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
