#include "ti_msp_dl_config.h"
#include "K230_RECEIVE.h"

#define K230_RX_BUF_SIZE 128U

static volatile uint8_t  RxBuf[128];    //缓冲区
static volatile uint16_t RxHead = 0;
static volatile uint16_t RxTail = 0;    //构成环形队列的头尾指针

static uint8_t State = 0U;              //状态位
static uint8_t FrameIdx = 0U;           //下标索引
static uint8_t FrameData[4];            //x,y暂存区
static uint8_t CheckByte = 0U;          


void K230_Init(void)
{
    NVIC_EnableIRQ(UART_CAMARE_INST_INT_IRQN);
}

void UART_CAMARE_INST_IRQHandler(void)
{
    if(DL_UART_Main_getPendingInterrupt(UART_CAMARE_INST) == DL_UART_MAIN_IIDX_RX)
    {
         while (!DL_UART_Main_isRXFIFOEmpty(UART_CAMARE_INST)) 
         {
                uint8_t byte = DL_UART_Main_receiveData(UART_CAMARE_INST);
                uint16_t next = RxHead + 1U;

                if (next >= K230_RX_BUF_SIZE) {
                    next = 0U;
                }

                if (next != RxTail) {
                    RxBuf[RxHead] = byte;
                    RxHead = next;
                }
        }

    }
}

uint8_t K230_ReadByte(uint8_t *byte)
{
    if (byte == NULL) {
        return 0U;
    }

    if (RxHead == RxTail) {
        return 0U;
    }

    *byte = RxBuf[RxTail];
    RxTail++;
    if (RxTail >= K230_RX_BUF_SIZE)
    {
        RxTail = 0U;
    }

    return 1U;
}


static uint8_t K230_ParseByte(uint8_t byte, K230_Target_t *target)
{
    uint8_t sum;

    switch (State) {
        case 0U:
            if (byte == 0xAAU) {
                State = 1U;
            }
            break;

        case 1U:
            if (byte == 0xAAU) {
                State = 2U;
                FrameIdx = 0U;
            } else {
                State = 0U;
            }
            break;

        case 2U:
            FrameData[FrameIdx++] = byte;
            if (FrameIdx >= 4U) {
                State = 3U;
            }
            break;

        case 3U:
            CheckByte = byte;
            State = 4U;
            break;

        case 4U:
            State = 0U;

            if (byte == 0xFFU) {
                sum = (uint8_t)(FrameData[0] + FrameData[1] +
                                FrameData[2] + FrameData[3]);

                if (sum == CheckByte) {
                    if (target != NULL) {
                        target->x = (uint16_t)((FrameData[0] << 8) | FrameData[1]);
                        target->y = (uint16_t)((FrameData[2] << 8) | FrameData[3]);
                        target->valid = 1U;
                    }
                    return 1U;
                }
            }
            break;

        default:
            State = 0U;
            break;
    }

    return 0U;
}

//判断是否接收成功，成功为1U,失败为0U
uint8_t K230_GetTarget(K230_Target_t *target)
{
    uint8_t byte;

    while (K230_ReadByte(&byte) != 0U) {
        if (K230_ParseByte(byte, target) != 0U) {
            return 1U;
        }
    }

    return 0U;
}