#include "AccelerationMagnitude.h"

void AccelerationMagnitude::enable()
{
    _isInitialized = false;
    _enabled = true;
}

void AccelerationMagnitude::disable()
{
    _enabled = false;
}

void AccelerationMagnitude::update()
{
    if (!_enabled) {
        return;
    }
    IMUSample sample = _imuDriver.getIMUSample(Accelerometer::DEFAULT_UNIT, UNIT_DEG_S, Magnetometer::DEFAULT_UNIT, true);

    float ax = sample.accel.x;
    float ay = sample.accel.y;
    float az = sample.accel.z;

    if (!_isInitialized) {
        _gx = ax;
        _gy = ay;
        _gz = az;
        _axLowPass = 0.0f;
        _ayLowPass = 0.0f;
        _azLowPass = 0.0f;
        _acceleration = 0.0f;
        _isInitialized = true;
        return;
    }

    // Gravity low pass filter
    _gx = GRAVITY_ALPHA * ax + (1 - GRAVITY_ALPHA) * _gx;
    _gy = GRAVITY_ALPHA * ay + (1 - GRAVITY_ALPHA) * _gy;
    _gz = GRAVITY_ALPHA * az + (1 - GRAVITY_ALPHA) * _gz;

    // Remove gravity from acceleration
    ax -= _gx;
    ay -= _gy;
    az -= _gz;

    // Acceleration low pass filter to reduce noise
    _axLowPass = ACCEL_ALPHA * ax + (1 - ACCEL_ALPHA) * _axLowPass;
    _ayLowPass = ACCEL_ALPHA * ay + (1 - ACCEL_ALPHA) * _ayLowPass;
    _azLowPass = ACCEL_ALPHA * az + (1 - ACCEL_ALPHA) * _azLowPass;

    float totalAcceleration = sqrtf(_axLowPass * _axLowPass + _ayLowPass * _ayLowPass + _azLowPass * _azLowPass);
    _acceleration = totalAcceleration;

}

float AccelerationMagnitude::getAcceleration()
{
    return _acceleration;
}
