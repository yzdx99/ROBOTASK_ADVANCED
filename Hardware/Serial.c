/**
  ******************************************************************************
  * @file    Serial.c
  * @brief   串口驱动实现（USART1，DMA + FreeRTOS 信号量）。
  *          采用 DMA + 空闲中断方式接收不定长数据，
  *          通过二值信号量通知任务；发送完成同样以信号量通知。
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"
#include "Serial.h"


/**
  * @brief  USART1 句柄，定义在 main.c 中。
  */
extern UART_HandleTypeDef huart1;

/**
  * @brief  DMA 接收、发送缓冲区。
  */
uint8_t  RxDmaBuf[RX_BUF_SIZE];
uint8_t  TxBuf[RX_BUF_SIZE];

/**
  * @brief  最近一次 DMA 接收的实际字节数。
  */
volatile uint16_t RxLen = 0;

/**
  * @brief  接收、发送完成信号量。
  */
SemaphoreHandle_t UsartRxSem = NULL;
SemaphoreHandle_t UsartTxSem = NULL;

/**
  * @brief  初始化串口驱动。
  * @note   执行流程：
  *         1. 创建接收、发送二值信号量；
  *         2. 启动 DMA 空闲中断接收（ReceiveToIdle_DMA）；
  *         3. 关闭 DMA 半传输中断，避免半满时误触发回调。
  */
void Usart_Init(void)
{
    UsartRxSem = xSemaphoreCreateBinary();
    UsartTxSem = xSemaphoreCreateBinary();
    
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxDmaBuf, RX_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

/**
  * @brief  串口接收事件回调（空闲中断 + DMA）。
  * @param  huart 触发回调的 UART 句柄。
  * @param  Size  本次实际接收到的字节数。
  * @note   由 HAL 在空闲中断中调用。记录长度后给出 UsartRxSem，
  *         若唤醒了更高优先级任务则触发一次上下文切换。
  */
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

/**
  * @brief  串口发送完成回调。
  * @param  huart 触发回调的 UART 句柄。
  * @note   由 HAL 在 DMA 发送完成中断中调用，给出 UsartTxSem 通知发送任务。
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        BaseType_t hpw = pdFALSE;
        xSemaphoreGiveFromISR(UsartTxSem, &hpw);
        portYIELD_FROM_ISR(hpw);
    }
}
