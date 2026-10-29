#include "ti_msp_dl_config.h"
#include "delay.h"

void delay_ms(int ms){
    for(int i = 0; i < 100; i++){
        delay_cycles(320000 / 1000 * ms);
    }
}

void delay_us(int us){
    delay_cycles(32000000 / 1000000 * us);
}
