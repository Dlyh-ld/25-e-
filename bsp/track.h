#ifndef TRACK_H_
#define TRACK_H_






#include <stdint.h>

#define TRACK_CHANNEL_NUM 8U
#define TRACK_SETTLE_US   20U
#define TRACK_LINE_BLACK  1U
#define TRACK_ALL_ON      INT16_MAX   /* 压角点/十字（8 路全压黑线，需标定） */
#define TRACK_LOST        INT16_MIN   /* 丢线（全白） */

void Track_Init(void);
uint8_t Track_ReadBit(uint8_t ch);
uint8_t Track_ReadAll(void);
int16_t Track_CalcError(uint8_t bits);

#endif
