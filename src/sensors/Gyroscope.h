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

        /**
         * @brief Sets raw values from gyroscope
         * 
         * @param gyroValue gyroscope values
         */
        void setValues(FloatTuple3 gyroValue);

        /**
         * @brief Returns lastly set values in given unit
         * 
         * @param unit unit of returned value, in case of invalid unit, default one is used and operation is logged
         * @return values in given unit 
         */
        FloatTuple3 getValues(Unit unit = DEFAULT_UNIT);
};

#endif