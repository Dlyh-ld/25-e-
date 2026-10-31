#include "ti_msp_dl_config.h"
#include "motor.h"
#include "param.h"
#include "key.h"

MOTOR_t gMotorLeft;
MOTOR_t gMotorRight;



void Motor_Init(MOTOR_t *motor, uint32_t port, uint32_t in1, uint32_t in2, uint32_t cc_idx, GPTIMER_Regs *enc_timer)
{
    if (motor == NULL) {
        return;
    }
    
    motor->GPIO_PORT = port;
    motor->PIN_IN_1 = in1;
    motor->PIN_IN_2 = in2;
    motor->PWM_CC_INDEX = cc_idx;
    motor->ENC_TIM = enc_timer;

    motor->ACTURL_SPEED = 0.0f;
    motor->CURRENT_ENC = 0;
    motor->LAST_ENC = 0;

    DL_GPIO_clearPins((GPIO_Regs *)motor->GPIO_PORT, motor->PIN_IN_1 | motor->PIN_IN_2);
    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, 0, (DL_TIMER_CC_INDEX)motor->PWM_CC_INDEX);

    DL_TimerA_startCounter(MOTOR_PWM_INST);
}


void Motor_SetSpeed(MOTOR_t *motor, float speed)
{
    uint32_t duty;

    DL_GPIO_setPins(GPIO_MOTOR_DIR_PORT, GPIO_MOTOR_DIR_STBY_PIN);


    if (motor == NULL) {
        return;
    }


    if (speed > 0.0f) {
        DL_GPIO_setPins((GPIO_Regs *)motor->GPIO_PORT, motor->PIN_IN_1);
        DL_GPIO_clearPins((GPIO_Regs *)motor->GPIO_PORT, motor->PIN_IN_2);
    } else if (speed < 0.0f) {
        DL_GPIO_clearPins((GPIO_Regs *)motor->GPIO_PORT, motor->PIN_IN_1);
        DL_GPIO_setPins((GPIO_Regs *)motor->GPIO_PORT, motor->PIN_IN_2);
    } else {
        DL_GPIO_clearPins((GPIO_Regs *)motor->GPIO_PORT,
                          motor->PIN_IN_1 | motor->PIN_IN_2);
    }

    if (speed < 0.0f) {
        speed = -speed;
    }
    if (speed > MOTOR_MaxOut) {
        speed = MOTOR_MaxOut;
    }
    duty = (uint32_t)((speed * (float)MOTOR_PWM_PERIOD) / MOTOR_MaxOut);
    if (duty > MOTOR_PWM_PERIOD) {
        duty = MOTOR_PWM_PERIOD;
    }

    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, duty, (DL_TIMER_CC_INDEX)motor->PWM_CC_INDEX);
    if (motor->ENC_TIM != NULL) {
        DL_Timer_startCounter(motor->ENC_TIM);
    }

}


void Motor_Stop(MOTOR_t *motor)
{
    if (motor == NULL) {
        return;
    }

    motor->TARGET_SPEED = 0.0f;

    DL_GPIO_setPins((GPIO_Regs *)motor->GPIO_PORT, motor->PIN_IN_1 | motor->PIN_IN_2);

    DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, 0, (DL_TIMER_CC_INDEX)motor->PWM_CC_INDEX);
}

void Motor_UpdateEncoder(MOTOR_t *motor)
{
    if (motor == NULL) {
        return;
    }

    motor->ACTURL_SPEED = (float)(motor->CURRENT_ENC - motor->LAST_ENC);
    motor->LAST_ENC = motor->CURRENT_ENC;
}



static void Motor_EncoderISR(void)
{
    uint32_t a;
    uint32_t b;
    uint32_t iidx;

    while ((iidx = (uint32_t)DL_GPIO_getPendingInterrupt(ENC_PORT)) != 0U) {
        switch (iidx) {
        case ENC_L_A_IIDX:
            a = DL_GPIO_readPins(ENC_PORT, ENC_L_A_PIN) ? 1U : 0U;
            b = DL_GPIO_readPins(ENC_PORT, ENC_L_B_PIN) ? 1U : 0U;
            gMotorLeft.CURRENT_ENC += (a == b) ? 1 : -1;
            DL_GPIO_clearInterruptStatus(ENC_PORT, ENC_L_A_PIN);
            break;

        case ENC_L_B_IIDX:
            a = DL_GPIO_readPins(ENC_PORT, ENC_L_A_PIN) ? 1U : 0U;
            b = DL_GPIO_readPins(ENC_PORT, ENC_L_B_PIN) ? 1U : 0U;
            gMotorLeft.CURRENT_ENC += (a != b) ? 1 : -1;
            DL_GPIO_clearInterruptStatus(ENC_PORT, ENC_L_B_PIN);
            break;

        case ENC_R_A_IIDX:
            a = DL_GPIO_readPins(ENC_PORT, ENC_R_A_PIN) ? 1U : 0U;
            b = DL_GPIO_readPins(ENC_PORT, ENC_R_B_PIN) ? 1U : 0U;
            gMotorRight.CURRENT_ENC += (a == b) ? 1 : -1;
            DL_GPIO_clearInterruptStatus(ENC_PORT, ENC_R_A_PIN);
            break;

        case ENC_R_B_IIDX:
            a = DL_GPIO_readPins(ENC_PORT, ENC_R_A_PIN) ? 1U : 0U;
            b = DL_GPIO_readPins(ENC_PORT, ENC_R_B_PIN) ? 1U : 0U;
            gMotorRight.CURRENT_ENC += (a != b) ? 1 : -1;
            DL_GPIO_clearInterruptStatus(ENC_PORT, ENC_R_B_PIN);
            break;

        default:
            break;
        }
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case ENC_INT_IIDX:
            Motor_EncoderISR();
            break;

        case KEY_INT_IIDX:
            Key_OnInterrupt();
            break;

        default:
            break;
    }
}
