#ifndef __MISC_H__
#define __MISC_H__

uint16_t * keyGen(int m);
uint16_t * keyGen32(int m);

uint16_t * randomme();

uint16_t reduction(int t, uint16_t * tempkey);
uint32_t reduction32(int t, uint16_t * tempkey);
uint64_t reduction64(int t, uint16_t * tempkey);
#endif //MISC_H
