#include "ti_msp_dl_config.h"
#include "key.h"

#define KEY_STABLE_MS 20U

static volatile uint8_t s_key_pressed;
volatile uint8_t sign = 0;

static uint8_t s_key_last_raw;
static uint8_t s_key_stable_ms;

static uint8_t Key_ReadRaw(void)
{
    return (DL_GPIO_readPins(KEY_PORT, KEY_SW1_PIN) != 0U) ? 1U : 0U;
}

void SysTick_Handler(void)
{
    uint8_t raw;

    raw = Key_ReadRaw();
    if (raw != s_key_last_raw) {
        s_key_last_raw = raw;
        s_key_stable_ms = 0U;
        return;
    }

    if (s_key_stable_ms < KEY_STABLE_MS) {
        s_key_stable_ms++;
        if (s_key_stable_ms == KEY_STABLE_MS) {
            if (raw == 0U) {
                s_key_pressed = 1U;
            }
        }
    }
}

void Key_Init(void)
{
    s_key_pressed = 0U;
    s_key_last_raw = Key_ReadRaw();
    s_key_stable_ms = 0U;

    NVIC_SetPriority(SysTick_IRQn, 1U);
    DL_SYSTICK_init(CPUCLK_FREQ / 1000U);
    DL_SYSTICK_enableInterrupt();
    DL_SYSTICK_enable();
}

void Key_OnInterrupt(void)
{
    DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_SW1_PIN);
}

uint8_t Key_GetPressed(void)
{
    uint8_t pressed;

    __disable_irq();
    pressed = (s_key_pressed != 0U) ? 1U : 0U;
    s_key_pressed = 0U;
    __enable_irq();

    return pressed;
}
