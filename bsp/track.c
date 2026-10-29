#include "ti_msp_dl_config.h"
#include "delay.h"
#include "track.h"

static const int8_t TRACK_WEIGHT[8] = {-7, -5, -3, -1, 1, 3, 5, 7};

static void Track_SelectChannel(uint8_t ch)
{
    if (ch & 0x01U) {
        DL_GPIO_setPins(GRAY_PORT, GRAY_A_0_PIN);
    } else {
        DL_GPIO_clearPins(GRAY_PORT, GRAY_A_0_PIN);
    }

    if (ch & 0x02U) {
        DL_GPIO_setPins(GRAY_PORT, GRAY_A_1_PIN);
    } else {
        DL_GPIO_clearPins(GRAY_PORT, GRAY_A_1_PIN);
    }

    if (ch & 0x04U) {
        DL_GPIO_setPins(GRAY_PORT, GRAY_A_2_PIN);
    } else {
        DL_GPIO_clearPins(GRAY_PORT, GRAY_A_2_PIN);
    }
}

void Track_Init(void)
{
    Track_SelectChannel(0U);
}

uint8_t Track_ReadBit(uint8_t ch)
{
    Track_SelectChannel(ch & 0x07U);
    delay_us(TRACK_SETTLE_US);

    return (DL_GPIO_readPins(GRAY_PORT, GRAY_OUT_PIN) != 0U) ? 1U : 0U;
}

uint8_t Track_ReadAll(void)
{
    uint8_t i;
    uint8_t bits = 0U;

    for (i = 0U; i < TRACK_CHANNEL_NUM; i++) {
        if (Track_ReadBit(i) == TRACK_LINE_BLACK) {
            bits |= (uint8_t)(1U << i);
        }
    }

    return bits;
}

int16_t Track_CalcError(uint8_t bits)
{
    int16_t sum = 0;
    uint8_t count = 0;
    uint8_t i;

    for (i = 0U; i < TRACK_CHANNEL_NUM; i++) {
        if (bits & (uint8_t)(1U << i)) {
            sum += TRACK_WEIGHT[i];
            count++;
        }
    }

    if (count == 0U) {
        return 0;
    }

    return (int16_t)(sum / count);
}
