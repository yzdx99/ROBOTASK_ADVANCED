#include "IMU.h"



void MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data;
    data = 0x00;
    HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x6B, 1, &data, 1, 100);
    data = 0x18;
    HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x1B, 1, &data, 1, 100);
    data = 0x00;
    HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x1C, 1, &data, 1, 100);
    data = 0x07;
    HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, 0x19, 1, &data, 1, 100);
}

void MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, int16_t *acc, int16_t *gyro)
{
    uint8_t buf[14];
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, 0x3B, 1, buf, 14, 100);

    acc[0]  = (int16_t)(buf[0]  << 8 | buf[1]);
    acc[1]  = (int16_t)(buf[2]  << 8 | buf[3]);
    acc[2]  = (int16_t)(buf[4]  << 8 | buf[5]);
    gyro[0] = (int16_t)(buf[8]  << 8 | buf[9]);
    gyro[1] = (int16_t)(buf[10] << 8 | buf[11]);
    gyro[2] = (int16_t)(buf[12] << 8 | buf[13]);
}
