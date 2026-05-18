#include "MotionFusion.h"

void MotionFusion::enable()
{
    if (!_started) {
        _fusion.begin(20);
        _orientation = {0.0f, 0.0f, 0.0f};
        _started = true;
    }
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

    // Rotate sensor frame 90 deg CW around Z so phone-holder tilt maps to Madgwick roll,
    // moving the gimbal-lock singularity away from the axis that can reach +-90 deg in use
    _fusion.updateIMU(
         sample.gyro.y,  -sample.gyro.x,  sample.gyro.z,
        sample.accel.y, -sample.accel.x, sample.accel.z
    );
}

FloatTuple3 MotionFusion::getOrientation()
{
    _orientation = {
        _fusion.getRoll() * (-1.0f) - 90, // Invert roll
        _fusion.getPitch(),
        _fusion.getYaw()
    };

    return _orientation;
}
