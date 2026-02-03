#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <Arduino.h>
#include <RTClib.h>
#include "definitions.h"


class TimeUtils {
    private:
        static RTC_DS1307 _rtc;
        static uint8_t lastSecond;
        static uint32_t baseMillis;
        static uint32_t baseUnix;

    public:
        static char TIME_FORMAT[32];
        static char DASHED_TIME_FORMAT[32];
        static char PRECISE_TIME_FORMAT[32];

        static void init();
        static DateTime getTime();
        static PreciseDateTime getPreciseTime();
        static void getTimeStr(char* buff);
        static void getPreciseTimeStr(char *buff);
        static void getTimeStrDashed(char* buff);
        static void convertPreciseTimeStr(PreciseDateTime preciseTime, char* buff);
        static void resyncTime();

        static void setTime(DateTime dateTime);
};

#endif