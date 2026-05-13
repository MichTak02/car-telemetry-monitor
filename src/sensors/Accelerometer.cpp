#include "Accelerometer.h"

void Accelerometer::setValues(FloatTuple3 accelValues)
{
    _rawValues = accelValues;
}

FloatTuple3 Accelerometer::getValues(Unit unit, bool calibrated)
{
    FloatTuple3 values = _rawValues;

    if (calibrated) {
        values.x -= _offset.x;
        values.y -= _offset.y;
        values.z -= _offset.z;
    }

    switch (unit)
    {
    case UNIT_MS2:
        return values;
    
    case UNIT_G:
        return {
                values.x * SENSORS_MS2_TO_G,
                values.y * SENSORS_MS2_TO_G,
                values.z * SENSORS_MS2_TO_G,
            };

    default:
        // Should not get there
        Logger::log(LOG_ERROR, "Wrong unit for accelerometer used, using default unit");
        return _rawValues;
    }
}
