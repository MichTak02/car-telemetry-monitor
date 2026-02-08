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
        float _temp;

        float ringBuff[RING_BUFF_LEN];
        uint8_t counter = 0;
        bool overflow = 0;

    public:
        static const Unit DEFAULT_UNIT = UNIT_PA;

        /**
         * @brief Initializes barometer module
         * 
         * @return true if initialization succeeds, otherwise false
         */
        bool init();

        /**
         * @brief Reads data from barometer module
         * 
         */
        void readData();

        /**
         * @brief Returns lastly set pressure in given unit
         * 
         * @param unit 
         * @return pressure in given unit 
         */
        float getValue(Unit unit = DEFAULT_UNIT);

        /**
         * @brief Get the Sample object
         * 
         * @param unit 
         * @return FloatSample 
         */
        FloatSample getSample(Unit unit = DEFAULT_UNIT);

        /**
         * @brief Logs measured data into SD card
         * 
         */
        void logData();
};

#endif