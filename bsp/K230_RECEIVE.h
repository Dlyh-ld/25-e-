#ifndef K230_RECEIVE_H_
#define K230_RECEIVE_H_


#include <stdint.h>

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t valid;
} K230_Target_t;

void K230_Init(void);
uint8_t K230_ReadByte(uint8_t *byte);
uint8_t K230_GetTarget(K230_Target_t *target);



#endif
