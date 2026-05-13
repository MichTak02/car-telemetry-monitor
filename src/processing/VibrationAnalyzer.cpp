#include "VibrationAnalyzer.h"

void VibrationAnalyzer::enable()
{
    _vibrationLevel = 0.0f;
    ax_prev = 0.0f;
    ay_prev = 0.0f;
    az_prev = 0.0f;
    ax_hp = 0.0f;
    ay_hp = 0.0f;
    az_hp = 0.0f;
    lastThresholdExceedTime = 0;
    _enabled = true;
}

void VibrationAnalyzer::disable()
{
    _enabled = false;
}

void VibrationAnalyzer::update()
{
    if (!_enabled) {
        return;
    }
    IMUSample sample = _imuDriver.getIMUSample();
    FloatTuple3 accelData = sample.accel;
        
    ax_hp = alpha * (ax_hp + accelData.x - ax_prev);
    ay_hp = alpha * (ay_hp + accelData.y - ay_prev);
    az_hp = alpha * (az_hp + accelData.z - az_prev);

    ax_prev = accelData.x;
    ay_prev = accelData.y;
    az_prev = accelData.z;

    _vibrationLevel = sqrt(ax_hp * ax_hp + ay_hp * ay_hp + az_hp * az_hp);
}

float VibrationAnalyzer::getVibrationLevel()
{
    return _vibrationLevel;
}

void VibrationAnalyzer::setImpactThresholdLevel(ImpactThresholdLevel sensitivityLevel)
{
    switch (sensitivityLevel) {
        case IMPACT_THRESHOLD_LOW:
            currentThreshold = LOW_THRESHOLD;
            break;
        case IMPACT_THRESHOLD_MEDIUM:
            currentThreshold = MEDIUM_THRESHOLD;
            break;
        case IMPACT_THRESHOLD_HIGH:
            currentThreshold = HIGH_THRESHOLD;
            break;
        
        // Should not get there
        default:
            Logger::log(LOG_WARN, "Invalid sensitivity level, defaulting to medium");
            currentThreshold = MEDIUM_THRESHOLD;
    }
}

void VibrationAnalyzer::checkVibrationLevel()
{
    if (_vibrationLevel <= currentThreshold) {
        return;
    }

    if (millis() - lastThresholdExceedTime < THRESHOLD_TIMEOUT) {
        lastThresholdExceedTime = millis();
        return;
    }

    lastThresholdExceedTime = millis();

    Logger::log(LOG_WARN, "High vibration detected, locking current file");
    SdReader::lockFile();
}
