#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define DEBUG
#define RING_BUFF_MAX_LEN 512

#define SENSORS_MS2_TO_G (0.10197162129779283F)
#define SENSORS_MICROTESLA_TO_GAUSS (0.01F)

#include <Arduino.h>

enum SensorType {
    NO_SENSOR = -1,
    SENSOR_ACCELEROMETER = 0,
    SENSOR_GYROSCOPE,
    SENSOR_MAGNETOMETER,
    SENSOR_IMU,
    SENSOR_BAROMETER,
    SENSOR_GPS
};

enum LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DATA
};

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

enum ImpactThresholdLevel {
    IMPACT_THRESHOLD_LOW,
    IMPACT_THRESHOLD_MEDIUM,
    IMPACT_THRESHOLD_HIGH
};

enum SegmentDurationLevel {
    SEGMENT_DURATION_SHORT,
    SEGMENT_DURATION_MEDIUM,
    SEGMENT_DURATION_LONG
};

struct AccelerometerData {
    float ax;
    float ay;
    float az;
};

struct FloatTuple3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };

        struct {
            float roll;
            float pitch;
            float yaw;
        };
    };
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

struct GPSSample {
    float latitude;
    float longitude;
    float speed; // km/h
    float heading; // degrees
    float altitude; // meters
    PreciseDateTime timestamp;
    PreciseDateTime gpsTime;
};

struct FloatSample {
    float value;
    PreciseDateTime timestamp;
};

struct InterruptStruct {
    uint32_t periodTicks;
    uint32_t lastTick;
    volatile uint16_t pendingTriggers;

    InterruptStruct(uint32_t periodTicks)
        : periodTicks(periodTicks),
          lastTick(0),
          pendingTriggers(0)
    {}
};


struct StatusFlags {
    bool gps;
    bool imu;
    bool magnetometer;
    bool barometer;
    bool sdCard;
    bool bluetooth;
};

struct EventFlags {
    bool settingsChanged;
    bool timeChanged;
    bool calibrationRequest;
    bool loadCalibrationRequest;
};

extern EventFlags eventFlags;

#endif
