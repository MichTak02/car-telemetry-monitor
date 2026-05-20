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

        /**
         * @brief Enables orientation fusion updates
         */
        void enable();

        /**
         * @brief Disables orientation fusion updates
         */
        void disable();

        /**
         * @brief Reads IMU data and updates the Madgwick filter
         */
        void update();

        /**
         * @brief Returns the latest estimated orientation as pitch, roll, and yaw
         *
         * @return orientation tuple in degrees
         */
        FloatTuple3 getOrientation();
};

#endif