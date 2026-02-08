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
        Accelerometer _accelerometer;
        Gyroscope _gyroscope;
        PreciseDateTime _timestamp;
 
    public:
        static const uint8_t MAX_INTERRUPTS = 5;
        
        /**
         * @brief Initializes MPU6050 sensor
         * 
         * @return true if initilization succeeds, otherwise false
         */
        bool init();
        
        /**
         * @brief Reads data from MPU6050 sensor and saves them into provided arguments
         * 
         * @param accelData reference to accelerometer data object
         * @param gyroData reference to gyroscope data object
         * @param temp reference to temperature variable
         */
        void readData(FloatTuple3& accelData, FloatTuple3& gyroData, float& temp);
        
        /**
         * @brief Reads data from MPU6050 sensor and saves them into sensors objects
         * 
         */
        void readData();

        /**
         * @brief Gets measured IMU data as @p IMUSample
         * 
         * @param accelUnit accelerometer unit
         * @param gyroUnit gyroscope unit
         * @param magUnit magnetometer unit
         * @return IMUSample 
         */
        IMUSample getIMUSample(Unit accelUnit = UNIT_MS2, Unit gyroUnit = UNIT_RAD_S, Unit magUnit = UNIT_MICROTESLA);
        
        /**
         * @brief Logs measured data into SD card
         */
        void logData();
};

#endif