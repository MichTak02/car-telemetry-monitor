#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "definitions.h"
#include "utils/RingBufferFloat.h"
#include "Logger.h"

#define MS2_TO_G 0.10197162129779283

class Accelerometer {
    private:
        FloatTuple3 _rawValues;
    public:
        static const Unit DEFAULT_UNIT = UNIT_MS2;

        void setValues(FloatTuple3 accelValues);
        FloatTuple3 getValues(Unit unit = DEFAULT_UNIT);
};

#endif