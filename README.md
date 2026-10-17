ROBO_TASK-ADVANCED

利用STM32F103C8T6使用FreeRTOS控制IMU、OLED、USART的初筛高级任务文件

硬件介绍：     
使用STM32F103C8T6、STlink、OLED（IIC）、MPU6050（IIC）、CH340（USART）

实现部分：     
1.该工程使用CubeMX生成初始化代码。   
2.IIC均使用硬件IIC，希望少占用CPU。   
  OLED使用IIC1、MPU6050使用IIC2    
3.USART使用硬件USART1


电路介绍：     
OLED：接在PB6、PB7，使用其默认复用功能（IIC1）      
MPU6050：接在PB10、PB11，使用其默认复用功能（IIC2）      
CH340：接在PA9、PA10，使用其默认复用功能（USART1）      
全部共地、使用STM32的VCC统一供电，STM32由STLink供电。


YZDX