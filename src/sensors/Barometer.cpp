#include "Barometer.h"

bool Barometer::init()
{
    _statusFlags.barometer = _bmp.begin(I2C_ADDRESS);
    if (!_statusFlags.barometer) {
        Logger::log(LOG_ERROR, SENSOR_BAROMETER, "Could not initialize BMP280");
    }
    return _statusFlags.barometer;
}

void Barometer::readData()
{
    _rawValue = _bmp.readPressure();
    _temp = _bmp.readTemperature();
    _timestamp = TimeUtils::getPreciseTime();
}

float Barometer::getValue(Unit unit)
{
    switch (unit)
    {
    case UNIT_PA:
        return _rawValue;
    
    case UNIT_HPA:
        return _rawValue / 100.0;

    default:
        // Should not get there
        Logger::log(LOG_ERROR, "Wrong unit for barometer used, using default unit");
        return _rawValue;
    }
}

FloatSample Barometer::getSample(Unit unit)
{
    float value = getValue(unit);
    return {
        value,
        _timestamp
    };
}

void Barometer::logSample()
{
    char msg[16] = {0};
    FloatSample pressureSample = getSample();
    GenericUtils::floatsToStr(&pressureSample.value, 1, '_', msg);

    Logger::log(pressureSample.timestamp, LOG_DATA, SENSOR_BAROMETER, msg);
}
