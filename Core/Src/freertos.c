/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#include "queue.h"

#include "i2c.h"
#include "usart.h"

#include "Serial.h"
#include "IMU.h"
#include "IMU_alg.h"
#include "OLED.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

QueueHandle_t ImuDataQueue;

/* USER CODE END Variables */
/* Definitions for OledTask */
osThreadId_t OledTaskHandle;
const osThreadAttr_t OledTask_attributes = {
  .name = "OledTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ImuTask */
osThreadId_t ImuTaskHandle;
const osThreadAttr_t ImuTask_attributes = {
  .name = "ImuTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UsartTask */
osThreadId_t UsartTaskHandle;
const osThreadAttr_t UsartTask_attributes = {
  .name = "UsartTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */


/* USER CODE END FunctionPrototypes */

void StartOledTask(void *argument);
void StartImuTask(void *argument);
void StartUsartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    ImuDataQueue = xQueueCreate(1, sizeof(Attitude_t));    // 长度1，Item 12
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of OledTask */
  OledTaskHandle = osThreadNew(StartOledTask, NULL, &OledTask_attributes);

  /* creation of ImuTask */
  ImuTaskHandle = osThreadNew(StartImuTask, NULL, &ImuTask_attributes);

  /* creation of UsartTask */
  UsartTaskHandle = osThreadNew(StartUsartTask, NULL, &UsartTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartOledTask */
/**
  * @brief  Function implementing the OledTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartOledTask */
void StartOledTask(void *argument)
{
  /* USER CODE BEGIN StartOledTask */
  char buf[14];
  Attitude_t att;

  OLED_Init(&hi2c1);
  
  OLED_Clear();

  /* Infinite loop */
  for(;;)
  {
      if (xQueuePeek(ImuDataQueue, &att, pdMS_TO_TICKS(100)) == pdTRUE)
      {
          sprintf(buf, "Roll :%4.1f", att.roll);
          OLED_ShowString(1, 1, buf);
          sprintf(buf, "Pitch:%4.1f", att.pitch);
          OLED_ShowString(2, 1, buf);
          sprintf(buf, "Yaw  :%4.1f", att.yaw);
          OLED_ShowString(3, 1, buf);
      }
      vTaskDelay(pdMS_TO_TICKS(50));
  }
  /* USER CODE END StartOledTask */
}

/* USER CODE BEGIN Header_StartImuTask */
/**
* @brief Function implementing the ImuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartImuTask */
void StartImuTask(void *argument)
{
  /* USER CODE BEGIN StartImuTask */
  int16_t acc[3], gyro[3];
  TickType_t lastWake = xTaskGetTickCount();

  MPU6050_Init(&hi2c2);


  /* Infinite loop */
  for(;;)
  {
      MPU6050_ReadRaw(&hi2c2, acc, gyro);
      IMU_UpdateAttitude(acc, gyro);
      xQueueOverwrite(ImuDataQueue, &attitude);
      vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(5));
  }
  /* USER CODE END StartImuTask */
}

/* USER CODE BEGIN Header_StartUsartTask */
/**
* @brief Function implementing the UsartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsartTask */
void StartUsartTask(void *argument)
{
  /* USER CODE BEGIN StartUsartTask */
    
  Usart_Init();
    
  /* Infinite loop */
  for(;;)
  {
    if (xSemaphoreTake(UsartRxSem, portMAX_DELAY) == pdTRUE)
    {
        uint16_t Len = RxLen;
        
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxDmaBuf, RX_BUF_SIZE);
        __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
        
        memcpy(TxBuf, RxDmaBuf, Len);
        HAL_UART_Transmit_DMA(&huart1, TxBuf, Len);
        
        xSemaphoreTake(UsartTxSem, portMAX_DELAY);
    }
  }
  /* USER CODE END StartUsartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

