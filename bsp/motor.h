#ifndef MOTOR_H_
#define MOTOR_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>

#define MOTOR_PWM_PERIOD 1600U

typedef struct{
    GPIO_Regs *GPIO_PORT;
    uint32_t PIN_IN_1;
    uint32_t PIN_IN_2;
    uint32_t PWM_CC_INDEX;
    GPTIMER_Regs *ENC_TIM;

    volatile int32_t CURRENT_ENC;
    volatile int32_t LAST_ENC;
    volatile float ACTUAL_SPEED;

    float TARGET_SPEED;

} MOTOR_t;

extern MOTOR_t gMotorLeft;
extern MOTOR_t gMotorRight;


void Motor_Init(MOTOR_t *motor, GPIO_Regs *port, uint32_t in1, uint32_t in2, uint32_t cc_idx, GPTIMER_Regs *enc_timer);
void Motor_SetSpeed(MOTOR_t *motor, float speed);
void Motor_Stop(MOTOR_t *motor);
void Motor_UpdateEncoder(MOTOR_t *motor);

#endif
