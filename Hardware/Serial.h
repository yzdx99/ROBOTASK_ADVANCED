#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define RX_BUF_SIZE 256

extern uint8_t  RxDmaBuf[RX_BUF_SIZE];
extern uint8_t  TxBuf[RX_BUF_SIZE];
extern volatile uint16_t RxLen;
extern SemaphoreHandle_t UsartRxSem;


// 使用DMA收发

void Usart_Init(void);
void Usart_IDLE_Handler(void);



#endif
