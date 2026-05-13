#ifndef ALTITUDE_FUSION_H
#define ALTITUDE_FUSION_H

#include <Arduino.h>
#include "definitions.h"
#include "sensors/GPS.h"
#include "sensors/Barometer.h"

class AltitudeFusion {
    private:
        bool _enabled = false;
        const float _alpha = 0.98;
        float _altitude;
        GPS& _gps;
        Barometer& _barometer;

    public:
        AltitudeFusion(GPS& gps, Barometer& barometer) : _altitude(0.0f), _gps(gps), _barometer(barometer) {}
        void enable();
        void disable();
        void update();
        float getAltitude();
};

#endif