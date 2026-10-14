#include "OLED.h"
#include "OLED_Font.h"

#include "FreeRTOS.h"
#include "task.h"

#define OLED_ADDR 0x78         // 从机地址（7位0x3C左移1位）

static I2C_HandleTypeDef *oled_hi2c;   // 保存传进来的 I2C 句柄




/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
    uint8_t buf[2] = {0x00, Command};   // 0x00 = 命令
    HAL_I2C_Master_Transmit(oled_hi2c, OLED_ADDR, buf, 2, 100);
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
    uint8_t buf[2] = {0x40, Data};      // 0x40 = 数据
    HAL_I2C_Master_Transmit(oled_hi2c, OLED_ADDR, buf, 2, 100);
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}


/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}



/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */

void OLED_Init(I2C_HandleTypeDef *hi2c)
{
    vTaskDelay(pdMS_TO_TICKS(200));
    
    oled_hi2c = hi2c;        // ← 保存句柄

    
    OLED_WriteCommand(0xAE);   // 关显示
    OLED_WriteCommand(0xD5);   // 时钟分频
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8);   // 多路复用
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3);   // 显示偏移
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);   // 起始行
    OLED_WriteCommand(0xA1);   // 左右方向
    OLED_WriteCommand(0xC8);   // 上下方向
    OLED_WriteCommand(0xDA);   // COM 引脚
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81);   // 对比度
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9);   // 预充电
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB);   // VCOMH
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);   // 整个显示开
    OLED_WriteCommand(0xA6);   // 正常显示
    OLED_WriteCommand(0x8D);   // 电荷泵
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);   // 开显示
    
    
    OLED_Clear();
}
