#include "Magnetometer.h"

bool Magnetometer::init()
{
    _statusFlags.magnetometer = _mag.begin();
    return _statusFlags.magnetometer;
}

FloatTuple3 Magnetometer::getValue(Unit unit, bool calibrated)
{
    if (!hasReadData)
    {
        Logger::log(LOG_ERROR, SENSOR_MAGNETOMETER, "Trying to get magnetometer values before reading");
        return {0, 0, 0};
    }
    
    FloatTuple3 values = _rawValues;

    if (calibrated) {
        values = {
            values.x - _offset.x,
            values.y - _offset.y,
            values.z - _offset.z
        };
    }

    switch (unit)
    {
        case UNIT_MICROTESLA:
            return values;
        
        case UNIT_GAUSS:
            return {
                values.x * SENSORS_MICROTESLA_TO_GAUSS,
                values.y * SENSORS_MICROTESLA_TO_GAUSS,
                values.z * SENSORS_MICROTESLA_TO_GAUSS,
            };

        // Should not get there
        default:
            Logger::log(LOG_ERROR, "Wrong unit for magnetometer used, using default unit");
            return values;
    }
}

void Magnetometer::readData()
{
    hasReadData = true;

    sensors_event_t event; 
    _mag.getEvent(&event);
    _rawValues = {
        event.magnetic.x,
        event.magnetic.y,
        event.magnetic.z
    };    
}


