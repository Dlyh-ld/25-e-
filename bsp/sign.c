#include "ti_msp_dl_config.h"
#include "sign.h"

void GPIO_OpenLaser()
{
    DL_GPIO_setPins(Light_PORT, Light_GPIO_LASER_PIN);
}

void GPIO_OffLaser()
{
    DL_GPIO_clearPins(Light_PORT, Light_GPIO_LASER_PIN);
}



void GPIO_OpenGimbalSign()
{
    DL_GPIO_setPins(Light_PORT, Light_GIMBAL_SIGN_PIN);
}

void GPIO_OffGimbalSign()
{
    DL_GPIO_clearPins(Light_PORT, Light_GIMBAL_SIGN_PIN);
}



void GPIO_OpenMotorSign()
{
    DL_GPIO_setPins(Light_PORT, Light_MOTOR_SIGN_PIN);
}

void GPIO_OffMotorSign()
{
    DL_GPIO_clearPins(Light_PORT, Light_MOTOR_SIGN_PIN);
}