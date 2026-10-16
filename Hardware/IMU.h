/**
  ******************************************************************************
  * @file    IMU.h
  * @brief   MPU6050 驱动头文件。
  *          提供 MPU6050 初始化与原始数据读取接口。
  ******************************************************************************
  */

#ifndef __IMU_H
#define __IMU_H

#include "main.h"
#include <stdint.h>


/**
  * @brief  MPU6050地址
  */
#define MPU6050_ADDR 0xD0


/**
  * @brief  初始化 MPU6050。
  *         依次配置电源管理、陀螺仪量程、加速度计量程与采样率。
  * @param  hi2c 指向已初始化的 I2C 句柄。
  */
void MPU6050_Init(I2C_HandleTypeDef *hi2c);


/**
  * @brief  读取 MPU6050 原始数据。
  *         一次性读取 14 字节，解析出加速度计与陀螺仪的 X、Y、Z 轴数据。
  * @param  hi2c 指向已初始化的 I2C 句柄。
  * @param  acc  输出加速度计原始数据，依次为 X、Y、Z 轴。
  * @param  gyro 输出陀螺仪原始数据，依次为 X、Y、Z 轴。
  */
void MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, int16_t *acc, int16_t *gyro);

#endif
