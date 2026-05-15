#include "MotionFusion.h"

void MotionFusion::enable()
{
    _fusion.begin(20);
    _orientation = {0.0f, 0.0f, 0.0f};
    _enabled = true;
}

void MotionFusion::disable()
{
    _enabled = false;
}

void MotionFusion::update()
{
    if (!_enabled) {
        return;
    }
    IMUSample sample = _imuDriver.getIMUSample(Accelerometer::DEFAULT_UNIT, UNIT_DEG_S, true);

    _fusion.updateIMU(sample.gyro.x, sample.gyro.y, sample.gyro.z, sample.accel.x, sample.accel.y, sample.accel.z);
}

FloatTuple3 MotionFusion::getOrientation()
{
    _orientation = {
        _fusion.getRoll(),
        _fusion.getPitch(),
        _fusion.getYaw()
    };

    return _orientation;
}
