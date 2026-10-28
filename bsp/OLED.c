#include "ti_msp_dl_config.h"
#include "OLED.h"
#include "OLED_Font.h"

static void OLED_I2C_Write(uint8_t control, uint8_t data)
{
    uint8_t buf[2];
    uint32_t timeout = 100000U;

    buf[0] = control;
    buf[1] = data;

    DL_I2C_flushControllerTXFIFO(I2C_OLED_INST);
    DL_I2C_fillControllerTXFIFO(I2C_OLED_INST, buf, 2U);
    DL_I2C_startControllerTransfer(I2C_OLED_INST, OLED_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_TX, 2U);

    while ((DL_I2C_getControllerStatus(I2C_OLED_INST) &
            DL_I2C_CONTROLLER_STATUS_BUSY) && (timeout != 0U)) {
        timeout--;
    }
}

void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Write(0x00U, Command);
}

void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Write(0x40U, Data);
}

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0U | Y);
    OLED_WriteCommand(0x10U | ((X & 0xF0U) >> 4));
    OLED_WriteCommand(0x00U | (X & 0x0FU));
}

void OLED_Clear(void)
{
    uint8_t i, j;

    for (j = 0U; j < 8U; j++) {
        OLED_SetCursor(j, 0U);
        for (i = 0U; i < 128U; i++) {
            OLED_WriteData(0x00U);
        }
    }
}

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;

    OLED_SetCursor((Line - 1U) * 2U, (Column - 1U) * 8U);
    for (i = 0U; i < 8U; i++) {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    }

    OLED_SetCursor((Line - 1U) * 2U + 1U, (Column - 1U) * 8U);
    for (i = 0U; i < 8U; i++) {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8U]);
    }
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;

    for (i = 0U; String[i] != '\0'; i++) {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1U;

    while (Y != 0U) {
        Result *= X;
        Y--;
    }
    return Result;
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;

    for (i = 0U; i < Length; i++) {
        OLED_ShowChar(Line, Column + i,
            (char)(Number / OLED_Pow(10U, Length - i - 1U) % 10U + '0'));
    }
}

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;

    if (Number >= 0) {
        OLED_ShowChar(Line, Column, '+');
        Number1 = (uint32_t)Number;
    } else {
        OLED_ShowChar(Line, Column, '-');
        Number1 = (uint32_t)(-Number);
    }

    for (i = 0U; i < Length; i++) {
        OLED_ShowChar(Line, Column + i + 1U,
            (char)(Number1 / OLED_Pow(10U, Length - i - 1U) % 10U + '0'));
    }
}

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;

    for (i = 0U; i < Length; i++) {
        SingleNumber = (uint8_t)(Number / OLED_Pow(16U, Length - i - 1U) % 16U);
        if (SingleNumber < 10U) {
            OLED_ShowChar(Line, Column + i, (char)(SingleNumber + '0'));
        } else {
            OLED_ShowChar(Line, Column + i, (char)(SingleNumber - 10U + 'A'));
        }
    }
}

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;

    for (i = 0U; i < Length; i++) {
        OLED_ShowChar(Line, Column + i,
            (char)(Number / OLED_Pow(2U, Length - i - 1U) % 2U + '0'));
    }
}

void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0U; i < 1000U; i++) {
        for (j = 0U; j < 1000U; j++) {
        }
    }

    OLED_WriteCommand(0xAEU);
    OLED_WriteCommand(0xD5U);
    OLED_WriteCommand(0x80U);
    OLED_WriteCommand(0xA8U);
    OLED_WriteCommand(0x3FU);
    OLED_WriteCommand(0xD3U);
    OLED_WriteCommand(0x00U);
    OLED_WriteCommand(0x40U);
    OLED_WriteCommand(0xA1U);
    OLED_WriteCommand(0xC8U);
    OLED_WriteCommand(0xDAU);
    OLED_WriteCommand(0x12U);
    OLED_WriteCommand(0x81U);
    OLED_WriteCommand(0xCFU);
    OLED_WriteCommand(0xD9U);
    OLED_WriteCommand(0xF1U);
    OLED_WriteCommand(0xDBU);
    OLED_WriteCommand(0x30U);
    OLED_WriteCommand(0xA4U);
    OLED_WriteCommand(0xA6U);
    OLED_WriteCommand(0x8DU);
    OLED_WriteCommand(0x14U);
    OLED_WriteCommand(0xAFU);

    OLED_Clear();
}
