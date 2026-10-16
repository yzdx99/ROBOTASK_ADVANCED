#include "stm32f1xx_hal.h"
#include "Serial.h"

extern UART_HandleTypeDef huart1;

uint8_t  RxDmaBuf[RX_BUF_SIZE];
uint8_t  TxBuf[RX_BUF_SIZE];
volatile uint16_t RxLen = 0;
SemaphoreHandle_t UsartRxSem = NULL;
SemaphoreHandle_t UsartTxSem = NULL;


void Usart_Init(void)
{
    UsartRxSem = xSemaphoreCreateBinary();
    UsartTxSem = xSemaphoreCreateBinary();
    
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxDmaBuf, RX_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
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


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        BaseType_t hpw = pdFALSE;
        xSemaphoreGiveFromISR(UsartTxSem, &hpw);
        portYIELD_FROM_ISR(hpw);
    }
}
