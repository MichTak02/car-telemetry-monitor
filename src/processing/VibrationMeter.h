#ifndef VIBRATION_METER_H
#define VIBRATION_METER_H

#include <Arduino.h>
#include "definitions.h"
#include "Logger.h"
#include "SdReader.h"
#include "sensors/IMUDriver.h"

constexpr uint16_t HIGH_THRESHOLD_G = 15;
constexpr uint16_t MEDIUM_THRESHOLD_G = 10;
constexpr uint16_t LOW_THRESHOLD_G = 5;
constexpr uint16_t THRESHOLD_TIMEOUT = 500; // Time in ms to wait before logging another high vibration event

class VibrationMeter {
    private:
        IMUDriver& _imuDriver;

        bool _enabled = true;
        float _vibrationLevel;
        const float alpha = 0.9f;

        float ax_prev = 0.0f;
        float ay_prev = 0.0f;
        float az_prev = 0.0f;
        float ax_hp = 0.0f;
        float ay_hp = 0.0f;
        float az_hp = 0.0f;

        uint16_t currentThreshold;
        uint32_t lastThresholdExceedTime = 0;
        bool thresholdExceeded = false;

    public:
        VibrationMeter(IMUDriver& imuDriver, ImpactThresholdLevel sensitivityLevel) : _imuDriver(imuDriver) {
            setImpactThresholdLevel(sensitivityLevel);
        }

        /**
         * @brief Enables vibration monitoring
         */
        void enable();

        /**
         * @brief Disables vibration monitoring
         */
        void disable();

        /**
         * @brief Reads IMU data, applies high-pass filter, and updates the vibration level
         */
        void update();

        /**
         * @brief Returns the current vibration level
         *
         * @return vibration level
         */
        float getVibrationLevel();

        /**
         * @brief Sets the impact detection threshold based on sensitivity level
         *
         * @param sensitivityLevel desired impact sensitivity
         */
        void setImpactThresholdLevel(ImpactThresholdLevel sensitivityLevel);

        /**
         * @brief Checks if vibration exceeds the threshold and logs an impact event if so
         */
        void checkVibrationLevel();
};

#endif