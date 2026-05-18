#ifndef MOTION_FUSION_H
#define MOTION_FUSION_H

#include <Arduino.h>
#include <MadgwickAHRS.h>
#include "definitions.h"
#include "sensors/IMUDriver.h"

class MotionFusion {
    private:
        bool _enabled = false;
        bool _started = false;
        Madgwick _fusion;
        FloatTuple3 _orientation;
        IMUDriver& _imuDriver;

    public:
        MotionFusion(IMUDriver& imuDriver) : _imuDriver(imuDriver) {}

        void enable();
        void disable();
        void update();
        FloatTuple3 getOrientation();
};

#endif