#ifndef ACCELERATION_MAGNITUDE_H
#define ACCELERATION_MAGNITUDE_H

#include <Arduino.h>
#include "sensors/IMUDriver.h"


class AccelerationMagnitude {
    private:
        IMUDriver& _imuDriver;
        float _acceleration = 0;
        bool _isInitialized = false;
        float _axLowPass = 0.0f;
        float _ayLowPass = 0.0f;
        float _azLowPass = 0.0f;

        float _gx = 0.0f;
        float _gy = 0.0f;
        float _gz = 0.0f;

        bool _enabled = false;
        static constexpr float ACCEL_ALPHA = 0.1f;
        static constexpr float GRAVITY_ALPHA = 0.01f;

    public:
        AccelerationMagnitude(IMUDriver& imuDriver) : _imuDriver(imuDriver) {}

        void enable();
        void disable();
        void update();
        float getAcceleration();
};

#endif
