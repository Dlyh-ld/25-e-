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

extern MOTOR_t gMotorLeft;
extern MOTOR_t gMotorRight;
extern volatile uint8_t sign;


void Motor_Init(MOTOR_t *motor, uint32_t port, uint32_t in1, uint32_t in2, uint32_t cc_idx, GPTIMER_Regs *enc_timer);
void Motor_SetSpeed(MOTOR_t *motor, float speed);
void Motor_Stop(MOTOR_t *motor);
void Motor_UpdateEncoder(MOTOR_t *motor);

#endif
