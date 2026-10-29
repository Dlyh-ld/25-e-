#include "oled.h"
#include "oledfont.h"
#include "delay.h"

static u8 OLED_GRAM[OLED_WIDTH][OLED_HEIGHT / 8U];

static void OLED_I2C_Write(u8 control, u8 data)
{
    uint8_t buf[2];
    uint32_t timeout = 100000U;

    buf[0] = control;
    buf[1] = data;

    while (!(DL_I2C_getControllerStatus(I2C_OLED_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE)) {
        if (timeout-- == 0U) {
            return;
        }
    }

    DL_I2C_fillControllerTXFIFO(I2C_OLED_INST, buf, 2U);
    DL_I2C_startControllerTransfer(I2C_OLED_INST, 0x3CU,
        DL_I2C_CONTROLLER_DIRECTION_TX, 2U);

    delay_cycles(100U);

    timeout = 100000U;
    while ((DL_I2C_getControllerStatus(I2C_OLED_INST) &
            DL_I2C_CONTROLLER_STATUS_BUSY) && (timeout != 0U)) {
        timeout--;
    }
}

void OLED_WR_Byte(u8 dat, u8 mode)
{
    OLED_I2C_Write((mode == OLED_DATA) ? 0x40U : 0x00U, dat);
}

u8 OLED_I2C_Probe(void)
{
    uint8_t buf[2] = {0x00U, 0xAEU};
    uint32_t timeout = 100000U;

    while (!(DL_I2C_getControllerStatus(I2C_OLED_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE)) {
        if (timeout-- == 0U) {
            return 0U;
        }
    }

    DL_I2C_fillControllerTXFIFO(I2C_OLED_INST, buf, 2U);
    DL_I2C_startControllerTransfer(I2C_OLED_INST, 0x3CU,
        DL_I2C_CONTROLLER_DIRECTION_TX, 2U);
    delay_cycles(100U);

    timeout = 100000U;
    while ((DL_I2C_getControllerStatus(I2C_OLED_INST) &
            DL_I2C_CONTROLLER_STATUS_BUSY) && (timeout != 0U)) {
        timeout--;
    }

    if ((DL_I2C_getControllerStatus(I2C_OLED_INST) &
         DL_I2C_CONTROLLER_STATUS_ADDR_ACK) != 0U) {
        return 0U;
    }

    return 1U;
}

void OLED_ColorTurn(u8 i)
{
    OLED_WR_Byte((i == 0U) ? 0xA6U : 0xA7U, OLED_CMD);
}

void OLED_DisplayTurn(u8 i)
{
    if (i == 0U) {
        OLED_WR_Byte(0xC8U, OLED_CMD);
        OLED_WR_Byte(0xA1U, OLED_CMD);
    } else {
        OLED_WR_Byte(0xC0U, OLED_CMD);
        OLED_WR_Byte(0xA0U, OLED_CMD);
    }
}

void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8DU, OLED_CMD);
    OLED_WR_Byte(0x14U, OLED_CMD);
    OLED_WR_Byte(0xAFU, OLED_CMD);
}

void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8DU, OLED_CMD);
    OLED_WR_Byte(0x10U, OLED_CMD);
    OLED_WR_Byte(0xAEU, OLED_CMD);
}

