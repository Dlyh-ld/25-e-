#include "ti_msp_dl_config.h"
#include "GIMBAL_SEND.h"
#include "delay.h"



static void Gimbal_SendBytes(const uint8_t *buf, uint8_t len)
{
    uint8_t i;

    for (i = 0U; i < len; i++) {
        DL_UART_Main_transmitDataBlocking(UART_GIMBAL_INST, buf[i]);
    }
}

static uint8_t Gimbal_CalcBcc(const uint8_t *buf, uint8_t len)
{
    uint8_t bcc = 0U;
    uint8_t i;

    for (i = 0U; i < len; i++) {
        bcc ^= buf[i];
    }

    return bcc;
}

static void Gimbal_SendFrame(uint8_t addr, uint8_t func,
                             const uint8_t *data, uint8_t dataLen)
{
    uint8_t buf[10];
    uint8_t len = 0U;
    uint8_t i;
    uint8_t bcc;

    buf[len++] = 0x7AU;
    buf[len++] = addr;
    buf[len++] = func;

    for (i = 0U; i < dataLen; i++) {
        buf[len++] = data[i];
    }

    bcc = Gimbal_CalcBcc(buf, len);
    buf[len++] = bcc;
    buf[len++] = 0x7BU;

    Gimbal_SendBytes(buf, len);
}

void Gimbal_Enable(uint8_t addr)
{
    Gimbal_SendFrame(addr, 0x06U, NULL, 0U);
}

void Gimbal_Disable(uint8_t addr)
{
    Gimbal_SendFrame(addr, 0x05U, NULL, 0U);
}

void Gimbal_SetMode(uint8_t addr, uint16_t mode)
{
    uint8_t data[2];

    data[0] = (uint8_t)(mode >> 8);
    data[1] = (uint8_t)(mode & 0xFFU);

    Gimbal_SendFrame(addr, 0x00U, data, 2U);
}

void Gimbal_SetMultiTurnAngle(uint8_t addr, int32_t angle_x10)
{
    uint8_t data[4];
    uint32_t value = (uint32_t)angle_x10;

    data[0] = (uint8_t)(value >> 24);
    data[1] = (uint8_t)(value >> 16);
    data[2] = (uint8_t)(value >> 8);
    data[3] = (uint8_t)(value & 0xFFU);

    Gimbal_SendFrame(addr, 0x02U, data, 4U);
}

void Gimbal_SetAngle(int32_t yaw_x10, int32_t pitch_x10)
{
    Gimbal_SetMultiTurnAngle(GIMBAL_YAW_ADDR, yaw_x10);
    delay_ms(1);

    Gimbal_SetMultiTurnAngle(GIMBAL_PITCH_ADDR, pitch_x10);
    delay_ms(1);
}

void Gimbal_Home(void)
{
    Gimbal_SetAngle(0, 0);
}

void Gimbal_Init(void)
{
    Gimbal_Enable(GIMBAL_YAW_ADDR);
    delay_ms(1);

    Gimbal_Enable(GIMBAL_PITCH_ADDR);
    delay_ms(1);

    Gimbal_SetMode(GIMBAL_YAW_ADDR, GIMBAL_MODE_MULTI_T);
    delay_ms(1);

    Gimbal_SetMode(GIMBAL_PITCH_ADDR, GIMBAL_MODE_MULTI_T);
    delay_ms(1);

    Gimbal_Home();
}
