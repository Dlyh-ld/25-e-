/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 */

#include "ti_msp_dl_config.h"
#include "bsp/OLED.h"

int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_setPins(LED_PORT,LED_LED1_PIN);

    while (1) {
    }
}
