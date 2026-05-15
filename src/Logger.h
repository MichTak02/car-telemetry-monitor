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
         * @brief Logs pressure
         * 
         * @param pressureSample pressure
         */
        static void logPressure(FloatSample pressureSample);
        
        /**
         * @brief Logs measurement from IMU sensors (accelerometer, gyroscope)
         *
         * @param sample sample from IMU sensors
         */
        static void logIMUSample(IMUSample sample);

        /**
         * @brief Get the last error message
         * 
         * @return const char* last error message
         */
        static const char* getLastErrorMsg();

        static bool hasUpdatedErrorMsg();
};

#endif