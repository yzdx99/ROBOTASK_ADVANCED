#ifndef __IMU_H
#define __IMU_H

#include "main.h"
#include <stdint.h>

#define MPU6050_ADDR 0xD0

void MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, int16_t *acc, int16_t *gyro);

#endif
