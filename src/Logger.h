#ifndef LOGGER_H
#define LOGGER_H
#define USE_LONG_FILE_NAMES 1

#include <Arduino.h>
#include <SdFat.h>
#include "definitions.h"

class Logger {
    private:
        const double logInterval = 1000.0; // in milliseconds
        static unsigned long lastLogTime;
        static String _path;
        static File _file;
        static SdFat _sd;
    
    public:
        /**
         * @brief Initializes SD card module over SPI interface and creates log file
         * 
         * @param csPin CS pin of SD card module
         * @param folder path to logs folder
         * @return true if initialization succeeds, otherwise false
         */
        static bool init(uint8_t csPin, const String& folder);
        
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
         * @brief Logs measurment from IMU sensors (accelerometer, gyroscope, magnetometer)
         * 
         * @param sample sample from IMU sensors
         */
        static void logIMUSample(IMUSample sample);
};

#endif