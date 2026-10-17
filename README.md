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


示例视频：    
包含了主要的代码部分（Keil以及CubeMX的部分配置）。      
单独提交了一份手机相机录制的现象视频，会更清晰一些。     


（很多地方自己还没完全学明白， 所以使用了AI一边生成一边学；也有很多头文件是从江协那里找来作了一些修改就用的）

NOTICE.md是过程中遇到的一些卡了比较久的地方，以及一些提醒自己以后注意的地方。

YZDX