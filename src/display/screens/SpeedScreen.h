#ifndef SPEED_SCREEN_H
#define SPEED_SCREEN_H

#include "GenericScreen.h"
#include "processing/SpeedGetter.h"
#include "processing/AccelerationMagnitude.h"

class SpeedScreen : public GenericScreen {
    private:
        SpeedGetter& _speedGetter;
        AccelerationMagnitude& _accelMagnitude;
        static constexpr char SPEED_FIELD[] = "spTr.val";
        static constexpr char ACCEL_FIELD[] = "accTr.val";
        static constexpr uint16_t DECIMAL_MULTIPLIER = 100; // 2 decimal places
        uint16_t lastSentSpeed = -1;
    public:
        SpeedScreen(EasyNex& nex, SpeedGetter& speedGetter, AccelerationMagnitude& accelMagnitude) : GenericScreen(nex), _speedGetter(speedGetter), _accelMagnitude(accelMagnitude)
        {
            updatePeriodTicks = 1;
        };
        /**
         * @brief Initializes speed and acceleration display fields
         */
        void init() override;

        /**
         * @brief Sends speed and acceleration data to the display
         */
        void update() override;

        /**
         * @brief Disables speed getter and acceleration computation
         */
        void disableProcessing() override;
};

#endif