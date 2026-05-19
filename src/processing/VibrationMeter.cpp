#include "VibrationMeter.h"

void VibrationMeter::enable()
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

void VibrationMeter::disable()
{
    _enabled = false;
}

void VibrationMeter::update()
{
    if (!_enabled) {
        return;
    }
    IMUSample sample = _imuDriver.getIMUSample(UNIT_G);
    FloatTuple3 accelData = sample.accel;
        
    ax_hp = alpha * (ax_hp + accelData.x - ax_prev);
    ay_hp = alpha * (ay_hp + accelData.y - ay_prev);
    az_hp = alpha * (az_hp + accelData.z - az_prev);

    ax_prev = accelData.x;
    ay_prev = accelData.y;
    az_prev = accelData.z;

    _vibrationLevel = sqrt(ax_hp * ax_hp + ay_hp * ay_hp + az_hp * az_hp);
}

float VibrationMeter::getVibrationLevel()
{
    return _vibrationLevel;
}

void VibrationMeter::setImpactThresholdLevel(ImpactThresholdLevel sensitivityLevel)
{
    switch (sensitivityLevel) {
        case IMPACT_THRESHOLD_LOW:
            currentThreshold = LOW_THRESHOLD_G;
            break;
        case IMPACT_THRESHOLD_MEDIUM:
            currentThreshold = MEDIUM_THRESHOLD_G;
            break;
        case IMPACT_THRESHOLD_HIGH:
            currentThreshold = HIGH_THRESHOLD_G;
            break;
        
        // Should not get there
        default:
            Logger::log(LOG_WARN, "Invalid sensitivity level, defaulting to medium");
            currentThreshold = MEDIUM_THRESHOLD_G;
    }
}

void VibrationMeter::checkVibrationLevel()
{
    if (_vibrationLevel <= currentThreshold) {
        return;
    }

    if (millis() - lastThresholdExceedTime < THRESHOLD_TIMEOUT && thresholdExceeded) {
        lastThresholdExceedTime = millis();
        return;
    }

    lastThresholdExceedTime = millis();
    thresholdExceeded = true;

    Logger::log(LOG_WARN, "High vibration detected, scheduling log lock");
    SdReader::scheduleLock();
}
