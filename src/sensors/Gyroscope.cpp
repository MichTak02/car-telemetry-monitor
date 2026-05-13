#include "Gyroscope.h"
#include "Settings.h"

void Gyroscope::setValues(FloatTuple3 gyroValue)
{
    _rawValues = gyroValue;
}

FloatTuple3 Gyroscope::getValues(Unit unit, bool calibrated)
{
    FloatTuple3 values = _rawValues;

    if (calibrated) {
        const FloatTuple3& offset = Settings::getCurrent().gyroShift;
        values = {
            values.x - offset.x,
            values.y - offset.y,
            values.z - offset.z
        };
    }

    switch (unit)
    {
        case UNIT_RAD_S:
            return values;
        
        case UNIT_DEG_S:
            return {
                values.x * SENSORS_RADS_TO_DPS,
                values.y * SENSORS_RADS_TO_DPS,
                values.z * SENSORS_RADS_TO_DPS,
            };

        // Should not get there
        default:
            Logger::log(LOG_ERROR, "Wrong unit for gyroscope used, using default unit");
            return _rawValues;
    }
}
