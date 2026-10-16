/**
  ******************************************************************************
  * @file    IMU.c
  * @brief   MPU6050 驱动实现。
  *          基于 STM32 HAL I2C 接口，完成 MPU6050 的初始化与原始数据读取。
  ******************************************************************************
  */

#include "IMU.h"


/**
  * @brief  初始化 MPU6050。
  * @param  hi2c 指向已初始化的 I2C 句柄。
  * @note   配置内容：
  *         - 0x6B 电源管理：退出睡眠模式
  *         - 0x1B 陀螺仪量程：±2000 dps
  *         - 0x1C 加速度计量程：±2g
  *         - 0x19 采样率分频：1kHz / (1+7) = 125Hz
  */
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


/**
  * @brief  读取 MPU6050 原始数据。
  * @param  hi2c 指向已初始化的 I2C 句柄。
  * @param  acc  输出加速度计原始数据，依次为 X、Y、Z 轴。
  * @param  gyro 输出陀螺仪原始数据，依次为 X、Y、Z 轴。
  * @note   从寄存器 0x3B 起连续读取 14 字节，buf[0..5] 为加速度计，
  *         buf[6..7] 为温度，buf[8..13] 为陀螺仪。
  */
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
