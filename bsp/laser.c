#include "ti_msp_dl_config.h"
#include "laser.h"

void GPIO_OpenLaser()
{
    DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);
}

void GPIO_OffLaser()
{
    DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);
}