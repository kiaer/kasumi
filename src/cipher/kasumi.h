#ifndef __KASUMI_H__
#define __KASUMI_H__

void keyschedule(uint16_t *key);
uint16_t * kasumi_enc(uint32_t *text);
static uint32_t fl(uint32_t input, int round);
static uint32_t fo(uint32_t input, int round);
static uint16_t fi(uint16_t ki, uint16_t input);

uint16_t KLi1[8], KLi2[8];
uint16_t KOi1[8], KOi2[8], KOi3[8];
uint16_t KIi1[8], KIi2[8], KIi3[8];

#endif //__KASUMI_H__

