#ifndef BAROMETER_H
#define BAROMETER_H

#include <Arduino.h>
#include <Adafruit_BMP280.h>

#include "definitions.h"
#include "Logger.h"
#include "utils/TimeUtils.h"

class Barometer {
    private:
        Adafruit_BMP280 _bmp;
        float _rawValue;
        PreciseDateTime _timestamp;

        float ringBuff[RING_BUFF_LEN];
        uint8_t counter = 0;
        bool overflow = 0;

    public:
        static const Unit DEFAULT_UNIT = UNIT_PA;

        bool init();
        void readData();
        float getPressure();
        float getAltitude();
        float getTemperature();
        uint16_t getNormalizedPressure();

        float getValue(Unit unit = DEFAULT_UNIT);
        FloatSample getSample(Unit unit = DEFAULT_UNIT);
        void logData();
};

#endif