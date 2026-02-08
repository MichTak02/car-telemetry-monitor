#include "TimeUtils.h"

#include <RTClib.h>

char TimeUtils::TIME_FORMAT[] = "YYYY-MM-DDThh:mm:ss";
char TimeUtils::DASHED_TIME_FORMAT[] = "YYYY-MM-DDThh-mm-ss";
char TimeUtils::PRECISE_TIME_FORMAT[] = "YYYY-MM-DDThh:mm:ss.XXX";

uint8_t TimeUtils::lastSecond;
uint32_t TimeUtils::baseMillis;
uint32_t TimeUtils::baseUnix;

RTC_DS1307 TimeUtils::_rtc;

void TimeUtils::init()
{
    if (!_rtc.begin()) {
        // TODO vymyslet co při failu
        Serial.println("Couldn't find RTC");
        //while (1);
    }

    // TODO pouze když se nenačte
    if (true) {
        Serial.println("RTC lost power, setting time to compile time");
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    DateTime time = _rtc.now();
    baseMillis = millis();

    baseUnix = time.unixtime();
    lastSecond = time.second();
}

DateTime TimeUtils::getTime()
{
    return _rtc.now();
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
    _rtc.adjust(dateTime);
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
        uint16_t (elapsedSinceResync % 1000)
    };
}

void TimeUtils::convertPreciseTimeStr(PreciseDateTime preciseTime, char *buff)
{
    strcpy(buff, PRECISE_TIME_FORMAT);
    DateTime dateTime = DateTime(preciseTime.unixTime);
    dateTime.toString(buff);
        
    char *millisPos = strstr(buff, "XXX");
    
    sprintf(millisPos, "%03d", preciseTime.milliseconds);
}

void TimeUtils::resyncTime()
{
    DateTime now = getTime();

    if (now.second() != lastSecond) {
        lastSecond = now.second();
        baseUnix = now.unixtime();
        baseMillis = millis();
    }

}
