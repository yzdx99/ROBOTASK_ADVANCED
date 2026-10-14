#ifndef __OLED_H
#define __OLED_H

#include "main.h"

void OLED_Init(I2C_HandleTypeDef *hi2c);
void OLED_WriteCommand(uint8_t Command);

void OLED_WriteData(uint8_t Data);
void OLED_SetCursor(uint8_t Y, uint8_t X);
void OLED_Clear(void);

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

#endif
