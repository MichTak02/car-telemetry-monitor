#ifndef ORIENTATION_SCREEN_H
#define ORIENTATION_SCREEN_H

#include "GenericScreen.h"
#include "processing/MotionFusion.h"

class OrientationScreen : public GenericScreen {
    private:
        MotionFusion& _motionFusion;
        static constexpr char PITCH_FIELD[] = "pitchTr.val";
        static constexpr char ROLL_FIELD[] = "rollTr.val";

        static constexpr uint16_t DECIMAL_MULTIPLIER = 100; // 2 decimal places

    public:
        OrientationScreen(EasyNex& nex, MotionFusion& motionFusion) : GenericScreen(nex), _motionFusion(motionFusion)
        {
            updatePeriodTicks = 1;
        };
        /**
         * @brief Initializes pitch and roll display fields
         */
        void init() override;

        /**
         * @brief Sends orientation angles to the display
         */
        void update() override;

        /**
         * @brief Disables motion fusion
         */
        void disableProcessing() override;
};

#endif