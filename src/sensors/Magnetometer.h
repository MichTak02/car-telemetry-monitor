#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <Arduino.h>
#include <Adafruit_HMC5883_U.h>
#include "definitions.h"
#include "Logger.h"

class Magnetometer {
    private:
        Adafruit_HMC5883_Unified _mag;
        StatusFlags& _statusFlags;
        FloatTuple3 _rawValues;
        const FloatTuple3 _offset = {29.41f, -2.223f, -39.945f};
        bool hasReadData = false;

    public:
        static const Unit DEFAULT_UNIT = UNIT_MICROTESLA;
        Magnetometer(StatusFlags& statusFlags) : _statusFlags(statusFlags) {}
        bool init();
        void readData();
        FloatTuple3 getValue(Unit unit = DEFAULT_UNIT, bool calibrated = false);
};


#endif