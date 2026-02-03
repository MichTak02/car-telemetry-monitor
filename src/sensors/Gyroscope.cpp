#include "Gyroscope.h"

void Gyroscope::setValues(FloatTuple3 gyroValue)
{
    _rawValues = gyroValue;
}

FloatTuple3 Gyroscope::getValues(Unit unit)
{
    switch (unit)
    {
        case UNIT_RAD_S:
            return _rawValues;
        
        case UNIT_DEG_S:
            return {
                _rawValues.x * SENSORS_RADS_TO_DPS,
                _rawValues.y * SENSORS_RADS_TO_DPS,
                _rawValues.z * SENSORS_RADS_TO_DPS,
            };

        // Should not get there
        default:
            Logger::log(LOG_ERROR, "Wrong unit for gyroscope used, using default unit");
            return _rawValues;
    }
}
