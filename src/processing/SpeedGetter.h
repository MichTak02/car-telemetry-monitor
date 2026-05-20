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

        /**
         * @brief Enables speed updates from GPS
         */
        void enable();

        /**
         * @brief Disables speed updates
         */
        void disable();

        /**
         * @brief Polls GPS for the latest speed value
         */
        void updateSpeed();

        /**
         * @brief Returns the last measured speed in km/h
         *
         * @return speed in km/h, or -1 if not available
         */
        float getSpeed();
};

#endif