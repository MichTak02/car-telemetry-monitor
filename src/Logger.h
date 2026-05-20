#ifndef LOGGER_H
#define LOGGER_H
#define USE_LONG_FILE_NAMES 1

#include <Arduino.h>
#include <SdFat.h>
#include "definitions.h"
#include "SdReader.h"

class Logger {
    private:
        static char lastErrorMsg[128];
        static bool updatedLastErrorMsg;
        static StatusFlags* _statusFlags;
    
    public:
        /**
         * @brief Initializes the logger with a reference to system status flags
         *
         * @param statusFlags reference to system status flags
         */
        static void init(StatusFlags& statusFlags);

        /**
         * @brief Logs message to SD card
         * 
         * @param logLevel log level
         * @param msg message
         */
        static void log(LogLevel logLevel, const char* msg);

        /**
         * @brief Logs message to SD card
         * 
         * @param logLevel log level
         * @param sensorType type of sensor
         * @param msg message
         */
        static void log(LogLevel logLevel, SensorType sensorType, const char *msg);

        /**
         * @brief Logs message to SD card
         * 
         * @param time Timestamp
         * @param logLevel log level
         * @param sensorType type of sensor
         * @param msg message
         */
        static void log(PreciseDateTime time, LogLevel logLevel, SensorType sensorType, const char* msg);

        /**
         * @brief Get the last error message
         * 
         * @return const char* last error message
         */
        static const char* getLastErrorMsg();

        /**
         * @brief Returns true if a new unread error message is available
         *
         * @return true if error message was updated
         */
        static bool hasUpdatedErrorMsg();
};

#endif