#ifndef ALTITUDE_FUSION_H
#define ALTITUDE_FUSION_H

#include <Arduino.h>
#include "definitions.h"
#include "sensors/GPS.h"
#include "sensors/Barometer.h"

class AltitudeFusion {
    private:
        static constexpr float SEA_LEVEL_PRESSURE = 1013.25f; // in hPa

        bool _enabled = false;
        const float _alpha = 0.98;
        float _altitude;
        GPS& _gps;
        Barometer& _barometer;
        
        float pressureToAltitude(float pressure);

    public:
        AltitudeFusion(GPS& gps, Barometer& barometer) : _altitude(0.0f), _gps(gps), _barometer(barometer) {}

        /**
         * @brief Enables altitude fusion updates
         */
        void enable();

        /**
         * @brief Disables altitude fusion updates
         */
        void disable();

        /**
         * @brief Fuses barometer and GPS altitude using a complementary filter
         */
        void update();

        /**
         * @brief Returns the latest fused altitude in meters
         *
         * @return altitude in meters
         */
        float getAltitude();
};

#endif