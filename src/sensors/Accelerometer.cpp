#include "Accelerometer.h"

void Accelerometer::setValues(FloatTuple3 accelValues)
{
    _rawValues = accelValues;
}

FloatTuple3 Accelerometer::getValues(Unit unit)
{
    switch (unit)
    {
    case UNIT_MS2:
        return _rawValues;
    
    case UNIT_G:
        return {
                _rawValues.x * SENSORS_MS2_TO_G,
                _rawValues.y * SENSORS_MS2_TO_G,
                _rawValues.z * SENSORS_MS2_TO_G,
            };

    default:
        // Should not get there
        Logger::log(LOG_ERROR, "Wrong unit for accelerometer used, using default unit");
        return _rawValues;
    }
}
