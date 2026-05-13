#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <Arduino.h>
#include <RTClib.h>
#include "definitions.h"
#include "Settings.h"
#include "Logger.h"


class TimeUtils {
    private:
        static constexpr uint32_t SYNC_THRESHOLD_SEC = 10;
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

        static void setTime(DateTime dateTime);
        static void setTime(PreciseDateTime preciseTime);

        /**
         * @brief Sync internal time from a GPS sample when drift exceeds threshold
         *
         *
         * @param sample GPS sample with a valid gpsTime field
         */
        static void syncFromGPS(const GPSSample& sample);

        /**
         * @brief Build a PreciseDateTime from raw GPS date/time fields and time zone
         *
         * Combines the satellite-reported fix time with the elapsed time since
         * the fix (ageMs) to produce a value representing "now" in GPS-derived UTC
         *
         * @param year  4-digit year from GPS
         * @param month month (1–12)
         * @param day   day (1–31)
         * @param hour  hour (0–23)
         * @param minute minute (0–59)
         * @param second second (0–59)
         * @param centisecond sub-second from GPS (0–99, 10 ms resolution)
         * @param ageMs milliseconds elapsed since the fix was received
         * @return PreciseDateTime adjusted to "now" using GPS-derived UTC
         */
        static PreciseDateTime fromGPSTime(uint16_t year, uint8_t month, uint8_t day,
                                           uint8_t hour, uint8_t minute, uint8_t second,
                                           uint8_t centisecond, uint32_t ageMs);
};

#endif