void OLED_Refresh(void)
{
    u8 i;
    u8 n;

    for (i = 0U; i < (OLED_HEIGHT / 8U); i++) {
        OLED_WR_Byte(0xB0U + i, OLED_CMD);
        OLED_WR_Byte(0x00U, OLED_CMD);
        OLED_WR_Byte(0x10U, OLED_CMD);
        for (n = 0U; n < OLED_WIDTH; n++) {
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

void OLED_Clear(void)
{
    u8 i;
    u8 n;

    for (i = 0U; i < (OLED_HEIGHT / 8U); i++) {
        for (n = 0U; n < OLED_WIDTH; n++) {
            OLED_GRAM[n][i] = 0U;
        }
    }
    OLED_Refresh();
}

void OLED_DrawPoint(u8 x, u8 y, u8 t)
{
    u8 page;
    u8 bit;

    if ((x >= OLED_WIDTH) || (y >= OLED_HEIGHT)) {
        return;
    }

    page = y / 8U;
    bit = 1U << (y % 8U);

    if (t != 0U) {
        OLED_GRAM[x][page] |= bit;
    } else {
        OLED_GRAM[x][page] &= (u8)~bit;
    }
}

void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode)
{
    int16_t deltaX;
    int16_t deltaY;
    int16_t xerr = 0;
    int16_t yerr = 0;
    int16_t distance;
    int8_t incx;
    int8_t incy;
    u8 row = x1;
    u8 col = y1;
    u16 t;

    deltaX = (int16_t)x2 - (int16_t)x1;
    deltaY = (int16_t)y2 - (int16_t)y1;

    incx = (deltaX > 0) ? 1 : ((deltaX < 0) ? -1 : 0);
    incy = (deltaY > 0) ? 1 : ((deltaY < 0) ? -1 : 0);
    if (deltaX < 0) {
        deltaX = (int16_t)-deltaX;
    }
    if (deltaY < 0) {
        deltaY = (int16_t)-deltaY;
    }

    distance = (deltaX > deltaY) ? deltaX : deltaY;

    for (t = 0U; t <= (u16)distance; t++) {
        OLED_DrawPoint(row, col, mode);
        xerr += deltaX;
        yerr += deltaY;
        if (xerr > distance) {
            xerr -= distance;
            row += incx;
        }
        if (yerr > distance) {
            yerr -= distance;
            col += incy;
        }
    }
}

void OLED_DrawCircle(u8 x, u8 y, u8 r)
{
    int16_t a = 0;
    int16_t b = (int16_t)r;
    int16_t num;

    while ((2 * b * b) >= ((int16_t)r * (int16_t)r)) {
        OLED_DrawPoint((u8)(x + a), (u8)(y - b), 1U);
        OLED_DrawPoint((u8)(x - a), (u8)(y - b), 1U);
        OLED_DrawPoint((u8)(x - a), (u8)(y + b), 1U);
        OLED_DrawPoint((u8)(x + a), (u8)(y + b), 1U);
        OLED_DrawPoint((u8)(x + b), (u8)(y + a), 1U);
        OLED_DrawPoint((u8)(x + b), (u8)(y - a), 1U);
        OLED_DrawPoint((u8)(x - b), (u8)(y - a), 1U);
        OLED_DrawPoint((u8)(x - b), (u8)(y + a), 1U);

        a++;
        num = (a * a + b * b) - ((int16_t)r * (int16_t)r);
        if (num > 0) {
            b--;
            a--;
        }
    }
}

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode)
{
    u8 i;
    u8 m;
    u8 temp;
    u8 size2;
    u8 chr1;
    u8 x0 = x;
    u8 y0 = y;

    if (size1 == 8U) {
        size2 = 6U;
    } else {
        size2 = (size1 / 8U + ((size1 % 8U) ? 1U : 0U)) * (size1 / 2U);
    }
    chr1 = chr - ' ';

    for (i = 0U; i < size2; i++) {
        if (size1 == 8U) {
            temp = asc2_0806[chr1][i];
        } else if (size1 == 12U) {
            temp = asc2_1206[chr1][i];
        } else if (size1 == 16U) {
            temp = asc2_1608[chr1][i];
        } else if (size1 == 24U) {
            temp = asc2_2412[chr1][i];
        } else {
            return;
        }

        for (m = 0U; m < 8U; m++) {
            if ((temp & 0x01U) != 0U) {
                OLED_DrawPoint(x, y, mode);
            } else {
                OLED_DrawPoint(x, y, (u8)!mode);
            }
            temp >>= 1;
            y++;
        }

        x++;
        if ((size1 != 8U) && ((x - x0) == (size1 / 2U))) {
            x = x0;
            y0 += 8U;
        }
        y = y0;
    }
}

void OLED_ShowString(u8 x, u8 y, const char *chr, u8 size1, u8 mode)
{
    while ((*chr >= ' ') && (*chr <= '~')) {
        OLED_ShowChar(x, y, (u8)*chr, size1, mode);
        x += (size1 == 8U) ? 6U : (size1 / 2U);
        chr++;
    }
}

static u32 OLED_Pow(u8 m, u8 n)
{
    u32 result = 1U;
    while (n-- != 0U) {
        result *= m;
    }
    return result;
}

void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1, u8 mode)
{
    u8 t;
    u8 temp;
    u8 m = (size1 == 8U) ? 2U : 0U;

    for (t = 0U; t < len; t++) {
        temp = (u8)((num / OLED_Pow(10U, (u8)(len - t - 1U))) % 10U);
        OLED_ShowChar((u8)(x + (size1 / 2U + m) * t), y,
            (u8)(temp + '0'), size1, mode);
    }
}

