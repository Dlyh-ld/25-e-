#ifndef KEY_H_
#define KEY_H_

#include <stdint.h>

extern volatile uint8_t sign;

void Key_Init(void);
void Key_OnInterrupt(void);
uint8_t Key_GetPressed(void);

#endif
