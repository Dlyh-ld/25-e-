#ifndef MOTOR_H_
#define MOTOR_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>

typedef struct{
    uint32_t GPIO_PORT;
    uint32_t PIN_IN_1;
    uint32_t PIN_IN_2;
    uint32_t PWM_CC_INDEX;
    GPTIMER_Regs *ENC_TIM;

    int16_t CURRENT_ENC;
    int16_t LAST_ENC;
    float ACTURL_SPEED;

    float TARGET_SPEED;

} MOTOR_t;


#endif