#include "stm32f1xx_hal.h"
#include "Serial.h"

extern UART_HandleTypeDef huart1;

uint8_t  RxDmaBuf[RX_BUF_SIZE];
uint8_t  TxBuf[RX_BUF_SIZE];
volatile uint16_t RxLen = 0;
SemaphoreHandle_t UsartRxSem = NULL;

void Usart_Init(void)
{
    UsartRxSem = xSemaphoreCreateBinary();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, RxDmaBuf, RX_BUF_SIZE);
}

void Usart_IDLE_Handler(void)
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        HAL_UART_DMAStop(&huart1);
        RxLen = RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);

        BaseType_t woken = pdFALSE;
        xSemaphoreGiveFromISR(UsartRxSem, &woken);
        portYIELD_FROM_ISR(woken);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        RxLen = Size;                          // 记录本次实际收到的长度
        BaseType_t hpw = pdFALSE;
        xSemaphoreGiveFromISR(UsartRxSem, &hpw); // 唤醒任务
        portYIELD_FROM_ISR(hpw);
    }
}
