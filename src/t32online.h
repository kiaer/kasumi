#ifndef __T32ONLINE_H__
#define __T32ONLINE_H__

uint16_t * reduction(uint32_t m);
int inTable(uint32_t text);
int onlinePhase(uint32_t * ciphertext, uint32_t * text);
uint16_t * randomme();
void online();

#endif //__T32ONLINE_H__
