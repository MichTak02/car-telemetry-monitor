#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "definitions.h"
#include "Logger.h"

class Gyroscope {
    private:
        FloatTuple3 _rawValues;
        FloatTuple3 _shift = {10.4, 0, -0.6};
        FloatTuple3 _calibratedValues;

    public:
        static const Unit DEFAULT_UNIT = UNIT_RAD_S;

        void setValues(FloatTuple3 gyroValue);
        FloatTuple3 getValues(Unit unit = DEFAULT_UNIT);
};

#endif