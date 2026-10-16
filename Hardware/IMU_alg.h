/**
  ******************************************************************************
  * @file    IMU_alg.h
  * @brief   IMU 姿态解算模块头文件。
  *          提供姿态角结构体定义与姿态更新接口，采用互补滤波融合
  *          加速度计与陀螺仪数据，输出 roll / pitch / yaw。
  ******************************************************************************
  */

#ifndef __IMU_ALG_H
#define __IMU_ALG_H

#include <stdint.h>


/**
  * @brief  姿态角结构体（单位：度）。
  */

typedef struct {
    float roll;               // 横滚角
    float pitch;              // 俯仰角
    float yaw;                // 偏航角
} Attitude_t;


/**
  * @brief  全局姿态角，由 IMU_UpdateAttitude() 更新。
  */

extern Attitude_t attitude;

/**
  * @brief  更新姿态角（互补滤波）。
  * @param  acc  指向加速度计原始数据的指针，依次为 X、Y、Z 轴。
  * @param  gyro 指向陀螺仪原始数据的指针，依次为 X、Y、Z 轴。
  * @note   调用周期应与 IMU_alg.c 中的 DT 保持一致。
  */

void IMU_UpdateAttitude(int16_t *acc, int16_t *gyro);

#endif
