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
        static constexpr uint32_t CALIBRATION_DURATION_MS = 3000;
        static constexpr uint32_t CALIBRATION_SAMPLE_INTERVAL_MS = 5;
        const byte I2C_ADDRESS = 0x69;
        StatusFlags& _statusFlags;
        Adafruit_MPU6050 _mpu;
        Accelerometer _accelerometer;
        Gyroscope _gyroscope;
        PreciseDateTime _timestamp;
 
    public:
        static const uint8_t MAX_INTERRUPTS = 5;

        IMUDriver(StatusFlags& statusFlags) : _statusFlags(statusFlags) {}
        
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
         * @param calibrated true for calibrated values, false for raw values
         * @return IMUSample
         */
        IMUSample getIMUSample(Unit accelUnit = UNIT_MS2, Unit gyroUnit = UNIT_RAD_S, bool calibrated = false);
        
        /**
         * @brief Overload for getIMUSample with default units, only calibrated parameter
         * 
         * @param calibrated true for calibrated values, false for raw values
         * @return IMUSample 
         */
        IMUSample getIMUSample(bool calibrated);

        /**
         * @brief Logs measured data into SD card
         */
        void logData();

        /**
         * @brief Calibrate gyroscope bias by averaging readings while stationary
         */
        void calibrate();
};

#endif