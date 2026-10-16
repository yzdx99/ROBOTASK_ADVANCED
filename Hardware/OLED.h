/**
  ******************************************************************************
  * @file    OLED.h
  * @brief   OLED 显示驱动头文件（SSD1306，I2C 接口，128x64）。
  *          提供初始化、清屏、字符/字符串/数字显示接口。
  ******************************************************************************
  */

#ifndef __OLED_H
#define __OLED_H

#include "main.h"


/**
  * @brief  初始化 OLED。
  * @param  hi2c 指向已初始化的 I2C 句柄。
  * @note   内部会延时 200ms 等待 OLED 上电稳定，并保存 I2C 句柄供后续使用。
  */
void OLED_Init(I2C_HandleTypeDef *hi2c);

/**
  * @brief  向 OLED 写入命令。
  * @param  Command 要写入的命令字节。
  */
void OLED_WriteCommand(uint8_t Command);

/**
  * @brief  向 OLED 写入数据。
  * @param  Data 要写入的数据字节。
  */
void OLED_WriteData(uint8_t Data);

/**
  * @brief  设置 OLED 光标位置。
  * @param  Y 页地址，范围：0~7（以左上角为原点，向下）。
  * @param  X 列地址，范围：0~127（以左上角为原点，向右）。
  */
void OLED_SetCursor(uint8_t Y, uint8_t X);

/**
  * @brief  清屏（全部写 0）。
  */
void OLED_Clear(void);

/**
  * @brief  显示一个字符（8x16 字模）。
  * @param  Line   行位置，范围：1~4。
  * @param  Column 列位置，范围：1~16。
  * @param  Char   要显示的 ASCII 可见字符。
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);

/**
  * @brief  显示字符串。
  * @param  Line   起始行位置，范围：1~4。
  * @param  Column 起始列位置，范围：1~16。
  * @param  String 要显示的字符串（ASCII 可见字符）。
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);

/**
  * @brief  显示带符号十进制整数。
  * @param  Line   起始行位置，范围：1~4。
  * @param  Column 起始列位置，范围：1~16。
  * @param  Number 要显示的数字，范围：-2147483648~2147483647。
  * @param  Length 要显示的数字位数，范围：1~10。
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

#endif
