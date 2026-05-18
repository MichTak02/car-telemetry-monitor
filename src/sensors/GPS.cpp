#include "GPS.h"

HardwareSerial GPSSerial(PA3, PA2);

bool GPS::init()
{
    GPSSerial.begin(9600);
    delay(100);
    GPSSerial.write(setBaud115200, sizeof(setBaud115200));
    GPSSerial.end();
    GPSSerial.begin(115200);
    delay(100);
    GPSSerial.write(disableGLL, sizeof(disableGLL));
    delay(100);

    GPSSerial.write(disableGSA, sizeof(disableGSA));
    delay(100);

    GPSSerial.write(disableGSV, sizeof(disableGSV));
    delay(100);

    GPSSerial.write(disableVTG, sizeof(disableVTG));
    delay(100);
    return true;
}

void GPS::readData()
{
    bool readSomething = false;
    bool overflow = false;
    while (GPSSerial.available())
    {
        char c = GPSSerial.read();
        if (!_buff.addValue(&c) && !overflow) {
            Logger::log(LOG_ERROR, SENSOR_GPS, "GPS buffer overflow");
            overflow = true;
        }
        readSomething = true;
    }
}

void GPS::parseData(uint16_t maxChars)
{
    char c;
    uint16_t charsRead = 0;

    while (charsRead < maxChars && _buff.readValue(&c))
    {
        _gps.encode(c);
        charsRead++;
    }
}

GPSSample GPS::getSample()
{
    GPSSample sample;
    sample.latitude = _gps.location.lat();
    sample.longitude = _gps.location.lng();
    sample.speed = _gps.speed.kmph();
    sample.heading = _gps.course.deg();
    sample.timestamp = TimeUtils::getPreciseTime();
    sample.altitude = _gps.altitude.meters();
    sample.timestamp.unixTime -= _gps.time.age() / 1000; // Adjust timestamp based on GPS data age

    sample.gpsTime = TimeUtils::fromGPSTime(
        _gps.date.year(), _gps.date.month(), _gps.date.day(),
        _gps.time.hour(), _gps.time.minute(), _gps.time.second(),
        _gps.time.centisecond(), _gps.time.age());


    return sample;
}

bool GPS::isValid()
{
    const bool valid = _gps.location.isValid() && _gps.speed.isValid() && _gps.course.isValid() && _gps.altitude.isValid() && _gps.date.isValid() && _gps.time.isValid();
    _statusFlags.gps = valid;
    return valid;
}

bool GPS::hasUpdatedData()
{
    return _gps.location.isUpdated() || _gps.speed.isUpdated() || _gps.course.isUpdated() || _gps.altitude.isUpdated();
}

void GPS::logSample()
{
    if (!isValid()) {
        Logger::log(LOG_WARN, SENSOR_GPS, "Invalid GPS data");
        return;
    }

    GPSSample sample = getSample();
    char msg[128] = {0};
    
    float positionValues[] = {
        sample.latitude,
        sample.longitude
    };

    float dataValues[] = {
        sample.speed,
        sample.heading
    };

    const char delim = ',';

    GenericUtils::floatsToStr(positionValues, 2, delim, msg);
    GenericUtils::floatsToStr(dataValues, 2, delim, msg + strlen(msg), 6);
    Logger::log(sample.timestamp, LOG_DATA, SENSOR_GPS, msg);
}