void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1, u8 mode)
{
    u8 m;
    u8 temp;
    u8 x0 = x;
    u8 y0 = y;
    u16 i;
    u16 size3 = (size1 / 8U + ((size1 % 8U) ? 1U : 0U)) * size1;

    for (i = 0U; i < size3; i++) {
        if (size1 == 16U) {
            temp = Hzk1[num][i];
        } else if (size1 == 24U) {
            temp = Hzk2[num][i];
        } else if (size1 == 32U) {
            temp = Hzk3[num][i];
        } else if (size1 == 64U) {
            temp = Hzk4[num][i];
        } else {
            return;
        }

        for (m = 0U; m < 8U; m++) {
            if ((temp & 0x01U) != 0U) {
                OLED_DrawPoint(x, y, mode);
            } else {
                OLED_DrawPoint(x, y, (u8)!mode);
            }
            temp >>= 1;
            y++;
        }

        x++;
        if ((x - x0) == size1) {
            x = x0;
            y0 += 8U;
        }
        y = y0;
    }
}

void OLED_ShowPicture(u8 x, u8 y, u8 sizex, u8 sizey, const u8 *BMP, u8 mode)
{
    u16 j = 0U;
    u8 i;
    u8 n;
    u8 m;
    u8 temp;
    u8 x0 = x;
    u8 y0 = y;

    sizey = sizey / 8U + ((sizey % 8U) ? 1U : 0U);

    for (n = 0U; n < sizey; n++) {
        for (i = 0U; i < sizex; i++) {
            temp = BMP[j++];
            for (m = 0U; m < 8U; m++) {
                if ((temp & 0x01U) != 0U) {
                    OLED_DrawPoint(x, y, mode);
                } else {
                    OLED_DrawPoint(x, y, (u8)!mode);
                }
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex) {
                x = x0;
                y0 += 8U;
            }
            y = y0;
        }
    }
}

void OLED_Init(void)
{
    delay_ms(100);

    OLED_WR_Byte(0xAEU, OLED_CMD);
    OLED_WR_Byte(0x00U, OLED_CMD);
    OLED_WR_Byte(0x10U, OLED_CMD);
    OLED_WR_Byte(0x40U, OLED_CMD);
    OLED_WR_Byte(0x81U, OLED_CMD);
    OLED_WR_Byte(0xCFU, OLED_CMD);
    OLED_WR_Byte(0xA1U, OLED_CMD);
    OLED_WR_Byte(0xC8U, OLED_CMD);
    OLED_WR_Byte(0xA6U, OLED_CMD);
    OLED_WR_Byte(0xA8U, OLED_CMD);
    OLED_WR_Byte(0x3FU, OLED_CMD);
    OLED_WR_Byte(0xD3U, OLED_CMD);
    OLED_WR_Byte(0x00U, OLED_CMD);
    OLED_WR_Byte(0xD5U, OLED_CMD);
    OLED_WR_Byte(0x80U, OLED_CMD);
    OLED_WR_Byte(0xD9U, OLED_CMD);
    OLED_WR_Byte(0xF1U, OLED_CMD);
    OLED_WR_Byte(0xDAU, OLED_CMD);
    OLED_WR_Byte(0x12U, OLED_CMD);
    OLED_WR_Byte(0xDBU, OLED_CMD);
    OLED_WR_Byte(0x30U, OLED_CMD);
    OLED_WR_Byte(0x20U, OLED_CMD);
    OLED_WR_Byte(0x02U, OLED_CMD);
    OLED_WR_Byte(0x8DU, OLED_CMD);
    OLED_WR_Byte(0x14U, OLED_CMD);
    OLED_WR_Byte(0xA4U, OLED_CMD);
    OLED_WR_Byte(0xA6U, OLED_CMD);
    OLED_WR_Byte(0xAFU, OLED_CMD);

    OLED_Clear();
}
