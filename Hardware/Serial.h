/**
  ******************************************************************************
  * @file    Serial.h
  * @brief   串口驱动头文件（USART1，DMA + FreeRTOS 信号量）。
  *          提供基于 DMA 的空闲中断接收与发送完成通知机制，
  *          通过二值信号量在中断与任务之间同步。
  ******************************************************************************
  */

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

/**
  * @brief  串口收发缓冲区大小（字节）。
  */
#define RX_BUF_SIZE 256

/**
  * @brief  DMA 接收缓冲区，由 HAL_UARTEx_ReceiveToIdle_DMA() 填充。
  */
extern uint8_t  RxDmaBuf[RX_BUF_SIZE];

/**
  * @brief  发送缓冲区，供任务组装待发送数据。
  */
extern uint8_t  TxBuf[RX_BUF_SIZE];

/**
  * @brief  最近一次 DMA 接收的实际字节数，由接收事件回调更新。
  * @note   volatile 修饰，因为它在中断中被写入、在任务中被读取。
  */
extern volatile uint16_t RxLen;

/**
  * @brief  接收完成信号量。空闲中断触发时由 ISR 给出，任务中获取。
  */
extern SemaphoreHandle_t UsartRxSem;

/**
  * @brief  发送完成信号量。DMA 发送完成中断触发时由 ISR 给出。
  */
extern SemaphoreHandle_t UsartTxSem;


/**
  * @brief  初始化串口驱动。
  * @note   创建收发信号量，启动 DMA 空闲中断接收，并关闭 DMA 半传输中断
  *         （DMA_IT_HT），避免每收一半就触发一次回调。
  */
void Usart_Init(void);

/**
  * @brief  串口接收事件回调（空闲中断 + DMA）。
  * @param  huart 触发回调的 UART 句柄。
  * @param  Size  本次实际接收到的字节数。
  * @note   由 HAL 在空闲中断中调用，内部给出 UsartRxSem 唤醒接收任务。
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);


#endif
