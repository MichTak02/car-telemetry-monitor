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

        // Dashes instead of colons (used for file name)
        static char DASHED_TIME_FORMAT[32];
        static char PRECISE_TIME_FORMAT[32];

        /**
         * @brief Initialize RTC module
         */
        static void init();

        /**
         * @brief Get current time from RTC module with seconds precision
         * 
         * @return DateTime current date and time
         */
        static DateTime getTime();

        /**
         * @brief Get current time with milliseconds precision
         * 
         * @return PreciseDateTime Current date and time as unix time and milliseconds
         */
        static PreciseDateTime getPreciseTime();
        
        /**
         * @brief Get current datetime as string
         * 
         * @param buff char buffer to store the time to
         */
        static void getTimeStr(char* buff);
        static void getPreciseTimeStr(char *buff);
        
        /**
         * @brief Get current datetime as string and replace colons with dashes
         * 
         * @param buff char buffer to store the time to
         */
        static void getTimeStrDashed(char* buff);
        
        /**
         * @brief Convert @ref PreciseDateTime into a string
         * 
         * @param preciseTime precise time
         * @param buff char buffer to save string to
         */
        static void convertPreciseTimeStr(PreciseDateTime preciseTime, char* buff);
        static void resyncTime();

        static void setTime(DateTime dateTime);
};

#endif