#ifndef __IMU_ALG_H
#define __IMU_ALG_H

#include <stdint.h>

typedef struct {
    float roll;
    float pitch;
    float yaw;
} Attitude_t;

extern Attitude_t attitude;

void IMU_UpdateAttitude(int16_t *acc, int16_t *gyro);

#endif
