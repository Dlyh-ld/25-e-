#include "ti_msp_dl_config.h"
#include "sign.h"

void GPIO_OpenLaser(void)
{
    DL_GPIO_setPins(Light_PORT, Light_GPIO_LASER_PIN);
}

void GPIO_CloseLaser(void)
{
    DL_GPIO_clearPins(Light_PORT, Light_GPIO_LASER_PIN);
}



void GPIO_OpenGimbalSign(void)
{
    DL_GPIO_setPins(Light_PORT, Light_GIMBAL_SIGN_PIN);
}

void GPIO_CloseGimbalSign(void)
{
    DL_GPIO_clearPins(Light_PORT, Light_GIMBAL_SIGN_PIN);
}



void GPIO_OpenMotorSign(void)
{
    DL_GPIO_setPins(Light_PORT, Light_MOTOR_SIGN_PIN);
}

void GPIO_CloseMotorSign(void)
{
    DL_GPIO_clearPins(Light_PORT, Light_MOTOR_SIGN_PIN);
}
