#ifndef SPEED_GETTER_H
#define SPEED_GETTER_H

#include "sensors/GPS.h"


class SpeedGetter {
    private:
        bool _enabled = false;
        float _speed = -1.0f;
        GPS& _gps;

    public:
        SpeedGetter(GPS& gps) : _gps(gps), _speed(0.0f) {};
        void enable();
        void disable();
        void updateSpeed();
        float getSpeed();
};

#endif