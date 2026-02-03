#include "Barometer.h"

bool Barometer::init()
{
    if (!_bmp.begin(0x76)) {
        return false;
    }
    return true;
}

float Barometer::getPressure()
{
    
    float pressure = _bmp.readPressure();
    ringBuff[counter % RING_BUFF_LEN] = pressure;
    counter += 1;

    return pressure;
}

float Barometer::getAltitude()
{
    return _bmp.readAltitude(1019);
}

float Barometer::getTemperature()
{
    return _bmp.readTemperature();
}


uint16_t Barometer::getNormalizedPressure()
{
    return 0;
}

void Barometer::readData()
{
    _rawValue = _bmp.readPressure();
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

void Barometer::logData()
{
    Logger::logPressure(getSample());
}
