#ifndef __T32ONLINE_H__
#define __T32ONLINE_H__

int inTable(uint16_t ciphertext, uint32_t text);
int onlinePhase(uint16_t * ciphertext, uint32_t * text);
int online();

#endif //__T32ONLINE_H__
