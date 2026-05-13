#ifndef VIBRATION_ANALYZER_H
#define VIBRATION_ANALYZER_H

#include <Arduino.h>
#include "definitions.h"
#include "Logger.h"
#include "SdReader.h"
#include "sensors/IMUDriver.h"

constexpr uint16_t HIGH_THRESHOLD = 1000;
constexpr uint16_t MEDIUM_THRESHOLD = 500;
constexpr uint16_t LOW_THRESHOLD = 100;
constexpr uint16_t THRESHOLD_TIMEOUT = 500; // Time in ms to wait before logging another high vibration event

class VibrationAnalyzer {
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

    public:
        VibrationAnalyzer(IMUDriver& imuDriver, ImpactThresholdLevel sensitivityLevel) : _imuDriver(imuDriver) {
            setImpactThresholdLevel(sensitivityLevel);
        }

        void enable();
        void disable();
        void update();
        float getVibrationLevel();

        void setImpactThresholdLevel(ImpactThresholdLevel sensitivityLevel);
        void checkVibrationLevel();
};

#endif
