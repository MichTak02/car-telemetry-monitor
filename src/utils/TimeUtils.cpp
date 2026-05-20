#include "TimeUtils.h"
#include "NextionUtils.h"
#include "../SdReader.h"

char TimeUtils::TIME_FORMAT[] = "YYYY-MM-DDThh:mm:ss";
char TimeUtils::DASHED_TIME_FORMAT[] = "YYYY-MM-DDThh-mm-ss";
char TimeUtils::PRECISE_TIME_FORMAT[] = "YYYY-MM-DDThh:mm:ss.XXX";

uint32_t TimeUtils::baseMillis;
uint32_t TimeUtils::baseUnix;

void TimeUtils::init()
{
    DateTime initTime;
    bool timeSync = false;
    int8_t timeZone = 0;
    if (NextionUtils::getTimeSettings(initTime, timeSync, timeZone))
    {
        Settings::setTimeZone(timeZone);
    }
    else
    {
        initTime = DateTime(F(__DATE__), F(__TIME__));
        Logger::log(LOG_ERROR, "Failed to get time from Nextion display, using compile time");
    }
    baseUnix = initTime.unixtime();
    baseMillis = millis();
}

DateTime TimeUtils::getTime()
{
    uint32_t elapsedSec = (millis() - baseMillis) / 1000;
    return DateTime(baseUnix + elapsedSec);
}

void TimeUtils::getTimeStr(char *buff)
{
    strcpy(buff, TIME_FORMAT);
    sprintf(buff, getTime().toString(buff));
}

void TimeUtils::getTimeStrDashed(char *buff)
{
    strcpy(buff, DASHED_TIME_FORMAT);
    sprintf(buff, getTime().toString(buff));
}

void TimeUtils::setTime(DateTime dateTime)
{
    baseUnix = dateTime.unixtime();
    baseMillis = millis();
}

void TimeUtils::setTime(PreciseDateTime preciseTime)
{
    baseUnix = preciseTime.unixTime;
    baseMillis = millis() - preciseTime.milliseconds;
}

void TimeUtils::getPreciseTimeStr(char *buff)
{
    PreciseDateTime preciseTime = getPreciseTime();
    convertPreciseTimeStr(preciseTime, buff);
}

PreciseDateTime TimeUtils::getPreciseTime()
{
    uint32_t nowMillis = millis();
    uint32_t elapsedSinceResync = nowMillis - baseMillis;

    return {
        baseUnix + (elapsedSinceResync / 1000),
        uint16_t(elapsedSinceResync % 1000)};
}

void TimeUtils::convertPreciseTimeStr(PreciseDateTime preciseTime, char *buff)
{
    strcpy(buff, PRECISE_TIME_FORMAT);
    DateTime dateTime = DateTime(preciseTime.unixTime);
    dateTime.toString(buff);

    char *millisPos = strstr(buff, "XXX");

    sprintf(millisPos, "%03d", preciseTime.milliseconds);
}

PreciseDateTime TimeUtils::fromGPSTime(uint16_t year, uint8_t month, uint8_t day,
                                       uint8_t hour, uint8_t minute, uint8_t second,
                                       uint8_t centisecond, uint32_t ageMs)
{
    DateTime dt(year, month, day, hour, minute, second);
    uint32_t totalMs = centisecond * 10u + ageMs;
    return {
        dt.unixtime() + (totalMs / 1000) + Settings::getCurrent().timeZone * 3600,
        (uint16_t)(totalMs % 1000)
    };
}

void TimeUtils::syncFromGPS(const GPSSample& sample)
{
    if (!Settings::getCurrent().timeSync) {
        return;
    }

    PreciseDateTime internal = getPreciseTime();
    uint32_t diffSec = (sample.gpsTime.unixTime > internal.unixTime)
        ? sample.gpsTime.unixTime - internal.unixTime
        : internal.unixTime - sample.gpsTime.unixTime;

    if (diffSec <= SYNC_THRESHOLD_SEC) {
        return;
    }

    setTime(sample.gpsTime);
    NextionUtils::setTime(DateTime(sample.gpsTime.unixTime));
    Logger::log(LOG_INFO, "Time synced from GPS");
    SdReader::switchFile();
}
