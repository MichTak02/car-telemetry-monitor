#ifndef IMU_DRIVER_H
#define IMU_DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "definitions.h"
#include "utils/TimeUtils.h"

class IMUDriver {
    private:
        Adafruit_MPU6050 _mpu;
        float gx, gy, gz, ax, ay, az, mx, my, mz;

        // TODO nastavitelné shifty
        float gx_shift = 10.4;
        float gy_shift = 0;
        float gz_shift = -0.6;

        Accelerometer _accelerometer;
        Gyroscope _gyroscope;
        PreciseDateTime _timestamp;
 
    public:
        static const uint8_t MAX_INTERRUPTS = 5;
        bool init();
        bool readData(FloatTuple3& accelData, FloatTuple3& gyroData, float& temp);
        bool readData();
        IMUSample getIMUSample(Unit accelUnit = UNIT_MS2, Unit gyroUnit = UNIT_RAD_S, Unit magUnit = UNIT_MICROTESLA);
        void logData();
};

#endif