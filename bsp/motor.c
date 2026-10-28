#include "ti_msp_dl_config.h"
#include "motor.h"
#include "param.h"



void Motor_Init(MOTOR_t *motor, uint32_t port, uint32_t in1, uint32_t in2, uint32_t cc_idx, GPTIMER_Regs *enc_timer)
{
    motor->GPIO_PORT = port;
    motor->PIN_IN_1 = in1;
    motor->PIN_IN_2 = in2;
    motor->PWM_CC_INDEX = cc_idx;
    motor->ENC_TIM = enc_timer;

    motor->ACTURL_SPEED = 0;
    motor->CURRENT_ENC = 0;
    motor->TARGET_SPEED = MOTOR_TARGET_SPEED;
}