#include "IMU_alg.h"
#include <math.h>

#define ALPHA        0.98f
#define DT           0.005f
#define GYRO_SCALE   16.4f
#define ACC_SCALE    16384.0f
#define RAD2DEG      57.2958f

Attitude_t attitude = {0};

void IMU_UpdateAttitude(int16_t *acc, int16_t *gyro)
{
    float ax = acc[0] / ACC_SCALE;
    float ay = acc[1] / ACC_SCALE;
    float az = acc[2] / ACC_SCALE;
    float gx = gyro[0] / GYRO_SCALE;
    float gy = gyro[1] / GYRO_SCALE;
    float gz = gyro[2] / GYRO_SCALE;

    float accRoll  = atan2f(ay, az) * RAD2DEG;
    float accPitch = atan2f(-ax, sqrtf(ay*ay + az*az)) * RAD2DEG;

    float gyroRoll  = attitude.roll  + gx * DT;
    float gyroPitch = attitude.pitch + gy * DT;

    attitude.roll  = ALPHA * gyroRoll  + (1 - ALPHA) * accRoll;
    attitude.pitch = ALPHA * gyroPitch + (1 - ALPHA) * accPitch;

    attitude.yaw += gz * DT;
    
    if (attitude.yaw >  180) 
    {
        attitude.yaw -= 360;
    }

    if (attitude.yaw < -180) 
    {
        attitude.yaw += 360;
    }

}

