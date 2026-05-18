#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "definitions.h"
#include "Logger.h"

#define MS2_TO_G 0.10197162129779283

class Accelerometer {
    private:
        FloatTuple3 _rawValues;
        const FloatTuple3 _offset = {0.0f, 0.0f, 0.0f};
    public:
        static const Unit DEFAULT_UNIT = UNIT_MS2;

        /**
         * @brief Sets raw values from accelerometer
         * 
         * @param accelValues accelerometer values
         */
        void setValues(FloatTuple3 accelValues);
        
        /**
         * @brief Returns lastly set values in given unit
         * 
         * @param unit unit of returned value, in case of invalid unit, default one is used and operation is logged
         * @param calibrated true for calibrated values, false for raw values
         * @return values in given unit
         */
        FloatTuple3 getValues(Unit unit = DEFAULT_UNIT, bool calibrated = false);
};

#endif