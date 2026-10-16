/**
  ******************************************************************************
  * @file    IMU_alg.c
  * @brief   IMU 姿态解算实现。
  *          采用互补滤波融合加速度计与陀螺仪数据，输出 roll / pitch / yaw。
  ******************************************************************************
  */
  
#include "IMU_alg.h"
#include <math.h>

/**
  * @name 姿态解算参数
  * @{
  */

#define ALPHA        0.98f      // 陀螺仪权重
#define DT           0.005f     // 采样周期（configTICK_RATE_HZ = 1000Hz）
#define GYRO_SCALE   16.4f      // 陀螺仪灵敏度
#define ACC_SCALE    16384.0f   // 加速度计灵敏度
#define RAD2DEG      57.2958f   // 弧度转角度系数


/**
  * @brief  全局姿态角，由 IMU_UpdateAttitude() 更新。
  */

Attitude_t attitude = {0};


/**
  * @brief  更新姿态角（互补滤波）。
  * @param  acc  指向加速度计原始数据的指针，依次为 X、Y、Z 轴。
  * @param  gyro 指向陀螺仪原始数据的指针，依次为 X、Y、Z 轴。
  * @note   处理流程：
  *         1. 原始数据转换为物理量；
  *         2. 由加速度计计算 roll / pitch；
  *         3. 由陀螺仪积分得到 roll / pitch；
  *         4. 互补滤波融合；
  *         5. yaw 仅由陀螺仪积分，并统一到 [-180, 180]。
  * @warning 调用周期必须与 DT 一致，否则姿态解算会出现偏差。
  */

void IMU_UpdateAttitude(int16_t *acc, int16_t *gyro)
{
    float ax = acc[0] / ACC_SCALE;
    float ay = acc[1] / ACC_SCALE;
    float az = acc[2] / ACC_SCALE;
    float gx = gyro[0] / GYRO_SCALE;
    float gy = gyro[1] / GYRO_SCALE;
    float gz = gyro[2] / GYRO_SCALE;
    
    
    // 加速度计算Roll、Pitch
    float accRoll  = atan2f(ay, az) * RAD2DEG;
    float accPitch = atan2f(-ax, sqrtf(ay*ay + az*az)) * RAD2DEG;
    
    
    // 陀螺仪积分计算Roll、Pitch
    float gyroRoll  = attitude.roll  + gx * DT;
    float gyroPitch = attitude.pitch + gy * DT;

    /* 互补滤波融合 */
    attitude.roll  = ALPHA * gyroRoll  + (1 - ALPHA) * accRoll;
    attitude.pitch = ALPHA * gyroPitch + (1 - ALPHA) * accPitch;

    /* Yaw仅由陀螺仪积分 */
    attitude.yaw += gz * DT;
    
    /* Yaw统一到 [-180, 180] */
    if (attitude.yaw >  180) 
    {
        attitude.yaw -= 360;
    }

    if (attitude.yaw < -180) 
    {
        attitude.yaw += 360;
    }

}

