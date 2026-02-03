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
        static bool init(uint8_t csPin, const String& folder);
        static void log(LogLevel logLevel, const char* msg);
        // static void log(LogLevel logLevel, SensorType sensorType, const char* msg);
        static void log(PreciseDateTime time, LogLevel logLevel, SensorType sensorType, const char* msg);
        // static void logAcc(AccelerometerData accData);
        // static void logGyroSensor();
        static void logPressure(FloatSample pressureSample);
        static void logIMUSample(IMUSample sample);
};

#endif