#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define DEBUG 1
#define RING_BUFF_LEN 8

#define SENSORS_MS2_TO_G (0.10197162129779283F)

#include <Arduino.h>

enum SensorType {
    NO_SENSOR = -1,
    ACCELEROMETER = 0,
    GYROSCOPE,
    MAGNETOMETER,
    IMU,
    BAROMETER,
    SENSOR_GPS
};

// TODO přidat enum s chybama? (špatná jednotka, truncated log, ...)

enum LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DATA
};

// TODO Každý modul svoje jednotky
enum Unit {
    UNIT_MS2,
    UNIT_G,
    UNIT_DEG_S,
    UNIT_RAD_S,
    UNIT_CELSIUS,
    UNIT_FAHRENHEIT,
    UNIT_MICROTESLA,
    UNIT_GAUSS,
    UNIT_PA,
    UNIT_HPA,
    UNIT_METER
};

struct AccelerometerData {
    float ax;
    float ay;
    float az;
};

struct FloatTuple3 {
    float x;
    float y;
    float z;
};

struct PreciseDateTime {
    uint32_t unixTime;
    uint16_t milliseconds;
};

struct IMUSample {
    FloatTuple3 accel;
    FloatTuple3 gyro;
    FloatTuple3 mag;
    bool hasMag;
    PreciseDateTime timestamp;
};

struct FloatSample {
    float value;
    PreciseDateTime timestamp;
};


#endif